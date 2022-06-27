
#ifndef BOT_MINESWEEPER_HPP
#define BOT_MINESWEEPER_HPP

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

#include "board.hpp"
#include "util/color.hpp"
#include "util/general/random.hpp"
#include "util/strutil/conversion.hpp"
#include "vec2.hpp"

extern const pos dir_offset_[8];

struct GameState {
  enum e { kContinue, kMineExploded, kMineSwept };
};

template <size_t width, size_t height>
class MineSweeper {
 public:
  MineSweeper(double mine_ratio);

 private:
  enum { kMine = '*', kEmpty = 0 };
  enum { kClose, kOpen, kMark };

 private:
  void shuffleBoard(size_t count);

  void initMines(double mine_ratio);

  void initBoardMask();

  void initMineCounts();

  bool isMine(pos p) const;

  size_t countAdjacentMines(pos p) const;

  std::string getBoardChar(pos p) const;

  void openTile(pos p);

  void _openRecursive(pos p);

  void openRecursive(pos p);

 public:
  bool isInBoard(pos p) const { return board_.in_bounds(p); }

  void exmine(pos p);

  void mark(pos p);

  void unMark(pos p);

  GameState::e getState();

  std::string toString(bool mask_board) const;

 private:
  size_t unknown_tiles_;
  board<char, height, width> board_;
  board<char, height, width> board_mask_;
  GameState::e state_;
};

template <size_t W, size_t H>
std::ostream &operator<<(std::ostream &stream, const MineSweeper<W, H> &ms) {
  stream << ms.toString(false);
  return stream;
}

template <size_t width, size_t height>
inline MineSweeper<width, height>::MineSweeper(double mine_ratio)
    : unknown_tiles_(width * height), state_(GameState::kContinue) {
  std::srand(std::clock());
  initMines(mine_ratio);
  initBoardMask();
  shuffleBoard(width * height * 100);
  initMineCounts();
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::shuffleBoard(size_t count) {
  for (size_t i = 0; i < count; ++i) {
    std::swap(board_.at(pos::random(height, width)),
              board_.at(pos::random(height, width)));
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initMines(double mine_ratio) {
  const size_t size = width * height;
  const size_t mine_count = round(size * mine_ratio);

  for (size_t i = 0; i < mine_count; ++i) {
    board_.at(i / height, i % width) = kMine;
  }
  for (size_t i = mine_count; i < size; ++i) {
    board_.at(i / height, i % width) = kEmpty;
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initBoardMask() {
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      board_mask_.at(i, j) = kClose;
    }
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initMineCounts() {
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      pos p(i, j);
      if (board_.at(p) == kEmpty) {
        board_.at(p) = countAdjacentMines(p);
      }
    }
  }
}

template <size_t width, size_t height>
inline bool MineSweeper<width, height>::isMine(pos p) const {
  return board_.at(p) == kMine;
}

template <size_t width, size_t height>
inline size_t MineSweeper<width, height>::countAdjacentMines(pos p) const {
  size_t count = 0;
  for (size_t i = 0; i < 8; ++i) {
    pos n = p + dir_offset_[i];
    if (isInBoard(n) and isMine(n))
      count++;
  }
  return count;
}

// TODO: board로 이동
template <size_t width, size_t height>
inline std::string MineSweeper<width, height>::getBoardChar(pos p) const {
  const char c = board_.at(p);
  if (c == 0) {
    return " ";
  } else if (0 < c && c <= 8) {
    std::string color;
    switch (c) {
      case 1:
        color = HBLU;
        break;
      case 2:
        color = GRN;
        break;
      case 3:
        color = RED;
        break;
      default:
        color = MAG;
    }
    return color + std::string(1, c + '0');
  } else if (c == kMine) {
    return BHRED + std::string(1, kMine);
  }

  return std::string(1, c);
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::openTile(pos p) {
  board_mask_.at(p) = kOpen;
  --unknown_tiles_;
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::_openRecursive(pos p) {
  if (board_mask_.at(p) != kClose) {
    return;
  }
  openTile(p);
  if (board_.at(p) == 0) {
    for (size_t i = 0; i < 8; ++i) {
      pos n = p + dir_offset_[i];
      if (isInBoard(n)) {
        _openRecursive(n);
      }
    }
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::openRecursive(pos p) {
  openTile(p);
  if (board_.at(p) == 0) {
    for (size_t i = 0; i < 8; ++i) {
      pos n = p + dir_offset_[i];
      if (isInBoard(n)) {
        _openRecursive(n);
      }
    }
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::exmine(pos p) {
  if (board_.at(p) == kMine) {
    state_ = GameState::kMineExploded;
    return;
  }
  if (board_mask_.at(p) == kClose)
    openRecursive(p);
  if (unknown_tiles_ == 0) {
    state_ = GameState::kMineSwept;
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::mark(pos p) {
  if (board_mask_.at(p) != kClose)
    return;
  board_mask_.at(p) = kMark;
  if (board_.at(p) == kMine)
    --unknown_tiles_;
  if (unknown_tiles_ == 0) {
    state_ = GameState::kMineSwept;
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::unMark(pos p) {
  if (board_mask_.at(p) != kMark)
    return;
  board_mask_.at(p) = kClose;
  if (board_.at(p) == kMine)
    ++unknown_tiles_;
  if (unknown_tiles_ == 0) {
    state_ = GameState::kMineSwept;
  }
}

template <size_t width, size_t height>
inline GameState::e MineSweeper<width, height>::getState() {
  return state_;
}

template <size_t width, size_t height>
inline std::string MineSweeper<width, height>::toString(bool mask_board) const {
  std::stringstream ss;

  ss << "  ";
  for (size_t i = 0; i < width; ++i) {
    ss << " " HMAG << static_cast<char>('A' + i);
  }
  ss << END;
  for (size_t i = 0; i < height; ++i) {
    ss << "\n ";
    ss << HBLU << static_cast<char>('0' + i + 1) << END;
    for (size_t j = 0; j < width; ++j) {
      pos p(i, j);
      if (mask_board) {
        switch (board_mask_.at(p)) {
          case kClose:
            ss << GRNHB " ." END;
            break;
          case kOpen:
            ss << " " << getBoardChar(p) << END;
            break;
          case kMark:
            ss << GRNHB BHRED " P" END;
            break;
        }
      } else {
        ss << " " << getBoardChar(p) << END;
      }
    }
  }
  return ss.str();
}

#endif  // BOT_MINESWEEPER_HPP
