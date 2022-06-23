#ifndef UTIL_ALGORITHM_ALGORITHM_HPP
#define UTIL_ALGORITHM_ALGORITHM_HPP

#include <functional>
#include <iterator>

namespace util {

/// 지정한 크기만큼 이터레이터를 앞으로 이동하기
template <class BidirIt>
BidirIt prev(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n = 1);

/// 지정한 크기만큼 이터레이터를 뒤로 이동하기
template <class BidirIt>
BidirIt next(BidirIt it,
             typename std::iterator_traits<BidirIt>::difference_type n = 1);

/// 모든 원소가 참인지 확인
template <typename InputIterator, typename UnaryPredicate>
bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred);

/// 모든 원소가 거짓인지 확인
template <typename InputIterator, typename UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred);

/// 원소가 하나라도 참인지 확인
template <typename InputIterator, typename UnaryPredicate>
bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred);

template <typename C>
std::vector<typename C::key_type> keys(const C &c);

template <typename C>
std::vector<typename C::value_type> values(const C &c);

}  // namespace util

#include "algorithm.tpp"

#endif  // UTIL_ALGORITHM_ALGORITHM_HPP
