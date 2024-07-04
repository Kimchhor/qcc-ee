//===-- QASMInstrInfo.cpp - QASM instruction information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the QASM implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "QASMInstrInfo.h"
#include "QASM.h"
#include "MCTargetDesc/QASMMCTargetDesc.h"
//#include "QASMInstrBuilder.h"
#include "QASMSubtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/LiveInterval.h"
#include "llvm/CodeGen/LiveIntervals.h"
#include "llvm/CodeGen/LiveVariables.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SlotIndexes.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/Support/BranchProbability.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Target/TargetMachine.h"
#include <cassert>
#include <cstdint>
#include <iterator>

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRMAP_INFO
#include "QASMGenInstrInfo.inc"

#define DEBUG_TYPE "qasm-ii"

// Pin the vtable to this file.
void QASMInstrInfo::anchor() {}

QASMInstrInfo::QASMInstrInfo(const QASMSubtarget &STI)
    : QASMGenInstrInfo(), RI(), STI(STI) {}

// QASMInstrInfo::QASMInstrInfo(QASMSubtarget &STI)
//     : QASMGenInstrInfo(), RI(),
//       Subtarget(ST) {}


/// isLoadFromStackSlot - If the specified machine instruction is a direct
/// load from a stack slot, return the virtual or physical register number of
/// the destination along with the FrameIndex of the loaded stack slot.  If
/// not, return 0.  This predicate must return 0 if the instruction has
/// any side effects other than loading from the stack slot.
unsigned
QASMInstrInfo::isLoadFromStackSlot(const MachineInstr &MI, int &FrameIndex) const{
  // assert(0 && "Unimplemented");
  return 0;
}
  
  /// isStoreToStackSlot - If the specified machine instruction is a direct
  /// store to a stack slot, return the virtual or physical register number of
  /// the source reg along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than storing to the stack slot.
unsigned
QASMInstrInfo::isStoreToStackSlot(const MachineInstr &MI,
                                   int &FrameIndex) const {
  assert(0 && "Unimplemented");
  return 0;
}


void QASMInstrInfo::copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const {
  unsigned Opc;

  if (QASM::GPRRegClass.contains(DestReg, SrcReg))
    Opc = QASM::MOV_rr;
  else if (QASM::QPRRegClass.contains(DestReg, SrcReg))
    Opc = QASM::QMOVrr;
  else
    llvm_unreachable("Impossible reg-to-reg copy");

  BuildMI(MBB, I, I->getDebugLoc(), get(Opc), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc)); 

}

void QASMInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI,
                           Register SrcReg, bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const {

    BuildMI(MBB, MBBI, MBBI->getDebugLoc(), get(QASM::STIradd))
    .addReg(SrcReg, getKillRegState(isKill))
    .addFrameIndex(FrameIndex);                        

  
}

void QASMInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI,
                            Register DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const {

  // BuildMI(MBB, MBBI, MBBI->getDebugLoc(), get(QASM::LDIradd), DestReg)
  //     .addFrameIndex(FrameIndex).addImm(0);

}

//   // Lower pseudo instructions after register allocation.
// bool QASMInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {

//   switch (MI->getOpcode())
//     {
//     default:
//       return false;
//     case QASMISD::MOVi32: {
//       DebugLoc DL = MI->getDebugLoc();
//       MachineBasicBlock &MBB = *MI->getParent();

//       const unsigned DstReg = MI->getOperand(0).getReg();
//       const bool DstIsDead = MI->getOperand(0).isDead();

//       const MachineOperand &MO = MI->getOperand(1);

//       auto LO16 = BuildMI(MBB, MI, DL, get(QASM::MOVLOi16), DstReg);
//       auto HI16 = BuildMI(MBB, MI, DL, get(QASM::MOVHIi16))
//                       .addReg(DstReg, RegState::Define | getDeadRegState(DstIsDead))
//                       .addReg(DstReg);

//       if (MO.isImm()) {
//         const unsigned Imm = MO.getImm();
//         const unsigned Lo16 = Imm & 0xffff;
//         const unsigned Hi16 = (Imm >> 16) & 0xffff;
//         LO16 = LO16.addImm(Lo16);
//         HI16 = HI16.addImm(Hi16);
//       } else {
//         const GlobalValue *GV = MO.getGlobal();
//         const unsigned TF = MO.getTargetFlags();
//         LO16 = LO16.addGlobalAddress(GV, MO.getOffset(), TF | LEGII::MO_LO16);
//         HI16 = HI16.addGlobalAddress(GV, MO.getOffset(), TF | LEGII::MO_HI16);
//       }

//       MBB.erase(MI);
//       return true;
//     }
//     }


// }