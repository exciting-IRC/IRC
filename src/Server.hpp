#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>

#include "event/event.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"

class Server;
class Client;

typedef std::list<Client *> ClientList;
typedef util::FixedBuffer<char, 512> Buffer;

class Server : public IEventHandler {
 public:
  Server(const char *listen_addr, int port, int backlog);
  ~Server();

 private:
  Server(const Server &);             // = delete
  Server &operator=(const Server &);  // = delete

 public:
  bool ok();

  int getFd() const;  // override, final

  int handle(Event e);  // override, final

  void removeClient(ClientList::iterator pos);

 private:
  ClientList client_list_;
  int sock_;
  bool ok_;
};

#endif
