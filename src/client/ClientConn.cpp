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
#include "util/general/logging.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/conversion.hpp"
#include "util/strutil/strutil.hpp"

using util::p;

const MPMap ClientConn::map_ = container_of<MPMap, MPMap::value_type>(
    p("PASS", &ClientConn::processPass), p("USER", &ClientConn::processUser),
    p("NICK", &ClientConn::processNick));

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
  if (!(state_ & ConnState::kPass)) {
    sendError("PASS needed before NICK command.");
    return;
  }
  if (m.params.empty()) {
    send(Message::as_numeric_reply(util::ERR_NONICKNAMEGIVEN,
                                   VA(("No nickname given"))));
    return;
  }

  const std::string &nick = m.params[0];
  if (!isValidNick(nick)) {
    send(Message::as_numeric_reply(util::ERR_ERRONEUSNICKNAME,
                                   VA((nick, "Erroneous nickname"))));
    return;
  }

  if (server.getClients().find(nick) != server.getClients().end()) {
    send(Message::as_numeric_reply(util::ERR_NICKNAMEINUSE,
                                   VA((nick, "Nickname is already in use"))));
    return;
  }
  ident_->nickname_ = nick;
  state_ |= ConnState::kNick;
}

bool isValidUser(const std::string &s) {
  return s.find("@") != std::string::npos;
}

void ClientConn::processUser(const Message &m) {
  if (!(state_ & ConnState::kPass)) {
    sendError("PASS needed before USER command.");
    return;
  }

  if (m.params.size() != 4) {
    send(Message::as_not_enough_params_reply(m.command));
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
  if (m.params.size() != 1) {
    send(Message::as_not_enough_params_reply(m.command));
    return;
  }

  ident_->password_ = m.params[0];
  state_ |= ConnState::kPass;
}

void ClientConn::processMessage(const Message &m) {
  MPMap::const_iterator it = map_.find(util::to_upper(m.command));
  const bool found = it != map_.end();
  debug_input(m.command, found);
  if (found)
    (this->*(it->second))(m);
}

int ClientConn::getFd() const { return sock_; }

void ClientConn::handleReadEvent(Event &e) {
  if (handleReceive(e) == result_t::kError) {
    server.removeClient(this_position_);
  }
  ParserResult::e result;
  while ((result = parse()) == ParserResult::kSuccess) {
    processMessage(getMessage());
  }
  if (result == ParserResult::kFailure) {
    server.removeClient(this_position_);
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
  util::debug_output(str);
  send_queue_.push(StringBuffer(str + "\r\n"));
  server.getPool().addEvent(EventKind::kWrite, this);
}

void ClientConn::send(const Message &msg) {
  typedef std::vector<util::LazyString>::const_iterator const_it;

  std::string str;

  if (msg.prefix != "") {
    str += ":" + msg.prefix + " ";
  }

  str += msg.command;

  if (not msg.params.empty()) {
    for (const_it it = msg.params.begin(), end = util::prev(msg.params.end());
         it != end; ++it) {
      str += " " + *it;
    }
    str += " :" + *msg.params.rbegin();
  } else {
    str += " :";
  }
  send(str);
}

void ClientConn::registerClient(const Event &e) {
  const std::string &nick = ident_->nickname_;
  Client *client = new Client(this);

  server.moveClientConn(this_position_);
  server.addClient(nick, client);

  e.pool.removeEvent(EventKind::kRead, this);
  e.pool.addEvent(EventKind::kRead, client);

  client->sendRegisterMessage();
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

void ClientConn::sendNotice(const std::string &msg) {
  send(FMT(":{servername} NOTICE {nickname} :{message}",
           (server.config_.name, ident_->nickname_, msg)));
}

void ClientConn::sendError(const std::string &msg) {
  send(FMT("ERROR :{message}", (msg)));
}
