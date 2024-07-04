//===-- QASMISelLowering.h - QASM DAG lowering interface --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that QASM uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QASM_QASMISELLOWERING_H
#define LLVM_LIB_TARGET_QASM_QASMISELLOWERING_H

#include "QASM.h"
#include "QASMInstrInfo.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class QASMSubtarget;
class QASMSubtarget;

namespace QASMISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return with a flag operand.  Operand 0 is the chain operand.
  RET_FLAG,

  // Calls a function.  Operand 0 is the chain operand and operand 1
  // is the target address.  The arguments start at operand 2.
  // There is an optional glue operand at the end.
  CALL,

  // Bit-field instructions.
  CLR,
  SET,
  EXT,
  EXTU,
  MAK,
  ROT,
  FF1,
  FF0,

  // Classical Control Command Orders
  BR_CC,
  INC,
  DEC,
  CMP,
  MOV,
  QMOV,
  FMR,
  STD,
  
  // Quantum instruction
  H_GATE,
  DEFORM,
  FLIP_EXPAND,
  FLIP_CONTRACT,
  FLIP_SHIFT,

  Z_GATE,
  X_GATE,
  Y_GATE,
  CNOT_GATE,
  CY_GATE,
  CZ_GATE,
  CH_GATE,
  CRX_GATE,
  CRY_GATE,
  CRZ_GATE,
  CU1_GATE,
  CU2_GATE,
  CU3_GATE,
  P_GATE,
  U_GATE,
  U1_GATE,
  U2_GATE,
  U3_GATE,
  RX_GATE,
  RY_GATE,
  RZ_GATE,
  CSWAP_GATE,
  CCX_GATE,
  CCZ_GATE,
  CCY_GATE,
  CCH_GATE,
  CCU_GATE,
  SWAP_GATE,
  MEAS_X_GATE,
  MEAS_Z_GATE,

  // Surgery-based instructions
  MERGE_MXX,
  MERGE_MZZ,
  SPLIT_MXX,
  SPLIT_MZZ,
  CNOT_POST_MXX,
  CNOT_POST_MZZ,
  MOVE_POST_MXX,
  MOVE_POST_MZZ,
};
} // end namespace QASMISD

// The TargetFrameLowering class is used to provide information about the stack frame 
// layout of the target. It holds the direction of stack growth, the known stack 
// alignment on entry to each function, and the offset to the local area. The offset 
// to the local area is the offset from the stack pointer on function entry to the 
// first location where function data (local variables, spill locations) can be stored.
class QASMTargetLowering : public TargetLowering {
  const QASMSubtarget &Subtarget;

public:
  explicit QASMTargetLowering(const TargetMachine &TM,
                              const QASMSubtarget &STI);

  // Override TargetLowering methods.
  bool hasAndNot(SDValue X) const override { return true; }

  // Override TargetLowering methods: TODO check the legal
  bool isTypeLegal(EVT VT) const { return true; }

  const char *getTargetNodeName(unsigned Opcode) const override;

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

  // Override required hooks.
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

  SDValue LowerCall(CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerMeasure(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerLoad(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerStore(SDValue Op, SelectionDAG &DAG) const;

  SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;


  SDValue LowerAdd(SDValue Op, SelectionDAG &DAG) const;

  SDValue LowerQMOV(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerUGate(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCtrlTargetGate(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerMultiCtrlTargetGate(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerRotationGate(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCtrlRotationGate(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerCtrlUGate(SDValue Op, SelectionDAG &DAG) const;

};

} // end namespace llvm

#endif
