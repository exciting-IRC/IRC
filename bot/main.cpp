#include "MineSweeper.hpp"

int main() {
  MineSweeper<9, 9> ms(0.12);

  GameState::e state;
  while ((state = ms.getState()) == GameState::kContinue) {
    std::cout << ms.toString(true) << std::endl;
    char cmd, x, y;

    std::cin >> cmd >> x >> y;
    if (!isalpha(x)) {
      std::cout << x << ": Unknown x coord" << std::endl;
      continue;
    }
    if (!isdigit(y)) {
      std::cout << y << ": Unknown y coord" << std::endl;
      continue;
    }

    x = toupper(x) - 'A';
    y = y - '0' - 1;

    switch (cmd) {
      case 'x':
        if (ms.isInBoard(y, x)) {
          ms.exmine(y, x);
        } else {
          std::cout << "(x, y)=(" << (int)x << "," << (int)y
                    << ") is not valid coord" << std::endl;
        }
        break;

      case 'm':
        if (ms.isInBoard(y, x)) {
          ms.mark(y, x);
        } else {
          std::cout << "(x, y)=(" << (int)x << "," << (int)y
                    << ") is not valid coord" << std::endl;
        }
        break;

      case 'u':
        if (ms.isInBoard(y, x)) {
          ms.unMark(y, x);
        } else {
          std::cout << "(x, y)=(" << (int)x << "," << (int)y
                    << ") is not valid coord" << std::endl;
        }
        break;

      case 't':
        std::cout << ms << "\n" << std::endl;
        break;

      default:
        std::cout << cmd << ": Unknown command" << std::endl;
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
