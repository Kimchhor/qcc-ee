
# Use an official LLVM image as a base
FROM ubuntu:20.04 as build
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Seoul

# Set the working directory in the container
WORKDIR /

# Copy the compiler source code into the container
COPY . /

# Install dependencies and build the compiler
RUN mkdir build; cd build &&\
    apt-get update && \
    apt-get install -y build-essential && \
    apt-get install -y binutils-gold && \
    apt-get install -y cmake &&\
    apt-get install -y ninja-build &&\
    apt-get install -y libboost-all-dev build-essential libssl-dev &&\
    apt-get install -y libblkid-dev e2fslibs-dev libaudit-dev &&\
    cmake -G "Ninja" ../llvm -DLLVM_ENABLE_PROJECTS="opt;llc" -DCMAKE_BUILD_TYPE=Release -DLLVM_BUILD_LLVM_DYLIB=ON -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="QASM" -DLLVM_TARGETS_TO_BUILD="host" &&\
    ninja &&\
    ninja &&\
    ninja &&\
    apt-get remove -y build-essential && \
    apt-get remove -y binutils-gold && \
    apt-get remove -y cmake &&\
    apt-get remove -y ninja-build &&\
    apt-get remove -y libboost-all-dev build-essential libssl-dev &&\
    apt-get remove -y libblkid-dev e2fslibs-dev libaudit-dev &&\
    apt-get autoremove -y &&\
    apt-get clean -y

# Set the entrypoint for the container
# ENTRYPOINT ["build/bin/llc", "--version"]

# Expose a volume for users to mount their input files
VOLUME /input

# Expose a port for users to access the compiler
EXPOSE 10010