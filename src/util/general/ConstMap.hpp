#ifndef UTIL_GENERAL_SAFEMAP_HPP
#define UTIL_GENERAL_SAFEMAP_HPP

#include <map>
#include <utility>

#include "util/strutil/strutil.hpp"

namespace util {
/**
 * @brief std::map을 상속받았지만 const operator[]가 있음
 */
template <typename K, typename T>
class ConstMap {
 private:
  std::map<K, T> m;

 public:
  template <typename Iter>
  ConstMap(Iter begin, Iter end) : m(begin, end) {}

  const T& operator[](const K& key) const {
    typename std::map<K, T>::const_iterator it = m.find(key);
    if (it == m.end())
      throw std::invalid_argument("key " + to_string(key) + " not found");
    return it->second;
  }
};
}  // namespace util

#endif  // UTIL_GENERAL_SAFEMAP_HPP
