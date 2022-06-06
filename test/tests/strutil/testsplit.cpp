#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include <algorithm>
#include <numeric>
#include <util/strutil/strutil.hpp>

using strvec = std::vector<std::string>;


RC_GTEST_PROP(SplitTest, general, (const strvec &v, const std::string &delim)) {
  RC_PRE(!delim.empty());
  RC_PRE(std::none_of(v.begin(), v.end(), [&](const std::string &s) {
    return s.find(delim) != std::string::npos;
  }));

  std::string joined = util::join(v, delim);

  strvec result = util::split(joined, delim);
  RC_ASSERT(util::join(result, delim) == joined);
  if (v.empty())
    RC_ASSERT(result.size() == 1u && result[0].empty());
  else
    RC_ASSERT(result == v);
}

TEST(SplitTest, empty) {
  {
    strvec expected{""};

    ASSERT_EQ(util::split("       "), expected);
    ASSERT_EQ(util::split(""), expected);
    ASSERT_EQ(util::split(" 		  			"), expected);
  }
  {
    strvec expected{"a", "b", "c"};
    ASSERT_EQ(util::split("a b c"), expected);
    ASSERT_EQ(util::split("a   b    c"), expected);
  }
  { ASSERT_EQ(util::split("a"), strvec{"a"}); }
}

TEST(SplitTest, edgeCases) {
  {
    strvec expected{"a", "b", "c"};
    for (const auto &kv : std::map<std::string, std::string>{
             {"a b c", " "},
             {"a,b,c", ","},
             {"a!!b!!c", "!!"},
             {"a->b->c", "->"},
         })
      ASSERT_EQ(util::split(kv.first, kv.second), expected);
  }
  {
    std::string tmp = "a!!b!!c";
    ASSERT_EQ(util::split(tmp, "->"), strvec{tmp});
  }
  {
    strvec expected{"a", "", "b", "c"};
    ASSERT_EQ(util::split("a,,b,c", ","), expected);
    ASSERT_EQ(util::split("a->->b->c", "->"), expected);
  }
}
