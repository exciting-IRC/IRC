#ifndef UTIL_ARRAY_HPP
#define UTIL_ARRAY_HPP

#include <algorithm>
// #include <cstdlib>  // for _LIBCPP_UNREACHABLE
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace util {

/**
 * @brief C++11 array STL을 c++ 98에서 쓸 수 있게 다운그레이드
 *
 * @tparam T 원소 타입
 * @tparam N 인자의 개수
 */
template <class T, size_t N>
struct array {
  // types:
  typedef array self;
  typedef T value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  T elems[N];

  /// 집합 생성을 위해 명시적인 생성자/소멸자 없음

  void fill(const value_type& val) { std::fill_n(elems, N, val); }

  void swap(array& other) { std::swap_ranges(elems, elems + N, other.elems); }

  /// 이터레이터
  iterator begin() throw() { return iterator(data()); }
  const_iterator begin() const throw() { return const_iterator(data()); }
  iterator end() throw() { return iterator(data() + N); }
  const_iterator end() const throw() { return const_iterator(data() + N); }

  reverse_iterator rbegin() throw() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const throw() {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() throw() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const throw() {
    return const_reverse_iterator(begin());
  }

  /// 용량
  size_type size() const throw() { return N; }
  size_type max_size() const throw() { return N; }
  bool empty() const throw() { return false; }

  /// 원소 접근
  reference operator[](size_type n) throw() { return elems[n]; }
  const_reference operator[](size_type n) const throw() { return elems[n]; }

  reference at(size_type n);
  const_reference at(size_type n) const;

  reference front() throw() { return elems[0]; }
  const_reference front() const throw() { return elems[0]; }
  reference back() throw() { return elems[N - 1]; }
  const_reference back() const throw() { return elems[N - 1]; }

  value_type* data() throw() { return elems; }
  const value_type* data() const throw() { return elems; }
};

template <class T, size_t N>
typename array<T, N>::reference array<T, N>::at(size_type n) {
  if (n >= N)
    throw std::out_of_range("array::at");

  return elems[n];
}

template <class T, size_t N>
typename array<T, N>::const_reference array<T, N>::at(size_type n) const {
  if (n >= N)
    throw std::out_of_range("array::at");

  return elems[n];
}
}  // namespace util

#endif  // UTIL_ARRAY_HPP
