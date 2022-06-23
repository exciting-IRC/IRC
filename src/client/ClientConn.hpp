#ifndef CLIENT_CONN_HPP
#define CLIENT_CONN_HPP

#include <arpa/inet.h>

#include <algorithm>
#include <list>
#include <map>
#include <queue>

#include "IRCParser.hpp"
#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/StringBuffer.hpp"
#include "util/algorithm/algorithm.hpp"
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
  std::string toString() {
    return nickname_ + "!" + username_ + "@" + hostname_;
  }
  std::string password_;
  std::string username_;
  std::string realname_;
  std::string nickname_;
  std::string hostname_;
  std::string servername_;
  struct sockaddr_in addr_;
  unsigned int mode_;
};

struct ConnState {
  enum e {
    kClear = 0,
    kPass = (1 << 0),
    kUser = (1 << 1),
    kNick = (1 << 2),
    kComplate = (kPass | kUser | kNick),
    kConnected = (1 << 3)
  };
};

class ClientConn : public IEventHandler {
 public:
  ClientConn(int sock, CCList::iterator this_position);

  virtual ~ClientConn();

 private:
  ClientConn(const ClientConn &);             // = delete
  ClientConn &operator=(const ClientConn &);  // = delete

 public:
  int getFd() const;

  int handle(Event e);

  int close();

  ssize_t recvBuffer(size_t length);

  ParserResult::e parse();

  Message getMessage();

  UserIdent *moveIdent();

  result_t::e handleReceive(Event &e);

  void send(const std::string &str);

  void send(const Message &msg);

  void sendNotice(const std::string &msg);

  void sendError(const std::string &msg);

 private:
  static const MPMap getMPMap();
  result_t::e handleWriteEvent(Event &e);
  void handleReadEvent(Event &e);
  void registerClient(const Event &e);
  void processMessage(const Message &m);
  void processNick(const Message &m);
  void processUser(const Message &m);
  void processPass(const Message &m);

 private:
  static const MPMap map_;
  util::Buffer recv_buffer_;
  std::queue<StringBuffer> send_queue_;
  IRCParser parser_;
  int sock_;
  CCList::iterator this_position_;
  UserIdent *ident_;
  unsigned char state_;
};

#endif  // CLIENT_CONN_HPP
