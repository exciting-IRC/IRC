#ifndef VEC2_HPP
#define VEC2_HPP

#include <cstddef>

#include "util/general/random.hpp"

template <typename T>
struct vec2 {
  T y, x;

  bool operator==(const vec2 &other) const;
  bool operator!=(const vec2 &other) const;
  vec2 operator+(const vec2 &other) const;
  vec2 operator-(const vec2 &other) const;
  vec2 &operator+=(const vec2 &other);
  vec2 &operator-=(const vec2 &other);
  static vec2 from(T y, T x);
  static vec2 random(T max_y, T max_x);
};

#include "vec2.tpp"

#endif  // VEC2_HPP
