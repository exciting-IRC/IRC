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
  Message reply;
  reply.prefix = config.name;

  reply.command = util::pad_num(util::RPL_WELCOME);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back("Welcome to the Internet Relay Network!");
  send(reply);
  reply.params.clear();

  reply.command = util::pad_num(util::RPL_YOURHOST);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back(
      FMT("Your host is {servername}, running version {version}",
          (config.name, EIRC_VERSION)));
  send(reply);
  reply.params.clear();

  reply.command = util::pad_num(util::RPL_CREATED);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back(
      FMT("This server was crated {datetime}", (config.create_time)));
  send(reply);
  reply.params.clear();

  reply.command = util::pad_num(util::RPL_MYINFO);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back(config.name);
  reply.params.push_back(EIRC_VERSION);
  reply.params.push_back("o");
  reply.params.push_back("o");
  reply.params.push_back("");
  send(reply);
  reply.params.clear();

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

void Client::processMessage(const Message &m) {
  std::cout << "CMD: <" << m.command << ">";
  MPClientMap::const_iterator it = map_.find(util::to_upper(m.command));
  if (it == map_.end()) {
    std::cout << ": Not found" << std::endl;
    return;
  }
  std::cout << ":Found" << std::endl;
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
    Message reply;
    reply.prefix = config.name;
    reply.command = util::pad_num(util::ERR_NOORIGIN);
    reply.params.push_back(":No origin specified");
    conn_->send(reply);
  }
  conn_->send(FMT(":{0} PONG {0} :{1}", (config.name, m.params[0])));
}

void Client::oper(const Message &m) {
  Message reply;

  reply.prefix = config.name;

  if (m.params.size() < 2) {
    reply.command = util::pad_num(util::ERR_NEEDMOREPARAMS);
    reply.params.push_back(m.command);
    reply.params.push_back("Not enough parameters");
    conn_->send(reply);
    return;
  }

  bool is_ok =
      config.oper_user == m.params[0] && config.oper_password == m.params[1];

  if (is_ok) {
    ident_->mode_ |= UserMode::o;
    reply.command = util::pad_num(util::RPL_YOUAREOPER);
    reply.params.push_back("You are now an IRC operator");
    conn_->send(reply);
    return;
  } else {  // 보안적인 이유로 비밀번호가 틀렸을 때, 유저네임이 틀렸을 때 모두
            // 같은 에러로 응답함.
    reply.command = util::pad_num(util::ERR_NOOPERHOST);
    reply.params.push_back("No O-lines for your host");
    conn_->send(reply);
    return;
  }
}

void Client::kill(const Message &m) {
  Message reply;

  reply.prefix = "";
  if (m.params.size() < 2) {
    reply.command = util::pad_num(util::ERR_NEEDMOREPARAMS);
    reply.params.push_back(m.command);
    reply.params.push_back("Not enough parameters");
    conn_->send(reply);
    return;
  }

  const ClientMap &clients = server.getClients();
  if (clients.find(m.params[0]) == clients.end()) {
    reply.command = util::pad_num(util::ERR_NOSUCHNICK);
    reply.params.push_back(m.params[0]);
    reply.params.push_back("No such nick/channel");
    conn_->send(reply);
    return;
  }

  bool has_privilege = (ident_->mode_ & UserMode::o);
  if (not has_privilege) {
    reply.command = util::pad_num(util::ERR_NOPRIVILEGES);
    reply.params.push_back("Permission Denied- You're not an IRC operator");
    conn_->send(reply);
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
  Message reply;

  reply.prefix = config.name;
  if (m.params.size() < 1) {
    reply.command = util::pad_num(util::ERR_NEEDMOREPARAMS);
    reply.params.push_back(m.command);
    reply.params.push_back("Not enough parameters");

    send(reply);
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
      reply.command = util::pad_num(util::RPL_TOPIC);
      reply.params.push_back(*it);
      reply.params.push_back("");
      send(reply);
      reply.params.clear();

      reply.command = util::pad_num(util::RPL_NAMREPLY);
      reply.params.push_back(*it);
      reply.params.push_back("");
      const ClientMap users = new_channel->getUsers();
      for (ClientMap::const_iterator user = users.begin(),
                                     end = util::prev(users.end());
           user != end; ++user) {
        reply.params[1] += user->first + " ";
      }
      reply.params[1] += util::prev(users.end())->first;
      send(reply);
      reply.params.clear();
    } else {
      reply.command = util::pad_num(util::ERR_NOSUCHCHANNEL);
      reply.params.push_back(*it);
      reply.params.push_back("No such Channel");
      send(reply);
      reply.params.clear();
    }
  }
}

void Client::part(const Message &m) {
  if (m.params.empty()) {
    sendNeedMoreParam(m.command);
    return;
  }

  Message reply;

  reply.prefix = config.name;
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
  Message reply;

  reply.prefix = config.name;
  if (m.params[0] != ident_->nickname_) {
    reply.command = util::pad_num(util::ERR_USERSDONTMATCH);
    reply.params.push_back(ident_->nickname_);
    reply.params.push_back("Cant change mode for other users");

    send(reply);
    return;
  }

  /* XXX handle MODE commands*/
}

void Client::mode(const Message &m) {
  // XXX mode is currently not supported.
  (void)m;
  Message reply;

  reply.prefix = config.name;
  reply.command = util::pad_num(501);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back("Unknown MODE flag");
  send(reply);

  // if (m.params.size() < 1) {
  //   sendNeedMoreParam(m.command);
  //   return;
  // }
  // if (util::isChannelPrefix(m.params[0][0])) {
  //   userMode(m);
  // } else {
  //   channelMode(m);
  // }
}

void Client::sendNeedMoreParam(const std::string &command) {
  Message reply;

  reply.prefix = config.name;
  reply.command = util::pad_num(util::ERR_NEEDMOREPARAMS);
  reply.params.push_back(command);
  reply.params.push_back("Not enough parameters");
  send(reply);
}

void Client::privmsg(const Message &m) {
  Message reply(m);

  reply.prefix = ident_->toString();
  if (util::isChannelPrefix(reply.params[0][0])) {
    typedef 
    std::map<std::string, Channel> &cm = server.getChannels();

    if (server.getChannels().find())
    joined_channels_.find(reply.params[0])->second->sendAll(reply, this);
  } else {
    ClientMap &clients = server.getClients();
    ClientMap::iterator it = clients.find(m.params[0]);
    if (it == clients.end()) {
      // send(Message::as_numeric_reply(util::ERR_NOSUCHNICK, VA(("No nickname given")))
      // return;
    } else {
      it->second->send(reply);
    }
  }
  return;
}
