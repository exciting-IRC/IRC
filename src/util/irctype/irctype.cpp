#include <algorithm>
#include <cctype>

#include "util/FixedBuffer/array.hpp"

namespace util {

bool isLetter(const char c) {
  return std::isalpha(static_cast<unsigned char>(c));
}

bool isDigit(const char c) {
  return std::isdigit(static_cast<unsigned char>(c));
}

bool isSeprator(const char c) { return c == ':' || c == ' '; }

bool isEol(const char c) { return c == '\r' || c == '\n' || c == '\0'; }

bool isSpecial(const char c) {
  const static util::array<char, 9> special = {'[', ']', '\\', '`', '_',
                                               '^', '{', '|',  '}'};
  return std::find(special.begin(), special.end(), c) != special.end();
}

bool isRegular(const char c) { return not isSeprator(c) and not isEol(c); }

/**
 * @brief '#' (공식 채널) 또는 '&' (사설 채널)
 */
bool isChannelPrefix(char c) { return c == '#' || c == '&'; }

}  // namespace util
