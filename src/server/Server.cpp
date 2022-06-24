#include "Server.hpp"

#include <err.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <list>

#include "client/Client.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/color.hpp"
#include "util/general/logging.hpp"
#include "util/strutil/format.hpp"

Server::Server() : config_(), sock_(-1) {}

Server::~Server() {
  for (ClientList::iterator it = unregistered_clients_.begin(),
                            end = unregistered_clients_.end();
       it != end; ++it) {
    delete *it;
  }
  for (ClientMap::iterator it = clients_.begin(), end = clients_.end();
       it != end; ++it) {
    delete it->second;
  }
  for (ChannelMap::iterator it = channels_.begin(), end = channels_.end();
       it != end; ++it) {
    delete it->second;
  }
  close(sock_);
}

result_t::e Server::init(int backlog) {
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
  in_addr_t in_addr = inet_addr(config_.address.c_str());
  if (in_addr == INADDR_NONE)
    return result_t::kError;

  addr.sin_addr.s_addr = in_addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(config_.port);

  if (util::bind_in(sock_, &addr) == result_t::kError)
    return result_t::kError;

  if (util::listen(sock_, backlog) == result_t::kError)
    return result_t::kError;

  if (pool_.init(backlog) == result_t::kError)
    return result_t::kError;

  if (getPool().addEvent(EventKind::kRead, this) == result_t::kError)
    return result_t::kError;

  return result_t::kOK;
}

int Server::getFd() const { return sock_; }

result_t::e Server::handle(Event e) {
  if (e.kind == EventKind::kRead) {
    struct sockaddr_in sin;
    socklen_t length = sizeof(sin);
    int client_socket =
        util::accept(sock_, reinterpret_cast<struct sockaddr *>(&sin), &length);
    if (client_socket == -1)
      return result_t::kError;

    util::debug_info(
        "connection accpepted at",
        addr2ascii(AF_INET, &sin.sin_addr, sizeof(sin.sin_addr), NULL));

    ClientList::iterator entry =
        unregistered_clients_.insert(unregistered_clients_.end(), NULL);

    *entry = new Client(client_socket, entry);
    return result_t::kOK;
  } else {
    return result_t::kError;  // unknown event
  }
}

void Server::handleError() {}

void Server::eraseFromClientList(ClientList::iterator pos) {
  unregistered_clients_.erase(pos);
}

void Server::removeClient(ClientList::iterator pos) {
  delete *pos;
  unregistered_clients_.erase(pos);
}

void Server::eraseFromClientMap(const std::string &nickname) {
  ClientMap::iterator target = clients_.find(nickname);
  clients_.erase(target);
}

void Server::removeClient(const std::string &nickname) {
  ClientMap::iterator target = clients_.find(nickname);
  delete target->second;
  clients_.erase(target);
}

void Server::addClient(const std::string &nick, Client *client) {
  clients_.insert(ClientMap::value_type(nick, client));
}

Channel *Server::addUserToChannel(const std::string &channel_name,
                                  Client *user) {
  ChannelMap::iterator channel_iter = channels_.find(channel_name);

  if (channel_iter == channels_.end()) {
    channel_iter = channels_
                       .insert(ChannelMap::value_type(
                           channel_name, new Channel(channel_name)))
                       .first;
  }
  Channel *&ch = channel_iter->second;

  ch->addUser(user);
  return ch;
}

void Server::removeChannel(const std::string &channel_name) {
  channels_.erase(channel_name);
}

ClientMap &Server::getClients() { return clients_; }

Server::ChannelMap &Server::getChannels() { return channels_; }

EventPool &Server::getPool() { return pool_; }

Channel::Channel(const std::string &name) : name_(name) {}

void Channel::addUser(Client *client) {
  users_[client->getIdent().nickname_] = client;
  sendAll(Message::as_reply(client->getIdent().toString(), "JOIN", VA((name_))),
          NULL);
}

void Channel::removeUser(Client *client) {
  sendAll(Message::as_reply(client->getIdent().toString(), "PART", VA((name_))),
          NULL);
  users_.erase(client->getIdent().nickname_);
}

ClientMap Channel::getUsers() { return users_; }
