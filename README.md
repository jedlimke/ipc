# IPC Project

This project demonstrates inter-process communication (IPC) in C++ using POSIX message queues. The setup includes a Dockerized Ubuntu 22.04 environment as requested. The development environment goes hand-in-hand with the container, allowing direct development within Ubuntu from any external HOST platform.

## Getting Started

### Why Docker Compose?

Although this project only uses a single container, Docker Compose simplifies the workflow by:
- Automating container creation and configuration.
- Mapping the project directory into the container for seamless code editing.
- Providing a single command to build and run the environment.

### Setup Instructions

1. **Build and Start the Docker Container**:
   Stay attached:
   ```bash
   docker-compose up --build
   ```
   OR do NOT stay attached:
   ```bash
   docker-compose up -d --build
   ```
   This will:
   - Build an Ubuntu 22.04-based development container.
   - Map the project directory into the container under `/app`.

1. **Access the Running Container**:
   ```bash
   docker exec -it ipc_dev bash
   ```

1. **Stop the Container**:
   ```bash
   docker-compose down
   ```

## Running Applications

### Running the Transmitter ([`main_tx`](./apps/main_tx.cpp))

1. **Access the Running Container**:
   ```bash
   docker exec -it ipc_dev bash
   ```

1. **Build the Application**:
   ```bash
   cmake -B build -S .
   cmake --build build
   ```

1. **Run the Transmitter**:
   Inside the container:
   ```bash
   ./build/apps/main_tx
   ```
   This will send data to the receiver using IPC.

### Running the Receiver ([`main_rx`](./apps/main_rx.cpp))

1. **Access the Running Container**:
   ```bash
   docker exec -it ipc_dev bash
   ```

1. **Build the Application**:
   (If not already built during the transmitter steps)
   ```bash
   cmake -B build -S .
   cmake --build build
   ```

1. **Run the Receiver**:
   Inside the container:
   ```bash
   ./build/apps/main_rx
   ```
   The receiver will listen for messages and process them as they arrive.

## Running Tests

This project uses GoogleTest for unit testing.

### Steps to Run the Tests

1. **Access the Running Container**:
   ```bash
   docker exec -it ipc_dev bash
   ```

1. **Build the Test Executable**:
   Navigate to the project directory (`/app`) and run:
   ```bash
   cmake -B build -S .
   cmake --build build
   ```

1. **Run the Tests**:
   After building, execute the test binary:
   ```bash
   ./build/test_t_ipc_data
   ```
   You should see output confirming that the tests pass successfully.

## Queue Constants

The constants that define the queue properties, such as name and size, are located in [`/shared/constants.h`](./shared/constants.h). This file ensures both the transmitter ([`main_tx`](./apps/main_tx.cpp)) and receiver ([`main_rx`](./apps/main_rx.cpp)) share the same queue definition. Modify this file if you need to adjust the queue properties.

## Data Structure Contract

The sender (Tx) and receiver (Rx) communicate using a shared `IPCData` structure defined in [`/shared/proto/ipc_data.proto`](./shared/proto/ipc_data.proto). This structure ensures that both sides use the same definition for the data being transmitted.

## Protobuf Integration

The Protobuf compiler (`protoc`) is explicitly invoked during the build process using a custom CMake command. The project defines the `.proto` schema in [`/shared/proto/ipc_data.proto`](./shared/proto/ipc_data.proto), and the following steps occur automatically:

1. The Protobuf files are compiled with `protoc` into `.pb.h` and `.pb.cc` files.
1. These files are placed in the build directory.
1. A library (`proto_generated_lib`) is created from the compiled Protobuf files and linked to the project.

If you modify the `.proto` file, these changes will be incorporated during the next build. You do not need to manually invoke the Protobuf compiler, as the CMake configuration handles everything automatically.

For reference, see the Protobuf-related configuration in [`CMakeLists.txt`](./CMakeLists.txt).
