#include "Server.hpp"

#include <err.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <list>

#include "Client.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"

Server::Server(const char *listen_addr, int port, int backlog) : ok_(false) {
  sock_ = util::socket(PF_INET, SOCK_STREAM);
  if (sock_ == -1) {
    return;
  }

  int option = true;
  if (util::setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &option,
                       sizeof(option)) == -1) {
    return;
  }

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));
  in_addr_t in_addr = inet_addr(listen_addr);
  if (in_addr == INADDR_NONE)
    return;
  addr.sin_addr.s_addr = in_addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (util::bind_in(sock_, &addr) == -1)
    return;

  if (util::listen(sock_, backlog) == -1)
    return;
  ok_ = true;
}

Server::~Server() {
  for (ClientList::iterator it = client_list_.begin(); it != client_list_.end();
       ++it) {
    delete *it;
  }
  close(sock_);
}

bool Server::ok() { return ok_; }

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
    ClientList::iterator entry = client_list_.insert(client_list_.end(), NULL);
    *entry = new Client(client_socket, *this, entry);
    e.ep->addEvent(EventKind::kRead, *entry);
  }
  return 0;
}

void Server::removeClient(ClientList::iterator pos) {
  delete *pos;
  client_list_.erase(pos);
}
