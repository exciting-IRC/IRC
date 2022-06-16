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
#include "util/strutil/strutil.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/irctype/irctype.hpp"

ClientConn::ClientConn(int sock, Server &server, CCList::iterator this_position)
    : sock_(sock), server_(server), this_position_(this_position) {}

ClientConn::~ClientConn() {
  if (sock_ != -1)
    close();
}

int ClientConn::getFd() const { return sock_; }

void ClientConn::handleReadEvent(Event &e) {
  if (e.data != 0) {
    ssize_t len = recv(e.data);
    if (len > 0) {
      ParserResult::e result = parser_.parse(buffer_);
      if (result == ParserResult::kFailure) {
        parser_.clear();
        std::cout << "Failed to parse message" << std::endl;
        // XXX log
      } else if (result == ParserResult::kSuccess) {
        parser_.clear();
        std::cout << "Parse Success" << std::endl;
      }
    }
  }
  if ((e.flags.test(EventFlag::kEOF)) || e.data == 0) {
    std::cout << "Connection closed." << std::endl;
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
