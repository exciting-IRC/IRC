#ifndef CLIENT_CONN_HPP
#define CLIENT_CONN_HPP

#include <list>
#include <map>

#include "IRCParser.hpp"
#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/moveptr.hpp"

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

struct UserIdent {
  std::string password_;
  std::string user_;
  std::string realname_;
  std::string nickname_;
  unsigned int mode_;
};

struct ConnState {
  enum e {
    kClear = 0,
    kPass = (1 << 0),
    kUser = (1 << 1),
    kNick = (1 << 3),
    kComplate = (kPass | kUser | kNick)
  };
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

  ParserResult::e parse();

  const Message &getMessage();

  UserIdent *moveIdent() {
    return util::moveptr(ident_);
  }

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
  CCList::iterator this_position_;
  UserIdent *ident_;
  unsigned char state_;
};

#endif  // CLIENT_CONN_HPP
