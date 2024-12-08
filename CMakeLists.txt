cmake_minimum_required(VERSION 3.10)
project(IPC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use strict C++17 compliance

# Include shared headers
include_directories(shared)



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
# Manually specify Protobuf paths that correspond to the specific version we installed, e.g. 3.15.8
set(Protobuf_INCLUDE_DIR /usr/local/include)
set(Protobuf_LIBRARIES /usr/local/lib/libprotobuf.so)
set(Protobuf_PROTOC_EXECUTABLE /usr/local/bin/protoc)

# Include Protobuf directories
include_directories(${Protobuf_INCLUDE_DIR})
link_directories(/usr/local/lib)

# Find Protobuf library
find_package(Protobuf REQUIRED)

# Define proto files
set(PROTO_FILES shared/proto/ipc_data.proto)

# Set Protobuf options
set(PROTOC_OPTIONS "--experimental_allow_proto3_optional")

# Generate Protobuf sources and headers using add_custom_command
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.h
  COMMAND ${Protobuf_PROTOC_EXECUTABLE}
  ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR} ${PROTOC_OPTIONS} -I ${CMAKE_SOURCE_DIR}/shared/proto ${CMAKE_SOURCE_DIR}/shared/proto/ipc_data.proto
  DEPENDS ${PROTO_FILES}
  COMMENT "Generating Protobuf files"
)

# Add the generated Protobuf files to a library with a unique name
add_library(proto_generated_lib ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.h)
target_include_directories(proto_generated_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto_generated_lib PUBLIC ${Protobuf_LIBRARIES})



### SHARED LIBRARY (T_IPCData)
# Shared library for T_IPCData
add_library(t_ipc_data_lib
    shared/t_ipc_data.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc
)
target_link_libraries(t_ipc_data_lib proto_generated_lib)
target_include_directories(t_ipc_data_lib PUBLIC shared)



### TESTS
# Define test sources
set(TEST_SOURCES
    tests/t_ipc_data.cpp
)

add_executable(test_t_ipc_data ${TEST_SOURCES})
target_link_libraries(test_t_ipc_data t_ipc_data_lib gtest gtest_main pthread)
target_include_directories(test_t_ipc_data PUBLIC shared)



### RX COMPONENT
# RX Executable
add_executable(main_rx apps/main_rx.cpp)
target_link_libraries(main_rx t_ipc_data_lib rt)
target_include_directories(main_rx PUBLIC shared)
