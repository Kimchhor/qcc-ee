//===-- QASMRegisterInfo.h - QASM Register Information Impl -----*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_QASM_QASMREGISTERINFO_H
#define LLVM_LIB_TARGET_QASM_QASMREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "QASMGenRegisterInfo.inc"

namespace llvm {

struct QASMRegisterInfo : public QASMGenRegisterInfo {
  QASMRegisterInfo();


  // Physical register used as IData Segment ptr
  unsigned IDSBasePtr;

  // Physical register used as UData Segment ptr
  unsigned UDSBasePtr;

  // Physical register used as RData Segment ptr
  unsigned RDSBasePtr;

  /// Code Generation virtual methods...
  // Returns a list of callee-saved registers in the order of the desired callee-save stack frame offset.
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  // Returns a bitset indexed by physical register numbers, indicating if a particular register is unavailable.
  BitVector getReservedRegs(const MachineFunction &MF) const override;

  // Eliminate abstract frame indices from instructions that may use them.
  void eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;

public:
  unsigned getIDSBaseRegister() const { return IDSBasePtr; }
  unsigned getUDSBaseRegister() const { return UDSBasePtr; }
  unsigned getRDSBaseRegister() const { return RDSBasePtr; }

#if 0
  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const override;

  const uint32_t* getRTCallPreservedMask(CallingConv::ID CC) const;

  const TargetRegisterClass *getPointerRegClass(const MachineFunction &MF,
                                                unsigned Kind) const override;

  bool canRealignStack(const MachineFunction &MF) const override;
#endif
};

} // end namespace llvm

#endif
