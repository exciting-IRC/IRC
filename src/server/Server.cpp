#include "Server.hpp"

#include <err.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <list>

#include "client/ClientConn.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"

Server::Server() : sock_(-1) {}

Server::~Server() {
  for (CCList::iterator it = client_conn_.begin(); it != client_conn_.end();
       ++it) {
    delete *it;
  }
  close(sock_);
}

result_t::e Server::init(const char *listen_addr, int port, int backlog) {
  sock_ = util::socket(PF_INET, SOCK_STREAM);
  if (sock_ == -1) {
    return result_t::kError;
  }

  int option = true;
  if (util::setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &option,
                       sizeof(option)) == -1) {
    return result_t::kError;
  }

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));
  in_addr_t in_addr = inet_addr(listen_addr);
  if (in_addr == INADDR_NONE)
    return;
  addr.sin_addr.s_addr = in_addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (util::bind_in(sock_, &addr) == result_t::kError)
    return result_t::kError;

  if (util::listen(sock_, backlog) == result_t::kError)
    return result_t::kError;
  
  return result_t::kOK;
}

int Server::getFd() const { return sock_; }

int Server::handle(Event e) {
  if (e.kind == EventKind::kRead) {
    struct sockaddr_in sin;
    socklen_t length = sizeof(sin);
    int client_socket =
        util::accept(sock_, reinterpret_cast<struct sockaddr *>(&sin), &length);
    if (client_socket == -1)
      err(1, "server handler");
    std::cout << "Accept connection: "
              << addr2ascii(AF_INET, &sin.sin_addr, sizeof(sin.sin_addr), NULL)
              << std::endl;
    CCList::iterator entry = client_conn_.insert(client_conn_.end(), NULL);
    *entry = new ClientConn(client_socket, *this, entry);
    e.pool.addEvent(EventKind::kRead, *entry);
  }
  return 0;
}

void Server::moveClientConn(CCList::iterator pos) { client_conn_.erase(pos); }

void Server::removeClientConn(CCList::iterator pos) {
  delete *pos;
  client_conn_.erase(pos);
}

void Server::addClient(const std::string &nick, Client *client) {
  clients_.insert(ClientMap::value_type(nick, client));
}
