#ifndef UTIL_ALGORITHM_ALGORITHM_TPP
#define UTIL_ALGORITHM_ALGORITHM_TPP

#include <util/algorithm/algorithm.hpp>

#include "algorithm.hpp"

namespace util {

template <class BidirIt>
BidirIt prev(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n) {
  std::advance(it, -n);
  return it;
}

template <class BidirIt>
BidirIt next(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n) {
  std::advance(it, n);
  return it;
}

template <typename InputIterator, typename UnaryPredicate>
bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  for (; first != last; ++first) {
    if (not pred(*first)) {
      return false;
    }
  }
  return true;
}

template <typename InputIterator, typename UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  return not any_of(first, last, pred);
}

template <typename InputIterator, typename UnaryPredicate>
bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  for (; first != last; ++first) {
    if (pred(*first)) {
      return true;
    }
  }
  return false;
}

template <typename C>
std::vector<typename C::key_type> keys(const C &c) {
  typedef typename C::key_type key_type;
  typedef typename C::const_iterator const_it;

  std::vector<key_type> ret;
  ret.reserve(c.size());
  for (const_it it = c.begin(); it != c.end(); ++it)
    ret.push_back(it->first);
  return ret;
}

template <typename C>
std::vector<typename C::value_type> values(const C &c) {
  typedef typename C::value_type value_type;
  typedef typename C::const_iterator const_it;

  std::vector<value_type> ret;
  ret.reserve(c.size());
  for (const_it it = c.begin(); it != c.end(); ++it)
    ret.push_back(it->second);
  return ret;
}

}  // namespace util

#endif  // UTIL_ALGORITHM_ALGORITHM_TPP
