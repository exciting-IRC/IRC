#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <map>

#include "client/Client.hpp"
#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/general/result.hpp"
class Server;
class Client;
class ClientConn;

typedef std::list<ClientConn *> CCList;
typedef std::map<std::string, Client *> ClientMap;

/**
 * p - private channel flag;
 * s - secret channel flag;
 * i - invite-only channel flag;
 * t - topic settable by channel operator only flag;
 * n - no messages to channel from clients on the outside;
 * m - moderated channel;
 *
 */
struct ChannelMode {
  enum e { p, s, i, t, n, m };
};

#include <algorithm>
#include <functional>
#include <utility>

class Channel {
 public:
  Channel() {}
  Channel(const std::string &name, Client *client) : name_(name) {
    users_.insert(std::make_pair(name, client));
  }

  template <typename T>
  void sendAll(const T &msg, Client *exclude) {
    for (ClientMap::iterator it = users_.begin(), end = users_.end(); it != end;
         ++it) {
      if (it->second != exclude)
        it->second->send(msg);
    }
  }

  void addUser(const std::string &name, Client *client) {
    users_[name] = client;
  }

  void removeUser(const std::string &name) { users_.erase(name); }

 private:
  std::map<std::string, Client *> users_;
  std::string name_;
};

class Server : public IEventHandler {
 public:
  Server();
  ~Server();

 private:
  Server(const Server &);             // = delete
  Server &operator=(const Server &);  // = delete

 private:
  typedef std::map<std::string, Channel> ChannelMap;

 public:
  result_t::e init(const char *listen_addr, int port, int backlog);

  int getFd() const;  // override, final

  int handle(Event e);  // override, final

  void moveClientConn(CCList::iterator pos);
  void removeClient(CCList::iterator pos);
  void removeClient(const std::string &nickname);

  void addClient(const std::string &nick, Client *client);

  Channel &addUserToChannel(const std::string &channel_name, Client *user);

  void removeChannel(const std::string &channel_name) {
    channels_.erase(channel_name);
  }

  const ClientMap &getClients();

  EventPool &getPool();

 private:
  EventPool pool_;
  CCList client_conn_;
  ClientMap clients_;
  ChannelMap channels_;
  int sock_;
};

extern Server server;

#endif
