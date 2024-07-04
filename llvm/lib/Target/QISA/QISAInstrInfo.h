//===-- QISAInstrInfo.h - QISA instruction information ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the QISA implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QISA_QISAINSTRINFO_H
#define LLVM_LIB_TARGET_QISA_QISAINSTRINFO_H

#include "QISA.h"
#include "QISARegisterInfo.h"
#include <QISACondCode.h>

#include "llvm/ADT/ArrayRef.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include <cstdint>

#define GET_INSTRINFO_HEADER
#include "QISAGenInstrInfo.inc"

// namespace llvm {

// class QISASubtarget;

// class QISAInstrInfo : public QISAGenInstrInfo {
//   const QISARegisterInfo RI;
//   QISASubtarget &STI;

//   virtual void anchor();

// public:
//   explicit QISAInstrInfo(QISASubtarget &STI);

//   // Return the QISARegisterInfo, which this class owns.
//   const QISARegisterInfo &getRegisterInfo() const { return RI; }
// };

// } // end namespace llvm

namespace llvm {

class QISASubtarget;

class QISAInstrInfo : public QISAGenInstrInfo {
  const QISARegisterInfo RI;
  const QISASubtarget &STI;

  virtual void anchor();

public:
  explicit QISAInstrInfo(const QISASubtarget &STI);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  const QISARegisterInfo &getRegisterInfo() const { return RI; }

  /// isLoadFromStackSlot - If the specified machine instruction is a direct
  /// load from a stack slot, return the virtual or physical register number of
  /// the destination along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than loading from the stack slot.
  unsigned isLoadFromStackSlot(const MachineInstr &MI,
                               int &FrameIndex) const override;

  /// isStoreToStackSlot - If the specified machine instruction is a direct
  /// store to a stack slot, return the virtual or physical register number of
  /// the source reg along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than storing to the stack slot.
  unsigned isStoreToStackSlot(const MachineInstr &MI,
                              int &FrameIndex) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI,
                           Register SrcReg, bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI,
                            Register DestReg, int FrameIndex,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

  // Lower pseudo instructions after register allocation.
  // bool expandPostRAPseudo(MachineInstr &MI) const override;
                              
};
}

#endif // LLVM_LIB_TARGET_QISA_QISAINSTRINFO_H
