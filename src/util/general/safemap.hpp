#ifndef UTIL_GENERAL_SAFEMAP_HPP
#define UTIL_GENERAL_SAFEMAP_HPP

#include <map>
#include <utility>

namespace util {
/**
 * @brief std::map을 상속받았지만 const operator[]가 있음
 */
template <typename K, typename T>
class SafeMap : public std::map<K, T> {
 public:
  template <typename Iter>
  SafeMap(Iter begin, Iter end) : std::map<K, T>(begin, end) {}

  const T& operator[](const K& key) const {
    typename std::map<K, T>::const_iterator it = this->find(key);
    if (it == this->end())
      throw std::invalid_argument("key not found");
    return it->second;
  }
};
}  // namespace util

#endif  // UTIL_GENERAL_SAFEMAP_HPP
