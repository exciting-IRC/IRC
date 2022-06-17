#ifndef CLIENT_CONN_HPP
#define CLIENT_CONN_HPP

#include <list>
#include <map>

#include "IRCParser.hpp"
#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"

class Server;
class ClientConn;

typedef std::list<ClientConn *> CCList;
typedef std::map<std::string, void (ClientConn::*)(const Message &)> MPMap;

struct UserMode {
  enum e {
    clear = 0,
    a = (1 << 0),
    r = (1 << 1),
    w = (1 << 2),
    i = (1 << 3),
    o = (1 << 4),
    O = (1 << 5),
    s = (1 << 6)
  };
};

struct UserInfo {
  std::string password_;
  std::string user_;
  std::string realname_;
  std::string nickname_;
  unsigned int mode_;
};

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
  static const MPMap getMPMap();
  void handleReadEvent(Event &e);
  void processMessage(const Message &m);
  void processNick(const Message &m);
  void processUser(const Message &m);
  void processPass(const Message &m);

 private:
  static const MPMap map_;
  util::Buffer buffer_;
  IRCParser parser_;
  int sock_;
  Server &server_;
  CCList::iterator this_position_;
  UserInfo info_;
  int flag_;
};

#endif  // CLIENT_CONN_HPP
