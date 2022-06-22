#ifndef UTIL_GENERAL_TIME_HPP
#define UTIL_GENERAL_TIME_HPP

#include <ctime>

#include "util/strutil/format.hpp"

namespace util {
using std::string;

timespec create_timespec_in_sec(time_t sec);

string get_current_time(const string &format = "%Y-%m-%d %H:%M:%S");

}  // namespace util

#endif  // UTIL_GENERAL_TIME_HPP
