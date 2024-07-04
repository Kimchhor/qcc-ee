//===-- QASMFrameLowering.h - Frame lowering for QASM -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QASM_QASMFRAMELOWERING_H
#define LLVM_LIB_TARGET_QASM_QASMFRAMELOWERING_H

#include "llvm/Support/TypeSize.h"
#include "llvm/ADT/IndexedMap.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class QASMTargetMachine;
class QASMSubtarget;

class QASMFrameLowering : public TargetFrameLowering {
  IndexedMap<unsigned> RegSpillOffsets;
  
public:
  QASMFrameLowering();

  // Override TargetFrameLowering.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  bool hasFP(const MachineFunction &MF) const override;
  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;
   bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 ArrayRef<CalleeSavedInfo> CSI,
                                 const TargetRegisterInfo *TRI) const override;                          
  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator MI) const override {
    return MBB.erase(MI);
  }
  StackOffset
  getFrameIndexReferencePreferSP(const MachineFunction &MF, int FI,
                                 Register &FrameReg,
                                 bool IgnoreSPUpdates) const override;
  StackOffset getFrameIndexReference(const MachineFunction &MF, int FI,
                                             Register &FrameReg) const override;
private:
  uint64_t computeStackSize(MachineFunction &MF) const;
};


} // end namespace llvm

#endif