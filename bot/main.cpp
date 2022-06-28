#include "Bot.hpp"
#include "util/general/logging.hpp"

static sig_atomic_t received_signal;

void server_close_handler(int sig) { received_signal = sig; }

void loop(Bot &bot) {
  while (true) {
    bot.dispatchEvent();
    if (received_signal) {
      return;
    }
  }
}

int main() {
  BotConfig config;

  config.password = "password";
  config.port = 6667;
  config.server_ip = "127.0.0.1";

  Bot bot(config);

  if (bot.init(1024) == result_t::kError) {
    util::debug_info("Failed to init bot.", false);
    return 1;
  }

  signal(SIGINT, server_close_handler);
  signal(SIGTERM, server_close_handler);

  loop(bot);

  return 0;
}
