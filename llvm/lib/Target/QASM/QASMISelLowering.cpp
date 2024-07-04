//===-- QASMISelLowering.cpp - QASM DAG lowering implementation -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the QASMTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "QASMISelLowering.h"
#include "QASMCondCode.h"
#include "QASMDefine.h"
//#include "QASMCallingConv.h"
//#include "QASMConstantPoolValue.h"
//#include "QASMMachineFunctionInfo.h"
#include "QASMTargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
//#include "llvm/IR/IntrinsicsQASM.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/KnownBits.h"
#include <cctype>

using namespace llvm;

#define DEBUG_TYPE "QASM-lower"

// If I is a shifted mask, set the size (Width) and the first bit of the
// mask (Offset), and return true.
// For example, if I is 0x003e, (Width, Offset) = (5, 1).
static bool isShiftedMask(uint64_t I, uint64_t &Width, uint64_t &Offset) {
  if (!isShiftedMask_64(I))
    return false;

  Width = countPopulation(I);
  Offset = countTrailingZeros(I);
  return true;
}

static QASMCC::CondCode ISDCCtoQASMCC(ISD::CondCode isdCC) {
  switch (isdCC) {
  case ISD::SETUEQ:
    return QASMCC::EQ;
  case ISD::SETUGT:
    return QASMCC::HI;
  case ISD::SETUGE:
    return QASMCC::HS;
  case ISD::SETULT:
    return QASMCC::LO;
  case ISD::SETULE:
    return QASMCC::LS;
  case ISD::SETUNE:
    return QASMCC::NE;
  case ISD::SETEQ:
    return QASMCC::EQ;
  case ISD::SETGT:
    return QASMCC::GT;
  case ISD::SETGE:
    return QASMCC::GE;
  case ISD::SETLT:
    return QASMCC::LT;
  case ISD::SETLE:
    return QASMCC::LE;
  case ISD::SETNE:
    return QASMCC::NE;
  default:
    llvm_unreachable("Unhandled ISDCC code.");
  }
}

static QASMISD::NodeType ISDtoQASM(unsigned OpType){
  switch (OpType)
  {
  case ISD::MEAS_X_GATE:        return QASMISD::MEAS_X_GATE;
  case ISD::MEAS_Z_GATE:        return QASMISD::MEAS_Z_GATE;
  case ISD::X_GATE:             return QASMISD::X_GATE;
  case ISD::Z_GATE:             return QASMISD::Z_GATE;  
  default:
    llvm_unreachable("Unhandled ISD in ISDtoQASM()");
  }
}

static QASMISD::NodeType ISDtoCtrlTarQASM(unsigned OpType){
  switch (OpType)
  {
  case ISD::X_GATE:             return QASMISD::CNOT_GATE;
  case ISD::Y_GATE:             return QASMISD::CY_GATE;
  case ISD::Z_GATE:             return QASMISD::CZ_GATE;
  case ISD::H_GATE:             return QASMISD::CH_GATE;
  case ISD::U_GATE:             return QASMISD::CU1_GATE;
  case ISD::SWAP_GATE:          return QASMISD::SWAP_GATE;
  default:
    llvm_unreachable("Unhandled ISD in ISDtoCtrlTarQASM()");
  }
}

static QASMISD::NodeType ISDtoMultiCtrlTarQASM(unsigned OpType){
  switch (OpType)
  {
  case ISD::X_GATE:             return QASMISD::CCX_GATE;
  case ISD::Y_GATE:             return QASMISD::CCY_GATE;
  case ISD::Z_GATE:             return QASMISD::CCZ_GATE;
  case ISD::H_GATE:             return QASMISD::CCH_GATE;
  case ISD::U_GATE:             return QASMISD::CCU_GATE;
  case ISD::SWAP_GATE:          return QASMISD::CSWAP_GATE;
  default:
    llvm_unreachable("Unhandled ISD in ISDtoMultiCtrlTarQASM()");
  }
}

static QASMISD::NodeType ISDtoRotationQASM(unsigned OpType){
  switch (OpType)
  {
  case ISD::X_GATE:             return QASMISD::RX_GATE;
  case ISD::Y_GATE:             return QASMISD::RY_GATE;
  case ISD::Z_GATE:             return QASMISD::RZ_GATE;
  case ISD::P_GATE:             return QASMISD::P_GATE;
  default:
    llvm_unreachable("Unhandled ISD in ISDtoRotationQASM()");
  }
}

static QASMISD::NodeType ISDtoUnatryGateQASM(unsigned numAngle){
  switch (numAngle)
  {
  case 1:             return QASMISD::U1_GATE;
  case 2:             return QASMISD::U2_GATE;
  case 3:             return QASMISD::U3_GATE;
  default:
    llvm_unreachable("Unhandled ISD in ISDtoUnatryGateQASM()");
  }
}

static QASMISD::NodeType ISDtoCtrlUnatryGateQASM(unsigned numAngle){
  switch (numAngle)
  {
  case 1:             return QASMISD::CU1_GATE;
  case 2:             return QASMISD::CU2_GATE;
  case 3:             return QASMISD::CU3_GATE;
  default:
    llvm_unreachable("Unhandled ISD in ISDtoUnatryGateQASM()");
  }
}

static QASMISD::NodeType ISDtoCtrlRotationQASM(unsigned OpType){
  switch (OpType)
  {
  case ISD::X_GATE:             return QASMISD::CRX_GATE;
  case ISD::Y_GATE:             return QASMISD::CRY_GATE;
  case ISD::Z_GATE:             return QASMISD::CRZ_GATE;
  default:
    llvm_unreachable("Unhandled ISD in ISDtoCtrlRotationQASM()");
  }
}


QASMTargetLowering::QASMTargetLowering(const TargetMachine &TM,
                                       const QASMSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {

  addRegisterClass(MVT::i32, &QASM::GPRRegClass);
  addRegisterClass(MVT::f32, &QASM::GPRRegClass);
  addRegisterClass(MVT::f64, &QASM::GPRRegClass);

  setOperationAction(ISD::ConstantFP,           MVT::f32,   Legal);
  setOperationAction(ISD::ConstantFP,           MVT::f64,   Legal);
  addRegisterClass(MVT::qubit, &QASM::QPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(Subtarget.getRegisterInfo());

  // Set up special registers.
  setStackPointerRegisterToSaveRestore(QASM::R61);
  setStackPointerRegisterToSaveRestore(QASM::QR61);

  // How we extend i1 boolean values.
  setBooleanContents(ZeroOrOneBooleanContent);

  setMinFunctionAlignment(Align(8));
  setPrefFunctionAlignment(Align(8));

  // setOperationAction(ISD::CTLZ, MVT::i32, Custom);
  setOperationAction(ISD::CTTZ, MVT::i32, Expand);

  setOperationAction(ISD::LOAD, MVT::qubit, Custom);

  // setOperationAction(ISD::FrameIndex, MVT::i32, Custom);

  setOperationAction(ISD::MEAS_Z_GATE, MVT::qubit, Custom);
  setOperationAction(ISD::MEAS_X_GATE, MVT::qubit, Custom);
  setOperationAction(ISD::X_GATE, MVT::qubit, Custom);


  setOperationAction(ISD::STORE, MVT::qubit, Custom);

  // setOperationAction(ISD::BRCOND, MVT::Other, Custom);

  setOperationAction(ISD::BR_CC, MVT::i32, Custom);
  setOperationAction(ISD::ADD, MVT::i32, Custom);
  setOperationAction(ISD::SUB, MVT::i32, Legal);

  // setOperationAction(ISD::H_GATE, MVT::qubit, Custom);
  setOperationAction(ISD::SWAP_GATE, MVT::qubit, Custom);

  // setOperationAction(ISD::FrameIndex, MVT::i32, Custom);

  // Special DAG combiner for bit-field operations.
  // setTargetDAGCombine(ISD::AND);
  // setTargetDAGCombine(ISD::OR);
  // setTargetDAGCombine(ISD::SHL);

  // All target gates need to perform DAG Combine
  setTargetDAGCombine(ISD::X_GATE);
}

SDValue QASMTargetLowering::LowerOperation(SDValue Op,
                                           SelectionDAG &DAG) const {
  // TODO Implement for ops not covered by patterns in .td files.

  switch (Op.getOpcode()) {
  case ISD::MEAS_X_GATE:
  case ISD::MEAS_Y_GATE:
  case ISD::MEAS_Z_GATE:        return LowerMeasure(Op, DAG);
  case ISD::LOAD:               return LowerLoad(Op, DAG);
  // case ISD::STORE:              return LowerStore(Op, DAG);
  case ISD::BR_CC:              return LowerBR_CC(Op, DAG);
  case ISD::ADD:                return LowerAdd(Op, DAG);

  case ISD::X_GATE:
  case ISD::Y_GATE:
  case ISD::Z_GATE:
  case ISD::H_GATE:
  case ISD::P_GATE:
  case ISD::SWAP_GATE:
      return LowerCtrlTargetGate(Op, DAG);
  case ISD::U_GATE:
      return LowerUGate(Op, DAG);
  }
  return Op;
}

// Clone from SMP430ISelLowering.cpp
static SDValue EmitCMP(SDValue &LHS, SDValue &RHS, SDValue &TargetCC,
                       ISD::CondCode CC, const SDLoc &dl, SelectionDAG &DAG) {
  // FIXME: Handle bittests someday
  assert(!LHS.getValueType().isFloatingPoint() && "We don't handle FP yet");

  // FIXME: Handle jump negative someday
  QASMCC::CondCode TCC;
  switch (CC) {
  default: llvm_unreachable("Invalid integer condition!");
  case ISD::SETEQ:
    TCC = QASMCC::EQ;     // aka Z
    // Minor optimization: if LHS is a constant, swap operands, then the
    // constant can be folded into comparison.
    if (LHS.getOpcode() == ISD::Constant)
      std::swap(LHS, RHS);
    break;
  case ISD::SETNE:
    TCC = QASMCC::NE;    // aka NZ
    // Minor optimization: if LHS is a constant, swap operands, then the
    // constant can be folded into comparison.
    if (LHS.getOpcode() == ISD::Constant)
      std::swap(LHS, RHS);
    break;
  case ISD::SETULE:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  case ISD::SETUGE:
    // Turn lhs u>= rhs with lhs constant into rhs u< lhs+1, this allows us to
    // fold constant into instruction.
    if (const ConstantSDNode * C = dyn_cast<ConstantSDNode>(LHS)) {
      LHS = RHS;
      RHS = DAG.getConstant(C->getSExtValue() + 1, dl, C->getValueType(0));
      TCC = QASMCC::LO;
      break;
    }
    TCC = QASMCC::HS;    // aka C
    break;
  case ISD::SETUGT:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  case ISD::SETULT:
    // Turn lhs u< rhs with lhs constant into rhs u>= lhs+1, this allows us to
    // fold constant into instruction.
    if (const ConstantSDNode * C = dyn_cast<ConstantSDNode>(LHS)) {
      LHS = RHS;
      RHS = DAG.getConstant(C->getSExtValue() + 1, dl, C->getValueType(0));
      TCC = QASMCC::HS;
      break;
    }
    TCC = QASMCC::LO;    // aka NC
    break;
  case ISD::SETLE:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  case ISD::SETGE:
    // Turn lhs >= rhs with lhs constant into rhs < lhs+1, this allows us to
    // fold constant into instruction.
    if (const ConstantSDNode * C = dyn_cast<ConstantSDNode>(LHS)) {
      LHS = RHS;
      RHS = DAG.getConstant(C->getSExtValue() + 1, dl, C->getValueType(0));
      TCC = QASMCC::LT;
      break;
    }
    TCC = QASMCC::GE;
    break;
  case ISD::SETGT:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  case ISD::SETLT:
    // Turn lhs < rhs with lhs constant into rhs >= lhs+1, this allows us to
    // fold constant into instruction.
    if (const ConstantSDNode * C = dyn_cast<ConstantSDNode>(LHS)) {
      LHS = RHS;
      RHS = DAG.getConstant(C->getSExtValue() + 1, dl, C->getValueType(0));
      TCC = QASMCC::GE;
      break;
    }
    TCC = QASMCC::LT;
    break;
  }
  TargetCC = DAG.getConstant(TCC, dl, MVT::i32);
  return DAG.getNode(QASMISD::CMP, dl, MVT::Glue, LHS, RHS);
}

SDValue QASMTargetLowering::LowerLoad(SDValue Op, SelectionDAG &DAG) const {
    SDLoc dl(Op);
    LoadSDNode *LD = cast<LoadSDNode>(Op);

    SDValue Chain = LD->getChain();
    SDValue BasePtr = LD->getBasePtr();
    MachineMemOperand *MMO = LD->getMemOperand();
    Align Alignment = commonAlignment(LD->getOriginalAlign(), 8);

    // Create new Load instruction
    // SDValue NewLD = DAG.getExtLoad(ISD::EXTLOAD, dl, MVT::i32, Chain, BasePtr,
    //                                MVT::i32, MMO);
    SDValue NewLD = DAG.getLoad(MVT::i32, dl, Chain, BasePtr, LD->getPointerInfo(), Alignment);

    // Create QMOV instruction
    SDValue QMOV = DAG.getNode(QASMISD::QMOV, dl, MVT::qubit, NewLD);

    // Merge them together
    SDValue Ops[] = {QMOV, SDValue(NewLD.getNode(), 1)};
    return DAG.getMergeValues(Ops, dl);
}

SDValue QASMTargetLowering::LowerMeasure(SDValue Op, SelectionDAG &DAG) const {
    // SDValue Op1 = Op.getOperand(0);

    // Create new Measure Z gate
    SDValue MeasZ =
        DAG.getNode(ISDtoQASM(Op.getOpcode()), Op, Op.getValueType(),  Op.getOperand(0));

    return MeasZ;
}

SDValue QASMTargetLowering::LowerStore(SDValue Op, SelectionDAG &DAG) const {
    // Remove store q1 %1, q1* %q, align 4 
    StoreSDNode *SD = cast<StoreSDNode>(Op);
    SDValue Chain = SD->getChain();

    // DAG.ReplaceAllUsesWith(Op, Chain);
    // DAG.makeEquivalentMemoryOrdering(Op, Op.getOperand(1));
    return Op;
}

SDValue QASMTargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG) const {
  SDValue Chain = Op.getOperand(0);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(1))->get();
  SDValue LHS   = Op.getOperand(2);
  SDValue RHS   = Op.getOperand(3);
  SDValue Dest  = Op.getOperand(4);
  SDLoc dl  (Op);

  SDValue TargetCC;
  SDValue Flag = EmitCMP(LHS, RHS, TargetCC, CC, dl, DAG);

  SDValue QASMBR_CC = DAG.getNode(QASMISD::BR_CC, dl, Op.getValueType(),
                  Chain, Dest, TargetCC, Flag);
  return QASMBR_CC;
}

SDValue QASMTargetLowering::LowerAdd(SDValue Op, SelectionDAG &DAG) const {

  SDLoc dl  (Op);

  if (Op.getOperand(1).getOpcode() == ISD::SHL){
      SDValue Op1 = Op.getOperand(0);
      SDValue SHL = Op.getOperand(1);

      SDValue Val = DAG.getConstant(0xA000, dl, MVT::i32);

      DAG.ReplaceAllUsesOfValueWith(SHL, Val);

      DAG.ReplaceAllUsesOfValueWith(Op1, SHL.getOperand(0));

      // Because of SHL R1, 2 equal to MUL R1, 4, then we add 4 of RO instead.
      SDValue INC = DAG.getNode(QASMISD::INC, dl, MVT::i32, Op.getOperand(1), Op.getOperand(0));
      INC = DAG.getNode(QASMISD::INC, dl, MVT::i32, INC, Op.getOperand(0));
      INC = DAG.getNode(QASMISD::INC, dl, MVT::i32, INC, Op.getOperand(0));
      INC = DAG.getNode(QASMISD::INC, dl, MVT::i32, INC, Op.getOperand(0));
      return INC;
  }

  ConstantSDNode *Mask = dyn_cast<ConstantSDNode>(Op->getOperand(1));
  if (Mask){
    if (Mask->getConstantIntValue()->isNegative()){
      SDLoc dl  (Op);

      int value = Mask->getConstantIntValue()->getSExtValue();
      
      // Create possitive value by add "-"
      SDValue Val = DAG.getConstant(-value, dl, MVT::i32);

      // Converse Add to Dec if first-right Op and left Op are the same
      return DAG.getNode(QASMISD::DEC, dl, MVT::i32, Op->getOperand(0), Val);

    }
  }
  return Op;
}

SDValue QASMTargetLowering::LowerQMOV(SDValue Op, SelectionDAG &DAG) const {
  SDValue Op0 = Op.getOperand(0);

  if (Op0.getOpcode() == QASM::LDDri) {
    errs() << "DO SOME THING HERE";
  }
  return Op;
}

SDValue QASMTargetLowering::LowerCtrlTargetGate(SDValue Op, SelectionDAG &DAG) const {
  
  if (Op->getNumOperands() < 2) {
      assert("CtrlTargetGate must have 2 operands");
      return SDValue();
  }

  SDValue Op0 = Op->getOperand(0);
  
  if (Op0.getOpcode() != ISD::CTRL_GATE)
    return LowerRotationGate(Op, DAG);


  SDLoc SL(Op);

  // X QubitT, Ctr(QubitC) -> CNOT QubitC, QubitT.
  SDValue QubitC = Op0.getOperand(0);
  SDValue QubitT = Op->getOperand(1);

  if (QubitT.getOpcode() == ISD::CTRL_GATE)
    return LowerMultiCtrlTargetGate(Op, DAG);

  if (Op->getNumOperands() > 2 )
    return LowerCtrlRotationGate(Op, DAG);

  SmallVector<EVT, 2> Tys = {MVT::qubit, MVT::qubit};
  
  SDValue GATE = DAG.getNode(ISDtoCtrlTarQASM(Op.getOpcode()), SL, Tys, {QubitC, QubitT});

  DAG.ReplaceAllUsesOfValueWith(Op0, SDValue(GATE.getNode(), 0));

  return SDValue(GATE.getNode(), 1);
}

SDValue QASMTargetLowering::LowerMultiCtrlTargetGate(SDValue Op, SelectionDAG &DAG) const {
  
  if (Op->getNumOperands() < 3) {
      assert("MultiCtrlTargetGate must have 3 operands");
      return SDValue();
  }


  SDLoc SL(Op);
  
  SDValue Op0 = Op->getOperand(0);
  SDValue Op1 = Op->getOperand(1);


  if (Op0.getOpcode() != ISD::CTRL_GATE || Op1.getOpcode() != ISD::CTRL_GATE)
    return SDValue();

  SDValue QubitC0  = Op0.getOperand(0);
  SDValue QubitC1 = Op1.getOperand(0);
  SDValue QubitT  = Op->getOperand(2);

  SmallVector<EVT, 3> Tys = {MVT::qubit, MVT::qubit, MVT::qubit};

  SDValue GATE = DAG.getNode(ISDtoMultiCtrlTarQASM(Op.getOpcode()), SL, Tys, {QubitC0, QubitC1, QubitT});

  DAG.ReplaceAllUsesOfValueWith(Op0, SDValue(GATE.getNode(), 0));
  DAG.ReplaceAllUsesOfValueWith(Op1, SDValue(GATE.getNode(), 1));

  return SDValue(GATE.getNode(), 2);
}

SDValue QASMTargetLowering::LowerRotationGate(SDValue Op, SelectionDAG &DAG) const {
  
  if (Op->getNumOperands() < 2) {
      assert("CtrlTargetGate must have 2 operands");
      return SDValue();
  }
  
  SDLoc SL(Op);

  SDValue Qubit = Op.getOperand(0);
  SDValue Angle = Op.getOperand(1);
  
  SDValue GATE = DAG.getNode(ISDtoRotationQASM(Op.getOpcode()), SL, MVT::qubit, {Qubit, Angle});

  // DAG.ReplaceAllUsesOfValueWith(Op0, SDValue(GATE.getNode(), 0));
  DAG.ReplaceAllUsesWith(Op, GATE);

  return GATE;
}

SDValue QASMTargetLowering::LowerUGate(SDValue Op, SelectionDAG &DAG) const {
  
  unsigned numOperands = Op->getNumOperands();

  if (numOperands < 2) {
      assert("UGate must have at least 2 operands");
      return SDValue();
  }

  if (Op.getOperand(0).getOpcode() == ISD::CTRL_GATE && numOperands > 2)
    return LowerCtrlUGate(Op, DAG);

  SDLoc SL(Op);

  SDValue Qubit = Op.getOperand(0);
  SmallVector<SDValue, 4> Angles;
  

  Angles.push_back(Qubit);
  
  for (unsigned i = 1; i < numOperands; i++){
    Angles.push_back(Op.getOperand(i));
  }

  // Create arrayRef for Angles
  ArrayRef<SDValue> AnglesRef(Angles);

  SDValue GATE = DAG.getNode(ISDtoUnatryGateQASM(numOperands - 1), SL, MVT::qubit, AnglesRef);

  DAG.ReplaceAllUsesWith(Op, GATE);

  return GATE;
}

SDValue QASMTargetLowering::LowerCtrlRotationGate(SDValue Op, SelectionDAG &DAG) const {
  
  if (Op->getNumOperands() < 3) {
      assert("CtrlRotationGate must have 3 operands");
      return SDValue();
  }

  SDLoc SL(Op);

  SDValue Op0 = Op->getOperand(0);
  SDValue Op1 = Op->getOperand(1);


  SDValue QubitC    = Op0.getOperand(0);
  // SDValue Target    = Op1.getOperand(0); // Target is Op1 in this case
  SDValue Angle     = Op->getOperand(2);

  SmallVector<EVT, 2> Tys = {MVT::qubit, MVT::qubit};

  SDValue GATE = DAG.getNode(ISDtoCtrlRotationQASM(Op.getOpcode()), SL, Tys, {QubitC, Op1, Angle});

  DAG.ReplaceAllUsesOfValueWith(Op0, SDValue(GATE.getNode(), 0));
  // DAG.ReplaceAllUsesOfValueWith(Op, SDValue(GATE.getNode(), 1));

  return SDValue(GATE.getNode(), 1);
}

SDValue QASMTargetLowering::LowerCtrlUGate(SDValue Op, SelectionDAG &DAG) const {
  
  if (Op->getNumOperands() < 3) {
      assert("CtrlUGate must have 3 operands");
      return SDValue();
  }

  SDLoc SL(Op);

  SDValue Op0 = Op->getOperand(0);
  SDValue Op1 = Op->getOperand(1);

  if (Op0.getOpcode() != ISD::CTRL_GATE)
    return SDValue();

  SDValue QubitC0  = Op0.getOperand(0); // Get qubit from CTRL gate
  // SDValue Target = Op1.getOperand(0);
  SmallVector<SDValue, 4> Angles;
  
  Angles.push_back(QubitC0);
  Angles.push_back(Op1);
  
  for (int i = 2; i < Op->getNumOperands(); i++){
    Angles.push_back(Op.getOperand(i));
  }
  SmallVector<EVT, 2> Tys = {MVT::qubit, MVT::qubit};
  // Create arrayRef for Angles
  ArrayRef<SDValue> AnglesRef(Angles);

  SDValue GATE = DAG.getNode(ISDtoCtrlUnatryGateQASM(Op->getNumOperands() - 2), SL, Tys, AnglesRef);

  DAG.ReplaceAllUsesOfValueWith(Op0, SDValue(GATE.getNode(), 0));
  // DAG.ReplaceAllUsesOfValueWith(Op, SDValue(GATE.getNode(), 1));


  return SDValue(GATE.getNode(), 1);
}

namespace {
SDValue performANDCombine(SDNode *N, TargetLowering::DAGCombinerInfo &DCI) {
  SelectionDAG &DAG = DCI.DAG;
  SDValue FirstOperand = N->getOperand(0);
  unsigned FirstOperandOpc = FirstOperand.getOpcode();
  // Second operand of and must be a constant.
  ConstantSDNode *Mask = dyn_cast<ConstantSDNode>(N->getOperand(1));
  if (!Mask)
    return SDValue();
  EVT ValTy = N->getValueType(0);
  SDLoc DL(N);

  SDValue NewOperand;
  unsigned Opc;

  uint64_t Offset;
  uint64_t MaskWidth, MaskOffset;
  if (isShiftedMask(Mask->getZExtValue(), MaskWidth, MaskOffset)) {
    if (FirstOperandOpc == ISD::SRL || FirstOperandOpc == ISD::SRA) {
      // Pattern match:
      // $dst = and (srl/sra $src, offset), (2**width - 1)
      // => EXTU $dst, $src, width<offset>

      // The second operand of the shift must be an immediate.
      ConstantSDNode *ShiftAmt =
          dyn_cast<ConstantSDNode>(FirstOperand.getOperand(1));
      if (!(ShiftAmt))
        return SDValue();

      Offset = ShiftAmt->getZExtValue();

      // Return if the shifted mask does not start at bit 0 or the sum of its
      // width and offset exceeds the word's size.
      if (MaskOffset != 0 || Offset + MaskWidth > ValTy.getSizeInBits())
        return SDValue();

      Opc = QASMISD::EXTU;
      NewOperand = FirstOperand.getOperand(0);
    } else
      return SDValue();
  } else if (isShiftedMask(~Mask->getZExtValue() &
                               ((0x1ULL << ValTy.getSizeInBits()) - 1),
                           MaskWidth, MaskOffset)) {
    // Pattern match:
    // $dst = and $src, ~((2**width - 1) << offset)
    // => CLR $dst, $src, width<offset>
    Opc = QASMISD::CLR;
    NewOperand = FirstOperand;
    Offset = MaskOffset;
  } else
    return SDValue();
  return DAG.getNode(Opc, DL, ValTy, NewOperand,
                     DAG.getConstant(MaskWidth << 5 | Offset, DL, MVT::i32));
}

SDValue performORCombine(SDNode *N, TargetLowering::DAGCombinerInfo &DCI) {
  SelectionDAG &DAG = DCI.DAG;
  uint64_t Width, Offset;

  // Second operand of or must be a constant shifted mask.
  ConstantSDNode *Mask = dyn_cast<ConstantSDNode>(N->getOperand(1));
  if (!Mask || !isShiftedMask(Mask->getZExtValue(), Width, Offset))
    return SDValue();

  // Pattern match:
  // $dst = or $src, ((2**width - 1) << offset
  // => SET $dst, $src, width<offset>
  EVT ValTy = N->getValueType(0);
  SDLoc DL(N);
  return DAG.getNode(QASMISD::SET, DL, ValTy, N->getOperand(0),
                     DAG.getConstant(Width << 5 | Offset, DL, MVT::i32));
}

SDValue performCNOTCombine(SDNode *N, TargetLowering::DAGCombinerInfo &DCI) {

  if (N->getNumOperands() < 2) {
      return SDValue();
  }

  SelectionDAG &DAG = DCI.DAG;
  EVT VT = N->getValueType(0);
  SDLoc SL(N);  

  SDValue Ctrl  = N->getOperand(1);
  
  if (Ctrl.getOpcode() != ISD::CTRL_GATE)
    return SDValue();

  // X QubitT, Ctr(QubitC) -> CNOT QubitC, QubitT.
  SDValue QubitT = N->getOperand(0);
  SDValue QubitC = Ctrl.getOperand(0);

  ///// check X or Z boundary

  ///// X boundary
  // TODO: X boundary

  /////  Z boundary

  
  // FIXME: int 12 will need to be flexibled
  SDValue QRT = DAG.getConstant(4, SL, MVT::i32);

  // INIT_Z QRT

  SDValue QMOV = DAG.getNode(QASMISD::QMOV, SL, MVT::qubit, QRT);

  SDValue INIT_Z = DAG.getNode(ISD::INIT_Z_GATE, SL, MVT::qubit, QMOV);
  
  // MERGE_MXX QRT, QubitT
  SDValue MERGE_MXX = DAG.getNode(QASMISD::MERGE_MXX, SL, MVT::qubit, INIT_Z, QubitT);

  // SPLIT_MXX QRT, QubitT 
  SDValue SPLIT_MXX = DAG.getNode(QASMISD::SPLIT_MXX, SL, MVT::qubit, MERGE_MXX,  QubitT);

  // MERGE_MZZ QubitC , QRT  
  SDValue MERGE_MZZ = DAG.getNode(QASMISD::MERGE_MZZ, SL, MVT::qubit, QubitC, INIT_Z);

  //  SPLIT_MZZ QubitC , QRT
  SDValue SPLIT_MZZ = DAG.getNode(QASMISD::SPLIT_MZZ, SL, MVT::qubit, MERGE_MZZ, INIT_Z);

  // CNOT_POST_MXX QubitC, QubitT, QRT
  SDValue CNOT_POST_MXX = DAG.getNode(QASMISD::CNOT_POST_MXX, SL, MVT::qubit, SPLIT_MZZ, QubitT, SPLIT_MXX);
  
  SDValue Ops[] = {CNOT_POST_MXX, SPLIT_MZZ, MERGE_MZZ, SPLIT_MXX, MERGE_MXX, INIT_Z, QRT};

  SDValue Merge = DAG.getMergeValues(Ops, SL);
  DAG.ReplaceAllUsesWith(Ctrl, Merge);

  return Merge;
}

} // namespace

SDValue QASMTargetLowering::PerformDAGCombine(SDNode *N,
                                              DAGCombinerInfo &DCI) const {
  if (DCI.isBeforeLegalizeOps())
    return SDValue();
  LLVM_DEBUG(dbgs() << "In PerformDAGCombine\n");

  // TODO: Match certain and/or/shift ops to ext/mak.
  unsigned Opc = N->getOpcode();

  switch (Opc) {
  default:
    break;
  case ISD::AND:
    return performANDCombine(N, DCI);
  case ISD::OR:
    return performORCombine(N, DCI);
  }

  return SDValue();
}

//===----------------------------------------------------------------------===//
// Calling conventions
//===----------------------------------------------------------------------===//

#include "QASMGenCallingConv.inc"

SDValue QASMTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_QASM);

  for (unsigned I = 0, E = ArgLocs.size(); I != E; ++I) {
    SDValue ArgValue;
    CCValAssign &VA = ArgLocs[I];
    EVT LocVT = VA.getLocVT();
    if (VA.isRegLoc()) {
      // Arguments passed in registers
      const TargetRegisterClass *RC;
      switch (LocVT.getSimpleVT().SimpleTy) {
      default:
        // Integers smaller than i64 should be promoted to i32.
        llvm_unreachable("Unexpected argument type");
      case MVT::i32:
        RC = &QASM::GPRRegClass;
      case MVT::f32:
        RC = &QASM::FPRRegClass;
        break;
      case MVT::qubit:
        RC = &QASM::QPRRegClass;
        break;
      }

      Register VReg = MRI.createVirtualRegister(RC);
      MRI.addLiveIn(VA.getLocReg(), VReg);
      ArgValue = DAG.getCopyFromReg(Chain, DL, VReg, LocVT);

      // If this is an 8/16-bit value, it is really passed promoted to 32
      // bits. Insert an assert[sz]ext to capture this, then truncate to the
      // right size.
      if (VA.getLocInfo() == CCValAssign::SExt)
        ArgValue = DAG.getNode(ISD::AssertSext, DL, LocVT, ArgValue,
                               DAG.getValueType(VA.getValVT()));
      else if (VA.getLocInfo() == CCValAssign::ZExt)
        ArgValue = DAG.getNode(ISD::AssertZext, DL, LocVT, ArgValue,
                               DAG.getValueType(VA.getValVT()));

      if (VA.getLocInfo() != CCValAssign::Full)
        ArgValue = DAG.getNode(ISD::TRUNCATE, DL, VA.getValVT(), ArgValue);

      InVals.push_back(ArgValue);
    } else {
      assert(VA.isMemLoc() && "Argument not register or memory");
      llvm_unreachable(
          "QASM - LowerFormalArguments - Memory argument not implemented");
    }
  }

  if (IsVarArg) {
    llvm_unreachable("QASM - LowerFormalArguments - VarArgs not Implemented");
  }

  return Chain;
}

SDValue
QASMTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {

  MachineFunction &MF = DAG.getMachineFunction();

  // Assign locations to each returned value.
  SmallVector<CCValAssign, 16> RetLocs;
  CCState RetCCInfo(CallConv, IsVarArg, MF, RetLocs, *DAG.getContext());
  RetCCInfo.AnalyzeReturn(Outs, RetCC_QASM);

  // Quick exit for void returns
  if (RetLocs.empty())
    return DAG.getNode(QASMISD::RET_FLAG, DL, MVT::Other, Chain);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps;
  RetOps.push_back(Chain);
  for (unsigned I = 0, E = RetLocs.size(); I != E; ++I) {
    CCValAssign &VA = RetLocs[I];
    SDValue RetValue = OutVals[I];

    // Make the return register live on exit.
    assert(VA.isRegLoc() && "Can only return in registers!");

    // Promote the value as required.
    // TODO: Refactor into own method?
    switch (VA.getLocInfo()) {
    case CCValAssign::SExt:
      RetValue = DAG.getNode(ISD::SIGN_EXTEND, DL, VA.getLocVT(), RetValue);
      break;
    case CCValAssign::ZExt:
      RetValue = DAG.getNode(ISD::ZERO_EXTEND, DL, VA.getLocVT(), RetValue);
      break;
    case CCValAssign::AExt:
      RetValue = DAG.getNode(ISD::ANY_EXTEND, DL, VA.getLocVT(), RetValue);
      break;
    case CCValAssign::Full:
      break;
    default:
      llvm_unreachable("Unhandled VA.getLocInfo()");
    }

    // Chain and glue the copies together.
    Register Reg = VA.getLocReg();
    Chain = DAG.getCopyToReg(Chain, DL, Reg, RetValue, Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(Reg, VA.getLocVT()));
  }

  // Update chain and glue.
  RetOps[0] = Chain;
  if (Glue.getNode())
    RetOps.push_back(Glue);

  return DAG.getNode(QASMISD::RET_FLAG, DL, MVT::Other, RetOps);
}

SDValue QASMTargetLowering::LowerCall(CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  llvm_unreachable("QASM - LowerCall - Not Implemented");
}

const char *QASMTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
#define OPCODE(Opc)                                                            \
  case Opc:                                                                    \
    return #Opc
    OPCODE(QASMISD::RET_FLAG);
    OPCODE(QASMISD::CALL);
    OPCODE(QASMISD::CLR);
    OPCODE(QASMISD::SET);
    OPCODE(QASMISD::EXT);
    OPCODE(QASMISD::EXTU);
    OPCODE(QASMISD::MAK);
    OPCODE(QASMISD::ROT);
    OPCODE(QASMISD::FF1);


    OPCODE(QASMISD::BR_CC);
    OPCODE(QASMISD::INC);
    OPCODE(QASMISD::DEC);
    OPCODE(QASMISD::CMP);
    OPCODE(QASMISD::MOV);
    OPCODE(QASMISD::QMOV);
    OPCODE(QASMISD::FMR);
    OPCODE(QASMISD::STD);

    OPCODE(QASMISD::Z_GATE);
    OPCODE(QASMISD::X_GATE);
    OPCODE(QASMISD::Y_GATE);
    OPCODE(QASMISD::U_GATE);
    OPCODE(QASMISD::P_GATE);

    OPCODE(QASMISD::H_GATE);

    OPCODE(QASMISD::RX_GATE);
    OPCODE(QASMISD::RY_GATE);
    OPCODE(QASMISD::RZ_GATE);

    OPCODE(QASMISD::DEFORM);
    OPCODE(QASMISD::FLIP_EXPAND);
    OPCODE(QASMISD::FLIP_CONTRACT);
    OPCODE(QASMISD::FLIP_SHIFT);

    OPCODE(QASMISD::CNOT_GATE);
    OPCODE(QASMISD::CY_GATE);
    OPCODE(QASMISD::CZ_GATE);
    OPCODE(QASMISD::CH_GATE);
    OPCODE(QASMISD::SWAP_GATE);

    OPCODE(QASMISD::CRX_GATE);
    OPCODE(QASMISD::CRY_GATE);
    OPCODE(QASMISD::CRZ_GATE);

    OPCODE(QASMISD::CU1_GATE);
    OPCODE(QASMISD::CU2_GATE);
    OPCODE(QASMISD::CU3_GATE);

    OPCODE(QASMISD::CCX_GATE);
    OPCODE(QASMISD::CCY_GATE);
    OPCODE(QASMISD::CCZ_GATE);
    OPCODE(QASMISD::CCH_GATE);
    OPCODE(QASMISD::CCU_GATE);
    OPCODE(QASMISD::CSWAP_GATE);

    OPCODE(QASMISD::MEAS_X_GATE);
    OPCODE(QASMISD::MEAS_Z_GATE);

    OPCODE(QASMISD::MERGE_MXX);
    OPCODE(QASMISD::MERGE_MZZ);
    OPCODE(QASMISD::SPLIT_MXX);
    OPCODE(QASMISD::SPLIT_MZZ);
    OPCODE(QASMISD::CNOT_POST_MXX);
    OPCODE(QASMISD::CNOT_POST_MZZ);
    OPCODE(QASMISD::MOVE_POST_MXX);
    OPCODE(QASMISD::MOVE_POST_MZZ);
    
#undef OPCODE
  default:
    return nullptr;
  }
}
