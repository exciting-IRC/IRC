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

#include "client/ClientConn.hpp"
#include "event/event.hpp"
#include "server/Server.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/color.hpp"
#include "util/general/logging.hpp"
#include "util/vargs/container_of.hpp"

const static char *bind_addr = "0.0.0.0";

static sig_atomic_t recived_sig;

void server_close_handler(int sig) { recived_sig = sig; }

Server server;

int main(const int argc, const char *argv[]) {
  using namespace util;
  using namespace std;

  signal(SIGINT, server_close_handler);
  signal(SIGTERM, server_close_handler);

  util::Config config = util::Config::from("config.yml");
  if (argc != 3) {
    debug_info("warning:", "using defaults", false);
    config.port = 6667;
    config.password = "1234";
  } else {
    config.port = convert_to<uint16_t>(argv[1]);
    config.password = argv[2];
  }

  server.config_ = config;
  cout << config << endl;
  debug_info("create time:", config.create_time);
  debug_info("name:", config.name);

  debug_info("bind_addr:", bind_addr);
  if (server.init(bind_addr, config.port, 64) == result_t::kError)
    err(1, "Server init");

  debug_info("Listen at", FMT("{bind_addr}:{port}", (bind_addr, config.port)));

  server.getPool().addEvent(EventKind::kRead, &server);

  try {
    while (true) {
      int k = server.getPool().dispatchEvent(1);
      (void)k;
      if (recived_sig) {
        debug(FMT("{red}shutdown... {}", (BHRED, strsignal(recived_sig))));
        return 0;
      }
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
