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

static util::Config init_config(const int argc, const char *argv[]) {
  using namespace util;

  Config config = util::Config::from("config.yml");
  if (argc != 3) {
    debug_info("warning:", "using defaults", false);
    config.port = 6667;
    config.password = "password";
  } else {
    config.port = convert_to<uint16_t>(argv[1]);
    config.password = argv[2];
  }
  return config;
}

static void loop() {
  while (not recived_sig) {
    server.getPool().dispatchEvent(1);
  }
  util::debug_info("shutdown", strsignal(recived_sig));
}

int main(const int argc, const char *argv[]) {
  using namespace util;
  using namespace std;

  signal(SIGINT, server_close_handler);
  signal(SIGTERM, server_close_handler);

  server.config_ = init_config(argc, argv);
  cout << server.config_ << endl;

  if (server.init(bind_addr, server.config_.port, 64) == result_t::kError) {
    debug_info("Fail at server initialization", strerror(errno), false);
    return 1;
  }

  debug_info("Listen at",
             FMT("{bind_addr}:{port}", (bind_addr, server.config_.port)));

  try {
    loop();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
