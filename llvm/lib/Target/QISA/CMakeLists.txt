add_llvm_component_group(QISA)

set(LLVM_TARGET_DEFINITIONS QISA.td)

tablegen(LLVM QISAGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM QISAGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM QISAGenCallingConv.inc -gen-callingconv)
tablegen(LLVM QISAGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM QISAGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM QISAGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM QISAGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM QISAGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM QISAGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(QISACommonTableGen)

add_llvm_target(QISACodeGen
  QISAAsmPrinter.cpp
  QISAFrameLowering.cpp
  QISAISelDAGToDAG.cpp
  QISAISelLowering.cpp
  QISAInstrInfo.cpp
  QISAMachineFunctionInfo.cpp
  QISAMCInstLower.cpp
  QISARegisterInfo.cpp
  QISASubtarget.cpp
  QISATargetMachine.cpp
  QISAAncillaAllocationPass.cpp
  QISAAddr2ImmPass.cpp
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
