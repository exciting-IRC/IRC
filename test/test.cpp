#include <gtest/gtest.h>
#include <rapidcheck.h>

#include <algorithm>
#include <vector>

// function to test
int addi(int a, int b);

// not covered by tests
float addf(float a, float b);

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);

  EXPECT_EQ(addi(1, 2), 3);
  // Should fail
//   EXPECT_NE(7 * 6, 42);

  rc::check("double reversal yields the original value",
            [](const std::vector<int> &l0) {
              auto l1 = l0;
              std::reverse(begin(l1), end(l1));
              std::reverse(begin(l1), end(l1));
              RC_ASSERT(l0 == l1);
            });
}

// TODO: use RC_GTEST_PROP
