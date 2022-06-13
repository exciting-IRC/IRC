#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <list>

#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"

class Server;
class Client;

typedef std::list<Client *> ClientList;
typedef util::FixedBuffer<char, 512> Buffer;

class Client : public EventHandler {
 public:
  Client(int sock, Server &server, ClientList::iterator this_position);

  ~Client();

 private:
  Client(const Client &);             // = delete
  Client &operator=(const Client &);  // = delete

 public:
  int getFd() const;

  int handle(Event e);

  int close();

  ssize_t recv(size_t length);

 private:
  void Client::handleReadEvent(Event &e);

 private:
  Buffer buffer;
  int sock_;
  Server &server_;
  ClientList::iterator this_position_;
};

#endif  // CLIENT_HPP