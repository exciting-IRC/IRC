#include <fstream>
#include <string>
#include <util/strutil/strutil.hpp>

namespace util {
using std::string;

string read_text(const string& filename) {
  std::ifstream ifs(filename);
  std::stringstream buffer;

  buffer << ifs.rdbuf();
  return buffer.str();
}

template <>
string to_string(const string& t) {
  return t;
}

}  // namespace util
