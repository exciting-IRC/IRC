#include "ClientConn.hpp"

#include <err.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "Server.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/strutil.hpp"

const MPMap ClientConn::map_ = ClientConn::getMPMap();

ClientConn::ClientConn(int sock, Server &server, CCList::iterator this_position)
    : sock_(sock), server_(server), this_position_(this_position), flag_(0) {}

ClientConn::~ClientConn() {
  if (sock_ != -1)
    close();
}

const MPMap ClientConn::getMPMap() {
  MPMap map;

  map.insert(MPMap::value_type("PASS", &ClientConn::processPass));
  map.insert(MPMap::value_type("USER", &ClientConn::processUser));
  map.insert(MPMap::value_type("NICK", &ClientConn::processNick));

  return map;
}

void ClientConn::processNick(const Message &m) {
  info_.nickname_ = m.params[0];
  std::cout << "NICK" << std::endl;
  ++flag_;
}

void ClientConn::processUser(const Message &m) {
  info_.user_ = m.params[0];

  // XXX 매우매우매우 위험함
  int k = atoi(m.params[1].c_str());
  info_.mode_ = (k & UserMode::w) | (k & UserMode::i);
  info_.realname_ = m.params[3];
  std::cout << "USER" << std::endl;
  ++flag_;
}

void ClientConn::processPass(const Message &m) {
  info_.password_ = m.params[0];
  std::cout << "PASS" << std::endl;
  ++flag_;
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
        COUT_FMT("Parse Success", ());
      }
      if (result == ParserResult::kFailure) {
        parser_.clear();
        COUT_FMT("Failed to parse message", ());
      }
      if (flag_ == 3) {
        COUT_FMT("sending welcome message", ());
        util::send(
            sock_,
            FMT(":eircd 001 :Welcome to the -Exciting- IRC [{nickname}]\r\n"
                ":eircd 002 :Your host is eircd-test version 0.0.2\r\n"
                ":eircd 003 :This server was created 0\r\n"
                ":eircd 004 eircd 0.0.2 ai :\r\n",
                (info_.nickname_)));
        flag_ = 0;
      }
    }
  }
  if ((e.flags.test(EventFlag::kEOF)) || e.data == 0) {
    COUT_FMT("Connection closed.", ());
    server_.removeClient(this_position_);
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
