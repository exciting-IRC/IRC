#include <err.h>
#include <signal.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <util/FixedBuffer/FixedBuffer.hpp>
#include <util/config/config.hpp>
#include <util/strutil/strutil.hpp>
#include <utility>
#include <vector>

#include "Client.hpp"
#include "Server.hpp"
#include "command/returnformat.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/general/result.hpp"

using namespace std;
struct User {
  string username;
  string nickname;
};

using namespace util;

typedef string (*command)(User& user, const vector<string>& args);
typedef pair<string, command> command_pair;
typedef map<string, command> command_map;

string commandNick(Server& server, User& user, const vector<string>& args) {
  (void)server;
  if (args.empty())
    return retfmt[ERR_NONICKNAMEGIVEN];

  string new_nickname = args[0];

  // TODO: do nothing and return
  // if (user.nickname == new_nickname)
  //   return;  // do nothing

  if (new_nickname.length() > 9)
    return retfmt[ERR_ERRONEUSNICKNAME];

  /* if nickname in use */
  return retfmt[ERR_NICKNAMEINUSE];

  user.nickname = new_nickname;
}

struct Command {
  // Server& server;
  User& user;
  vector<string> args;

  void reply(const string& fmt, const vector<string>& args) {
    (void)fmt;
    (void)args;
    // TODO: send to server
  }
};

int main() {
  cout << to_string("Hello, world!") << endl;
  cout << FMT(retfmt[RPL_WELCOME], ("scarf", "scarf", "localhost")) << endl;
  cout << FMT(retfmt[RPL_YOURHOST], ("localhost", "0.0.1")) << endl;
  cout << FMT(retfmt[RPL_CREATED], ("2020-01-01")) << endl;
  cout << FMT(retfmt[RPL_MYINFO], ("localhost", "0.0.1", "", "")) << endl;
  // container_of<command_map>(p("PASS", commandNick));
}

/*
const static char *bind_addr = "127.0.0.1";
const static int port = 7778;

static sig_atomic_t recived_sig;

void server_close_handler(int sig) { recived_sig = sig; }

int main() {
  using namespace util;
  using namespace std;

  signal(SIGINT, server_close_handler);
  signal(SIGTERM, server_close_handler);
  signal(SIGQUIT, server_close_handler);

  Config config = Config::from("config.yml");
  cout << config << endl;

  Server srv(bind_addr, port, 64);
  if (!srv.ok())
    err(1, "server_init");

  std::cout << "Listen at " << bind_addr << ":" << port << std::endl;

  EventPool pool(64);
  if (!pool.ok())
    err(1, "event_pool_init");

  pool.addEvent(EventKind::kRead, &srv);

  while (true) {
    int k = pool.dispatchEvent(1);
    (void)k;
    if (recived_sig) {
      printf("shutdown... %s\n", strsignal(recived_sig));
      return 0;
    }
  }
  return 0;
}
*/
