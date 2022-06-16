#ifndef UTIL_GENERAL_PRETTYPRINT_HPP
#define UTIL_GENERAL_PRETTYPRINT_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

namespace util {

template <class BidirIt>
static BidirIt prev(
    BidirIt it, typename std::iterator_traits<BidirIt>::difference_type n = 1) {
  std::advance(it, -n);
  return it;
}
template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
  os << p.first << ": " << p.second;
  return os;
}

template <typename C>
std::ostream& put_with_comma(std::ostream& os, const C& c) {
  if (!c.empty()) {
    typename C::const_iterator last = util::prev(c.end());
    for (typename C::const_iterator it = c.begin(); it != last; ++it) {
      os << *it << ", ";
    }
    os << *last;
  }
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  return os << "[" << put_with_comma(os, v) << "]";
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
  return os << "{" << put_with_comma(os, m) << "}";
}

}  // namespace util

#endif  // UTIL_STRUTIL_COUT_HPP
