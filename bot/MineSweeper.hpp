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

  typedef board<char, height, width> board_type;
  typedef typename board_type::iterator iterator;
  static const size_t size = width * height;

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
  board_type board_;
  board_type board_mask_;
  GameState::e state_;
};

template <size_t W, size_t H>
std::ostream &operator<<(std::ostream &stream, const MineSweeper<W, H> &ms);

#include "MineSweeper.tpp"

#endif  // BOT_MINESWEEPER_HPP
