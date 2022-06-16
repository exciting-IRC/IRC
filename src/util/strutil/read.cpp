#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace util {
using std::string;

string read_text(const string& filename) {
  std::ifstream ifs(filename);
  std::stringstream buffer;

  buffer << ifs.rdbuf();
  return buffer.str();
}

}  // namespace util
