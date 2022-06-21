#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <algorithm>
#include <string>
#include <map>

#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/vargs/container_of.hpp"
#include "util/strutil/strutil.hpp"
#include "client/ClientConn.hpp"

class Channel;


#define VL(param) container_of<std::vector<util::LazyString> > param

/**
 *a - user is flagged as away;
 *i - marks a users as invisible;
 *w - user receives wallops;
 *r - restricted user connection;
 *o - operator flag;
 *O - local operator flag;
 *s - marks a user for receipt of server notices. (deprecated)
 */

class ClientConn;
struct UserIdent;
struct Message;
class Client;
typedef std::map<std::string, void (Client::*)(const Message &)> MPClientMap;


class Client : public IEventHandler {
 public:
  Client(ClientConn *conn);

  virtual ~Client();

 private:
  Client(const Client &);             // = delete;
  Client &operator=(const Client &);  // = delete;

 
 public:
  int getFd() const;

  template <typename T>
  void send(const T &msg);

  std::string &getNick();

  void handleWriteEvent(Event &e);

  void handleReadEvent(Event &e);

  int handle(Event e);

  void ping(const Message &m);

  void oper(const Message &m);

  void kill(const Message &m);

  void quit(const Message &m);

  void join(const Message &m);

  void privmsg(const Message &m);

  void processMessage(const Message &m);

 private:
  std::map<std::string, Channel *> joined_channels_;
  ClientConn *conn_;
  UserIdent *ident_;
  static const MPClientMap map_;
};

#include "client/Client.tpp"

#endif  // CLIENT_CLIENT_HPP
