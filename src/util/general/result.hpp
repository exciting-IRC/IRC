#ifndef UTIL_GENERAL_RESULT_HPP
#define UTIL_GENERAL_RESULT_HPP

/// 시스템 콜 함수 결과값에 대한 추상화
struct return_t {
  enum e { kOK = 0, kError = -1 };
};

// template <typename T, typename E>
// struct result {
//   T value;
//   E error;
// };

#endif  // UTIL_GENERAL_RESULT_HPP
