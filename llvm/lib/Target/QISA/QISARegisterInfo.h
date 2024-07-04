//===-- QISARegisterInfo.h - QISA Register Information Impl -----*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_QISA_QISAREGISTERINFO_H
#define LLVM_LIB_TARGET_QISA_QISAREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "QISAGenRegisterInfo.inc"

namespace llvm {

struct QISARegisterInfo : public QISAGenRegisterInfo {
  QISARegisterInfo();


  // Physical register used as IData Segment ptr
  unsigned IDSBasePtr;

  // Physical register used as UData Segment ptr
  unsigned UDSBasePtr;


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
