Quantum Classical Compiler (QCC)
================================
QCC is a compiler for hybrid quantum-classical programming language. It is written using pure python as frontend compiler and the LLVM infrastructure as backend. It is for the purpose of writing and anlyzing code for quantum computing application.

QCC allows programmers to compile quantum circuit to low-level quantum assembly format (QASM) and quantum instruction set architecture (QISA). The quantum circuit is written in python then tranfrom to LLVM IR (immidiate representation), the backend compiler analyzes and perform qubit mapping including data and ancilla selection for specific topology archtecture.

Installation
===========

## Prerequisites

#### For Unix build

First you need to install the following dependencies. For each one, you
can either install by building from source, or use the package manager
of your system (“yum" on Red Hat or “apt-get" on Ubuntu).  

1.  Static libraries for libstdc++ and glibc

    -   “Ubuntu": Install GNU gold linker

        You can check if you have this now by doing ‘ld -v’ and if it
        says ‘GNU gold’ you have it

                sudo apt-get install binutils-gold
                

    -   “Red Hat"

                sudo yum install libstdc++-static 
                sudo yum install glibc-static
                

2.  GCC 5.1.0 or higher: if you need to preserve an older build,
    consider using ‘update-alternatives’ as system-wide method for
    preserving and maintaining these.

3.  CMake

    -   “Ubuntu"

                sudo apt-get install cmake
                

    -   “Source Build"

        There are instructions for downloading and building CMake from
        source at:
        <https://cmake.org/install>

4.  Python >=3.6 Automated test suite

5.  Ninja, for much faster builds: <https://ninja-build.org>
 - "Ubuntu"

                sudo apt-get install ninja-build

6.  Boost 1.48

    -   “Source Build": Boost installation instructions are here:
        <http://www.boost.org/doc/libs/1_48_0/doc/html/bbv2/installation.html>

                wget http://sourceforge.net/projects/boost/files/boost/1.48.0/boost_1_48_0.tar.gz
                tar zxf boost_1_48_0.tar.gz && cd boost_1_48_0/
                sudo ./bootstrap.sh
                sudo ./b2 install --prefix=/usr/local
                

        Alternatively, Ubuntu users can install the current Boost
        version via:

                sudo apt-get install libboost-all-dev

                

## Installing
The installation is followed the installaion of llvm.

### Configure and build LLVM :

 ``cd qcc-backend``
 
 ``mkdir build``
 
 ``cd build``

 ``cmake -G <generator> ../llvm``

  Some common build system generators are:

  * ``Ninja`` --- for generating [Ninja](https://ninja-build.org)
    build files. Most llvm developers use Ninja.
  * ``Unix Makefiles`` --- for generating make-compatible parallel makefiles.
  * ``Visual Studio`` --- for generating Visual Studio projects and
    solutions.
  * ``Xcode`` --- for generating Xcode projects.


* ``ninja`` or your build systspecified above
  directly.


* For more information see [CMake](https://llvm.org/docs/CMake.html)


### Config path
``ln -sf path/to/qcc-backend/llvm/build/bin/* /usr/local/bin/``

Usage
-----
To compile the quantum circuit, use the llvm `opt` and give the file name of llvm ir.

  ``opt --passes='qasm-gen' [llvm_ir.ll] [Options]``

### Passes
    
    swap-decompose    - Apply SWAP decomposition
    qasm-gen          - Apply QASM Generation
    decompose2native  - Apply Decompose to Native gate

**Node** swap-decompose and qasm-gen can be used together.

### Options

    -md       - Input path to machine description
    -mapping  - Input Qubit mapping algorithm

### Example

  ``opt -passes='swap-decompose,qasm-gen' ./QCC/tst/cnot.ll -md='./QCC/res/machine_description.json' -mapping=BFS``
