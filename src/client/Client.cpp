#include "Client.hpp"

#include <unistd.h>

#include <iostream>

#include "ClientConn.hpp"
#include "command/returncode.hpp"
#include "server/Server.hpp"
#include "util/algorithm/functor.hpp"
#include "util/color.hpp"
#include "util/config/config.hpp"
#include "util/general/logging.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/conversion.hpp"
#include "util/strutil/format.hpp"
#include "util/vargs/container_of.hpp"
#include "version.hpp"

using util::p;
const Client::MPClientMap Client::map_ =
    container_of<MPClientMap, MPClientMap::value_type>(
        p("PING", &Client::ping), p("QUIT", &Client::quit),
        p("JOIN", &Client::join), p("PRIVMSG", &Client::privmsg),
        p("KILL", &Client::kill), p("OPER", &Client::oper),
        p("MODE", &Client::mode), p("PART", &Client::part));

/*CLIENT===============================*/

Client::Client(ClientConn *conn) : conn_(conn), ident_(conn->moveIdent()) {}

Client::~Client() {
  for (ChannelMap::iterator it = joined_channels_.begin(),
                            end = joined_channels_.end();
       it != end; ++it) {
    it->second->removeUser(ident_->nickname_);
  }
  delete conn_;
  delete ident_;
}

int Client::getFd() const { return conn_->getFd(); }

void Client::sendRegisterMessage() {
  util::debug_info("sending register message to", ident_->nickname_);
  send(Message::as_numeric_reply(
      util::RPL_WELCOME,
      VA((ident_->nickname_, "Welcome to the Internet Relay Network!"))));
  send(Message::as_numeric_reply(
      util::RPL_YOURHOST,
      VA((ident_->nickname_,
          FMT("Your host is {servername}, running version {version}",
              (server.config_.name, VERSION_STRING))))));
  send(Message::as_numeric_reply(
      util::RPL_CREATED,
      VA((ident_->nickname_, FMT("This server was created {create_time}",
                                 (server.config_.create_time))))));
  send(Message::as_numeric_reply(util::RPL_MYINFO,
                                 VA((ident_->nickname_, server.config_.name,
                                     VERSION_STRING, "o", "o", ""))));
  sendMOTD();
}

void Client::sendMOTD() {
  std::stringstream ss(server.config_.motd);

  std::string line;
  while (std::getline(ss, line)) {
    send(Message::as_numeric_reply(util::RPL_MOTD,
                                   VA((ident_->nickname_, line))));
  }
  send(Message::as_numeric_reply(util::RPL_ENDOFMOTD,
                                 VA((ident_->nickname_, "End of MOTD"))));
}

void Client::sendNotice(const std::string &msg) {
  send(FMT(":{servername} NOTICE {nickname} :{message}",
           (server.config_.name, ident_->nickname_, msg)));
}

void Client::sendError(const std::string &msg) {
  send(FMT("ERROR :{message}", (msg)));
}

std::string &Client::getNick() { return ident_->nickname_; }

// FIXME: ClientConn::processMessage와 너무 겹침
void Client::processMessage(const Message &m) {
  MPClientMap::const_iterator it = map_.find(util::to_upper(m.command));

  const bool found = it != map_.end();
  util::debug_input(m.command, found);
  if (found)
    (this->*(it->second))(m);
}

void Client::handleReadEvent(Event &e) {
  if (conn_->handleReceive(e) == result_t::kError) {
    delete conn_;
    conn_ = NULL;
  } else {
    ParserResult::e result;  // TODO: 안 쓸거면 지우기
    while ((result = conn_->parse()) == ParserResult::kSuccess) {
      Message msg = conn_->getMessage();
      processMessage(msg);
      if (util::to_upper(msg.command) == "QUIT")
        return;
    }
  }
}

int Client::handle(Event e) {
  switch (e.kind) {
    case EventKind::kRead:
      handleReadEvent(e);
      break;
    default:
      abort();
      break;
  }
  return 0;
}

void Client::ping(const Message &m) {
  if (m.params.size() != 1) {
    send(Message::as_numeric_reply(util::ERR_NOORIGIN,
                                   VA(("No origin specified"))));
  }
  send(FMT(":{0} PONG {0} :{1}", (server.config_.name, m.params[0])));
}

void Client::oper(const Message &m) {
  if (m.params.size() < 2) {
    send(Message::as_not_enough_params_reply(m.command));
    return;
  }

  const bool is_ok = server.config_.oper_user == m.params[0] &&
                     server.config_.oper_password == m.params[1];

  if (is_ok) {
    ident_->mode_ |= UserMode::o;
    send(Message::as_numeric_reply(util::RPL_YOUAREOPER,
                                   VA(("You are now an IRC operator"))));
    return;
  } else {  // 보안적인 이유로 비밀번호가 틀렸을 때, 유저네임이 틀렸을 때 모두
            // 같은 에러로 응답함.
    send(Message::as_numeric_reply(util::ERR_NOOPERHOST,
                                   VA(("No O-lines for your host"))));
    return;
  }
}

void Client::kill(const Message &m) {
  if (m.params.size() < 2) {
    send(Message::as_not_enough_params_reply(m.command));
    return;
  }
  Message reply;

  reply.prefix = "";
  const ClientMap &clients = server.getClients();
  if (clients.find(m.params[0]) == clients.end()) {
    reply.command = util::pad_num(util::ERR_NOSUCHNICK);
    reply.params.push_back(m.params[0]);
    reply.params.push_back("No such nick/channel");
    send(reply);
    return;
  }

  bool has_privilege = (ident_->mode_ & UserMode::o);
  if (not has_privilege) {
    reply.command = util::pad_num(util::ERR_NOPRIVILEGES);
    reply.params.push_back("Permission Denied- You're not an IRC operator");
    send(reply);
    return;
  } else {
    server.removeClient(m.params[0]);
  }
}

void Client::quit(const Message &m) {
  Message reply = m;

  reply.prefix = ident_->toString();
  for (ChannelMap::iterator it = joined_channels_.begin(),
                            end = joined_channels_.end();
       it != end; ++it) {
    it->second->sendAll(reply, this);
  }
  server.removeClient(ident_->nickname_);
}

void Client::join(const Message &m) {
  if (m.params.size() < 1) {
    send(Message::as_not_enough_params_reply(m.command));
    return;
  }

  if (m.params[0] == "0") {
    // XXX part all
    return;
  }

  std::vector<std::string> channels = util::split(m.params[0], ",");
  for (std::vector<std::string>::iterator it = channels.begin(),
                                          end = channels.end();
       it != end; ++it) {
    Channel *new_channel = server.addUserToChannel(*it, this);
    if (new_channel) {
      joined_channels_.insert(std::make_pair(*it, new_channel));
      send(Message::as_numeric_reply(util::RPL_TOPIC, VA((*it, ""))));
      send(Message::as_numeric_reply(
          util::RPL_NAMREPLY,
          VA((*it, util::join(util::keys(new_channel->getUsers()), " ")))));
    } else {
      send(Message::as_numeric_reply(util::ERR_NOSUCHCHANNEL,
                                     VA((*it, "No such channel"))));
    }
  }
}

void Client::part(const Message &m) {
  if (m.params.empty()) {
    send(Message::as_not_enough_params_reply(m.command));
    return;
  }

  Message reply;

  reply.prefix = server.config_.name;
  std::vector<std::string> channels = util::split(m.params[0], ",");
  for (std::vector<std::string>::iterator it = channels.begin(),
                                          end = channels.end();
       it != end; ++it) {
    ChannelMap::iterator channel = joined_channels_.find(*it);
    if (channel == joined_channels_.end()) {
      reply.command = util::pad_num(util::ERR_NOTONCHANNEL);
      reply.params.push_back(*it);
      reply.params.push_back("You're not on that channel");
      send(reply);
      reply.params.clear();
    } else {
      Channel *ch = channel->second;

      ch->removeUser(ident_->nickname_);
      joined_channels_.erase(channel);
      if (ch->getUsers().empty()) {
        server.removeChannel(*it);
      }
    }
  }
}

void Client::channelMode(const Message &m) { (void)m; }
/* XXX handle MODE commands*/

void Client::userMode(const Message &m) {
  if (m.params[0] != ident_->nickname_) {
    send(Message::as_numeric_reply(
        util::ERR_USERSDONTMATCH,
        VA((ident_->nickname_, "Cant change mode for other users"))));
    return;
  }

  /* XXX handle MODE commands*/
}

void Client::mode(const Message &m) {
  // XXX mode is currently not supported.
  (void)m;
  send(Message::as_numeric_reply(util::ERR_UMODEUNKNOWNFLAG,
                                 VA((ident_->nickname_, "Unknown MODE flag"))));
  // if (m.params.size() < 1) {
  // send(Message::as_not_enough_params_reply(m.command))
  return;
  //   return;
  // }
  // if (util::isChannelPrefix(m.params[0][0])) {
  //   userMode(m);
  // } else {
  //   channelMode(m);
  // }
}

void Client::privmsg(const Message &m) {
  if (m.params.empty() or m.params[0].empty()) {
    send(Message::as_numeric_reply(util::ERR_NORECIPIENT, VA((m.command))));
    return;
  }
  if (m.params.size() < 2 or m.params[1].empty()) {
    send(Message::as_numeric_reply(util::ERR_NOTEXTTOSEND,
                                   VA(("No text to send"))));
    return;
  }
  const Message reply = {ident_->toString(), m.command, m.params};
  const std::string &recipient = reply.params[0];

  if (util::isChannelPrefix(recipient.front())) {
    ChannelMap &channels = server.getChannels();
    ChannelMap::iterator it = channels.find(recipient);
    if (it == channels.end()) {
      send(Message::as_numeric_reply(util::ERR_NOSUCHNICK,
                                     VA(("No such channel"))));
      return;
    } else {
      it->second->sendAll(reply, this);
    }
  } else {
    ClientMap &clients = server.getClients();
    ClientMap::iterator it = clients.find(recipient);
    if (it == clients.end()) {
      send(Message::as_numeric_reply(util::ERR_NOSUCHNICK,
                                     VA(("No such nick"))));
      return;
    } else {
      it->second->send(reply);
    }
  }
  return;
}
