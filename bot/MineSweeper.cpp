#include "MineSweeper.hpp"

#include <cmath>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

const pos dir_offset_[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                            {0, 1},   {1, -1}, {1, 0},  {1, 1}};
