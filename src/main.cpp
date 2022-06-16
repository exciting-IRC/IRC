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
#include "IRCParser.hpp"
#include "Server.hpp"
#include "command/command.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/general/prettyprint.hpp"
#include "util/general/map_get.hpp"
#include "util/vargs/container_of.hpp"
#define VL(param) VEC_OF(util::LazyString, param)

using namespace std;
using namespace util;

void dispatchMessage(const Message& message) {
  try {
    command cmd = map_get(commandsMap, to_upper(message.command));
    User user = {message.prefix, message.prefix, ""};
    cout << "<- " << cmd(user, message.params) << "\n";
  } catch (const exception& e) {
    cerr << "ERR: dispatchMessage: " << e.what() << endl;
  }
}

// typedef string (ICommand::*cmd)();

int main() {
  vector<string> available_commands =
      V(("pass", "nick", "user", "join", "part", "privmsg", "admin", "kill",
         "quit", "afddafs"));

  for (size_t i = 0; i < available_commands.size(); i++) {
    Message m = {"foo", available_commands[i], VL(("asdf"))};
    cout << "-> " << m.command << m.params << endl;
    dispatchMessage(m);
  }
  // cout << to_string("Hello, world!") << endl;
  // cout << FMT(retfmt[RPL_WELCOME], ("scarf", "scarf", "localhost")) << endl;
  // cout << FMT(retfmt[RPL_YOURHOST], ("localhost", "0.0.1")) << endl;
  // cout << FMT(retfmt[RPL_CREATED], ("2020-01-01")) << endl;
  // cout << FMT(retfmt[RPL_MYINFO], ("localhost", "0.0.1", "", "")) << endl;
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
