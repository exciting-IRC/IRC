#ifndef STRUTIL_HPP
#define STRUTIL_HPP

#include <sstream>

namespace util {
using std::string;

/// stringstream 출력을 지원하는 원소에 한해 문자열로 변경
template <typename T>
string to_string(const T& t) {
  std::stringstream ss;

  ss << t;
  return ss.str();
}

template <typename T>
T convert_to(const string& from) {
  T val;
  std::stringstream iss(from);

  iss >> val;
  return val;
}

}  // namespace util

#endif  // STRUTIL_HPP
