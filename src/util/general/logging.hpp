#ifndef UTIL_GENERAL_LOGGING_HPP
#define UTIL_GENERAL_LOGGING_HPP

#include <iostream>
#include <string>
#include <vector>

#include "util/color.hpp"

namespace util {
using std::string;
using std::vector;

/// 타임스탬프와 오류 여부, 보내고 싶은 메시지를 stdout에 출력하기
void debug(const string& msg, bool ok = true);

/// 메시지를 받았을 때
void debug_input(const string& command, bool ok = true);

/// 메시지를 보낼 때
void debug_output(const string& payload, bool ok = true);

/// 간단하게 하이라이트된 메시지를 출력하기
template <typename T>
void debug_info(const string& msg, const T& highlight, bool ok = true) {
  debug(FMT("{s}{msg} {m}{hl}", (ok ? HGRN : HRED, msg, BHMAG, highlight)), ok);
}

}  // namespace util

#endif  // UTIL_GENERAL_LOGGING_HPP
