# Base image
FROM ubuntu:22.04

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    libgtest-dev \
    git \
    vim \
    wget \
    curl \
    gdb \
    && apt-get clean

# Build and install GoogleTest
RUN cd /usr/src/gtest && cmake . && make && cp lib/libgtest*.a /usr/lib/

# Set the working directory
WORKDIR /app

# Default command
CMD ["/bin/bash"]
