#ifndef UTIL_STRUTIL_TRIM_HPP
#define UTIL_STRUTIL_TRIM_HPP

#include <string>

namespace util {

using std::string;

extern const string white_space;

/**
 * @brief 발견한 토큰과 그 이후를 지우기
 *
 * @param match 발견할 토큰
 */
string& erase_from(string& line, const string& match);

/**
 * @brief 발견한 토큰과 그 전을 지우기
 *
 * @param match 발견할 토큰
 */
string& erase_before(string& line, const string& match);

/**
 * @brief 문자열 왼쪽 지우기
 *
 * @param match 해당하지 않는 문자열 바로 앞을 지움
 */
string& ltrim(string& s, const string& match = white_space);

/**
 * @brief 문자열 오른쪽 지우기
 *
 * @param match 해당하지 않는 문자열 바로 뒤를 지움
 */
string& rtrim(string& s, const string& match = white_space);

/**
 * @brief 문자열 양쪽 지우기
 *
 * @param match 해당하지 않는 문자열 전후를 지움
 */
string& trim(string& s, const string& match = white_space);

typedef string& (*trim_func)(string& s, const string& match);

}  // namespace util

#endif  // UTIL_STRUTIL_TRIM_HPP
