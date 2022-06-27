#include "MineSweeper.hpp"
#include "util/strutil/format.hpp"

int main() {
  MineSweeper<9, 9> ms(0.12);

  GameState::e state;
  while ((state = ms.getState()) == GameState::kContinue) {
    std::cout << ms.toString(true) << std::endl;
    char cmd, x, y;

    std::cin >> cmd >> x >> y;
    if (std::cin.eof()) {
      break;
    }

    if (!isalpha(x)) {
      COUT_FMT("{}: Unknown x coord", (x));
      continue;
    }
    if (!isdigit(y)) {
      COUT_FMT("{}: Unknown y coord", (y));
      continue;
    }

    x = toupper(x) - 'A';
    y = y - '0' - 1;

    switch (cmd) {
      case 'x':
        if (ms.isInBoard(y, x)) {
          ms.exmine(y, x);
        } else {
          COUT_FMT("(x, y)=({x}, {y}) is not valid coord", (x, y));
        }
        break;

      case 'm':
        if (ms.isInBoard(y, x)) {
          ms.mark(y, x);
        } else {
          COUT_FMT("(x, y)=({x}, {y}) is not valid coord", (x, y));
        }
        break;

      case 'u':
        if (ms.isInBoard(y, x)) {
          ms.unMark(y, x);
        } else {
          COUT_FMT("(x, y)=({x}, {y}) is not valid coord", (x, y));
        }
        break;

      case 't':
        std::cout << ms << "\n" << std::endl;
        break;

      default:
        COUT_FMT("{}: Unknown command", (cmd));
        break;
    }
  }
  if (state == GameState::kMineSwept) {
    std::cout << "All mine swept. congratulations!!!" << std::endl;
  }
  if (state == GameState::kMineExploded) {
    std::cout << "Mine exploded. try again..." << std::endl;
  }
  return 0;
}
