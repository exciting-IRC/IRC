#ifndef BOT_MINESWEEPER_TPP
#define BOT_MINESWEEPER_TPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

#include "MineSweeper.hpp"
#include "util/algorithm/functor.hpp"

const pos dir_offset_[8] = {pos(-1, -1), pos(-1, 0), pos(-1, 1), pos(0, -1),
                            pos(0, 1),   pos(1, -1), pos(1, 0),  pos(1, 1)};

template <size_t width, size_t height>
std::ostream &operator<<(std::ostream &stream,
                         const MineSweeper<width, height> &ms) {
  stream << ms.toString(false);
  return stream;
}

template <size_t width, size_t height>
inline MineSweeper<width, height>::MineSweeper(double mine_ratio)
    : unknown_tiles_(size), board_(kEmpty), board_mask_(kClose), state_(GameState::kContinue) {
  if (not(6 <= width && width <= 10) or not(6 <= height && height <= 10)) {
    throw std::invalid_argument("width and height must be between 6 and 10");
  }
  std::srand(std::clock());
  initMines(mine_ratio);
  shuffleBoard(size * 100);
  initMineCounts();
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::shuffleBoard(size_t count) {
  for (size_t i = 0; i < count; ++i) {
    std::swap(board_.random(), board_.random());
  }
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initMines(double mine_ratio) {
  const size_t mine_count = round(size * mine_ratio);

  std::fill_n(board_.begin(), mine_count, static_cast<char>(kMine));
}

template <size_t width, size_t height>
inline void MineSweeper<width, height>::initMineCounts() {
  for (typename board_type::iterator it = board_.begin(); it != board_.end();
       ++it) {
    if (*it == kEmpty)
      *it = countAdjacentMines(board_.to_pos(it));
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
  FUNCTOR(void, add_column, (std::stringstream & ss), {
    const static std::string col_str = "abcdefghij";
    ss << "  ";
    for (size_t i = 0; i < width; ++i)
      ss << " " HMAG << col_str[i];
    ss << END << "\n";
  });
  FUNCTOR(void, add_row, (std::stringstream & ss, size_t i), {
    const static std::string row_str = "0123456789";
    ss << HBLU << row_str[i] << " " << END;
  });

  add_column(ss);
  for (size_t i = 0; i < height; ++i) {
    add_row(ss, i);
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
    ss << "\n";
  }
  return ss.str();
}

#endif  // BOT_MINESWEEPER_TPP
