#ifndef UTIL_STRUTIL_JOIN_TPP
#define UTIL_STRUTIL_JOIN_TPP

#include <sstream>
#include <util/strutil/join.hpp>
namespace util {

template <typename Container>
string join(const Container& c, const string& sep) {
  std::stringstream ss;

  for (size_t i = 0; i < c.size(); ++i) {
    if (i != 0)
      ss << sep;
    ss << c[i];
  }
  return ss.str();
}

}  // namespace util

#endif  // UTIL_STRUTIL_JOIN_TPP
