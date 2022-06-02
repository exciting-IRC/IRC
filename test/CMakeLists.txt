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
    GIT_TAG 8fafda42e732164db58003e542196e94a28481f9) # latest master
FetchContent_MakeAvailable(rapidcheck)

add_executable(test test.cpp)

target_include_directories(test PRIVATE
                           ${rapidcheck_SOURCE_DIR}/include)
target_link_libraries(test rapidcheck)
