#ifndef VEC2_TPP
#define VEC2_TPP

#include "vec2.hpp"

template <typename T>
inline vec2<T> vec2<T>::random(T max_y, T max_x) {
  return vec2<T>(util::randRange(max_y), util::randRange(max_x));
}

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
  return vec2(y + other.y, x + other.x);
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
inline vec2<T> vec2<T>::operator*(T scalar) const {
  return from(y * scalar, x * scalar);
}

template <typename T>
inline vec2<T> &vec2<T>::operator*=(T scalar) {
  *this = *this * scalar;
  return *this;
}

template <typename T>
inline vec2<T> vec2<T>::operator/(T scalar) const {
  return from(y / scalar, x / scalar);
}

template <typename T>
inline vec2<T> &vec2<T>::operator/=(T scalar) {
  *this = *this / scalar;
  return *this;
}
#endif  // VEC2_TPP
