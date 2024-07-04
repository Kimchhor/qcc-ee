

add_llvm_component_library(LLVMQCCUNROLL

GateUnroll.cpp

ADDITIONAL_HEADER_DIRS
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/GateUnroll


DEPENDS
intrinsics_gen

LINK_COMPONENTS
Analysis
Core
)