set(TEST_LIST
  test.cpp
  strutil/teststrutil.cpp
  testFixedBuffer.cpp
)
list(TRANSFORM TEST_LIST PREPEND tests/)
