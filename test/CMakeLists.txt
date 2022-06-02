cmake_minimum_required(VERSION 3.11)
project(test
        DESCRIPTION "C++ Property-based Testing Example (w/ RapidCheck)"
        HOMEPAGE_URL https://github.com/exciting-IRC/IRC
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_CXX_FLAGS
#   "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -fprofile-instr-generate -fcoverage-mapping"
# ) # I think it applies these flags to libraries as well

# target_compile_options(
#   -Wall -Wextra -Werror -pedantic
#   -fprofile-instr-generate -fcoverage-mapping
# )
# For some reason they doesn't work

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
# FetchContent_Declare(
#     codecov
#     GIT_REPOSITORY https://github.com/RWTH-HPC/CMake-codecov
#     GIT_TAG        master
# )

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest rapidcheck) #codecov)

# include(codecov_SOURCE_DIR)
# target_link_libraries(test googletest rapidcheck)
# find_package(codecov)

# googletests
enable_testing()

add_executable(
  irc_test
  test.cpp
  ../dummyfunc.cpp
)

target_compile_options(
  irc_test PRIVATE
  -Wall -Wextra -Werror -pedantic -fprofile-instr-generate -fcoverage-mapping
)

target_link_libraries(
  irc_test
  gtest_main
  rapidcheck
)

include(GoogleTest)
gtest_discover_tests(irc_test)

# https://int-i.github.io/cpp/2021-08-14/cpp-codecov/
# https://int-i.github.io/github/2021-12-19/github-actions-workflow-dispatch-for-benchmarking/
