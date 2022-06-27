
#ifndef BOT_MINESWEEPER_HPP
#define BOT_MINESWEEPER_HPP

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

#include "util/color.hpp"
#include "util/strutil/conversion.hpp"

extern const int dir_offset_[8][2];

namespace util {

/**
 * @brief return pseudo random value
 *        in range [min, max)
 */
size_t randRange(size_t min, size_t max);

}  // namespace util

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

  bool isMine(size_t y, size_t x) const;

  size_t countAdjacentMines(size_t y, size_t x) const;

  char getBoardChar(size_t y, size_t x) const;

  void openTile(size_t y, size_t x);

  void _openRecursive(size_t y, size_t x);

  void openRecursive(size_t y, size_t x);

 public:
  bool isInBoard(size_t y, size_t x) const;

  void exmine(size_t y, size_t x);

  void mark(size_t y, size_t x);

  void unMark(size_t y, size_t x);

  GameState::e getState();

  std::string toString(bool mask_board) const;

 private:
  size_t unknown_tiles_;
  char board_[height][width];
  char board_mask_[height][width];
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
    size_t ox = util::randRange(0, width), oy = util::randRange(0, height);
    size_t nx = util::randRange(0, width), ny = util::randRange(0, height);
    std::swap(board_[oy][ox], board_[ny][nx]);
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initMines(double mine_ratio) {
  size_t size = width * height;
  size_t mine_count = round(size * mine_ratio);

  for (size_t i = 0; i < mine_count; ++i) {
    board_[i / height][i % width] = kMine;
  }
  for (size_t i = mine_count; i < size; ++i) {
    board_[i / height][i % width] = kEmpty;
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initBoardMask() {
  std::memset(board_mask_, kClose, width * height);
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initMineCounts() {
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      if (board_[i][j] == kEmpty) {
        board_[i][j] = countAdjacentMines(i, j);
      }
    }
  }
}

template <size_t width, size_t height>
inline bool MineSweeper<width, height>::isMine(size_t y, size_t x) const {
  return board_[y][x] == kMine;
}

template <size_t width, size_t height>
inline size_t MineSweeper<width, height>::countAdjacentMines(size_t y,
                                                             size_t x) const {
  size_t count = 0;
  for (size_t i = 0; i < 8; ++i) {
    size_t dy = dir_offset_[i][0], dx = dir_offset_[i][1];
    if (isInBoard(y + dy, x + dx) && isMine(y + dy, x + dx))
      count++;
  }
  return count;
}

template <size_t width, size_t height>
inline char MineSweeper<width, height>::getBoardChar(size_t y, size_t x) const {
  if (0 <= board_[y][x] && board_[y][x] <= 8)
    return board_[y][x] + '0';
  return board_[y][x];
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::openTile(size_t y, size_t x) {
  board_mask_[y][x] = kOpen;
  --unknown_tiles_;
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::_openRecursive(size_t y, size_t x) {
  if (board_mask_[y][x] != kClose) {
    return;
  }
  openTile(y, x);
  if (board_[y][x] == 0) {
    for (size_t i = 0; i < 8; ++i) {
      size_t dy = dir_offset_[i][0], dx = dir_offset_[i][1];
      if (isInBoard(y + dy, x + dx)) {
        _openRecursive(y + dy, x + dx);
      }
    }
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::openRecursive(size_t y, size_t x) {
  openTile(y, x);
  if (board_[y][x] == 0) {
    for (size_t i = 0; i < 8; ++i) {
      size_t dy = dir_offset_[i][0], dx = dir_offset_[i][1];
      if (isInBoard(y + dy, x + dx)) {
        _openRecursive(y + dy, x + dx);
      }
    }
  }
}

template <size_t width, size_t height>
inline bool MineSweeper<width, height>::isInBoard(size_t y, size_t x) const {
  return y >= 0 && y < height && x >= 0 && x < width;
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::exmine(size_t y, size_t x) {
  if (board_[y][x] == kMine) {
    state_ = GameState::kMineExploded;
    return;
  }
  if (board_mask_[y][x] == kClose)
    openRecursive(y, x);
  if (unknown_tiles_ == 0) {
    state_ = GameState::kMineSwept;
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::mark(size_t y, size_t x) {
  if (board_mask_[y][x] != kClose)
    return;
  board_mask_[y][x] = kMark;
  if (board_[y][x] == kMine)
    --unknown_tiles_;
  if (unknown_tiles_ == 0) {
    state_ = GameState::kMineSwept;
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::unMark(size_t y, size_t x) {
  if (board_mask_[y][x] != kMark)
    return;
  board_mask_[y][x] = kClose;
  if (board_[y][x] == kMine)
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
    ss << "│ " << static_cast<char>('A' + i);
  }
  for (size_t i = 0; i < height; ++i) {
    ss << "\n──┼";
    for (size_t j = 0; j < width; ++j) {
      if (j != 0)
        ss << "┼";
      ss << "──";
    }
    ss << "\n ";
    ss << static_cast<char>('0' + i + 1);
    ss << "│";
    for (size_t j = 0; j < width; ++j) {
      if (j != 0)
        ss << "│";
      if (mask_board) {
        switch (board_mask_[i][j]) {
          case kClose:
            ss << " .";
            break;
          case kOpen:
            ss << ' ' << getBoardChar(i, j);
            break;
          case kMark:
            ss << " P";
            break;
        }
      } else {
        ss << ' ' << getBoardChar(i, j);
      }
    }
  }
  return ss.str();
}

#endif  // BOT_MINESWEEPER_HPP
