#ifndef UTIL_LAZYSTRING_HPP
#define UTIL_LAZYSTRING_HPP

#include <string>

namespace util {

/**
 * @brief apply()를 하기 전까지는 문자열의 포인터만 가지고 있다가
 *        apply()를 하는 순간 실제 string으로 적용해주는 클래스
 */
class LazyString : public std::string {
 public:
  /**
   * @brief 새로운 빈 LazyString을 생성한다
   */
  LazyString();

  template <typename T>
  LazyString(const T &str) : std::string(str), start_(NULL), end_(NULL) {}

  // LazyString &operator=(const LazyString &) = default
  // LazyString (const LazyString &) = default
  // ~LazyString() = default

 public:
  /**
   * @brief 문자열을 초기화한다
   */
  void clear();

  /**
   * @brief 만약 start와 end의 값이 설정되어 있다면,
   * 현재 문자열 맨 뒤에 [start, end)범위의 문자열을 덧붙인다.
   */
  void apply();

  /**
   * @brief 추가할 문자열의 시작 부분을 지정한다.
   *
   * @param start 문자열 포인터
   */
  void setStart(const char *start);

  /**
   * @brief 추가할 문자열의 끝 다음 부분을 지정한다.
   *
   * @param end 문자열 포인터
   */
  void setEnd(const char *end);

 private:
  const char *start_;
  const char *end_;
};

}  // namespace util

#endif  // UTIL_LAZYSTRING_HPP
