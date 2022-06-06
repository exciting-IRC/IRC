#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include <algorithm>
#include <array>
#include <string>
#include <util/FixedBuffer/array.hpp>
#include <vector>

RC_GTEST_PROP(ArrayTest, utilArrayProperties, (const std::array<int, 10> &l0)) {
  util::array<int, 10> l1;

  std::copy(begin(l0), end(l0), l1.begin());
  for (int i = 0; i < 10; ++i) {
    RC_ASSERT(l0[i] == l1[i]);
    RC_ASSERT(l0.at(i) == l1.at(i));
  }
  // methods should be same
  RC_ASSERT(l0.size() == l1.size());
  RC_ASSERT(l0.max_size() == l1.max_size());
  RC_ASSERT(std::distance(l0.begin(), l0.end()) ==
            std::distance(l1.begin(), l1.end()));
  RC_ASSERT(std::distance(l0.rbegin(), l0.rend()) ==
            std::distance(l1.rbegin(), l1.rend()));
  RC_ASSERT(l1.data() + l1.size() == l1.end());
  RC_ASSERT(l1.rbegin() + l1.size() == l1.rend());
  RC_ASSERT(l1.begin() == l1.data());
  RC_ASSERT(l1.front() == l1.at(0));
  RC_ASSERT_THROWS_AS(l1.at(10), std::out_of_range);
}

