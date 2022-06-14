#ifndef UTIL_GENERAL_RESULT_HPP
#define UTIL_GENERAL_RESULT_HPP

struct return_t {
  enum e { kOK = 0, kError = -1 };
};

// template <typename T, typename E>
// struct result {
//   T value;
//   E error;
// };

#endif  // UTIL_GENERAL_RESULT_HPP
