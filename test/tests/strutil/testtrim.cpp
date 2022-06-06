#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/Gen.h>
#include <rapidcheck/gtest.h>

#include <algorithm>
#include <array>
#include <numeric>
#include <util/strutil/strutil.hpp>

using strvec = std::vector<std::string>;

static const std::array whitespaces{" ", "\t", "\n", "\r", "\f", "\v"};

auto gen_whitespace(size_t max_length = 10) -> std::string {
  size_t length = *rc::gen::inRange(0ul, max_length);

  std::string res;
  for (size_t i = 0; i < length; ++i)
    res += *rc::gen::elementOf(whitespaces);
  return res;
};

RC_GTEST_PROP(TrimTest, general, (const std::string &s)) {
  RC_PRE(std::none_of(
      whitespaces.begin(), whitespaces.end(),
      [&](const std::string &ws) { return s.find(ws) != std::string::npos; }));

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

RC_GTEST_PROP(TrimTest, empty, (const std::string &delim)) {
  util::trim_func funcs[3] = {util::ltrim, util::rtrim, util::trim};
  for (auto func : funcs) {
    std::string s = "";
    RC_ASSERT(func(s, delim) == "");
  }
}

RC_GTEST_PROP(eraseTest, general,
              (const std::string &s, const std::string &f)) {
  RC_PRE(!f.empty() && !s.empty() && s.find(f) == std::string::npos);

  std::string tmp1 = s + f, tmp2 = f + s;

  RC_ASSERT(util::erase_from(tmp1, f) == s);
  RC_ASSERT(util::erase_before(tmp2, f) == s);
}
