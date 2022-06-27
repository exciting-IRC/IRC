#include "MineSweeper.hpp"

#include <cmath>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

const pos dir_offset_[8] = {pos(-1, -1), pos(-1, 0), pos(-1, 1), pos(0, -1),
                            pos(0, 1),   pos(1, -1), pos(1, 0),  pos(1, 1)};
