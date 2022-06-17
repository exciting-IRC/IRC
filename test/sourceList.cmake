include(utils.cmake)

CreateSourceFor(util/strutil
  conversion
  format
  read
  split
  trim
)

CreateSRC_LISTFrom(
  SRC_util/strutil
)
