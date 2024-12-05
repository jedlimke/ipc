cmake_minimum_required(VERSION 3.10)
project(IPC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use strict C++17 compliance

# Include headers (/include)
include_directories(include)



### GOOGLE TEST for testing
# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)

# Make GoogleTest available to the project
FetchContent_MakeAvailable(googletest)



### PROTOBUF for serialization
# Find Protobuf library
find_package(Protobuf REQUIRED)

# Define proto files
set(PROTO_FILES proto/ipc_data.proto)

# Set Protobuf options
set(PROTOC_OPTIONS "--experimental_allow_proto3_optional")

# Generate Protobuf sources and headers using add_custom_command
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.h
  COMMAND ${Protobuf_PROTOC_EXECUTABLE}
  ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR} ${PROTOC_OPTIONS} -I ${CMAKE_SOURCE_DIR}/proto ${CMAKE_SOURCE_DIR}/proto/ipc_data.proto
  DEPENDS ${PROTO_FILES}
  COMMENT "Generating Protobuf files"
)

# Add the generated Protobuf files to a library
add_library(proto_lib ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.h)
target_include_directories(proto_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto_lib PUBLIC protobuf::libprotobuf)



### COMMON LIBRARY
# rx_lib is a shared library containing the app's core logic (src/rx.cpp)
# It links with the Protobuf library to handle serialization
add_library(rx_lib src/rx.cpp)
target_link_libraries(rx_lib proto_lib rt protobuf::libprotobuf)
target_include_directories(rx_lib PUBLIC include)



### RX APPLICATION
# The main executable for the Rx app
# src/main_rx.cpp is the entry point
add_executable(rx src/main_rx.cpp)
# Link the Rx application with the shared rx_lib.
target_link_libraries(rx rx_lib)



### TESTING SETUP
# The test_rx executable is used to run unit tests for the Rx app
add_executable(test_rx tests/rx.cpp)
# Link the test_rx executable with:
# - rx_lib for application logic
# - gtest and gtest_main for the testing framework
# - pthread for threading (required by GoogleTest)
target_link_libraries(test_rx rx_lib gtest gtest_main pthread)
