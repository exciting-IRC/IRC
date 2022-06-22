#include "time.hpp"

namespace util {

timespec create_timespec_in_sec(time_t sec) {
  timespec ts;

  ts.tv_nsec = 0;
  ts.tv_sec = sec;
  return ts;
}

string get_current_time(const string &format) {
  char buf[32];
  time_t t = std::time(NULL);

  if (std::strftime(buf, sizeof(buf), format.c_str(), std::localtime(&t)) == 0)
    throw std::runtime_error(FMT("strftime failed to format {}", (format)));
  return string(buf);
}
}  // namespace util
