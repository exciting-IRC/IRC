#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <util/strutil/strutil.hpp>

namespace util {
using std::string;

template <>
string to_string(const string& t) {
  return t;
}

template <>
uint16_t convert_to(const string& from) {
  if (not util::all_of(from.begin(), from.end(), util::isDigit))
    throw std::invalid_argument("invalid number " + from);

  FUNCTOR(bool, is_valid, (const std::string& str, char* end_str), {
    return not(errno == ERANGE or *end_str != '\0' or end_str == str.c_str());
  });

  FUNCTOR(bool, in_range, (long rep), { return 0 <= rep and rep <= 65535; });

  errno = 0;
  const char* s = from.c_str();
  char* end_str;
  long rep = std::strtol(s, &end_str, 10);
  if (not is_valid(from, end_str) or not in_range(rep))
    throw std::invalid_argument("invalid number " + from);
  return static_cast<uint16_t>(rep);
}

string to_upper(string str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

string to_lower(string str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

}  // namespace util
