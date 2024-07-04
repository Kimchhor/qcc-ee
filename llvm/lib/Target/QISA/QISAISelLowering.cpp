//===-- QISAISelLowering.cpp - QISA DAG lowering implementation -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the QISATargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "QISAISelLowering.h"
#include "QISACondCode.h"
#include "QISADefine.h"
//#include "QISACallingConv.h"
//#include "QISAConstantPoolValue.h"
//#include "QISAMachineFunctionInfo.h"
#include "QISATargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
//#include "llvm/IR/IntrinsicsQISA.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/KnownBits.h"
#include <cctype>

using namespace llvm;

#define DEBUG_TYPE "QISA-lower"

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

static QISACC::CondCode ISDCCtoQISACC(ISD::CondCode isdCC) {
  switch (isdCC) {
  case ISD::SETUEQ:
    return QISACC::EQ;
  case ISD::SETUGT:
    return QISACC::HI;
  case ISD::SETUGE:
    return QISACC::HS;
  case ISD::SETULT:
    return QISACC::LO;
  case ISD::SETULE:
    return QISACC::LS;
  case ISD::SETUNE:
    return QISACC::NE;
  case ISD::SETEQ:
    return QISACC::EQ;
  case ISD::SETGT:
    return QISACC::GT;
  case ISD::SETGE:
    return QISACC::GE;
  case ISD::SETLT:
    return QISACC::LT;
  case ISD::SETLE:
    return QISACC::LE;
  case ISD::SETNE:
    return QISACC::NE;
  default:
    llvm_unreachable("Unhandled ISDCC code.");
  }
}

static QISAISD::NodeType ISDtoQISA(unsigned OpType){
  switch (OpType)
  {
  case ISD::MEAS_X_GATE:        return QISAISD::MEAS_X_GATE;
  case ISD::MEAS_Z_GATE:        return QISAISD::MEAS_Z_GATE;
  case ISD::X_GATE:             return QISAISD::X_GATE;
  case ISD::Z_GATE:             return QISAISD::Z_GATE;  
  case ISD::U_GATE:             return QISAISD::U_GATE;
  default:
    llvm_unreachable("Unhandled ISD in ISDtoQISA()");
  }
}


QISATargetLowering::QISATargetLowering(const TargetMachine &TM,
                                       const QISASubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {

  addRegisterClass(MVT::i32, &QISA::GPRRegClass);
  addRegisterClass(MVT::qubit, &QISA::QPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(Subtarget.getRegisterInfo());

  // Set up special registers.
  setStackPointerRegisterToSaveRestore(QISA::R61);
  setStackPointerRegisterToSaveRestore(QISA::QR61);

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

  setOperationAction(ISD::H_GATE, MVT::qubit, Custom);
  setOperationAction(ISD::SWAP_GATE, MVT::qubit, Custom);

  // setOperationAction(ISD::FrameIndex, MVT::i32, Custom);

  // Special DAG combiner for bit-field operations.
  // setTargetDAGCombine(ISD::AND);
  // setTargetDAGCombine(ISD::OR);
  // setTargetDAGCombine(ISD::SHL);

  // All target gates need to perform DAG Combine
  setTargetDAGCombine(ISD::X_GATE);
}

SDValue QISATargetLowering::LowerOperation(SDValue Op,
                                           SelectionDAG &DAG) const {
  // TODO Implement for ops not covered by patterns in .td files.

  switch (Op.getOpcode()) {
  case ISD::MEAS_X_GATE:
  case ISD::MEAS_Y_GATE:
  case ISD::MEAS_Z_GATE:        return LowerMeasure(Op, DAG);
  case ISD::LOAD:               return LowerLoad(Op, DAG);
  // case ISD::STORE:              return LowerStore(Op, DAG);
  case ISD::BR_CC:              return LowerBR_CC(Op, DAG);
  case ISD::H_GATE:             return LowerHGate(Op, DAG);
  case ISD::ADD:                return LowerAdd(Op, DAG);
  case ISD::X_GATE:             return LowerCNOTGate(Op, DAG);
  case ISD::SWAP_GATE:          return LowerSWAPGate(Op, DAG);
  }
  return Op;
}

// Clone from SMP430ISelLowering.cpp
static SDValue EmitCMP(SDValue &LHS, SDValue &RHS, SDValue &TargetCC,
                       ISD::CondCode CC, const SDLoc &dl, SelectionDAG &DAG) {
  // FIXME: Handle bittests someday
  assert(!LHS.getValueType().isFloatingPoint() && "We don't handle FP yet");

  // FIXME: Handle jump negative someday
  QISACC::CondCode TCC;
  switch (CC) {
  default: llvm_unreachable("Invalid integer condition!");
  case ISD::SETEQ:
    TCC = QISACC::EQ;     // aka Z
    // Minor optimization: if LHS is a constant, swap operands, then the
    // constant can be folded into comparison.
    if (LHS.getOpcode() == ISD::Constant)
      std::swap(LHS, RHS);
    break;
  case ISD::SETNE:
    TCC = QISACC::NE;    // aka NZ
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
      TCC = QISACC::LO;
      break;
    }
    TCC = QISACC::HS;    // aka C
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
      TCC = QISACC::HS;
      break;
    }
    TCC = QISACC::LO;    // aka NC
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
      TCC = QISACC::LT;
      break;
    }
    TCC = QISACC::GE;
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
      TCC = QISACC::GE;
      break;
    }
    TCC = QISACC::LT;
    break;
  }
  TargetCC = DAG.getConstant(TCC, dl, MVT::i32);
  return DAG.getNode(QISAISD::CMP, dl, MVT::Glue, LHS, RHS);
}

SDValue QISATargetLowering::LowerLoad(SDValue Op, SelectionDAG &DAG) const {
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
    SDValue QMOV = DAG.getNode(QISAISD::QMOV, dl, MVT::qubit, NewLD);

    // Merge them together
    SDValue Ops[] = {QMOV, SDValue(NewLD.getNode(), 1)};
    return DAG.getMergeValues(Ops, dl);
}

SDValue QISATargetLowering::LowerMeasure(SDValue Op, SelectionDAG &DAG) const {
    SDValue Op1 = Op.getOperand(0);

    // Create new Measure Z gate
    SDValue MeasZ =
        DAG.getNode(ISDtoQISA(Op.getOpcode()), Op, Op1.getValueType(), Op1);

    // Create FMR (Fetch Measure Result)
    SDValue FMR = DAG.getNode(QISAISD::FMR, MeasZ, MVT::i32, MeasZ);
    return FMR;
}

SDValue QISATargetLowering::LowerStore(SDValue Op, SelectionDAG &DAG) const {
    // Remove store q1 %1, q1* %q, align 4 
    StoreSDNode *SD = cast<StoreSDNode>(Op);
    SDValue Chain = SD->getChain();

    // DAG.ReplaceAllUsesWith(Op, Chain);
    // DAG.makeEquivalentMemoryOrdering(Op, Op.getOperand(1));
    return Op;
}

SDValue QISATargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG) const {
  SDValue Chain = Op.getOperand(0);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(1))->get();
  SDValue LHS   = Op.getOperand(2);
  SDValue RHS   = Op.getOperand(3);
  SDValue Dest  = Op.getOperand(4);
  SDLoc dl  (Op);

  SDValue TargetCC;
  SDValue Flag = EmitCMP(LHS, RHS, TargetCC, CC, dl, DAG);

  SDValue QISABR_CC = DAG.getNode(QISAISD::BR_CC, dl, Op.getValueType(),
                  Chain, Dest, TargetCC, Flag);
  return QISABR_CC;
}

SDValue QISATargetLowering::LowerHGate(SDValue Op, SelectionDAG &DAG) const {
  SDLoc dl(Op);
  SDValue Op1 = Op.getOperand(0);

  // // FIXME: int 12 will need to be flexibled
  // SDValue QRT = DAG.getConstant(12, dl, MVT::i32);

  // // Create QMOV instruction
  // SDValue QMOV = DAG.getNode(QISAISD::QMOV, dl, MVT::qubit, QRT);

  // // HADAMARD QR0
  // SDValue H = DAG.getNode(QISAISD::H_GATE, dl, MVT::qubit, Op1);
  
  // // DEFORM QR0 
  // SDValue DEFORM = DAG.getNode(QISAISD::DEFORM, dl, MVT::qubit, H);

  // // FLIP_EXPAND QR1, QR0
  // SDValue EXPAND = DAG.getNode(QISAISD::FLIP_EXPAND, dl, MVT::qubit, QMOV, DEFORM);

  // // FLIP_CONTRACT QR0, QR1
  // SDValue CONTRACT = DAG.getNode(QISAISD::FLIP_CONTRACT, dl, MVT::qubit, EXPAND, QMOV);

  // // FLIP_SHIFT QR0, QR1
  // SDValue SHIFT = DAG.getNode(QISAISD::FLIP_SHIFT, dl, MVT::qubit, CONTRACT, QMOV);

  //  // Merge them together
  //   SDValue Ops[] = {SHIFT, CONTRACT, EXPAND, DEFORM, H, QMOV, QRT};
  // return DAG.getMergeValues(Ops, dl);

  return DAG.getNode(QISAISD::H_GATE, dl, MVT::qubit, Op1);
}

SDValue QISATargetLowering::LowerAdd(SDValue Op, SelectionDAG &DAG) const {

  SDLoc dl  (Op);

  if (Op.getOperand(1).getOpcode() == ISD::SHL){
      SDValue Op1 = Op.getOperand(0);
      SDValue SHL = Op.getOperand(1);

      SDValue Val = DAG.getConstant(0xA000, dl, MVT::i32);

      DAG.ReplaceAllUsesOfValueWith(SHL, Val);

      DAG.ReplaceAllUsesOfValueWith(Op1, SHL.getOperand(0));

      // Because of SHL R1, 2 equal to MUL R1, 4, then we add 4 of RO instead.
      SDValue INC = DAG.getNode(QISAISD::INC, dl, MVT::i32, Op.getOperand(1), Op.getOperand(0));
      INC = DAG.getNode(QISAISD::INC, dl, MVT::i32, INC, Op.getOperand(0));
      INC = DAG.getNode(QISAISD::INC, dl, MVT::i32, INC, Op.getOperand(0));
      INC = DAG.getNode(QISAISD::INC, dl, MVT::i32, INC, Op.getOperand(0));
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
      return DAG.getNode(QISAISD::DEC, dl, MVT::i32, Op->getOperand(0), Val);

    }
  }
  return Op;
}

SDValue QISATargetLowering::LowerQMOV(SDValue Op, SelectionDAG &DAG) const {
  SDValue Op0 = Op.getOperand(0);

  if (Op0.getOpcode() == QISA::LDDri) {
    errs() << "DO SOME THING HERE";
  }
  return Op;
}

SDValue QISATargetLowering::LowerCNOTGate(SDValue Op, SelectionDAG &DAG) const {
   if (Op->getNumOperands() < 2) {
      return SDValue();
  }

  SDLoc SL(Op);  

  SDValue Ctrl  = Op->getOperand(1);
  
  if (Ctrl.getOpcode() != ISD::CTRL_GATE)
    return SDValue();

  // X QubitT, Ctr(QubitC) -> CNOT QubitC, QubitT.
  SDValue QubitT = Op->getOperand(0);
  SDValue QubitC = Ctrl.getOperand(0);

  ///// check X or Z boundary

  ///// X boundary
  // TODO: X boundary

  /////  Z boundary

  // FIXME: int 12 will need to be flexibled
  SDValue QRT = DAG.getConstant(3, SL, MVT::i32);

  // QMOV QRT
  SDValue QMOV = DAG.getNode(QISAISD::QMOV, SL, MVT::qubit, QRT);

  // SmallVector<EVT, 2> Tys = {MVT::qubit, MVT::qubit};
  SmallVector<EVT, 3> Tys3 = {MVT::qubit, MVT::qubit, MVT::qubit};

  // CNOT_POST_MXX QubitC, QubitT, QRT
  SDValue CNOT_POST_MXX = DAG.getNode(QISAISD::CNOT_POST_MXX, SL, Tys3 , {QubitC, QubitT, QMOV});
  
  DAG.ReplaceAllUsesWith(Ctrl, SDValue(CNOT_POST_MXX.getNode(), 0));

  return SDValue(CNOT_POST_MXX.getNode(), 1);
}

SDValue QISATargetLowering::LowerSWAPGate(SDValue Op, SelectionDAG &DAG) const {
  EVT VT = Op->getValueType(0);
  SDLoc SL(Op);  

  SDValue Ctrl  = Op->getOperand(1);
  
  if (Ctrl.getOpcode() != ISD::CTRL_GATE)
    return SDValue();

  // SWAP T, Ctr(S) -> SWAP T, S.
  SDValue QubitT = Op->getOperand(0);
  SDValue QubitS = Ctrl.getOperand(0);

  SmallVector<SDValue, 2> Ops = {QubitS, QubitT};
  SmallVector<EVT, 2> Res = {MVT::qubit, MVT::qubit};

  SDValue SWAP = DAG.getNode(QISAISD::SWAP_GATE, SL, Res, Ops);

  QubitS = SDValue(SWAP.getNode(), 0);

  // DAG.ReplaceAllUsesOfValuesWith(Ctrl, QubitS);
  DAG.ReplaceAllUsesOfValueWith(Ctrl, QubitS);


  return SDValue(SWAP.getNode(), 1);
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

      Opc = QISAISD::EXTU;
      NewOperand = FirstOperand.getOperand(0);
    } else
      return SDValue();
  } else if (isShiftedMask(~Mask->getZExtValue() &
                               ((0x1ULL << ValTy.getSizeInBits()) - 1),
                           MaskWidth, MaskOffset)) {
    // Pattern match:
    // $dst = and $src, ~((2**width - 1) << offset)
    // => CLR $dst, $src, width<offset>
    Opc = QISAISD::CLR;
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
  return DAG.getNode(QISAISD::SET, DL, ValTy, N->getOperand(0),
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

  SDValue QMOV = DAG.getNode(QISAISD::QMOV, SL, MVT::qubit, QRT);

  SDValue INIT_Z = DAG.getNode(ISD::INIT_Z_GATE, SL, MVT::qubit, QMOV);
  
  // MERGE_MXX QRT, QubitT
  SDValue MERGE_MXX = DAG.getNode(QISAISD::MERGE_MXX, SL, MVT::qubit, INIT_Z, QubitT);

  // SPLIT_MXX QRT, QubitT 
  SDValue SPLIT_MXX = DAG.getNode(QISAISD::SPLIT_MXX, SL, MVT::qubit, MERGE_MXX,  QubitT);

  // MERGE_MZZ QubitC , QRT  
  SDValue MERGE_MZZ = DAG.getNode(QISAISD::MERGE_MZZ, SL, MVT::qubit, QubitC, INIT_Z);

  //  SPLIT_MZZ QubitC , QRT
  SDValue SPLIT_MZZ = DAG.getNode(QISAISD::SPLIT_MZZ, SL, MVT::qubit, MERGE_MZZ, INIT_Z);

  // CNOT_POST_MXX QubitC, QubitT, QRT
  SDValue CNOT_POST_MXX = DAG.getNode(QISAISD::CNOT_POST_MXX, SL, MVT::qubit, SPLIT_MZZ, QubitT, SPLIT_MXX);
  
  SDValue Ops[] = {CNOT_POST_MXX, SPLIT_MZZ, MERGE_MZZ, SPLIT_MXX, MERGE_MXX, INIT_Z, QRT};

  SDValue Merge = DAG.getMergeValues(Ops, SL);
  DAG.ReplaceAllUsesWith(Ctrl, Merge);

  return Merge;
}

} // namespace

SDValue QISATargetLowering::PerformDAGCombine(SDNode *N,
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
  // case ISD::X_GATE:
  //   return performCNOTCombine(N, DCI);
  }

  return SDValue();
}

//===----------------------------------------------------------------------===//
// Calling conventions
//===----------------------------------------------------------------------===//

#include "QISAGenCallingConv.inc"

SDValue QISATargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_QISA);

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
        RC = &QISA::GPRRegClass;
        break;
      case MVT::qubit:
        RC = &QISA::QPRRegClass;
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
          "QISA - LowerFormalArguments - Memory argument not implemented");
    }
  }

  if (IsVarArg) {
    llvm_unreachable("QISA - LowerFormalArguments - VarArgs not Implemented");
  }

  return Chain;
}

SDValue
QISATargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {

  MachineFunction &MF = DAG.getMachineFunction();

  // Assign locations to each returned value.
  SmallVector<CCValAssign, 16> RetLocs;
  CCState RetCCInfo(CallConv, IsVarArg, MF, RetLocs, *DAG.getContext());
  RetCCInfo.AnalyzeReturn(Outs, RetCC_QISA);

  // Quick exit for void returns
  if (RetLocs.empty())
    return DAG.getNode(QISAISD::RET_FLAG, DL, MVT::Other, Chain);

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

  return DAG.getNode(QISAISD::RET_FLAG, DL, MVT::Other, RetOps);
}

SDValue QISATargetLowering::LowerCall(CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  llvm_unreachable("QISA - LowerCall - Not Implemented");
}

const char *QISATargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
#define OPCODE(Opc)                                                            \
  case Opc:                                                                    \
    return #Opc
    OPCODE(QISAISD::RET_FLAG);
    OPCODE(QISAISD::CALL);
    OPCODE(QISAISD::CLR);
    OPCODE(QISAISD::SET);
    OPCODE(QISAISD::EXT);
    OPCODE(QISAISD::EXTU);
    OPCODE(QISAISD::MAK);
    OPCODE(QISAISD::ROT);
    OPCODE(QISAISD::FF1);


    OPCODE(QISAISD::BR_CC);
    OPCODE(QISAISD::INC);
    OPCODE(QISAISD::DEC);
    OPCODE(QISAISD::CMP);
    OPCODE(QISAISD::MOV);
    OPCODE(QISAISD::QMOV);
    OPCODE(QISAISD::FMR);
    OPCODE(QISAISD::STD);

    OPCODE(QISAISD::Z_GATE);
    OPCODE(QISAISD::X_GATE);
    OPCODE(QISAISD::U_GATE);

    OPCODE(QISAISD::H_GATE);
    OPCODE(QISAISD::DEFORM);
    OPCODE(QISAISD::FLIP_EXPAND);
    OPCODE(QISAISD::FLIP_CONTRACT);
    OPCODE(QISAISD::FLIP_SHIFT);

    OPCODE(QISAISD::CNOT_GATE);
    OPCODE(QISAISD::SWAP_GATE);

    OPCODE(QISAISD::MEAS_X_GATE);
    OPCODE(QISAISD::MEAS_Z_GATE);

    OPCODE(QISAISD::MERGE_MXX);
    OPCODE(QISAISD::MERGE_MZZ);
    OPCODE(QISAISD::SPLIT_MXX);
    OPCODE(QISAISD::SPLIT_MZZ);
    OPCODE(QISAISD::CNOT_POST_MXX);
    OPCODE(QISAISD::CNOT_POST_MZZ);
    OPCODE(QISAISD::MOVE_POST_MXX);
    OPCODE(QISAISD::MOVE_POST_MZZ);
    
#undef OPCODE
  default:
    return nullptr;
  }
}
