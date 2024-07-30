project(tests)

enable_testing()

# Download gtest binary
include(FetchContent)
FetchContent_Declare(gtest
        URL https://github.com/google/googletest/archive/release-1.11.0.tar.gz
)
# configure build of googletest
FetchContent_MakeAvailable(gtest)

add_executable(scsunit scstest-unit.cpp)
target_link_libraries(scsunit scs)
target_link_libraries(scsunit gtest_main)
