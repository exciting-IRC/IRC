cmake_minimum_required(VERSION 3.11)
project(test
        DESCRIPTION "C++ Property-based Testing Example (w/ RapidCheck)"
        HOMEPAGE_URL https://github.com/exciting-IRC/IRC
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Werror -pedantic)

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

# target_link_libraries(test googletest rapidcheck)

enable_testing()

add_executable(
  irc_test
  test.cpp
  ../dummyfunc.cpp
)
target_link_libraries(
  irc_test
  gtest_main
  rapidcheck
)

include(GoogleTest)
gtest_discover_tests(irc_test)
