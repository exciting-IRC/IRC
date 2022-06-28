#ifndef TIMEUTIL_HPP
#define TIMEUTIL_HPP

#include <ctime>

namespace util {

void timespec_sub(const struct timespec &a, const struct timespec &b,
                  struct timespec &result);

bool timespec_less(const struct timespec &a, const struct timespec &b);

}  // namespace util

#endif  // TIMEUTIL_HPP
