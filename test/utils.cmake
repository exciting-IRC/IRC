# Works similar to python's itertools.chain()
# name: output variable name to bind the merged list
# ARGV: List of Lists
function(ChainListsTo name)
list(SUBLIST ARGV 1 ${ARGC} lists)

  foreach(list ${lists})
    list(APPEND ${name} ${${list}})
  endforeach()

  set(${name} ${${name}} PARENT_SCOPE)
endfunction()

# ARGV: list of .cpp files without extension
function(ListSubDirFiles name prefix suffix)
  list(SUBLIST ARGV 3 ${ARGC} args)

  set(${name} ${args})
  list(TRANSFORM ${name} PREPEND ${prefix})
  list(TRANSFORM ${name} APPEND .cpp)
  set(${name} ${${name}} PARENT_SCOPE)
endfunction()

# name: subdirectory to look for
# ARGV: list of .cpp files without extension
function(CreateTestFor name)
  list(SUBLIST ARGV 1 ${ARGC} args)

  ListSubDirFiles(TEST_${name} ${name}/ .cpp ${args})
  set(TEST_${name} ${TEST_${name}} PARENT_SCOPE)
endfunction()

# name: subdirectory to look for
# ARGV: list of .cpp files without extension
function(CreateSourceFor name)
  list(SUBLIST ARGV 1 ${ARGC} args)

  ListSubDirFiles(SRC_${name} ../src/${name}/ .cpp ${args})
  set(SRC_${name} ${SRC_${name}} PARENT_SCOPE)
endfunction()

# ARGV: List of tests
function(CreateTEST_LISTFrom)
  ChainListsTo(TEST_LIST ${ARGV})
  list(TRANSFORM TEST_LIST PREPEND tests/)
  set(TEST_LIST ${TEST_LIST} PARENT_SCOPE)
endfunction()
