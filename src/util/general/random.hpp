#ifndef UTIL_GENERAL_RANDOM_HPP
#define UTIL_GENERAL_RANDOM_HPP

namespace util {

/**
 * @brief return pseudo random value
 *        in range [min, max)
 */
template <typename T>
T randRange(T min, T max);

template <typename T>
T randRange(T max);
}  // namespace util

#include "random.tpp"

#endif  // UTIL_GENERAL_RANDOM_HPP
