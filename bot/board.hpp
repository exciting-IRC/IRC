#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstddef>

#include "vec2.hpp"

template <typename T, size_t width, size_t height>
struct board {
  T data_[height][width];

  T at(size_t y, size_t x) const { return data_[y][x]; }
  T &at(size_t y, size_t x) { return data_[y][x]; }
  T at(pos p) const { return data_[p.y][p.x]; }
  T &at(pos p) { return data_[p.y][p.x]; }
  bool in_bounds(pos p) const { return p.y < height and p.x < width; }
  bool in_bounds(size_t y, size_t x) const { return y < height and x < width; }
};

#endif  // BOARD_HPP
