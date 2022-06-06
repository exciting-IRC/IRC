#include "strutil.hpp"

namespace util {
const string white_space = " \t\n\r\f\v";

/**
 * @brief 문자열 오른쪽 지우기
 *
 * @param match 해당하지 않는 문자열 바로 뒤를 지움
 */
string& rtrim(string& s, const string& match) {
  s.erase(s.find_last_not_of(match) + 1);
  return s;
}

/**
 * @brief 문자열 왼쪽 지우기
 *
 * @param match 해당하지 않는 문자열 바로 앞을 지움
 */
string& ltrim(string& s, const string& match) {
  s.erase(0, s.find_first_not_of(match));
  return s;
}

/**
 * @brief 문자열 양쪽 지우기
 *
 * @param match 해당하지 않는 문자열 전후를 지움
 */
string& trim(string& s, const string& match) {
  return ltrim(rtrim(s, match), match);
}

/**
 * @deprecated 이거 rtrim으로 안댐?
 *
 * @param match 해당하지 않는 문자열 바로 뒤를 지움
 */
string& erase_from(string& line, const string& match) {
  string::size_type index = line.find(match);

  if (index != string::npos)
    line.erase(index);
  return line;
}
}  // namespace util
