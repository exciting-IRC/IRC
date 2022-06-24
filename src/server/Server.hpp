#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <map>

#include "client/Client.hpp"
#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/config/config.hpp"
#include "util/general/result.hpp"
class Server;
class Client;
class ClientConn;

typedef std::list<Client *> ClientList;
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
  Channel(const std::string &name);

  template <typename T>
  void sendAll(const T &msg, Client *exclude) {
    for (ClientMap::iterator it = users_.begin(), end = users_.end(); it != end;
         ++it) {
      if (it->second != exclude)
        it->second->send(msg);
    }
  }

  void addUser(Client *client);

  void removeUser(Client *client);

  void changeUserName(const std::string &oldnick, const std::string &newnick);

  ClientMap getUsers();

 private:
  ClientMap users_;
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
  typedef std::map<std::string, Channel *> ChannelMap;

 public:
  result_t::e init(int backlog);

  int getFd() const;  // override, final

  result_t::e handle(Event e);  // override, final

  void handleError();

  void eraseFromClientList(ClientList::iterator pos);
  void eraseFromClientMap(const std::string &nickname);
  void removeClient(ClientList::iterator pos);
  void removeClient(const std::string &nickname);

  void addClient(const std::string &nick, Client *client);

  Channel *addUserToChannel(const std::string &channel_name, Client *user);

  void removeChannel(const std::string &channel_name);

  ClientMap &getClients();

  EventPool &getPool();

  ChannelMap &getChannels();

 public:
  util::Config config_;

 private:
  EventPool pool_;
  ClientList unregistered_clients_;
  ClientMap clients_;
  ChannelMap channels_;
  int sock_;
};

extern Server server;

#endif
