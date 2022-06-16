#ifndef UTIL_ALGORITHM_ALGORITHM_TPP
#define UTIL_ALGORITHM_ALGORITHM_TPP

#include <util/algorithm/algorithm.hpp>

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
}  // namespace util

#endif  // UTIL_ALGORITHM_ALGORITHM_TPP
