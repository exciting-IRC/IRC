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

#include "event/Event.hpp"
#include "server/Server.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/color.hpp"
#include "util/general/logging.hpp"
#include "util/vargs/container_of.hpp"

static sig_atomic_t recived_sig;
const static std::string config_file = "config.yml";

void server_close_handler(int sig) { recived_sig = sig; }

static util::Config init_config(const int argc, const char* argv[]) {
  using namespace util;
  try {
    Config config = util::Config::from(config_file);
    if (argc != 3) {
      debug_info("no port and password given:", "using defaults", false);
      config.port = 6667;
      config.password = "password";
    } else {
      config.port = convert_to<uint16_t>(argv[1]);
      config.password = argv[2];
    }
    return config;
  } catch (std::exception& e) {
    debug_info("on reading:", e.what(), false);
    exit(1);
  }
}

static void loop() {
  while (not recived_sig) {
    server.getPool().dispatchEvent(1);
  }
  util::debug_info("shutdown", strsignal(recived_sig));
}

int main(const int argc, const char* argv[]) {
  using namespace util;
  using namespace std;

  signal(SIGINT, server_close_handler);
  signal(SIGTERM, server_close_handler);

  server.config_ = init_config(argc, argv);
  if (server.init(64) == result_t::kError) {
    debug_info("Fail at server initialization: ", strerror(errno), false);
    return 1;
  }

  cout << server.config_ << endl;
  debug_info("Listen at", FMT("{address}:{port}",
                              (server.config_.address, server.config_.port)));

  try {
    loop();
  } catch (std::bad_alloc& e) {
    debug(e.what(), false);
    return 1;
  }

  return 0;
}
