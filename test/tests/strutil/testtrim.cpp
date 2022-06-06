#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/Gen.h>
#include <rapidcheck/gtest.h>

#include <algorithm>
#include <array>
#include <numeric>
#include <util/strutil/strutil.hpp>

using strvec = std::vector<std::string>;

RC_GTEST_PROP(TrimTest, general, (const std::string &s)) {
  static const std::array whitespaces{" ", "\t", "\n", "\r", "\f", "\v"};

  RC_PRE(std::none_of(
      whitespaces.begin(), whitespaces.end(),
      [&](const std::string &ws) { return s.find(ws) != std::string::npos; }));
  const auto gen_whitespace = [&](size_t max_length = 100) {
    size_t length = *rc::gen::inRange(0ul, max_length);

    std::string res;
    for (size_t i = 0; i < length; ++i)
      res += *rc::gen::elementOf(whitespaces);
    return res;
  };
  const std::string lws = gen_whitespace(), rws = gen_whitespace();
  const auto gen_test_strings = [&]() {
    return std::array<std::string, 4>{lws + s, s + rws, lws + s + rws, s};
  };

  {
    auto [l, r, a, s] = gen_test_strings();
    RC_ASSERT(util::ltrim(l) == s);
    RC_ASSERT(util::ltrim(r) == r);
    RC_ASSERT(util::ltrim(a) == r);
    RC_ASSERT(util::ltrim(s) == s);
  }
  {
    auto [l, r, a, s] = gen_test_strings();
    RC_ASSERT(util::rtrim(l) == l);
    RC_ASSERT(util::rtrim(r) == s);
    RC_ASSERT(util::rtrim(a) == l);
    RC_ASSERT(util::rtrim(s) == s);
  }
  {
    for (auto t : gen_test_strings())
      RC_ASSERT(util::trim(t) == s);
  }
}
