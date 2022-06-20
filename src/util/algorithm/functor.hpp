#ifndef UTIL_ALGORITHM_FUNCTOR_HPP
#define UTIL_ALGORITHM_FUNCTOR_HPP

#define FUNCTOR(returns, name, parameter, body) \
  static const struct name { returns operator() parameter const body } name

#endif  // UTIL_GENERAL_FUNCTOR_HPP
