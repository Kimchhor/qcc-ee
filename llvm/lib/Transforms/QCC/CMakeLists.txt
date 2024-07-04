
add_subdirectory(MD)
add_subdirectory(Layout)
add_subdirectory(Routing)
add_subdirectory(QubitMapping)
add_subdirectory(QISA)
add_subdirectory(GateUnroll)

add_llvm_component_library(LLVMQCC

  CountCNOT.cpp
  EmitSwap.cpp
  DecomposeSwap.cpp
  MultiGateAnalysis.cpp
  QODGAnalysis.cpp
  QASMGen.cpp
  QISAGen.cpp


  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/QubitMapping
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/Routing
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/QISA
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/QCC/GateUnroll

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  QCCMD
  QCCLAYOUT
  QCCROUT
  QCCQM
  QCCQISA
  QCCUNROLL
  Analysis
  Core
  )
  