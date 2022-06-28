#include "MineSweeper.hpp"

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

const pos dir_offset_[8] = {pos(-1, -1), pos(-1, 0), pos(-1, 1), pos(0, -1),
                            pos(0, 1),   pos(1, -1), pos(1, 0),  pos(1, 1)};

namespace util {

int randRange(int min, int max) {
  unsigned int k = std::rand();

  k %= max - min;
  k += min;

  return k;
}

}  // namespace util
