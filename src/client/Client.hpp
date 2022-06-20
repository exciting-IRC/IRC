#include <algorithm>
#include <string>

#include "ClientConn.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"

/**
 *a - user is flagged as away;
 *i - marks a users as invisible;
 *w - user receives wallops;
 *r - restricted user connection;
 *o - operator flag;
 *O - local operator flag;
 *s - marks a user for receipt of server notices. (deprecated)
 */

class StringBuffer {
 public:
  StringBuffer();
  StringBuffer(const std::string &str);
  // StringBuffer(const StringBuffer &) = default;
  // StringBuffer &operator=(const StringBuffer &other) = default;
  // ~StringBuffer() = default;

 public:
  void reset();

  void reset(const std::string &str);

  bool empty() const;

  const char *data() const;

  std::size_t size() const;

  void advance(std::size_t len);

 private:
  std::string data_;
  std::size_t cursor_;
};

class Client : public IEventHandler {
 public:
  Client(ClientConn *conn);

  //~Client(); = default;  // ClientConn을 지우면 안됨.

 private:
  Client(const Client &);             // = delete;
  Client &operator=(const Client &);  // = delete;

 public:
  int getFd() const;

  void handleWriteEvent(Event &e);

  void handleReadEvent(Event &e);

  int handle(Event e);

  void setMessageBuffer(const std::string &msg);

  void ping();

 private:
  ClientConn *conn_;
  UserIdent *ident_;
  StringBuffer buffer_;
};
