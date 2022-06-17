#ifndef UTIL_STRUTIL_FORMAT_HPP
#define UTIL_STRUTIL_FORMAT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "util/vargs/vec_of_any.hpp"

#define FMT(fmt, param) util::format(fmt, VA(param))
#define COUT_FMT(fmt, param) std::cout << FMT(fmt, param) << "\n"
#define CERR_FMT(fmt, param) std::cerr << FMT(fmt, param) << "\n"

namespace util {
using std::string;
using std::vector;

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
  size_t arg_index;
  size_t curs;

 private:
  bool on_char(const char c) const;
  bool on_escaped_bracket(const string& match);
  void advance_and_append(const string& to_add);
  void advance_to_closing_bracket();
  void insert_arg();              //< 인자 삽입후 다음 인자로 이동
  void insert_arg(size_t index);  //< 인덱스의 인자 삽입 후 이동 없음

 public:
  format(const string& fmt, const vector<string>& args);

  string str() const;
  operator string() const;
};

std::ostream& operator<<(std::ostream& os, const format& f);

}  // namespace util

#endif  // UTIL_STRUTIL_FORMAT_HPP
