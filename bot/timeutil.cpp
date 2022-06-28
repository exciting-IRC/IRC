#include "timeutil.hpp"

namespace util {

void timespec_sub(const struct timespec &a, const struct timespec &b,
                  struct timespec &result) {
  result.tv_sec = a.tv_sec - b.tv_sec;
  result.tv_nsec = a.tv_nsec - b.tv_nsec;

  if (result.tv_nsec < 0) {
    --result.tv_sec;
    result.tv_nsec += 1000000000;
  }
}

bool timespec_less(const struct timespec &a, const struct timespec &b) {
  if (a.tv_sec < b.tv_sec)
    return true;
  if (a.tv_sec == b.tv_sec)
    return a.tv_nsec < b.tv_nsec;
  return false;
}

}  // namespace util
