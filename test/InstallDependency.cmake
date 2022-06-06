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

FetchContent_MakeAvailable(googletest rapidcheck)
add_subdirectory(${rapidcheck_SOURCE_DIR}/extras/gtest)
