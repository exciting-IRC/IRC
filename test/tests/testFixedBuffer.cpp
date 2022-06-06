#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include <algorithm>
#include <util/FixedBuffer/FixedBuffer.hpp>

RC_GTEST_PROP(FixedBufferTest, defaultConstructor, ()) {
  const auto buffer1 = util::FixedBuffer<char, 1>();
  RC_ASSERT((buffer1.size() == 1));

  const auto buffer10 = util::FixedBuffer<char, 10>();
  RC_ASSERT((buffer10.size() == 10));

  const auto buffer42 = util::FixedBuffer<char, 42>();
  RC_ASSERT((buffer42.size() == 42));

  const auto buffer100 = util::FixedBuffer<char, 100>();
  RC_ASSERT((buffer100.size() == 100));

  RC_ASSERT((buffer100.tellg() == 0));
  RC_ASSERT((buffer100.tellp() == 0));
}

RC_GTEST_PROP(FixedBufferTest, peekPop, (const std::array<char, 100> &l0)) {
  auto l1 = l0;

  util::FixedBuffer<char, 100> buf;
  std::copy(l1.begin(), l1.end(), buf.begin());

  for (int i = 0; i < 100; ++i) {
    RC_ASSERT((buf.peek() == l1[i]));
    RC_ASSERT((buf.pop() == l1[i]));
  }
}

RC_GTEST_PROP(FixedBufferTest, seekg, (const std::array<char, 100> &l0)) {
  auto l1 = l0;

  util::FixedBuffer<char, 100> buf;
  std::copy(l1.begin(), l1.end(), buf.begin());

  int pos = *rc::gen::inRange(0, 100);
  buf.seekg(pos);
  for (int i = pos; i < 100; ++i) {
    RC_ASSERT((buf.peek() == l1[i]));
    RC_ASSERT((buf.pop() == l1[i]));
  }
}

RC_GTEST_PROP(FixedBufferTest, tellg, (const std::array<char, 100> &l0)) {
  auto l1 = l0;

  util::FixedBuffer<char, 100> buf;
  std::copy(l1.begin(), l1.end(), buf.begin());

  int pos = *rc::gen::inRange(0, 100);
  buf.seekg(pos);
  for (std::size_t i = pos; i < 100; ++i) {
    RC_ASSERT((i == buf.tellg()));
    buf.pop();
  }
}
