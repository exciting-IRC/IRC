#ifndef STRUTIL_STRUTIL_HPP
#define STRUTIL_STRUTIL_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace util {

using std::string;
using std::vector;

enum { MAXSPLIT_ALL = -1 };

extern const string white_space;

string& erase_from(string& line, const string& match);
string& erase_before(string& line, const string& match);
string& ltrim(string& s, const string& match = white_space);
string& rtrim(string& s, const string& match = white_space);
string& trim(string& s, const string& match = white_space);
typedef string& (*trim_func)(string& s, const string& match);

vector<string> split(const string& str, const string& sep = "");

/// stringstream 출력을 지원하는 원소에 한해 문자열로 변경
template <typename T>
string to_string(const T& t) {
  std::stringstream ss;

  ss << t;
  return ss.str();
}

/**
 * @brief 파이썬의 str.join(이터러블)과 유사한 기능을 제공하는 함수
 *
 * @tparam Container 이터레이터의 원소가 << 출력을 지원하는 컨테이너
 * @param sep 연결 문자열, 기본값 ""
 */
template <typename Container>
string join(const Container& c, const string& sep = "");

/// @deprecated prettyprint()로 대신하기
template <typename T>
void print_vector(vector<T> v, bool oneline = true) {
  const string sep = oneline ? ", " : "\n";
  for (typename vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << "'" << *it << "'" << sep;
  }
  std::cout << "\n";
}

}  // namespace util

#include "strutil.tpp"

#endif  // STRUTIL_STRUTIL_HPP
