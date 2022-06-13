#include <iostream>
#include <string>
#include <util/FixedBuffer/FixedBuffer.hpp>
#include <util/strutil/strutil.hpp>
#include <vector>

#include "container_of.hpp"

int main() {
  using namespace std;
  util::print_vector(V(("hello", "world", "bye", "world")));
}
