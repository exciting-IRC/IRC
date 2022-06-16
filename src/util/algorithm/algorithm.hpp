#ifndef UTIL_ALGORITHM_ALGORITHM_HPP
#define UTIL_ALGORITHM_ALGORITHM_HPP

#include <algorithm>
#include <iterator>

namespace util {

/// 지정한 크기만큼 이터레이터를 앞으로 이동하기
template <class BidirIt>
BidirIt prev(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n = 1) {
  std::advance(it, -n);
  return it;
}

/// 지정한 크기만큼 이터레이터를 뒤로 이동하기
template <class BidirIt>
BidirIt next(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n = 1) {
  std::advance(it, n);
  return it;
}

/// 모든 원소가 참인지 여부 확인
template <typename InputIterator, typename UnaryPredicate>
bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  for (; first != last; ++first) {
    if (not pred(*first)) {
      return false;
    }
  }
  return true;
}

// TODO: none_of, any_of 추가

}  // namespace util

#endif  // UTIL_ALGORITHM_ALGORITHM_HPP
