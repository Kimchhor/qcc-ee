//===-- QISAMachineFuctionInfo.h - QISA machine function info -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares QISA-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_QISA_MACHINE_FUNCTION_INFO_H
#define LLVM_QISA_MACHINE_FUNCTION_INFO_H

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/IR/Instructions.h"

namespace llvm {
enum DataRegisterType{
  IDS = 0,
  UDS = 1
};
/// Contains QISA-specific information for each MachineFunction.
class QISAMachineFunctionInfo : public MachineFunctionInfo {
  /// Indicates if a register has been spilled by the register
  /// allocator.
  bool HasSpills;

  /// Indicates if there are any fixed size allocas present.
  /// Note that if there are only variable sized allocas this is set to false.
  bool HasAllocas;

  /// Indicates if arguments passed using the stack are being
  /// used inside the function.
  bool HasStackArgs;

  /// Whether or not the function is an interrupt handler.
  bool IsInterruptHandler;

  /// Whether or not the function is an non-blocking interrupt handler.
  bool IsSignalHandler;

  /// Size of the callee-saved register portion of the
  /// stack frame in bytes.
  unsigned CalleeSavedFrameSize;

  /// FrameIndex for start of varargs area.
  int VarArgsFrameIndex;

  /// IData Segment register size
  unsigned IDSRegisterSize = 0;

  /// UData Segment register size
  unsigned UDSRegisterSize = 0;
  
  /// Tracking the Data Register type wether it is IDS or UDS
  std::vector<DataRegisterType> dataRegisterType = {};

  /// Set the Data Segment (IDS and UDS) Register size
  void getDataSegmentRegsiterSize(MachineFunction &MF);
public:
  QISAMachineFunctionInfo()
      : HasSpills(false), HasAllocas(false), HasStackArgs(false),
        IsInterruptHandler(false), IsSignalHandler(false),
        CalleeSavedFrameSize(0), VarArgsFrameIndex(0), IDSRegisterSize(0), UDSRegisterSize(0) {}

  explicit QISAMachineFunctionInfo(MachineFunction &MF)
      : HasSpills(false), HasAllocas(false), HasStackArgs(false),
        CalleeSavedFrameSize(0), VarArgsFrameIndex(0) {
    unsigned CallConv = MF.getFunction().getCallingConv();

    this->IsInterruptHandler = CallConv == MF.getFunction().hasFnAttribute("interrupt");
    this->IsSignalHandler = CallConv ==  MF.getFunction().hasFnAttribute("signal");

    getDataSegmentRegsiterSize(MF);
  }

  bool getHasSpills() const { return HasSpills; }
  void setHasSpills(bool B) { HasSpills = B; }

  bool getHasAllocas() const { return HasAllocas; }
  void setHasAllocas(bool B) { HasAllocas = B; }

  bool getHasStackArgs() const { return HasStackArgs; }
  void setHasStackArgs(bool B) { HasStackArgs = B; }

  /// Checks if the function is some form of interrupt service routine.
  bool isInterruptOrSignalHandler() const { return isInterruptHandler() || isSignalHandler(); }

  bool isInterruptHandler() const { return IsInterruptHandler; }
  bool isSignalHandler() const { return IsSignalHandler; }

  unsigned getCalleeSavedFrameSize() const { return CalleeSavedFrameSize; }
  void setCalleeSavedFrameSize(unsigned Bytes) { CalleeSavedFrameSize = Bytes; }

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Idx) { VarArgsFrameIndex = Idx; }

  unsigned getIDSRegisterSize() const {return IDSRegisterSize;}
  void setIDSRegisterSize(unsigned size) { IDSRegisterSize = size; }
  
  unsigned getUDSRegisterSize() const {return UDSRegisterSize;}
  void setUDSRegisterSize(unsigned size) { UDSRegisterSize = size; }

  bool isIDSRegister(unsigned FrameIndex) const { return dataRegisterType[FrameIndex] == IDS;}
  bool isUDSRegister(unsigned FrameIndex) const { return dataRegisterType[FrameIndex] == UDS;}

 
};

} // end llvm namespace

#endif // LLVM_QISA_MACHINE_FUNCTION_INFO_H
