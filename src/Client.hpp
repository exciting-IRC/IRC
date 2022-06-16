#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <list>

#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "IRCParser.hpp"

class Server;
class Client;

typedef std::list<Client *> ClientList;

class Client : public IEventHandler {
 public:
  Client(int sock, Server &server, ClientList::iterator this_position);

  virtual ~Client();

 private:
  Client(const Client &);             // = delete
  Client &operator=(const Client &);  // = delete

 public:
  int getFd() const;

  int handle(Event e);

  int close();

  ssize_t recv(size_t length);

 private:
  void handleReadEvent(Event &e);

 private:
  util::Buffer buffer_;
  IRCParser parser_;
  int sock_;
  Server &server_;
  ClientList::iterator this_position_;
};

#endif  // CLIENT_HPP
