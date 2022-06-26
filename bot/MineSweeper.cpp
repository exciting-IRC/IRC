#include "MineSweeper.hpp"

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

namespace util {

int randRange(int min, int max) {
  unsigned int k = std::rand();

  k %= max - min;
  k += min;

  return k;
}

}  // namespace util

const int dir_offset_[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                               {0, 1},   {1, -1}, {1, 0},  {1, 1}};
