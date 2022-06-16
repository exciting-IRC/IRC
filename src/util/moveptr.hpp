#ifndef UTIL_MOVEPTR_HPP
#define UTIL_MOVEPTR_HPP

#include <cstddef>

namespace util {

template <typename T>
T *moveptr(T *&ptr) {
  T *temp = ptr;
  ptr = NULL;
  return temp;
}

}  // namespace util

#endif  // UTIL_MOVEPTR_HPP
