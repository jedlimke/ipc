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
    libprotobuf-dev \
    protobuf-compiler \
    protobuf-c-compiler \
    && apt-get clean
    
# Set the desired protobuf version
ARG PROTOBUF_VERSION=3.15.8

# Download and install protobuf
RUN curl -OL https://github.com/protocolbuffers/protobuf/releases/download/v${PROTOBUF_VERSION}/protoc-${PROTOBUF_VERSION}-linux-x86_64.zip && \
    unzip protoc-${PROTOBUF_VERSION}-linux-x86_64.zip -d protoc3 && \
    mv protoc3/bin/* /usr/local/bin/ && \
    mv protoc3/include/* /usr/local/include/ && \
    rm -rf protoc3 protoc-${PROTOBUF_VERSION}-linux-x86_64.zip

# Build and install GoogleTest
RUN cd /usr/src/gtest && cmake . && make && cp lib/libgtest*.a /usr/lib/

# Set the working directory
WORKDIR /app

# Default command
CMD ["/bin/bash"]
    
