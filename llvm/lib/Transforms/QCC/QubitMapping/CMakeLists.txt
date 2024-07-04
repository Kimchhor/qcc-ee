add_llvm_component_library(LLVMQCCQM

QCCBFSQubitMapping.cpp
QCCAncillaQubitMapping.cpp
QCCSABREQubitMapping.cpp

ADDITIONAL_HEADER_DIRS
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/QubitMapping
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/Routing

DEPENDS
intrinsics_gen

LINK_COMPONENTS
QCCMD
QCCLAYOUT
QCCROUT
Analysis
Core
)