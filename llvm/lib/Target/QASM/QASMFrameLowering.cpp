//===-- QASMFrameLowering.cpp - Frame lowering for QASM -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QASMFrameLowering.h"
#include "QASM.h"
#include "QASMInstrInfo.h"

#include "QASMRegisterInfo.h"
#include "QASMSubtarget.h"

#include "QASMMachineFunctionInfo.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DataLayout.h"

#include "llvm/Target/TargetMachine.h"
#include <algorithm>
#include "llvm/Support/MathExtras.h"

using namespace llvm;

QASMFrameLowering::QASMFrameLowering()
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(8), 0,
                          Align(8), false /* StackRealignable */),
      RegSpillOffsets(0) {}

uint64_t QASMFrameLowering::computeStackSize(MachineFunction &MF) const {
  uint64_t StackSize = MF.getFrameInfo().getStackSize();
  unsigned StackAlign = getStackAlignment();

  if (StackAlign > 0) {
    // Round Up to alignment
    /// Returns the next integer (mod 2**64) that is greater than or equal to
    /// \p StackSize and is a multiple of \p StackSize. \p StackAlign must be non-zero.
    uint64_t skew = 0;
    skew %= StackAlign;
    StackSize = (StackSize + StackAlign - 1 - skew) / StackAlign * StackAlign + skew;
  } 
  return StackSize;  
}
// Materialize an offset for a ADD/SUB stack operation.
// Return zero if the offset fits into the instruction as an immediate,
// or the number of the register where the offset is materialized.
static unsigned materializeOffset(MachineFunction &MF, MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MBBI,
                                  unsigned Offset) {
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  const uint64_t MaxSubImm = 0xfff;
  if (Offset <= MaxSubImm) {
    // The stack offset fits in the ADD/SUB instruction.
    return 0;
  } else {
    // The stack offset does not fit in the ADD/SUB instruction.
    // Materialize the offset using MOVLO/MOVHI.
    unsigned OffsetReg = QASM::R4;
    unsigned OffsetLo = (unsigned)(Offset & 0xffff);
    unsigned OffsetHi = (unsigned)((Offset & 0xffff0000) >> 16);
    BuildMI(MBB, MBBI, dl, TII.get(QASM::MOVLOi16), OffsetReg)
        .addImm(OffsetLo)
        .setMIFlag(MachineInstr::FrameSetup);
    if (OffsetHi) {
      BuildMI(MBB, MBBI, dl, TII.get(QASM::MOVHIi16), OffsetReg)
          .addReg(OffsetReg)
          .addImm(OffsetHi)
          .setMIFlag(MachineInstr::FrameSetup);
    }
    return OffsetReg;
  }
}

void QASMFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {

         // Compute the stack size, to determine if we need a prologue at all.
  // const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  // MachineBasicBlock::iterator MBBI = MBB.begin();
  // DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  // uint64_t StackSize = computeStackSize(MF);
  // if (!StackSize) {
  //   return;
  // }

  // // // // Adjust the stack pointer.
  // unsigned StackReg = QASM::R61; // SP
  // unsigned OffsetReg = materializeOffset(MF, MBB, MBBI, (unsigned)StackSize);
  // if (OffsetReg) {
  //   BuildMI(MBB, MBBI, dl, TII.get(QASM::DECrr), StackReg)
  //       .addReg(StackReg)
  //       .addReg(OffsetReg)
  //       .setMIFlag(MachineInstr::FrameSetup);
  // } else {
  //   BuildMI(MBB, MBBI, dl, TII.get(QASM::DECri), StackReg)
  //       .addReg(StackReg)
  //       .addImm(StackSize)
  //       .setMIFlag(MachineInstr::FrameSetup);
  // }                               

}

void QASMFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {

  // Compute the stack size, to determine if we need an epilogue at all.
  // const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  // MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  // DebugLoc dl = MBBI->getDebugLoc();
  // uint64_t StackSize = computeStackSize(MF);
  // if (!StackSize) {
  //   return;
  // }

  // // Restore the stack pointer to what it was at the beginning of the function.
  // unsigned StackReg = QASM::R61; // SP
  // unsigned OffsetReg = materializeOffset(MF, MBB, MBBI, (unsigned)StackSize);
  // if (OffsetReg) {
  //   BuildMI(MBB, MBBI, dl, TII.get(QASM::INCrr), StackReg)
  //       .addReg(StackReg)
  //       .addReg(OffsetReg)
  //       .setMIFlag(MachineInstr::FrameSetup);
  // } else {
  //   BuildMI(MBB, MBBI, dl, TII.get(QASM::INCri), StackReg)
  //       .addReg(StackReg)
  //       .addImm(StackSize)
  //       .setMIFlag(MachineInstr::FrameSetup);
  // }


}

bool QASMFrameLowering::hasFP(const MachineFunction &MF) const { 
  
  // bool hasFP = MF.getTarget().Options.DisableFramePointerElim(MF) ||
  //        MF.getFrameInfo().hasVarSizedObjects();
  return false; 
  
  }

void QASMFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                            BitVector &SavedRegs,
                                            RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  SavedRegs.reset(QASM::R14);
  SavedRegs.reset(QASM::R15);
  SavedRegs.reset(QASM::R16);
  SavedRegs.reset(QASM::R17);
  SavedRegs.reset(QASM::R18);
  SavedRegs.reset(QASM::R19);
  SavedRegs.reset(QASM::R20);
  SavedRegs.reset(QASM::R21);
  SavedRegs.reset(QASM::R23);
  SavedRegs.reset(QASM::R24);
  SavedRegs.reset(QASM::R25);
  SavedRegs.reset(QASM::R3);
}

bool QASMFrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty()) {
    return false;
  return false; 
    return false;
  }

  unsigned CalleeFrameSize = 0;
  DebugLoc DL = MBB.findDebugLoc(MI);
  MachineFunction &MF = *MBB.getParent();
  // const QASMSubtarget &STI = MF.getSubtarget<QASMSubtarget>();
  // const TargetInstrInfo &TII = *STI.getInstrInfo();
  QASMMachineFunctionInfo *QASMFI = MF.getInfo<QASMMachineFunctionInfo>();

  for (unsigned i = CSI.size(); i != 0; --i) {
    unsigned Reg = CSI[i - 1].getReg();
    bool IsNotLiveIn = !MBB.isLiveIn(Reg);

    assert(TRI->getRegSizeInBits(*TRI->getMinimalPhysRegClass(Reg)) == 8 &&
           "Invalid register size");

    // Add the callee-saved register as live-in only if it is not already a
    // live-in register, this usually happens with arguments that are passed
    // through callee-saved registers.
    if (IsNotLiveIn) {
      MBB.addLiveIn(Reg);
    }

    // Do not kill the register when it is an input argument.
    // BuildMI(MBB, MI, DL, TII.get(QASM::PUSHRr))
    //     .addReg(Reg, getKillRegState(IsNotLiveIn))
    //     .setMIFlag(MachineInstr::FrameSetup);
    ++CalleeFrameSize;
  }

  QASMFI->setCalleeSavedFrameSize(CalleeFrameSize);

  return true;
}

StackOffset QASMFrameLowering::getFrameIndexReferencePreferSP(
    const MachineFunction &MF, int FI, Register &FrameReg,
    bool IgnoreSPUpdates) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  if (IgnoreSPUpdates) {
    errs() << "Offset from the SP for " << FI << " is "
                      << MFI.getObjectOffset(FI) << "\n";
    FrameReg = QASM::QR9;
    return StackOffset::getFixed(MFI.getObjectOffset(FI));
  }

  return getFrameIndexReference(MF, FI, FrameReg);
}

StackOffset QASMFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                             Register &FrameReg) const {

  // auto &MFI = MF.getFrameInfo();
  auto &HRI = *MF.getSubtarget<QASMSubtarget>().getRegisterInfo();

  unsigned DataOffset = 0;

  auto &HMFI = *MF.getInfo<QASMMachineFunctionInfo>();

  if (HMFI.isIDSRegister(FI)){
    FrameReg = HRI.getIDSBaseRegister();
    DataOffset = HMFI.getIDSRegisterSize() * 4;
      
  }else if (HMFI.isUDSRegister(FI)){
    FrameReg = HRI.getUDSBaseRegister();
    DataOffset = HMFI.getUDSRegisterSize() * 4;
    
  } else if (HMFI.isRDSRegister(FI)){
    FrameReg = HRI.getRDSBaseRegister();
    DataOffset = HMFI.getRDSRegisterSize() * 4;
  }
  else{
    llvm_unreachable("Can't get register in FrameIndexReference");
  }
  
                             
  return StackOffset::getFixed(DataOffset);
}