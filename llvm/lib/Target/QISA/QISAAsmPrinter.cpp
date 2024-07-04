//===-- QISAAsmPrinter.cpp - QISA LLVM assembly writer ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format QISA assembly language.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/QISAInstPrinter.h"
//#include "MCTargetDesc/QISAMCExpr.h"

#include "QISADefine.h"
#include "QISA.h"
#include "QISAInstrInfo.h"
#include "QISAMCInstLower.h"
#include "QISATargetMachine.h"
#include "QISASubtarget.h"
#include "MCTargetDesc/QISATargetStreamer.h"

#include "TargetInfo/QISATargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/GCMetadata.h"
#include "llvm/CodeGen/MachineFrameInfo.h"

#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCSymbolELF.h"
#include "llvm/MC/MCRegister.h"

#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Timer.h"

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/BinaryFormat/COFF.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// TODO:
// %hi16() and %lo16() for addresses

namespace {
class QISAAsmPrinter : public AsmPrinter {

  QISATargetStreamer *getTargetStreamer() {
    return static_cast<QISATargetStreamer*>(OutStreamer->getTargetStreamer());
  }

public:
  explicit QISAAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "QISA Assembly Printer"; }

  void emitConstantPool() override;

  // void emitFunctionHeader() override;

  void emitStartOfAsmFile(Module &M) override;
  
  void emitFunctionBodyStart() override;
  // void emitFunctionBodyEnd() override;
  // void emitBasicBlockEnd(const MachineBasicBlock &MBB) override;

  void emitFunctionEntryLabel() override;

  void emitEndOfAsmFile(Module &M) override;

  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       const char *ExtraCode, raw_ostream &OS) override;
  void emitInstruction(const MachineInstr *MI) override;


  /// This method decides whether the specified basic block requires a label.
  bool shouldEmitLabelForBasicBlock(const MachineBasicBlock &MBB) const;
};
} // end of anonymous namespace

bool QISAAsmPrinter::shouldEmitLabelForBasicBlock(
    const MachineBasicBlock &MBB) const {
  // With `-fbasic-block-sections=`, a label is needed for every non-entry block
  // in the labels mode (option `=labels`) and every section beginning in the
  // sections mode (`=all` and `=list=`).
  if ((MF->hasBBLabels() || MBB.isBeginSection()) && !MBB.isEntryBlock())
    return true;
  // A label is needed for any block with at least one predecessor (when that
  // predecessor is not the fallthrough predecessor, or if it is an EH funclet
  // entry, or if a label is forced).
  return !MBB.pred_empty() &&
         (!isBlockOnlyReachableByFallthrough(&MBB) || MBB.isEHFuncletEntry() ||
          MBB.hasLabelMustBeEmitted());
}


// Emit the info before the function body start
// after the header of function (inside function)
void QISAAsmPrinter::emitFunctionBodyStart() { }

// This function will emit "kernel:"
void QISAAsmPrinter::emitFunctionEntryLabel() {

  getTargetStreamer()->emitSymbol("kernel_main", MCSA_CODE_AREA);

}

// Emit the info when the Asm File start 
// after .text
void QISAAsmPrinter::emitStartOfAsmFile(Module &M){

  // Current Version
  KernelHeader version    =   {"eQISA Version", KERNEL_VER};

  // QMap Info
  KernelHeader qmap_base  =   {"QMap base", 0x0};
  KernelHeader qmap_size  =   {"QMap size", 0x0};
  
  // Symbol table info
  KernelHeader symbol_table_base  =   {"symbol_table base", 0x0};
  KernelHeader symbol_table_size  =   {"symbol_table size", 0x0};
  
  // IData area info
  KernelHeader idata_base  =      {"idata base", 0x0};
  KernelHeader idata_size  =      {"idata size", 0x0};

  // UData area info
  KernelHeader udata_base  =      {"udata base", 0x0};
  KernelHeader udata_size  =      {"udata size", 0x0};
  
  // Code area info
  KernelHeader code_area_base  =   {"code_area base", 0x0};
  KernelHeader code_area_size  =   {"code_area size", 0x0};

  std::vector<KernelHeader> kernel_headers = {version, qmap_base, qmap_size, symbol_table_base, 
  symbol_table_size, idata_base, idata_size, udata_base, udata_size, code_area_base, code_area_size};

  getTargetStreamer()->emitKernelHeader(kernel_headers);
  
}

void QISAAsmPrinter::emitEndOfAsmFile(Module &M){}
 
/// Print out constants referenced by the function
void QISAAsmPrinter::emitConstantPool(){

  
  QISATargetStreamer &Steamer = *getTargetStreamer();
  const Function &F = MF->getFunction();
  
  // Set the CPU and FPU Bitmasks
  const MachineFrameInfo &MFI = MF->getFrameInfo();
  const TargetRegisterInfo *TRI = MF->getSubtarget().getRegisterInfo();

  // QCL_TODO: 
  // - Need to get qubit mapping inf0
  

  Register stackReg = TRI->getFrameRegister(*MF);
  unsigned returnReg = TRI->getRARegister();
  unsigned stackSize = MF->getFrameInfo().getStackSize();

  unsigned numQubit = 0;
  unsigned numBit = 0;

  unsigned numRegs = MFI.getNumObjects();
  for (unsigned i = 0; i < numRegs; i++){
    const llvm::AllocaInst* inst = MFI.getObjectAllocation(i);
    if (inst){
      if (inst->getAllocatedType()->isQubitTy()){
        numQubit++;
      }else if (inst->getAllocatedType()->isArrayTy()){
          ArrayType *arrTy = dyn_cast<ArrayType>(inst->getAllocatedType());
          if (arrTy->getElementType()->isIntegerTy()){
            numBit += arrTy->getNumElements();
          }else if (arrTy->getElementType()->isQubitTy()){
            numQubit += arrTy->getNumElements();
          }
      } else{
        numBit++;
      }
    }
  }
  std::vector<unsigned> dataQubit = {0, 4, 2};

  //        Qubit layout
  // LQ0(D)-----LQ1(A)-----LQ2(D)
  //  |           |          |
  //  |           |          |
  //  LQ3(A)-----LQ4(D)-----LQ5(A)


  std::vector<unsigned> usedDataQubit;

  for(int i = 0; i < numQubit; i++){
    if (i > dataQubit.size()){
      llvm_unreachable("The number of qubit over the 3 data qubits");
    }
    usedDataQubit.push_back(dataQubit[i]);
  }
 


  // This will map from qreg[i] to dataQubit traversally.
  QMap QM;
  QM.qmap_entry_cnt = usedDataQubit.size();     // number of qubits
  for (uint32_t i = 0; i < QM.qmap_entry_cnt; i++){	
		QM.qmap_entries.push_back( {i, (uint32_t) usedDataQubit[i]} );	// (program qubit, logical qubit)
  }

  SymTable ST;
  ST.symtbl_entry_cnt = 4;      // symbol table entry count
  ST.symtbl_entries = {
    {"ca_kernel_main",    OPCODE_TYPE_QC,         		0x0},
    {"ida_krf_ent_cnt",   OPERAND_TYPE_IDADDR,    		0x0}, 	// kernel return frame addresss (measure results)
    {"ida_dlq",           OPERAND_TYPE_IDADDR + 4,    0x0},	 	// data qubit
    {"uda_krf",           OPERAND_TYPE_UDADDR,    		0x0},
  };


  DataArea ID  =  {"ida_krf_ent_cnt", {numBit}};	// kernel return frame size count
  DataArea ID1 =  {"ida_dlq",         usedDataQubit};													// data qubit address
  
  DataArea UD  =  {"uda_krf", {}}; 			// result address

  std::vector<DataArea> DA = {ID, ID1, UD};

  Steamer.emitDirectiveQmapArea(QM);

  Steamer.emitSymbolTable(ST);

  Steamer.emitDirectiveDataArea(DA);
}


bool QISAAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                     const char *ExtraCode, raw_ostream &OS) {
  if (ExtraCode)
    return AsmPrinter::PrintAsmOperand(MI, OpNo, ExtraCode, OS);
  QISAMCInstLower Lower(MF->getContext(), *this);
  MCOperand MO(Lower.lowerOperand(MI->getOperand(OpNo)));
  QISAInstPrinter::printOperand(MO, MAI, OS);
  return false;
}

void QISAAsmPrinter::emitInstruction(const MachineInstr *MI) {
  MCInst LoweredMI;
  switch (MI->getOpcode()) {
  case QISA::RET:
    return; // QCL_FIXME: it must be removed in instruction selection phase
    LoweredMI = MCInstBuilder(QISA::JMP).addReg(QISA::R10);
    break;
  // QCL_FIXME: should remove in FrameLower or Ins selection phase
  case QISA::STDrqi: // Turn off the store qubit type
  case QISA::STDrqr:
  case QISA::STIrqadd:
    return;
    break;
  // case QISA::X_GATErr:
  //   LoweredMI = MCInstBuilder(QISA::X_GATErr).addReg(QISA::R4);
  //   break;
  // case QISA::MEASURE_Z_GATErr:
  //   LoweredMI = MCInstBuilder(QISA::MEASURE_Z_GATErr).addReg(QISA::R17);
  //   break;
  default:
    QISAMCInstLower Lower(MF->getContext(), *this);
    Lower.lower(MI, LoweredMI);
    // doLowerInstr(MI, LoweredMI);
    break;
  }
  EmitToStreamer(*OutStreamer, LoweredMI);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQISAAsmPrinter() {
  RegisterAsmPrinter<QISAAsmPrinter> X(getTheQISATarget());
}