include(utils.cmake)

CreateSourceFor(util/strutil
  split
  trim
)

set(SRC_LIST
  ${SRC_util/strutil}
)
