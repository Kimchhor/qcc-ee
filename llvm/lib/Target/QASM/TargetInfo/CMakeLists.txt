add_llvm_component_library(LLVMQASMInfo
  QASMTargetInfo.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  QASM
  )
