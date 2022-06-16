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

}  // namespace util

#include "conversion.tpp"

#endif  // UTIL_STRUTIL_CONVERSION_HPP
