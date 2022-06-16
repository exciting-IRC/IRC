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
 * @brief ':'(colon) or ' '(space)
 */
bool isSeprator(char c);

/**
 * @brief '\\r'(CR) or '\\n'(LF) or '\\0'(NUL)
 */
bool isSpecial(char c);

/**
 * @brief every character except
 *  '\\r' (CR),
 *  '\\n' (LF),
 *  '\\0' (NUL),
 *  ':'   (colon),
 *  ' '   (space)
 */
bool isRegular(char c);

}

#endif  // UTIL_IRCTYPE_HPP
