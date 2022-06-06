# Reference: https://cliutils.gitlab.io/modern-cmake

# Version
cmake_minimum_required(VERSION 3.18...3.23)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Project setup
project(
  TestIrc
  DESCRIPTION "property based tests for IRC subject"
  HOMEPAGE_URL https://github.com/exciting-IRC/IRC
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

include(InstallDependency.cmake)
include(testList.cmake)
include(sourceList.cmake)

# Create Executable
add_executable(irc_test
  ${SRC_LIST}
  ${TEST_LIST}
)

target_include_directories(
  irc_test PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
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

# googletests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()
include(GoogleTest)
gtest_discover_tests(irc_test)

# https://int-i.github.io/cpp/2021-08-14/cpp-codecov/
# https://int-i.github.io/github/2021-12-19/github-actions-workflow-dispatch-for-benchmarking/
