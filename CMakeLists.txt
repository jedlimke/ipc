cmake_minimum_required(VERSION 3.10)
project(IPC)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)
# Fetch and configure GoogleTest
FetchContent_MakeAvailable(googletest)

# Rx app executables
add_executable(rx src/main_rx.cpp src/rx.cpp)

# Link required libraries for Rx
target_link_libraries(rx rt)

# Test executable
add_executable(test_rx tests/rx.cpp src/rx.cpp)

# Link GoogleTest to test executable
target_link_libraries(test_rx gtest gtest_main pthread)