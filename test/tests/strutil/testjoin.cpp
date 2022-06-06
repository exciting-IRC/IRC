#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include <algorithm>
#include <numeric>
#include <util/strutil/strutil.hpp>

using strvec = std::vector<std::string>;

RC_GTEST_PROP(JoinTest, general, (const strvec &v, const std::string &delim)) {
  RC_PRE(!v.empty());
  const size_t expected_length =
      std::max(v.size() - 1, 0UL) * delim.length() +
      std::accumulate(
          v.begin(), v.end(), 0,
          [](size_t acc, const std::string &s) { return acc + s.size(); });

  const std::string joined = util::join(v, delim);
  RC_ASSERT(joined.length() == expected_length);
}

RC_GTEST_PROP(JoinTest, empty, (const std::string &delim)) {
  strvec v = {};
  const std::string joined = util::join(v, delim);
  RC_ASSERT(joined.empty());
}
