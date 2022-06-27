#ifndef VEC2_HPP
#define VEC2_HPP

#include <cstddef>

#include "util/general/random.hpp"

template <typename T>
struct vec2 {
  T y, x;

  vec2() : y(0), x(0) {}
  vec2(T y, T x) : y(y), x(x) {}
  vec2(const vec2 &v) : y(v.y), x(v.x) {}

  static vec2 random(T max_y, T max_x);

  bool operator==(const vec2 &other) const;
  bool operator!=(const vec2 &other) const;

  vec2 operator+(const vec2 &other) const;
  vec2 operator-(const vec2 &other) const;
  vec2 &operator+=(const vec2 &other);
  vec2 &operator-=(const vec2 &other);
  vec2 operator*(T scalar) const;
  vec2 &operator*=(T scalar);
  vec2 operator/(T scalar) const;
  vec2 &operator/=(T scalar);
};

#include "vec2.tpp"

typedef vec2<size_t> pos;

#endif  // VEC2_HPP
