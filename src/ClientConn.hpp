#ifndef CLIENT_CONN_HPP
#define CLIENT_CONN_HPP

#include <list>

#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "IRCParser.hpp"

class Server;
class ClientConn;

typedef std::list<ClientConn *> CCList;

class ClientConn : public IEventHandler {
 public:
  ClientConn(int sock, Server &server, CCList::iterator this_position);

  virtual ~ClientConn();

 private:
  ClientConn(const ClientConn &);             // = delete
  ClientConn &operator=(const ClientConn &);  // = delete

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
  CCList::iterator this_position_;
};

#endif  // CLIENT_CONN_HPP
