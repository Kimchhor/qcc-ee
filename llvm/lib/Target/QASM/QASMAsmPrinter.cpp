//===-- QASMAsmPrinter.cpp - QASM LLVM assembly writer ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format QASM assembly language.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/QASMInstPrinter.h"
//#include "MCTargetDesc/QASMMCExpr.h"

#include "QASMDefine.h"
#include "QASM.h"
#include "QASMInstrInfo.h"
#include "QASMMCInstLower.h"
#include "QASMTargetMachine.h"
#include "QASMSubtarget.h"
#include "MCTargetDesc/QASMTargetStreamer.h"

#include "TargetInfo/QASMTargetInfo.h"
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
using namespace std;

#define DEBUG_TYPE "asm-printer"

// TODO:
// %hi16() and %lo16() for addresses

namespace {
class QASMAsmPrinter : public AsmPrinter {

  QASMTargetStreamer *getTargetStreamer() {
    return static_cast<QASMTargetStreamer*>(OutStreamer->getTargetStreamer());
  }

public:

  std::map<Register, int64_t> QMOVmap;

  explicit QASMAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "QASM Assembly Printer"; }

  void emitConstantPool() override;

  void doLowerInstr(const MachineInstr *MI, MCInst &OutMI) const;

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

bool QASMAsmPrinter::shouldEmitLabelForBasicBlock(
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
void QASMAsmPrinter::emitFunctionBodyStart() {}

// This function will emit "kernel:"
void QASMAsmPrinter::emitFunctionEntryLabel() {

  //getTargetStreamer()->emitSymbol("kernel_main", MCSA_CODE_AREA);

}

// Emit the info when the Asm File start 
// after .text
void QASMAsmPrinter::emitStartOfAsmFile(Module &M){

  static bool headerPrinted = false;
    if (!headerPrinted) {
      OutStreamer->emitRawText("OPENQASM 2.0;\n");
      OutStreamer->emitRawText("include \"qelib1.inc\";\n\n");
      headerPrinted = true;
    }

}

void QASMAsmPrinter::emitEndOfAsmFile(Module &M){}

/// Print out constants referenced by the function
void QASMAsmPrinter::emitConstantPool(){

  
  QASMTargetStreamer &Steamer = *getTargetStreamer();
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
      } else if (inst->getAllocatedType()->isIntegerTy()){
        numBit++;
      }
    }
  }

  if (numQubit > 0) {
    OutStreamer->emitRawText("    qreg q[" + std::to_string(numQubit) + "];");
  }
  if (numBit > 0) {
    OutStreamer->emitRawText("    creg c[" + std::to_string(numBit) + "];");
  }

}


bool QASMAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                     const char *ExtraCode, raw_ostream &OS) {

  if (ExtraCode)
    return AsmPrinter::PrintAsmOperand(MI, OpNo, ExtraCode, OS);
  QASMMCInstLower Lower(MF->getContext(), *this);
  MCOperand MO(Lower.lowerOperand(MI->getOperand(OpNo)));
  QASMInstPrinter::printOperand(MO, MAI, OS);
  return false;

}

void QASMAsmPrinter::emitInstruction(const MachineInstr *MI) {
  MCInst LoweredMI;
  switch (MI->getOpcode()) {
  case QASM::RET:
    return; // QCL_FIXME: it must be removed in instruction selection phase
    LoweredMI = MCInstBuilder(QASM::JMP).addReg(QASM::R10);
    break;
  // QCL_FIXME: should remove in FrameLower or Ins selection phase
  case QASM::STDrqi: // Turn off the store qubit type
  case QASM::STDrqr:
  case QASM::STIrqadd:
  case QASM::STIradd:
  case QASM::STDri:
    return;
    break;
  case QASM::QMOVri:
  case QASM::QMOVrr:{
    MachineOperand Op1 = MI->getOperand(1);
    if (Op1.isImm()){
      QMOVmap[MI->getOperand(0).getReg()] = Op1.getImm();
    }
    return;
  }

  default:
    QASMMCInstLower Lower(MF->getContext(), *this);
    Lower.lower(MI, LoweredMI);
    doLowerInstr(MI, LoweredMI);
    break;
  }
  EmitToStreamer(*OutStreamer, LoweredMI);
}

void QASMAsmPrinter::doLowerInstr(const MachineInstr *MI, MCInst &OutMI) const{
  for (unsigned i = 0; i < OutMI.getNumOperands(); i++){
    MCOperand Op = OutMI.getOperand(i);
    if (Op.isReg()){
      Register reg = Op.getReg();
      if (QMOVmap.find(reg) != QMOVmap.end()){
        // Create Register using int64_t
        Register newReg = QMOVmap.at(reg);

        OutMI.erase(OutMI.begin() + i);
        OutMI.insert(OutMI.begin() + i, MCOperand::createReg(newReg));
        
      }
    }
  }
}


// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQASMAsmPrinter() {
  RegisterAsmPrinter<QASMAsmPrinter> X(getTheQASMTarget());
}