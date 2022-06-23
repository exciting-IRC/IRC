#ifndef UTIL_STRUTIL_SPLIT_HPP
#define UTIL_STRUTIL_SPLIT_HPP

#include <string>
#include <vector>

namespace util {

using std::string;
using std::vector;

vector<string> split(const string& str, const string& sep = "");

}  // namespace util

#endif  // UTIL_STRUTIL_SPLIT_HPP
