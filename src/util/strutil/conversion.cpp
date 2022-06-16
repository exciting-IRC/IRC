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

string to_upper(string str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

string to_lower(string str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

}  // namespace util
