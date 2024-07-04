add_llvm_component_library(LLVMQASMDisassembler
  QASMDisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  Support
  QASMDesc
  QASMInfo

  ADD_TO_COMPONENT
  QASM
  )
