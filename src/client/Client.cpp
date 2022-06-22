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
          (config.name, config.version)));
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
  reply.params.push_back(config.version);
  reply.params.push_back("o");
  reply.params.push_back("o");
  reply.params.push_back("");
  send(reply);
  reply.params.clear();

  reply.command = util::pad_num(5);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back("NETWORK=Localnet");
  reply.params.push_back("are supported by this server");
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
    Message reply;
    reply.prefix = config.name;
    reply.command = util::pad_num(util::ERR_NOORIGIN);
    reply.params.push_back(":No origin specified");
    send(reply);
  }
  send(FMT(":{0} PONG {0} :{1}", (config.name, m.params[0])));
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
    Channel *new_channel = &server.addUserToChannel(*it, this);
    joined_channels_.insert(std::make_pair(*it, new_channel));
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
    joined_channels_.find(reply.params[0])->second->sendAll(reply, this);
  } else {
    send(reply);
  }
  return;
}
