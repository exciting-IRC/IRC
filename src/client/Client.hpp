#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <string>

#include "client/IRCParser.hpp"
#include "event/event.hpp"
#include "returncode.hpp"
#include "socket/socket.hpp"
#include "util/StringBuffer.hpp"
#include "util/config/config.hpp"
#include "util/strutil/strutil.hpp"
#include "util/vargs/container_of.hpp"
class Channel;

#define VL(param) container_of<std::vector<util::LazyString> > param

struct Message;
class Client;

typedef std::list<Client *> ClientList;

struct UserMode {
  enum e {
    clear = 0,
    a = (1 << 0),  ///< user is flagged as away;
    r = (1 << 1),  ///< marks a users as invisible;
    w = (1 << 2),  ///< user receives wallops;
    i = (1 << 3),  ///< restricted user connection;
    o = (1 << 4),  ///< operator flag;
    O = (1 << 5),  ///< local operator flag;
    s = (1 << 6)   ///< marks a user for receipt of server notices. (deprecated)
  };
};

struct UserIdent {
  std::string toString() const {
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
    kRegistered = (kPass | kUser | kNick)
  };
};

class Client : public IEventHandler {
 public:
  Client(int sock, ClientList::iterator pos);

  virtual ~Client();

 private:
  Client(const Client &);             // = delete;
  Client &operator=(const Client &);  // = delete;

 private:
  typedef std::map<std::string, result_t::e (Client::*)(const Message &)>
      CmdMap;
  typedef std::map<std::string, Channel *> ChannelMap;

 public:
  int getFd() const;

  void send(const std::string &str);

  template <typename Cont>
  void sendList(const std::string &list_base, const Cont &list,
                const std::string &list_end);

  void sendRegisterMessage();

  void sendMOTD();

  void sendNotice(const std::string &msg);

  void sendError(const std::string &msg);

  const UserIdent &getIdent();

  result_t::e handleWriteEvent(Event &e);

  result_t::e handleReadEvent(Event &e);

  ssize_t recvToBuffer(size_t length);

  result_t::e handle(Event e);

  void handleError();

  int addEvent(EventKind::e kind);

  int removeEvent(EventKind::e kind);

  result_t::e ping(const Message &m);

  result_t::e oper(const Message &m);

  result_t::e kill(const Message &m);

  result_t::e quit(const Message &m);

  result_t::e join(const Message &m);

  result_t::e part(const Message &m);

  result_t::e mode(const Message &m);

  result_t::e privmsg(const Message &m);

  result_t::e nick(const Message &m);

  result_t::e alredyRegistered(const Message &m);

  result_t::e notRegistered(const Message &m);

  result_t::e registerPass(const Message &m);

  result_t::e registerUser(const Message &m);

  result_t::e registerNick(const Message &m);

  result_t::e handleReceive(Event &e);

  void complateRegister();

  void updateCommandMap();

  result_t::e processMessage(const Message &m);

 private:
  const static CmdMap map_before_register_;
  const static CmdMap map_after_register_;
  const CmdMap *map_;

  IRCParser parser_;
  int sock_;
  ClientList::iterator pos_;
  UserIdent ident_;
  ChannelMap joined_channels_;
  unsigned int event_state_;
  unsigned int conn_state_;
  util::Buffer recv_buffer_;
  std::queue<StringBuffer> send_queue_;
};

#include "Client.tpp"

#endif  // CLIENT_CLIENT_HPP
