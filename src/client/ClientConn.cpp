#include "ClientConn.hpp"

#include <err.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "client/Client.hpp"
#include "command/returncode.hpp"
#include "command/returnformat.hpp"
#include "event/event.hpp"
#include "server/Server.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/algorithm/algorithm.hpp"
#include "util/config/config.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/strutil.hpp"

const MPMap ClientConn::map_ = ClientConn::getMPMap();

ClientConn::ClientConn(int sock, CCList::iterator this_position)
    : sock_(sock),
      this_position_(this_position),
      ident_(new UserIdent()),
      state_(ConnState::kClear) {}

ClientConn::~ClientConn() {
  if (sock_ != -1)
    close();
  delete ident_;
}

const MPMap ClientConn::getMPMap() {
  MPMap map;

  map.insert(MPMap::value_type("PASS", &ClientConn::processPass));
  map.insert(MPMap::value_type("USER", &ClientConn::processUser));
  map.insert(MPMap::value_type("NICK", &ClientConn::processNick));

  return map;
}

result_t::e ClientConn::handleWriteEvent(Event &e) {
  StringBuffer &buffer = send_queue_.front();

  if (e.flags.test(EventFlag::kEOF)) {
    return result_t::kError;
  }

  std::size_t write_size =
      std::min(buffer.size(), static_cast<std::size_t>(e.data));

  ssize_t send_length = util::send(getFd(), buffer.data(), write_size);
  if (send_length == -1) {
    return result_t::kError;
  } else {
    buffer.advance(send_length);
  }

  if (buffer.empty()) {
    send_queue_.pop();
    if (send_queue_.empty()) {
      e.pool.removeEvent(EventKind::kWrite, this);
    }
  }
  return result_t::kOK;
}

bool isValidFirstNickChar(char c) {
  return util::isLetter(c) || util::isSpecial(c);
}

bool isValidNickChar(char c) {
  return util::isLetter(c) || util::isDigit(c) || util::isSpecial(c) ||
         c == '-';
}

bool isValidNick(const std::string &nick) {
  if (nick.size() > 9)
    return false;
  if (!isValidFirstNickChar(nick[0]))
    return false;
  for (std::string::const_iterator it = util::next(nick.begin()),
                                   end = nick.end();
       it != end; ++it) {
    if (!isValidNickChar(*it)) {
      return false;
    }
  }
  return true;
}

void ClientConn::processNick(const Message &m) {
  Message reply;
  reply.prefix = config.name;

  if (m.params.empty()) {
    reply.command = util::to_string(util::ERR_NONICKNAMEGIVEN);
    reply.params.push_back("No nickname given");

    send(reply);
    return;
  }

  const std::string &nick = m.params[0];
  if (!isValidNick(nick)) {
    reply.command = util::to_string(util::ERR_ERRONEUSNICKNAME);
    reply.params.push_back(nick);
    reply.params.push_back("Erroneous nickname");

    send(reply);
    return;
  }

  if (server.getClients().find(nick) != server.getClients().end()) {
    reply.command = util::to_string(util::ERR_NICKNAMEINUSE);
    reply.params.push_back(nick);
    reply.params.push_back("Nickname is already in use");

    send(reply);
    return;
  }
  ident_->nickname_ = nick;
  state_ |= ConnState::kNick;
}

bool isValidUser(const std::string &s) {
  for (std::string::const_iterator it = s.begin(), end = s.end(); it != end;
       ++it) {
    if (*it == '@') {
      return false;
    }
  }
  return true;
}

void ClientConn::processUser(const Message &m) {
  Message reply;
  reply.prefix = config.name;

  if (m.params.size() != 4) {
    reply.command = util::to_string(util::ERR_NEEDMOREPARAMS);
    reply.params.push_back(m.command);
    reply.params.push_back("Not enough parameters");

    send(reply);
    return;
  }

  // XXX ERR_ALREADYREGISTERED

  ident_->username_ = m.params[0];
  ident_->hostname_ = m.params[1];
  ident_->servername_ = m.params[2];
  ident_->realname_ = m.params[3];

  state_ |= ConnState::kUser;
}

void ClientConn::processPass(const Message &m) {
  Message reply;
  reply.prefix = config.name;

  if (m.params.size() != 1) {
    reply.command = util::to_string(util::ERR_NEEDMOREPARAMS);
    reply.params.push_back(m.command);
    reply.params.push_back("Not enough parameters");

    send(reply);
    return;
  }

  ident_->password_ = m.params[0];
  state_ |= ConnState::kPass;
}

void ClientConn::processMessage(const Message &m) {
  std::cout << "CMD: <" << m.command << ">";
  MPMap::const_iterator it = map_.find(m.command);
  if (it == map_.end()) {
    std::cout << ": Not found" << std::endl;
    return;
  }
  std::cout << ":Found" << std::endl;
  (this->*(it->second))(m);
}

int ClientConn::getFd() const { return sock_; }

void ClientConn::handleReadEvent(Event &e) {
  if (handleReceive(e) == result_t::kError) {
    server.removeClientConn(this_position_);
  }
  ParserResult::e result;
  while ((result = parse()) == ParserResult::kSuccess) {
    processMessage(getMessage());
  }
  if (result == ParserResult::kFailure) {
    server.removeClientConn(this_position_);
  }
  if (state_ == ConnState::kComplate) {
    registerClient(e);
    state_ = ConnState::kConnected;
  }
}

result_t::e ClientConn::handleReceive(Event &e) {
  ssize_t len = e.data;

  if (e.data != 0)
    len = recvBuffer(e.data);

  if (len <= 0 || e.flags.test(EventFlag::kEOF))
    return result_t::kError;

  return result_t::kOK;
}

void ClientConn::send(const std::string &str) {
  send_queue_.push(StringBuffer(str));
  server.getPool().addEvent(EventKind::kWrite, this);
}

void ClientConn::send(const Message &msg) {
  std::string str = ":" + msg.prefix;

  str += " ";
  str += msg.command;
  if (not msg.prefix.empty()) {
    for (std::vector<util::LazyString>::const_iterator
             it = msg.params.begin(),
             end = util::prev(msg.params.end());
         it != end; ++it) {
      str += " " + *it;
    }
    str += " :" + *msg.params.rbegin();
  }
  str += "\r\n";
  send(str);
}

void ClientConn::registerClient(const Event &e) {
  Message reply;
  reply.prefix = config.name;

  char __buf[4]; __buf[3] = '\0'; sprintf(__buf, "%03d", util::RPL_WELCOME);
  reply.command = util::to_string(__buf);
  reply.params.push_back(ident_->nickname_);
  reply.params.push_back("Welcome to the Internet Relay Network!");

  const std::string &nick = ident_->nickname_;
  Client *client = new Client(this);

  server.moveClientConn(this_position_);
  server.addClient(nick, client);

  e.pool.removeEvent(EventKind::kRead, this);
  e.pool.addEvent(EventKind::kRead, client);

  std::cout << "Register" << std::endl;

  send(reply);
}

int ClientConn::handle(Event e) {
  switch (e.kind) {
    case EventKind::kRead:
      handleReadEvent(e);
      break;

    case EventKind::kWrite:
      handleWriteEvent(e);
      break;
    default:
      abort(/* XXX Unknown event*/);
      break;
  }
  return 0;
}

int ClientConn::close() { return ::close(sock_); }

ssize_t ClientConn::recvBuffer(size_t length) {
  ssize_t ret = util::recv(sock_, recv_buffer_.begin(), length);
  write(1, recv_buffer_.data(), ret);
  if (ret == -1)
    return ret;
  recv_buffer_.seekg(0);
  recv_buffer_.seekp(ret);
  return ret;
}

ParserResult::e ClientConn::parse() { return parser_.parse(recv_buffer_); }

Message ClientConn::getMessage() { 
  Message msg = parser_.getMessage();
  parser_.clear();
  return msg;
}

UserIdent *ClientConn::moveIdent() { return util::moveptr(ident_); }
