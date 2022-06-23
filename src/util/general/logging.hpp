#ifndef UTIL_GENERAL_LOGGING_HPP
#define UTIL_GENERAL_LOGGING_HPP

#include <iostream>
#include <string>
#include <vector>

namespace util {
using std::string;
using std::vector;

/// 타임스탬프와 오류 여부, 보내고 싶은 메시지를 stdout에 출력하기
void debug(const string& msg, bool ok = true);

/// 간단하게 하이라이트된 메시지를 출력하기
void debug_info(const string& msg, const string& highlight, bool ok = true);

/// 메시지를 받았을 때
void debug_input(const string& command, bool ok = true);

/// 메시지를 보낼 때
void debug_output(const string& payload, bool ok = true);

}  // namespace util

#endif  // UTIL_GENERAL_LOGGING_HPP
