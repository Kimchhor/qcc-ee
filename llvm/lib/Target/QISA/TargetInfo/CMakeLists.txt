add_llvm_component_library(LLVMQISAInfo
  QISATargetInfo.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  QISA
  )
