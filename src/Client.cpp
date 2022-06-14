#include "Client.hpp"

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

Client::Client(int sock, Server &server, ClientList::iterator this_position)
    : sock_(sock), server_(server), this_position_(this_position) {}

Client::~Client() {
  if (sock_ != -1)
    close();
}

int Client::getFd() const { return sock_; }

void Client::handleReadEvent(Event &e) {
  if (e.data != 0) {
    std::cout << "-recv: " << e.data << std::endl;
    ssize_t len = recv(e.data);
    if (len > 0) {
      while (!buffer.eof()) {
        std::cout << buffer.pop();
      }
      std::cout << std::endl;
    }
  }
  if ((e.flags & EventFlags::kEOF) || e.data == 0) {
    std::cout << "Connection closed." << std::endl;
    server_.removeClient(this_position_);
  }
}

int Client::handle(Event e) {
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

int Client::close() { return ::close(sock_); }

ssize_t Client::recv(size_t length) {
  ssize_t ret = util::recv(sock_, buffer.begin(), length);
  if (ret == -1)
    return ret;
  buffer.seekg(0);
  buffer.seekp(ret);
  return ret;
}
