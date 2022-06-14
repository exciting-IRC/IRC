#ifndef UTIL_GENERAL_TIME_HPP
#define UTIL_GENERAL_TIME_HPP

#include <ctime>

namespace util {
struct timespec create_timespec_in_sec(time_t sec) {
  struct timespec ts;

  ts.tv_nsec = 0;
  ts.tv_sec = sec;
  return ts;
}

}  // namespace util

#endif  // UTIL_GENERAL_TIME_HPP
