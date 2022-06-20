#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include <stack>
#include <util/strutil/strutil.hpp>

using strvec = std::vector<std::string>;

TEST(FormatTest, edgeCases) {
  RC_ASSERT(FMT("{foo} {} {{}}", ("hello", 3)) == "hello 3 {}");
  for (auto fmt : strvec{"{", "}", "{}"})
    RC_ASSERT_THROWS_AS(FMT(fmt, ()), std::invalid_argument);
  RC_ASSERT(FMT("hello {0} {0} {0}", ("world")) == "hello world world world");
}

auto create_fmtbrace() -> std::string {
  const std::string candidate = *rc::gen::arbitrary<std::string>();

  if (candidate.find("{") != std::string::npos or
      candidate.find("}") != std::string::npos)
    return "{}";

  return "{" + candidate + "}";
}

RC_GTEST_PROP(FormatTest, general, (const strvec& v)) {
  RC_PRE(std::none_of(v.begin(), v.end(), [](const std::string& s) {
    return s.find("{") != std::string::npos;
  }));
  RC_PRE(std::none_of(v.begin(), v.end(), [](const std::string& s) {
    return s.find("}") != std::string::npos;
  }));

  std::string result = [&]() {
    std::string s;
    for (auto e : v)
      s += e + e;
    return s;
  }();
  std::string fmt = [&]() {
    std::string s;
    for (auto e : v)
      s += e + create_fmtbrace();
    return s;
  }();
  ASSERT_EQ(util::format(fmt, v), result);
}
