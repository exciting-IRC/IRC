#include <fstream>
#include <string>
#include <util/strutil/strutil.hpp>

namespace util {
using std::string;

template <>
string to_string(const string& t) {
  return t;
}

}  // namespace util
