#ifndef UTIL_STRUTIL_CONVERSION_HPP
#define UTIL_STRUTIL_CONVERSION_HPP

#include <string>

namespace util {

using std::string;

/// stringstream 출력을 지원하는 원소에 한해 문자열로 변경
template <typename T>
string to_string(const T& t);

template <>
string to_string(const string& t);

/// istringstream 에서 변환이 가능한 타입에 한해 해당 타입으로 변환
template <typename T>
T convert_to(const string& from);

/// 문자열을 대문자로 변경
string to_upper(string str);

/// 문자열을 소문자로 변경
string to_lower(string str);

template <typename T>
string pad_num(const T& t, size_t width = 3, char fill = '0');

}  // namespace util

#include "conversion.tpp"

#endif  // UTIL_STRUTIL_CONVERSION_HPP
