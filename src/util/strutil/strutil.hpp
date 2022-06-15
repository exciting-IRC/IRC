#ifndef STRUTIL_STRUTIL_HPP
#define STRUTIL_STRUTIL_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "util/strutil/vec_of_any.hpp"

#define FMT(fmt, param) util::format(fmt, VA(param))

namespace util {

using std::string;
using std::vector;

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
string to_string(const T& t);

/// istringstream 에서 변환이 가능한 타입에 한해 해당 타입으로 변환
template <typename T>
T convert_to(const string& from) {
  T val;
  std::istringstream iss(from);

  iss >> val;
  return val;
}

/// 파이썬 Path.read_text와 같은 역할
string read_text(const string& filename);

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

/**
 * @brief 파이썬 str.format()과 비슷한 문자열 포매팅
 *
 * 중괄호 한 쌍마다 args의 인자 하나로 치환됨
 * util::format("{} {foo} {{bar}}", V(("hello", "world")))
 * -> "hello world {{bar}}"
 *
 * @param fmt 문자열 포매팅 형식. 중괄호 이스케이핑: "{{", "}}"
 * @param args 포매팅에 사용할 변수 목록
 */
// TODO: {0}, {1} 등의 순서 지원
class format {
 private:
  string fmt;
  vector<string> args;
  std::string result;
  int arg_index;
  size_t curs;

 private:
  bool on_char(const char c) const;
  bool on_escaped_bracket(const string& match);
  void advance_and_append(const string& to_add);
  void advance_to_closing_bracket();

 public:
  format(const string& fmt, const vector<string>& args);

  string str() const;
  operator string() const;
};

std::ostream& operator<<(std::ostream& os, const format& f);

}  // namespace util

#include "strutil.tpp"

#endif  // STRUTIL_STRUTIL_HPP
