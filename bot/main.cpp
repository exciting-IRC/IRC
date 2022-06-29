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

static BotConfig init_config() {
  try {
    return BotConfig::from("config.yml");
  } catch (const std::exception &e) {
    util::debug_info("on bot init:", e.what(), false);
    exit(1);
  }
}

int main() {
  Bot bot(init_config());

  if (bot.init(1024) == result_t::kError) {
    util::debug_info("Failed to init bot.:", strerror(errno), false);
    return 1;
  }

  signal(SIGINT, server_close_handler);
  signal(SIGTERM, server_close_handler);

  loop(bot);

  return 0;
}
