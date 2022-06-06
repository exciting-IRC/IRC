set(TEST_LIST
  test.cpp
  testFixedBuffer.cpp
  strutil/testjoin.cpp
  strutil/testsplit.cpp
  strutil/testtrim.cpp
)
list(TRANSFORM TEST_LIST PREPEND tests/)
