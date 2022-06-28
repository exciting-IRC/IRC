#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "util/strutil/format.hpp"

namespace util {
using std::string;

string read_text(const string& filename) {
  std::ifstream ifs(filename.c_str());
  std::stringstream buffer;

  if (ifs.fail())
    throw std::invalid_argument(FMT("file not found: {}", (filename)));

  buffer << ifs.rdbuf();
  return buffer.str();
}

}  // namespace util
