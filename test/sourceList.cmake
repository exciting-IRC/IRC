set(SRC_LIST
  util/strutil/split.cpp
  util/strutil/trim.cpp
  server/dummy/dummyfunc.cpp
)
list(TRANSFORM SRC_LIST PREPEND ../src/)
