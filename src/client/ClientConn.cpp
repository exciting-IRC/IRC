#include "ClientConn.hpp"

#include <err.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "event/event.hpp"
#include "server/Server.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/strutil.hpp"
#include "client/Client.hpp"

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

void ClientConn::processNick(const Message &m) {
  
  if (args.empty())
    return NUMERIC_REPLY(ERR_NONICKNAMEGIVEN, ());

  string new_nickname = args[0];

  if (user.nickname == new_nickname)
    return "";

  if (not(nick_is_valid(new_nickname)))
    return NUMERIC_REPLY(ERR_ERRONEUSNICKNAME, (new_nickname));

  if (false /* if nickname in use */)
    REPLY(ERR_NICKNAMEINUSE, (new_nickname));

  user.nickname = new_nickname;
  return FMT("NICK {nick}", (new_nickname));
  state_ |= ConnState::kNick;
  
}

void ClientConn::processUser(const Message &m) {
  ident_->user_ = m.params[0];

  // XXX 매우매우매우 위험함
  int k = atoi(m.params[1].c_str());
  ident_->mode_ = (k & UserMode::w) | (k & UserMode::i);
  ident_->realname_ = m.params[3];
  std::cout << "USER" << std::endl;
  state_ |= ConnState::kUser;
}

void ClientConn::processPass(const Message &m) {
  ident_->password_ = m.params[0];
  std::cout << "PASS" << std::endl;
  state_ |= ConnState::kPass;
}

#include <functional>
void ClientConn::processMessage(const Message &m) {
  std::cout << "CMD: <" << m.command << ">" << std::endl;
  MPMap::const_iterator it = map_.find(m.command);
  if (it == map_.end())
    return;
  std::cout << "found!" << std::endl;
  (this->*(it->second))(m);
}

int ClientConn::getFd() const { return sock_; }

void ClientConn::handleReadEvent(Event &e) {
  if (e.data != 0) {
    ssize_t len = recv(e.data);

    if (len > 0) {
      ParserResult::e result;
      while ((result = parser_.parse(buffer_)) == ParserResult::kSuccess) {
        processMessage(parser_.getMessage());
        parser_.clear();
        std::cout << "Parse Success" << std::endl;
      }
      if (result == ParserResult::kFailure) {
        parser_.clear();
        std::cout << "Failed to parse message" << std::endl;
      }
    }
  } else if ((e.flags.test(EventFlag::kEOF)) || e.data == 0) {
    std::cout << "Connection closed." << std::endl;
    server.removeClientConn(this_position_);
  }
  if (state_ == ConnState::kComplate) {
    const std::string &nick = ident_->nickname_;
    Client *client = new Client(this);
    client->setMessageBuffer("aaaaaa");

    server.moveClientConn(this_position_);
    server.addClient(nick, client);

    e.pool.removeEvent(EventKind::kRead, this);
    e.pool.addEvent(EventKind::kRead, client);
    e.pool.addEvent(EventKind::kWrite, client);
    //client->send ...
  }
}

int ClientConn::handle(Event e) {
  switch (e.kind) {
    case EventKind::kRead:
      handleReadEvent(e);
      break;

    default:
      // XXX throw invalid_argument, logging
      break;
  }
  return 0;
}

int ClientConn::close() { return ::close(sock_); }

ssize_t ClientConn::recv(size_t length) {
  ssize_t ret = util::recv(sock_, buffer_.begin(), length);
  if (ret == -1)
    return ret;
  buffer_.seekg(0);
  buffer_.seekp(ret);
  return ret;
}

ParserResult::e ClientConn::parse() { return parser_.parse(buffer_); }

const Message &ClientConn::getMessage() { return parser_.getMessage(); }
