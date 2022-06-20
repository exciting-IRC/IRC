#include <err.h>
#include <signal.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ClientConn.hpp"
#include "Server.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
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

  Config config = Config::from("config.yml");
  cout << config << endl;

  Server srv(bind_addr, port, 64);
  if (!srv.ok())
    err(1, "server_init");

  COUT_FMT("Listen at {bind_addr}:{port}", (bind_addr, port));

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
