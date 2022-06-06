#ifndef STRUTIL_HPP
#define STRUTIL_HPP

#include <util/strutil/strutil.hpp>

namespace util {

/**
 * @brief 파이썬의 str.join(이터러블)과 유사한 기능을 제공하는 함수
 *
 * @tparam Container 이터레이터의 원소가 << 출력을 지원하는 컨테이너
 * @param sep 연결 문자열, 기본값 ""
 */
template <typename Container>
string join(const Container& c, const string& sep) {
  std::stringstream ss;

  for (size_t i = 0; i < c.size(); ++i) {
    if (i != 0)
      ss << sep;
    ss << c[i];
  }
  return ss.str();
}


}  // namespace util

#endif // STRUTIL_HPP
