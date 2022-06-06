#include <iostream>
#include <util/strutil/strutil.hpp>
#include <vector>
#include <string>

int main() {
  using namespace std;
  vector<string> v = util::split("a,b,c", ",");
  util::print_vector(v);
}
