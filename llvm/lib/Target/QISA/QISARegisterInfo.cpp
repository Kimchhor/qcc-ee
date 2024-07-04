//===-- QISARegisterInfo.cpp - QISA Register Information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the QISA implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "QISARegisterInfo.h"
#include "QISA.h"
#include "QISADefine.h"

//#include "QISAMachineFunctionInfo.h"
#include "QISASubtarget.h"
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

#define DEBUG_TYPE "qisa-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "QISAGenRegisterInfo.inc"

QISARegisterInfo::QISARegisterInfo() : QISAGenRegisterInfo(QISA::R1) {
  IDSBasePtr = QISA::QR7;
  UDSBasePtr = QISA::QR8;
}


const MCPhysReg *
// Returns a list of callee-saved registers in the order of the desired callee-save stack frame offset.
QISARegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_QISA_SaveList;
}

// Returns a bitset indexed by physical register numbers, indicating if a particular register is unavailable.
BitVector QISARegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  // R61 is the stack pointer.
  Reserved.set(QISA::R61);

  Reserved.set(QISA::QR7);
  Reserved.set(QISA::QR8);

  return Reserved;
}

// Eliminate abstract frame indices from instructions that may use them.
void QISARegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MII,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  MachineInstr &MI = *MII;
  const MachineFunction &MF = *MI.getParent()->getParent();
  const MachineFrameInfo MFI = MF.getFrameInfo();
  MachineOperand &FIOp = MI.getOperand(FIOperandNum);
  unsigned FI = FIOp.getIndex();
  DebugLoc DL = MI.getDebugLoc();

  const MachineRegisterInfo &MRI = MF.getRegInfo();

  const QISAInstrInfo &TII = *MF.getSubtarget<QISASubtarget>().getInstrInfo();
  const QISAFrameLowering *TFI = getFrameLowering(MF);
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
  case QISA::LDIradd:
  case QISA::LDDrr:
  case QISA::LDDri:


  case QISA::STIradd:
  case QISA::STDrr:
  case QISA::STDri:

  case QISA::LDIrqadd:
  case QISA::LDDrqr:
  case QISA::LDDrqi:
  case QISA::STIrqadd:
  case QISA::STDrqr:
  case QISA::STDrqi:

  case QISA::MOVHIi16:
  case QISA::INCri:
  case QISA::MOV_rr:
  // case QISA::SHL8mCLS:

  // case QISA::STDrm:
  // case QISA::LDDrm:

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

  int Offset = ObjectOffset + StackSize + ImmOp.getImm();

  auto Operand = OPERAND_TYPE_UDADDR;

  if (IgnoreFrameReg == IDSBasePtr) {
    Operand = OPERAND_TYPE_IDADDR;
    // Add 4, because the IData address start from 0xA004
    Offset -= (StackSize - DataOffset);
    Offset += 4;
  }

  FIOp.ChangeToRegister(IgnoreFrameReg, false);
  ImmOp.setImm(Offset);

  Register Reg = MI.getOperand(FIOperandNum - 1).getReg();

  if (MI.getOpcode() == QISA::LDIradd){

    BuildMI(*MI.getParent(), MII, DL,TII.get(QISA::LDDri), Reg)
      .addImm( Operand + (Offset));    
    
    MI.eraseFromParent();

  }
  else if (MI.getOpcode() == QISA::STIradd){
    
    BuildMI(*MI.getParent(), MII, DL, TII.get(QISA::STDri), Reg)
      .addImm(Operand + Offset);
    MI.eraseFromParent();
  }

}

Register QISARegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return QISA::R61;
}
