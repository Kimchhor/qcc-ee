add_llvm_component_group(QASM)

set(LLVM_TARGET_DEFINITIONS QASM.td)

tablegen(LLVM QASMGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM QASMGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM QASMGenCallingConv.inc -gen-callingconv)
tablegen(LLVM QASMGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM QASMGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM QASMGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM QASMGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM QASMGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM QASMGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(QASMCommonTableGen)

add_llvm_target(QASMCodeGen
  QASMAsmPrinter.cpp
  QASMFrameLowering.cpp
  QASMISelDAGToDAG.cpp
  QASMISelLowering.cpp
  QASMInstrInfo.cpp
  QASMMachineFunctionInfo.cpp
  QASMMCInstLower.cpp
  QASMRegisterInfo.cpp
  QASMSubtarget.cpp
  QASMTargetMachine.cpp
  QASMAncillaAllocationPass.cpp
  QASMAddr2ImmPass.cpp
  QASMLowerMeasPass.cpp
  QASMTargetObjectFile.cpp

  DecomposeLogical2NativePass.cpp
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
