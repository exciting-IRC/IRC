#include "Bot.hpp"
#include "MineSweeper.hpp"

int main() {
  BotConfig config = BotConfig::from("config.yml");
  Bot bot(config);

  bot.init(1024);
  bot.loop();
}
