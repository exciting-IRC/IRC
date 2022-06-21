#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <algorithm>
#include <map>
#include <string>

#include "client/ClientConn.hpp"
#include "command/returncode.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/config/config.hpp"
#include "util/strutil/strutil.hpp"
#include "util/vargs/container_of.hpp"

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

  void sendRegisterMessage() {
    Message reply;
    reply.prefix = config.name;

    reply.command = util::pad_num(util::RPL_WELCOME);
    reply.params.push_back(ident_->nickname_);
    reply.params.push_back("Welcome to the Internet Relay Network!");
    send(reply);
    reply.params.clear();

    reply.command = util::pad_num(util::RPL_YOURHOST);
    reply.params.push_back(ident_->nickname_);
    reply.params.push_back(
        FMT("Your host is {servername}, running version {version}",
            (config.name, EIRC_VERSION)));
    send(reply);
    reply.params.clear();

    reply.command = util::pad_num(util::RPL_CREATED);
    reply.params.push_back(ident_->nickname_);
    reply.params.push_back(
        FMT("This server was crated {datetime}", (config.create_time)));
    send(reply);
    reply.params.clear();

    reply.command = util::pad_num(util::RPL_MYINFO);
    reply.params.push_back(ident_->nickname_);
    reply.params.push_back(config.name);
    reply.params.push_back(EIRC_VERSION);
    reply.params.push_back("o");
    reply.params.push_back("o");
    reply.params.push_back("");
    send(reply);
    reply.params.clear();
  }

  std::string &getNick();

  void handleWriteEvent(Event &e);

  void handleReadEvent(Event &e);

  int handle(Event e);

  void ping(const Message &m);

  void oper(const Message &m);

  void kill(const Message &m);

  void quit(const Message &m);

  void join(const Message &m);

  void userMode(const Message &m);

  void channelMode(const Message &m);

  void mode(const Message &m);

  void sendNeedMoreParam(const std::string &command);

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
