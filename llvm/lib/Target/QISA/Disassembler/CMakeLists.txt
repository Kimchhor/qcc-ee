add_llvm_component_library(LLVMQISADisassembler
  QISADisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  Support
  QISADesc
  QISAInfo

  ADD_TO_COMPONENT
  QISA
  )
