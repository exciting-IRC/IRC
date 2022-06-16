#ifndef UTIL_STRUTIL_READ_HPP
#define UTIL_STRUTIL_READ_HPP

#include <string>

namespace util {
using std::string;

/// 파이썬 Path.read_text와 같은 역할
string read_text(const string& filename);

}  // namespace util

#endif  // UTIL_STRUTIL_READ_HPP
