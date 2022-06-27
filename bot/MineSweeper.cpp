#include "MineSweeper.hpp"

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

namespace util {

size_t randRange(size_t min, size_t max) {
  const size_t range = max - min;
  const size_t n = std::rand() % range;

  return n + min;
}

size_t randRange(size_t max) { return randRange(0, max); }

}  // namespace util

const pos_t dir_offset_[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                              {0, 1},   {1, -1}, {1, 0},  {1, 1}};
