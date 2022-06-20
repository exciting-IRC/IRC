#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <map>

#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/general/result.hpp"

class Server;
class Client;
class ClientConn;

typedef std::list<ClientConn *> CCList;
typedef std::map<std::string, Client *> ClientMap;

class Server : public IEventHandler {
 public:
  Server();
  ~Server();

 private:
  Server(const Server &);             // = delete
  Server &operator=(const Server &);  // = delete

 public:
  result_t::e init(const char *listen_addr, int port, int backlog);

  int getFd() const;  // override, final

  int handle(Event e);  // override, final

  void moveClientConn(CCList::iterator pos);
  void removeClientConn(CCList::iterator pos);

  void addClient(const std::string &nick, Client *client);

 private:
  CCList client_conn_;
  ClientMap clients_;
  int sock_;
};

extern Server server;

#endif
