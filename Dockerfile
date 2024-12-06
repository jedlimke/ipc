# # Base image
# FROM ubuntu:22.04

# # Install system dependencies
# RUN apt-get update && apt-get install -y \
#     build-essential \
#     g++ \
#     cmake \
#     libgtest-dev \
#     git \
#     vim \
#     wget \
#     curl \
#     gdb \
#     libprotobuf-dev \
#     protobuf-compiler \
#     protobuf-c-compiler \
#     && apt-get clean

# # Build and install GoogleTest
# RUN cd /usr/src/gtest && cmake . && make && cp lib/libgtest*.a /usr/lib/

# # Set the working directory
# WORKDIR /app

# # Default command
# CMD ["/bin/bash"]

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    libgtest-dev \
    git \
    vim \
    wget \
    curl \
    unzip \
    gdb \
    && apt-get clean
    # libprotobuf-dev \
    # protobuf-compiler \
    # protobuf-c-compiler \
    
# Set the desired protobuf version
ARG PROTOBUF_VERSION=3.15.8

# Download and build Protobuf from source (compiler and library)
RUN curl -OL https://github.com/protocolbuffers/protobuf/releases/download/v${PROTOBUF_VERSION}/protobuf-cpp-${PROTOBUF_VERSION}.tar.gz && \
    tar -xvzf protobuf-cpp-${PROTOBUF_VERSION}.tar.gz && \
    cd protobuf-${PROTOBUF_VERSION} && \
    ./configure --prefix=/usr/local && \
    make -j$(nproc) && \
    make install && \
    ldconfig && \
    cd .. && \
    rm -rf protobuf-${PROTOBUF_VERSION} protobuf-cpp-${PROTOBUF_VERSION}.tar.gz

# Build and install GoogleTest
RUN cd /usr/src/gtest && cmake . && make && cp lib/libgtest*.a /usr/lib/

# Set the working directory
WORKDIR /app

# Default command
CMD ["/bin/bash"]
    
