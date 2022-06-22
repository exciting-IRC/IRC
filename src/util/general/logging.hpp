#ifndef UTIL_GENERAL_LOGGING_HPP
#define UTIL_GENERAL_LOGGING_HPP

#include <iostream>
#include <string>
#include <vector>

namespace util {
using std::string;
using std::vector;

void debug(const string& fmt, const vector<string>& args, bool ok = true);

void debug_input(const string& command, bool ok = true);

void debug_output(const string& payload, bool ok = true);

}  // namespace util

#endif  // UTIL_GENERAL_LOGGING_HPP
