#include "Client.hpp"

#include <unistd.h>

#include <iostream>

#include "ClientConn.hpp"
#include "command/returncode.hpp"
#include "server/Server.hpp"
#include "util/config/config.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/conversion.hpp"
#include "util/vargs/container_of.hpp"

using util::p;
const Client::MPClientMap Client::map_ =
    container_of<MPClientMap, MPClientMap::value_type>(
        p("PING", &Client::ping), p("QUIT", &Client::quit),
        p("JOIN", &Client::join), p("PRIVMSG", &Client::privmsg),
        p("KILL", &Client::kill), p("OPER", &Client::oper),
        p("MODE", &Client::mode));

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
  std::cout << "sending register message\n";
  send(Message::as_numeric_reply(
      util::RPL_WELCOME,
      VA((ident_->nickname_, "Welcome to the Internet Relay Network!"))));
  send(Message::as_numeric_reply(
      util::RPL_YOURHOST,
      VA((ident_->nickname_,
          FMT("Your host is {servername}, running version {version}",
              (config.name, config.version))))));
  send(Message::as_numeric_reply(
      util::RPL_CREATED,
      VA((ident_->nickname_, FMT("This server was created {create_time}",
                                 (config.create_time))))));
  send(Message::as_numeric_reply(
      util::RPL_MYINFO,
      VA((ident_->nickname_, config.name, config.version, "o", "o", ""))));
  send(Message::as_numeric_reply((util::returnCode)5,  // FIXME: 추가하기
                                 VA((ident_->nickname_, "NETWORK=Localnet",
                                     "are supported by this server"))));
  sendMOTD();
}

void Client::sendMOTD() {
  std::stringstream ss(config.motd);

  std::string line;
  Message reply;

  reply.prefix = config.name;
  reply.command = util::pad_num(util::RPL_MOTD);
  while (std::getline(ss, line)) {
    reply.params.push_back(ident_->nickname_);
    reply.params.push_back(line);
    send(reply);
    reply.params.clear();
  }
  reply.command = util::pad_num(util::RPL_ENDOFMOTD);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back("END of MOTD.");
  send(reply);
}

std::string &Client::getNick() { return ident_->nickname_; }

// XXX 여기 어케할까용
// void Client::handleWriteEvent(Event &e) {
//   if (e.flags.test(EventFlag::kEOF)) {
//     delete conn_;
//     conn_ = NULL;
//   }
//   std::size_t write_size =
//       std::min(buffer_.size(), static_cast<std::size_t>(e.data));
//   write(1, buffer_.data(), write_size);
//   ssize_t send_length = util::send(getFd(), buffer_.data(), write_size);
//   if (send_length == -1) {
//     std::cerr << "failed to write" << std::endl;
//     // XXX LOG ...
//   } else {
//     buffer_.advance(send_length);
//   }
//   if (buffer_.empty())
//     e.pool.removeEvent(EventKind::kWrite, this);
// }

// FIXME: ClientConn::processMessage와 너무 겹침
void Client::processMessage(const Message &m) {
  MPClientMap::const_iterator it = map_.find(m.command);
  const bool found = it != map_.end();
  const std::string status = found ? "" : "UNKNOWN";
  // FIXME: log 함수로 빼기
  COUT_FMT("{0} {1}-> \"{2}\"",
           (util::get_current_time("[%H:%M:%S]"), status, m.command));
  if (found)
    (this->*(it->second))(m);
}

void Client::handleReadEvent(Event &e) {
  if (conn_->handleReceive(e) == result_t::kError) {
    delete conn_;
    conn_ = NULL;
  } else {
    ParserResult::e result;
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
  send(FMT(":{0} PONG {0} :{1}", (config.name, m.params[0])));
}

void Client::oper(const Message &m) {
  if (m.params.size() < 2) {
    return send(Message::as_not_enough_params_reply(m.command));
  }

  const bool is_ok =
      config.oper_user == m.params[0] && config.oper_password == m.params[1];

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
    return send(Message::as_not_enough_params_reply(m.command));
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
    return send(Message::as_not_enough_params_reply(m.command));
  }

  if (m.params[0] == "0") {
    // XXX part all
    return;
  }

  std::vector<std::string> channels = util::split(m.params[0], ",");
  for (std::vector<std::string>::iterator it = channels.begin(),
                                          end = channels.end();
       it != end; ++it) {
    Channel *new_channel = &server.addUserToChannel(*it, this);
    joined_channels_.insert(std::make_pair(*it, new_channel));
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
  // return send(Message::as_not_enough_params_reply(m.command));
  //   return;
  // }
  // if (util::isChannelPrefix(m.params[0][0])) {
  //   userMode(m);
  // } else {
  //   channelMode(m);
  // }
}

void Client::privmsg(const Message &m) {
  Message reply(m);

  reply.prefix = ident_->toString();
  if (util::isChannelPrefix(reply.params[0][0])) {
    joined_channels_.find(reply.params[0])->second->sendAll(reply, this);
  } else {
    send(reply);
  }
  return;
}
