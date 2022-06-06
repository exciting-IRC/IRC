include(utils.cmake)

CreateTestFor(FixedBuffer
  testarray
  testFixedBuffer
)

CreateTestFor(strutil
  testjoin
  testsplit
  testtrim
)

CreateTEST_LISTFrom(
  TEST_FixedBuffer
  TEST_strutil
)
