#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <util/FixedBuffer/FixedBuffer.hpp>
#include <util/config/config.hpp>
#include <util/strutil/strutil.hpp>
#include <utility>
#include <vector>

#include "container_of.hpp"
using namespace std;


int main(int argc, const char *argv[]) {
  using namespace util;
  (void)argc;
  (void)argv;
  // if (argc != 3) {
  //   std::cout << "Usage: " << argv[0] << " <input_file> <output_file>"
  //             << std::endl;
  //   return 1;
  // }
  // util::print_vector(V(("hello", "world", "bye", "world")));
  Config config = Config::from("config.yml");
  cout << config << endl;
}
