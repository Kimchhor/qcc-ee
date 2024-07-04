add_llvm_component_library(LLVMQCCMD

MachineDescription.cpp
QCCMachineDescription.cpp

ADDITIONAL_HEADER_DIRS
${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/MD

DEPENDS
intrinsics_gen
) 