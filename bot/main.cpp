#include "Bot.hpp"
#include "MineSweeper.hpp"

int main() {
  BotConfig config;

  config.password = "password";
  config.port = 6667;
  config.server_ip = "127.0.0.1";

  Bot bot(config);

  bot.init(1024);

  while (true)
    bot.loop();
}
