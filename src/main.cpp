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

#include "ClientConn.hpp"
#include "Server.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/general/functor.hpp"
#include "util/vargs/container_of.hpp"

const static char* bind_addr = "127.0.0.1";
const static int port = 6667;

static sig_atomic_t recived_sig;

void server_close_handler(int sig) { recived_sig = sig; }

int main() {
  using namespace util;
  using namespace std;

  signal(SIGINT, server_close_handler);
  signal(SIGTERM, server_close_handler);
  signal(SIGQUIT, server_close_handler);

  FUNCTOR(int, exitwith, (const string& msg), {
    std::cout << msg << std::endl;
    return 0;
  });

  Config config = Config::from("config.yml");
  // cout << config << endl;

  Server server(bind_addr, port, 64);
  if (not server.ok())
    err(1, "server_init");

  std::cout << "Listen at " << bind_addr << ":" << port << std::endl;

  EventPool pool(64);
  if (not pool.ok())
    err(1, "event_pool_init");

  pool.addEvent(EventKind::kRead, &server);

  while (true) {
    pool.dispatchEvent(1);
    if (recived_sig) {
      exitwith(FMT("shutdown... {}", (strsignal(recived_sig))));
      return 0;
    }
  }
  return 0;
}
