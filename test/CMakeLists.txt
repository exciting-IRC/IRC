cmake_minimum_required(VERSION 3.11)
project(test
        DESCRIPTION "C++ Property-based Testing Example (w/ RapidCheck)"
        HOMEPAGE_URL https://github.com/exciting-IRC/IRC
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)


include(FetchContent)
FetchContent_Declare(
    rapidcheck
    GIT_REPOSITORY https://github.com/emil-e/rapidcheck.git
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        main
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest rapidcheck)
add_subdirectory(${rapidcheck_SOURCE_DIR}/extras/gtest)

# googletests
enable_testing()

add_executable(
  irc_test
  test.cpp
  testFixedBuffer.cpp
  ../dummyfunc.cpp
)
target_link_libraries(
  irc_test
  rapidcheck_gtest
  rapidcheck
  gtest_main
)
target_compile_options(
  irc_test PRIVATE
  -Wall -Wextra -Werror -pedantic -fprofile-instr-generate -fcoverage-mapping
)
target_link_options(
  irc_test PRIVATE
  -O0 -fprofile-instr-generate -fcoverage-mapping
)
target_include_directories(
  irc_test PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

include(GoogleTest)
gtest_discover_tests(irc_test)

# https://int-i.github.io/cpp/2021-08-14/cpp-codecov/
# https://int-i.github.io/github/2021-12-19/github-actions-workflow-dispatch-for-benchmarking/
