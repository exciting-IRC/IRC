#ifndef UTIL_IRCTYPE_HPP
#define UTIL_IRCTYPE_HPP

namespace util {

/**
 * @brief [A-Za-z]
 */
bool isLetter(char c);

/**
 * @brief [0-9]
 */
bool isDigit(char c);

/**
 * @brief ':'(colon) 또는 ' '(space)
 */
bool isSeprator(char c);

/**
 * @brief '\\r'(CR) 또는 '\\n'(LF) 또는 '\\0'(NUL)
 */
bool isEol(char c);

/**
 * @brief 다음 중 하나: "[]\`_^{|}"
 */
bool isSpecial(const char c);

/**
 * @brief 다음을 제외한 문자들
 *  '\\r' (CR),
 *  '\\n' (LF),
 *  '\\0' (NUL),
 *  ':'   (colon),
 *  ' '   (space)
 */
bool isRegular(char c);

bool isChannelPrefix(char c);

}  // namespace util

#endif  // UTIL_IRCTYPE_HPP
