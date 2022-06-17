
#include <string>

#include "ClientConn.hpp"
#include "event/event.hpp"

/**
 *a - user is flagged as away;
 *i - marks a users as invisible;
 *w - user receives wallops;
 *r - restricted user connection;
 *o - operator flag;
 *O - local operator flag;
 *s - marks a user for receipt of server notices. (deprecated)
 */


class Client : IEventHandler {
 public:
  Client(ClientConn *conn, const std::string &nickname,
         const std::string &password)
      : conn_(conn),
        nickname_(nickname),
        password_(password),
        mode_(UserMode::clear) {}

  //~Client(); = default;  // ClientConn을 지우면 안됨.

 private:
  Client(const Client &);             // = delete;
  Client &operator=(const Client &);  // = delete;

 public:
  int getFd() { return conn_->getFd(); }
  int handle() { conn_ return 0; }

 private:
  ClientConn *const conn_;
  UserInfo info_;
};
