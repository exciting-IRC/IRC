
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

namespace util {

/**
 * @brief return pseudo random value
 *        in range [min, max)
 */
size_t randRange(size_t min, size_t max);
size_t randRange(size_t max);
}  // namespace util

struct pos_t {
  size_t y, x;

  // 연산자 오버로딩
  bool operator==(const pos_t &other) const {
    return y == other.y and x == other.x;
  }
  bool operator!=(const pos_t &other) const { return not(*this == other); }
  pos_t operator+(const pos_t &other) const {
    return from(y + other.y, x + other.x);
  }
  pos_t operator-(const pos_t &other) const {
    return from(y - other.y, x - other.x);
  }
  pos_t &operator+=(const pos_t &other) {
    *this = *this + other;
    return *this;
  }
  pos_t &operator-=(const pos_t &other) {
    *this = *this - other;
    return *this;
  }
  static pos_t from(size_t y, size_t x) {
    pos_t tmp = {y, x};
    return tmp;
  }
  static pos_t random(size_t max_y, size_t max_x) {
    return from(util::randRange(max_y), util::randRange(max_x));
  }
};

extern const pos_t dir_offset_[8];

struct GameState {
  enum e { kContinue, kMineExploded, kMineSwept };
};

template <typename T, size_t width, size_t height>
struct board {
  T data_[height][width];
  T at(size_t y, size_t x) const { return data_[y][x]; }
  T &at(size_t y, size_t x) { return data_[y][x]; }
  T at(pos_t p) const { return data_[p.y][p.x]; }
  T &at(pos_t p) { return data_[p.y][p.x]; }
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

  bool isMine(pos_t p) const;

  size_t countAdjacentMines(pos_t p) const;

  std::string getBoardChar(pos_t p) const;

  void openTile(pos_t p);

  void _openRecursive(pos_t p);

  void openRecursive(pos_t p);

 public:
  bool isInBoard(pos_t p) const;

  void exmine(pos_t p);

  void mark(pos_t p);

  void unMark(pos_t p);

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
    std::swap(board_.at(pos_t::random(height, width)),
              board_.at(pos_t::random(height, width)));
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
      pos_t p = {i, j};
      if (board_.at(p) == kEmpty) {
        board_.at(p) = countAdjacentMines(p);
      }
    }
  }
}

template <size_t width, size_t height>
inline bool MineSweeper<width, height>::isMine(pos_t p) const {
  return board_.at(p) == kMine;
}

template <size_t width, size_t height>
inline size_t MineSweeper<width, height>::countAdjacentMines(pos_t p) const {
  size_t count = 0;
  for (size_t i = 0; i < 8; ++i) {
    pos_t n = p + dir_offset_[i];
    if (isInBoard(n) and isMine(n))
      count++;
  }
  return count;
}

// TODO: board로 이동
template <size_t width, size_t height>
inline std::string MineSweeper<width, height>::getBoardChar(pos_t p) const {
  if (0 <= board_.at(p) && board_.at(p) <= 8) {
    std::string color;
    switch (board_.at(p)) {
      case 0:
        break;
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
    return color + std::string(1, board_.at(p) + '0');
  }
  return std::string(1, board_.at(p));
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::openTile(pos_t p) {
  board_mask_.at(p) = kOpen;
  --unknown_tiles_;
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::_openRecursive(pos_t p) {
  if (board_mask_.at(p) != kClose) {
    return;
  }
  openTile(p);
  if (board_.at(p) == 0) {
    for (size_t i = 0; i < 8; ++i) {
      pos_t n = p + dir_offset_[i];
      if (isInBoard(n)) {
        _openRecursive(n);
      }
    }
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::openRecursive(pos_t p) {
  openTile(p);
  if (board_.at(p) == 0) {
    for (size_t i = 0; i < 8; ++i) {
      pos_t n = p + dir_offset_[i];
      if (isInBoard(n)) {
        _openRecursive(n);
      }
    }
  }
}

template <size_t width, size_t height>
inline bool MineSweeper<width, height>::isInBoard(pos_t p) const {
  return (0 <= p.y && p.y < height) && (0 <= p.x && p.x < width);
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::exmine(pos_t p) {
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
inline void MineSweeper<width, height>::mark(pos_t p) {
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
inline void MineSweeper<width, height>::unMark(pos_t p) {
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
    ss << " " << static_cast<char>('A' + i);
  }
  for (size_t i = 0; i < height; ++i) {
    ss << "\n ";
    ss << static_cast<char>('0' + i + 1);
    for (size_t j = 0; j < width; ++j) {
      pos_t p = {i, j};
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
        ss << " " << getBoardChar(p);
      }
    }
  }
  return ss.str();
}

#endif  // BOT_MINESWEEPER_HPP
