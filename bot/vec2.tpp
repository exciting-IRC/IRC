#ifndef VEC2_TPP
#define VEC2_TPP

#include "vec2.hpp"

// 연산자 오버로딩
template <typename T>
inline bool vec2<T>::operator==(const vec2 &other) const {
  return y == other.y and x == other.x;
}

template <typename T>
inline bool vec2<T>::operator!=(const vec2 &other) const {
  return not(*this == other);
}

template <typename T>
inline vec2<T> vec2<T>::operator+(const vec2 &other) const {
  return from(y + other.y, x + other.x);
}

template <typename T>
inline vec2<T> vec2<T>::operator-(const vec2 &other) const {
  return from(y - other.y, x - other.x);
}

template <typename T>
inline vec2<T> &vec2<T>::operator+=(const vec2 &other) {
  *this = *this + other;
  return *this;
}

template <typename T>
inline vec2<T> &vec2<T>::operator-=(const vec2 &other) {
  *this = *this - other;
  return *this;
}

template <typename T>
inline vec2<T> vec2<T>::from(T y, T x) {
  vec2 tmp = {y, x};
  return tmp;
}

template <typename T>
inline vec2<T> vec2<T>::random(T max_y, T max_x) {
  return from(util::randRange(max_y), util::randRange(max_x));
}

#endif  // VEC2_TPP
