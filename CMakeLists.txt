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

# Add the generated Protobuf files to a library with a unique name
add_library(proto_generated_lib ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.h)
target_include_directories(proto_generated_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto_generated_lib PUBLIC ${Protobuf_LIBRARIES})



### RX COMPONENT
# Define RX sources
set(RX_SOURCES
    src/rx/main_rx.cpp
    src/rx/rx.cpp
)

# RX Library
add_library(rx_lib src/rx/rx.cpp)
target_link_libraries(rx_lib proto_generated_lib rt)
target_include_directories(rx_lib PUBLIC include)

# RX Executable
add_executable(main_rx ${RX_SOURCES})
target_link_libraries(main_rx rx_lib)



# ### TX COMPONENT
# # Define TX sources
# set(TX_SOURCES
#     src/tx/main_tx.cpp
#     src/tx/tx.cpp
# )

# # TX Library
# add_library(tx_lib src/tx/tx.cpp)
# target_link_libraries(tx_lib proto_generated_lib rt)
# target_include_directories(tx_lib PUBLIC include)

# # TX Executable
# add_executable(main_tx ${TX_SOURCES})
# target_link_libraries(main_tx tx_lib)



### TESTS
# Define test sources
set(TEST_RX_SOURCES
    tests/rx/rx.cpp
)

# set(TEST_TX_SOURCES
#     tests/tx/tx.cpp
# )

# RX Tests
add_executable(rx_test ${TEST_RX_SOURCES} src/rx/rx.cpp ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc)
target_link_libraries(rx_test rx_lib gtest gtest_main pthread)

# # TX Tests
# add_executable(tx_test ${TEST_TX_SOURCES} src/tx/tx.cpp ${CMAKE_CURRENT_BINARY_DIR}/ipc_data.pb.cc)
# target_link_libraries(tx_test tx_lib gtest gtest_main pthread)
