#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include <FixedBuffer/array.hpp>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

// function to test
int addi(int a, int b);

// not covered by tests, will be shown on code coverage report
float addf(float a, float b);

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
  EXPECT_EQ(addi(1, 2), 3);
  EXPECT_NE(7 * 6, 41);
}

// Using rapidcheck gtest extension.
RC_GTEST_PROP(VectorTest, vectorDoubleInversion, (const std::vector<int> &l0)) {
  auto l1 = l0;

  std::reverse(begin(l1), end(l1));
  std::reverse(begin(l1), end(l1));
  // try chaning to != to see how failure result looks like
  RC_ASSERT(l0 == l1);
}

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
  RC_ASSERT(l0.empty() == l1.empty());
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

// TODO: use RC_GTEST_PROP
