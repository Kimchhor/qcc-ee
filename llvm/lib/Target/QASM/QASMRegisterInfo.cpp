//===-- QASMRegisterInfo.cpp - QASM Register Information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the QASM implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "QASMRegisterInfo.h"
#include "QASM.h"
#include "QASMDefine.h"

//#include "QASMMachineFunctionInfo.h"
#include "QASMSubtarget.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "qasm-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "QASMGenRegisterInfo.inc"

QASMRegisterInfo::QASMRegisterInfo() : QASMGenRegisterInfo(QASM::R1) {
  IDSBasePtr = QASM::QR7;
  UDSBasePtr = QASM::QR8;
}


const MCPhysReg *
// Returns a list of callee-saved registers in the order of the desired callee-save stack frame offset.
QASMRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_QASM_SaveList;
}

// Returns a bitset indexed by physical register numbers, indicating if a particular register is unavailable.
BitVector QASMRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  // R61 is the stack pointer.
  Reserved.set(QASM::R61);

  Reserved.set(QASM::QR7);
  Reserved.set(QASM::QR8);

  return Reserved;
}

// Eliminate abstract frame indices from instructions that may use them.
void QASMRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MII,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  MachineInstr &MI = *MII;
  const MachineFunction &MF = *MI.getParent()->getParent();
  const MachineFrameInfo MFI = MF.getFrameInfo();
  MachineOperand &FIOp = MI.getOperand(FIOperandNum);
  unsigned FI = FIOp.getIndex();
  DebugLoc DL = MI.getDebugLoc();

  const MachineRegisterInfo &MRI = MF.getRegInfo();

  const QASMInstrInfo &TII = *MF.getSubtarget<QASMSubtarget>().getInstrInfo();
  const QASMFrameLowering *TFI = getFrameLowering(MF);
  int ObjectOffset = MF.getFrameInfo().getObjectOffset(FI);
  int ObjSize = MF.getFrameInfo().getObjectSize(FI);
  int StackSize = MF.getFrameInfo().getStackSize();
  int LocalFrameSize = MF.getFrameInfo().getLocalFrameSize();

  LLVM_DEBUG(errs() << "\nFunction         : " << MF.getName() << "\n");
  LLVM_DEBUG(errs() << "<--------->\n");
  LLVM_DEBUG(errs() << MI << "\n");
  LLVM_DEBUG(errs() << "FrameIndex         : " << FI << "\n");
  LLVM_DEBUG(errs() << "ObjSize            : " << ObjSize << "\n");
  LLVM_DEBUG(errs() << "FrameOffset        : " << ObjectOffset << "\n");
  LLVM_DEBUG(errs() << "StackSize          : " << StackSize << "\n");
  LLVM_DEBUG(errs() << "LocalFrameSize     : " << LocalFrameSize << "\n");

  // Determine if we can eliminate the index from this kind of instruction.
  unsigned ImmOpIdx = 0;
  switch (MI.getOpcode()) {
  default:
    // Not supported yet.
    errs() << "Not support yet";
    return;
  case QASM::LDIradd:
  case QASM::LDDrr:
  case QASM::LDDri:


  case QASM::STIradd:
  case QASM::STDrr:
  case QASM::STDri:

  case QASM::LDIrqadd:
  case QASM::LDDrqr:
  case QASM::LDDrqi:
  case QASM::STIrqadd:
  case QASM::STDrqr:
  case QASM::STDrqi:

  case QASM::MOVHIi16:
  case QASM::INCri:
  case QASM::MOV_rr:
  // case QASM::SHL8mCLS:

  // case QASM::STDrm:
  // case QASM::LDDrm:

    ImmOpIdx = FIOperandNum + 1;
    break;
  }

  Register IgnoreFrameReg;
  int64_t DataOffset = TFI->getFrameIndexReference(MF, FI, IgnoreFrameReg).getFixed();
  
  // WE assume IData register always above UData register address.
  //  |------------------|
  //  |                  |
  //  |   IData address  |
  //  |                  |
  //  |------------------|  <- IData base address = "StackSize - UDataOffset"
  //  |                  |
  //  |   UData address  |
  //  |                  |
  //  |------------------| <-  "Offset" start from here

    // FIXME: check the size of offset.
  MachineOperand &ImmOp = MI.getOperand(ImmOpIdx);

  // int Offset = StackSize - ObjectOffset;

  int Offset = -ObjectOffset;

  LLVM_DEBUG(errs() << "Offset     : " << Offset << "\n");


  auto Operand = OPERAND_TYPE_IDADDR; 
  // auto Operand = 0; // cbit
  
  if (IgnoreFrameReg == UDSBasePtr) {
    Operand = OPERAND_TYPE_UDADDR;
    // Operand = 0; // qbit
    // Add 4, because the IData address start from 0xA004
    Offset = Offset - (StackSize - DataOffset);
  }
  
  LLVM_DEBUG(errs() << "Finall Offset     : " << Offset << "\n");

  FIOp.ChangeToRegister(IgnoreFrameReg, false);
  ImmOp.setImm(Offset);

  MachineOperand MCO = MI.getOperand(FIOperandNum - 1);
  if (!MCO.isReg()){
    return;
  }

  Register Reg = MI.getOperand(FIOperandNum - 1).getReg();

  if (MI.getOpcode() == QASM::LDIradd){

    BuildMI(*MI.getParent(), MII, DL,TII.get(QASM::LDDri), Reg)
      .addImm( Operand + (Offset));    
    
    MI.eraseFromParent();

  }
  else if (MI.getOpcode() == QASM::STIradd){
    
    BuildMI(*MI.getParent(), MII, DL, TII.get(QASM::STDri), Reg)
      .addImm(Operand + Offset);
    MI.eraseFromParent();
  }

}

Register QASMRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return QASM::R61;
}
