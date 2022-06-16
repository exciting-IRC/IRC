#include <cctype>

namespace util {

bool isLetter(char c) { return std::isalpha(c); }

bool isDigit(char c) { return std::isdigit(c); }

bool isSeprator(char c) { return c == ':' || c == ' '; }

bool isSpecial(char c) { return c == '\r' || c == '\n' || c == '\0'; }

bool isRegular(char c) { return !isSeprator(c) && !isSpecial(c); }

}  // namespace util
