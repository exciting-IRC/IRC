#ifndef UTIL_STRUTIL_JOIN_HPP
#define UTIL_STRUTIL_JOIN_HPP

#include <string>

namespace util {

using std::string;

/**
 * @brief 파이썬의 str.join(이터러블)과 유사한 기능을 제공하는 함수
 *
 * @tparam Container 이터레이터의 원소가 << 출력을 지원하는 컨테이너
 * @param sep 연결 문자열, 기본값 ""
 */
template <typename Container>
string join(const Container& c, const string& sep = "");

}  // namespace util

#include "join.tpp"

#endif  // UTIL_STRUTIL_JOIN_HPP
