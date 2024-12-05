# IPC Project

This project demonstrates inter-process communication (IPC) in C++ using POSIX message queues. The setup includes a Dockerized Ubuntu 22.04 environment as requested. The development environment goes hand-in-hand with the container, allowing direct development within Ubuntu from any external HOST platform.

---

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

## Running Tests

This project uses GoogleTest for unit testing.

### Steps to Run the Tests

1. **Access the Running Container**:
   ```bash
   docker exec -it ipc_dev bash
   ```

1. **Build the Test Executable:** Navigate to the project directory (`/app`) and run:
   ```bash
   cmake -B build -S .
   cmake --build build
   ```

1. **Run the Tests:** After building, execute the test binary:
   ```bash
   ./build/test_rx
   ```
   Output should be something like:
   ```bash
   [==========] Running 3 tests from 1 test suite.
   [----------] Global test environment set-up.
   [----------] 3 tests from RxTests
   [ RUN      ] RxTests.AddPositiveNumbers
   [       OK ] RxTests.AddPositiveNumbers (0 ms)
   [ RUN      ] RxTests.AddNegativeNumbers
   [       OK ] RxTests.AddNegativeNumbers (0 ms)
   [ RUN      ] RxTests.AddMixedNumbers
   [       OK ] RxTests.AddMixedNumbers (0 ms)
   [----------] 3 tests from RxTests (1 ms total)

   [----------] Global test environment tear-down
   [==========] 3 tests from 1 test suite ran. (3 ms total)
   [  PASSED  ] 3 tests.
   ```