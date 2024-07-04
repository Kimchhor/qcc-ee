add_llvm_component_library(LLVMQCCQISA

ObjectFile.cpp
Instruction.cpp
Instructions.cpp
Block.cpp
Program.cpp

ADDITIONAL_HEADER_DIRS
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/Layout
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/QISA

DEPENDS
intrinsics_gen

LINK_COMPONENTS
QCCMD
QCCLAYOUT
QCCROUT
Analysis
Core
) 