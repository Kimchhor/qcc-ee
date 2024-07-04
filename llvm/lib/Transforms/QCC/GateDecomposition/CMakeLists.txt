add_llvm_component_library(LLVMQCCGD
Decompose2Native.cpp
EmitNativeMultiple.cpp
EmitNativeSingle.cpp

DEPENDS
intrinsics_gen
  
LINK_COMPONENTS
QCCMD
QCCLAYOUT
QCCROUT
Analysis
Core
)

