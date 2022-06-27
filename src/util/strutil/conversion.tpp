#ifndef STRUTIL_TPP
#define STRUTIL_TPP

#include <iomanip>
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

/// 숫자를 패딩하기
template <typename T>
string pad_num(const T& t, size_t width, char fill) {
  std::stringstream ss;

  ss << std::setw(width) << std::setfill(fill) << t;
  return ss.str();
}

}  // namespace util

#endif  // STRUTIL_TPP
