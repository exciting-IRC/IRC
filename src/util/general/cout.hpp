#ifndef UTIL_GENERAL_COUT_HPP
#define UTIL_GENERAL_COUT_HPP

#include <iostream>
#include <iterator>
#include <map>
#include <vector>

namespace util {
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  os << "[";
  std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, ", "));
  os << "]";
  return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
  os << "{";
  std::copy(m.begin(), m.end(),
            std::ostream_iterator<std::pair<K, V> >(os, ", "));
  os << "}";
  return os;
}

}  // namespace util

#endif  // UTIL_STRUTIL_COUT_HPP
