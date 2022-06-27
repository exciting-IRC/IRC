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
// #include <algorithm>
// #include <iostream>

// #include "MineSweeper.hpp"
// #include "util/algorithm/functor.hpp"
// #include "util/strutil/format.hpp"
// struct command {
//   bool typed_x;
//   bool typed_y;
//   pos p;
//   GameAction::e action;

//   command() : typed_x(false), typed_y(false), action(GameAction::kOpen) {}

//   bool is_typed() const { return typed_x && typed_y; }

//   void parseline(std::string s);
// };

// /**
//  * @brief 메인 게임 루프
//  *
//  * 명령어 일람
//  * - (a-jA-J): x 좌표 입력
//  * - (0-9): y 좌표 입력
//  * - p, (m)ark: 깃발 추가 및 제거
//  * - x, (q)uit: 게임 종료
//  */
// void run_game() {
//   MineSweeper<9, 9> ms(0.12);

//   GameState::e state;
//   std::string line;

//   FUNCTOR(void, erase_spaces, (std::string & str), {
//     str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
//   });

//   std::cout << ms.toString(true) << std::endl;
//   while ((state = ms.getState()) == GameState::kContinue) {
//     std::getline(std::cin, line);
//     if (std::cin.eof()) {
//       break;
//     }
//     erase_spaces(line);
//     command cmd;
//     for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
//       const char c = tolower(*it);
//       if (isdigit(c)) {
//         cmd.typed_y = true;
//         cmd.p.y = c - '0';
//       } else {
//         switch (c) {
//           case 'p':
//           case 'm':
//             cmd.action = GameAction::kFlag;
//             break;

//           case 'x':
//           case 'q':
//             cmd.action = GameAction::kExit;
//             break;

//           default:
//             cmd.typed_x = true;
//             cmd.p.x = c - 'a';
//             break;
//         }
//       }
//     }
//     if (cmd.action == GameAction::kExit) {
//       return;
//     }
//     if (not cmd.is_typed()) {
//       continue;
//     }
//     if (not ms.isInBoard(cmd.p)) {
//       COUT_FMT("{}, {} is not in board", (cmd.p.x, cmd.p.y));
//       continue;
//     }
//     switch (cmd.action) {
//       case GameAction::kFlag:
//         ms.toggleFlag(cmd.p);
//         break;
//       default:
//         ms.exmine(cmd.p);
//         break;
//     }
//     std::cout << ms.toString(true) << std::endl;
//   }
//   if (state == GameState::kMineSwept) {
//     std::cout << "All mine swept. congratulations!!!" << std::endl;
//   }
//   if (state == GameState::kMineExploded) {
//     std::cout << ms.toString(false) << std::endl;
//     std::cout << "Mine exploded. try again..." << std::endl;
//   }
//   return;
// }

// int main() {
//   run_game();
//   return 0;
// }
