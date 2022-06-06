# - 리스트 원소에 접두사와 접미사 붙이기.
#
# SubDirFilesToList(  [ name <생성된 리스트를 담을 변수명> ]
#                     [ prefix <접두사> ]
#                     [ suffix <접미사> ]
#                     [ ARGV <합칠 리스트 목록> ]
# )
function(SubDirFilesToList name prefix suffix)
  list(SUBLIST ARGV 3 ${ARGC} args)

  set(${name} ${args})
  list(TRANSFORM ${name} PREPEND ${prefix})
  list(TRANSFORM ${name} APPEND .cpp)
  set(${name} ${${name}} PARENT_SCOPE)
endfunction()

# - 디렉토리 내 파일 목록을 TEST_{디렉토리} 변수에 담기.
#
# CreateTestFor(  [ name <생성된 리스트를 TEST_{name} 변수에 담기> ]
#                 [ ARGV <테스트에 추가할 name/{원소} .cpp 파일 (확장자 제외)> ]
# )
#
# 예시) CreateTestFor(FixedBuffer
#         testarray
#         testFixedBuffer
#       )
#
# -> ${TEST_FixedBuffer} === FixedBuffer/testarray.cpp;FixedBuffer/testFixedBuffer.cpp
function(CreateTestFor name)
  list(SUBLIST ARGV 1 ${ARGC} args)

  SubDirFilesToList(TEST_${name} ${name}/ .cpp ${args})
  set(TEST_${name} ${TEST_${name}} PARENT_SCOPE)
endfunction()

# - 디렉토리 내 파일 목록을 SRC_{디렉토리} 변수에 담기.
#
# CreateSourceFor(  [ name <생성된 리스트를 SRC_{name} 변수에 담기> ]
#                 [ ARGV <테스트에 추가할 name/{원소} .cpp 파일 (확장자 제외)> ]
# )
#
# 예시) CreateSourceFor(util/strutil
#         split
#         trim
# )
#
# -> ${SRC_util/strutil} === util/strutil/split.cpp;util/strutil/trim.cpp
function(CreateSourceFor name)
  list(SUBLIST ARGV 1 ${ARGC} args)

  SubDirFilesToList(SRC_${name} ${name}/ .cpp ${args})
  set(SRC_${name} ${SRC_${name}} PARENT_SCOPE)
endfunction()

# - 여러 리스트를 하나로 합치기.
#
# 파이썬 itertools.chain()과 비슷함
#
# ChainListsTo(  [ name <생성된 리스트를 담을 변수명> ]
#                [ ARGV <합칠 리스트 목록> ]
# )
#
# 예시) ChainListsTo(foo A;B;C D;E;F) -> ${foo} === A;B;C;D;E;F
function(ChainListsTo name)
list(SUBLIST ARGV 1 ${ARGC} lists)

  foreach(list ${lists})
    list(APPEND ${name} ${${list}})
  endforeach()

  set(${name} ${${name}} PARENT_SCOPE)
endfunction()

# - 테스트 목록을 합친 TEST_LIST 만들기.
#
# CreateTEST_LISTFrom(
#         [ ARGV <TEST_LIST에 담을 테스트 리스트 목록> ]
# )
#
# 예시) CreateTEST_LISTFrom(
#        TEST_FixedBuffer
#        TEST_strutil
#      )
function(CreateTEST_LISTFrom)
  ChainListsTo(TEST_LIST ${ARGV})
  list(TRANSFORM TEST_LIST PREPEND tests/)
  set(TEST_LIST ${TEST_LIST} PARENT_SCOPE)
endfunction()

# - 소스 파일 목록을 합친 SRC_LIST 만들기.
#
# CreateSRC_LISTFrom(
#         [ ARGV <SRC_LIST 담을 테스트 리스트 목록> ]
# )
#
# 예시) CreateSRC_LISTFrom(
#        SRC_util/strutil
#        SRC_util/strutil
#      )
function(CreateSRC_LISTFrom)
  ChainListsTo(SRC_LIST ${ARGV})
  list(TRANSFORM SRC_LIST PREPEND ../src/)
  set(SRC_LIST ${SRC_LIST} PARENT_SCOPE)
endfunction()
