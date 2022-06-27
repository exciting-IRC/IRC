#include <cstdlib>

#include "random.hpp"

namespace util {

template <typename T>
T randRange(T min, T max) {
  const T range = max - min;
  const T n = std::rand() % range;

  return n + min;
}

template <typename T>
T randRange(T max) {
  return randRange<T>(0, max);
}

}  // namespace util
