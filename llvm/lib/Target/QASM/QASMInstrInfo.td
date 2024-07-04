//===-- QASMInstrInfo.td - QASM Instructions ---------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the QASM instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

// ---------------------------------------------------------------------------//
// Selection DAG Nodes.
// ---------------------------------------------------------------------------//

// Selection DAG types.
// These are target-independent nodes, but have target-specific formats.
def SDT_CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_Call         : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

// QCL QCC - backend: defines the target-independent interfaces used by SelectionDAG
// first param: number of results, second param: number of operands
def SDTSingleQubitGate        : SDTypeProfile<1, 1, []>;
def SDTMultiQubitGate         : SDTypeProfile<1, 2, []>;
def SDTRotateGate             : SDTypeProfile<1, 2, []>;
def SDTMeasureGate            : SDTypeProfile<1, 1, []>;
def SDTQMov                   : SDTypeProfile<1, 1, []>;
def SDTFmr                    : SDTypeProfile<1, 1, []>; // Fetch Measurement Result
def SDTIncDec                 : SDTypeProfile<1, 2, []>;
def SDTCnotMoveGate           : SDTypeProfile<3, 3, []>;
def SDTMov                    : SDTypeProfile<1, 2, []>;
def SDTCtrlTargetGate         : SDTypeProfile<2, 2, []>;
def SDTMultiCtrlTargetGate    : SDTypeProfile<3, 3, []>;
def SDTU1Gate                 : SDTypeProfile<1, 2, []>;
def SDTU2Gate                 : SDTypeProfile<1, 3, []>;
def SDTU3Gate                 : SDTypeProfile<1, 4, []>;

def SDTCtrlRotationGate       : SDTypeProfile<2, 3, []>;
def STDCU1Gate                : SDTypeProfile<2, 3, []>;
def STDCU2Gate                : SDTypeProfile<2, 4, []>;
def STDCU3Gate                : SDTypeProfile<2, 5, []>;

def SDT_QASMCmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
def SDT_QASMBrCC         : SDTypeProfile<0, 2, [SDTCisVT<0, OtherVT>, SDTCisVT<1, i32>]>;


// Selection DAG nodes.

def call             : SDNode<"QASMISD::CALL", SDT_Call,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def retflag          : SDNode<"QASMISD::RET_FLAG", SDTNone,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

 // QCL QCC - backend : define predefined operators of Selection DAG Type Constraint

def init_x_gate     : SDNode<"ISD::INIT_X_GATE",  SDTSingleQubitGate, []>;
def init_z_gate     : SDNode<"ISD::INIT_Z_GATE",  SDTSingleQubitGate, []>;
def x_gate          : SDNode<"ISD::X_GATE",       SDTSingleQubitGate, []>;
def y_gate          : SDNode<"ISD::Y_GATE",       SDTSingleQubitGate, []>;
def z_gate          : SDNode<"ISD::Z_GATE",       SDTSingleQubitGate, []>;
def h_gate          : SDNode<"ISD::H_GATE",       SDTSingleQubitGate, []>;
def sx_gate         : SDNode<"ISD::SX_GATE",      SDTSingleQubitGate, []>;
def s_gate          : SDNode<"ISD::S_GATE",       SDTSingleQubitGate, []>;
def t_gate          : SDNode<"ISD::T_GATE",       SDTSingleQubitGate, []>;
def i_gate          : SDNode<"ISD::I_GATE",       SDTSingleQubitGate, []>;
def sdag_gate       : SDNode<"ISD::SDAG_GATE",    SDTSingleQubitGate, []>;
def tdag_gate       : SDNode<"ISD::TDAG_GATE",    SDTSingleQubitGate, []>;

def rx_gate         : SDNode<"QASMISD::RX_GATE",      SDTRotateGate, [SDNPSideEffect]>;
def ry_gate         : SDNode<"QASMISD::RY_GATE",      SDTRotateGate, [SDNPSideEffect]>;
def rz_gate         : SDNode<"QASMISD::RZ_GATE",      SDTRotateGate, [SDNPSideEffect]>;
def p_gate          : SDNode<"QASMISD::P_GATE",       SDTRotateGate, [SDNPSideEffect]>;

def u1_gate         : SDNode<"QASMISD::U1_GATE",      SDTU1Gate, []>;
def u2_gate         : SDNode<"QASMISD::U2_GATE",      SDTU2Gate, []>;
def u3_gate         : SDNode<"QASMISD::U3_GATE",      SDTU3Gate, []>;

def crx_gate        : SDNode<"QASMISD::CRX_GATE",     SDTCtrlRotationGate, [SDNPSideEffect]>;
def cry_gate        : SDNode<"QASMISD::CRY_GATE",     SDTCtrlRotationGate, [SDNPSideEffect]>;
def crz_gate        : SDNode<"QASMISD::CRZ_GATE",     SDTCtrlRotationGate, [SDNPSideEffect]>;
def cu1_gate        : SDNode<"QASMISD::CU1_GATE",     STDCU1Gate, [SDNPSideEffect]>;
def cu2_gate        : SDNode<"QASMISD::CU2_GATE",     STDCU2Gate, [SDNPSideEffect]>;
def cu3_gate        : SDNode<"QASMISD::CU3_GATE",     STDCU3Gate, [SDNPSideEffect]>;


def cx_gate         : SDNode<"QASMISD::CNOT_GATE",    SDTCtrlTargetGate,      [SDNPSideEffect]>;
def cy_gate         : SDNode<"QASMISD::CY_GATE",      SDTCtrlTargetGate,      [SDNPSideEffect]>;
def cz_gate         : SDNode<"QASMISD::CZ_GATE",      SDTCtrlTargetGate,      [SDNPSideEffect]>;
def ch_gate         : SDNode<"QASMISD::CH_GATE",      SDTCtrlTargetGate,      [SDNPSideEffect]>;
def swap_gate       : SDNode<"QASMISD::SWAP_GATE",    SDTCtrlTargetGate,      [SDNPSideEffect]>;

def ccx_gate        : SDNode<"QASMISD::CCX_GATE",     SDTMultiCtrlTargetGate, [SDNPSideEffect]>;
def ccy_gate        : SDNode<"QASMISD::CCY_GATE",     SDTMultiCtrlTargetGate, [SDNPSideEffect]>;
def ccz_gate        : SDNode<"QASMISD::CCZ_GATE",     SDTMultiCtrlTargetGate, [SDNPSideEffect]>;
def cch_gate        : SDNode<"QASMISD::CCU_GATE",     SDTMultiCtrlTargetGate, [SDNPSideEffect]>;

def cswap_gate      : SDNode<"QASMISD::CSWAP_GATE",   SDTMultiCtrlTargetGate, [SDNPSideEffect]>;

def ctrl_gate       : SDNode<"ISD::CTRL_GATE", SDTSingleQubitGate, []>;

def deform          : SDNode<"QASMISD::DEFORM",         SDTSingleQubitGate, []>;
def flip_expand     : SDNode<"QASMISD::FLIP_EXPAND",    SDTMultiQubitGate, []>;
def flip_contract   : SDNode<"QASMISD::FLIP_CONTRACT",  SDTMultiQubitGate, []>;
def flip_shift      : SDNode<"QASMISD::FLIP_SHIFT",     SDTMultiQubitGate, []>;

def meas_x_gate     : SDNode<"QASMISD::MEAS_X_GATE", SDTMeasureGate, [SDNPSideEffect]>;
def meas_z_gate     : SDNode<"QASMISD::MEAS_Z_GATE", SDTMeasureGate, [SDNPSideEffect]>;

// Merge and Split
def merge_xx        :  SDNode<"QASMISD::MERGE_MXX",     SDTCtrlTargetGate, []>;
def merge_zz        :  SDNode<"QASMISD::MERGE_MZZ",     SDTCtrlTargetGate, [SDNPSideEffect]>;
def split_xx        :  SDNode<"QASMISD::SPLIT_MXX",     SDTCtrlTargetGate, [SDNPSideEffect]>;
def split_zz        :  SDNode<"QASMISD::SPLIT_MZZ",     SDTCtrlTargetGate, [SDNPSideEffect]>;

def cnot_post_xx    :  SDNode<"QASMISD::CNOT_POST_MXX", SDTCnotMoveGate, []>;
def cnot_post_zz    :  SDNode<"QASMISD::CNOT_POST_MZZ", SDTCnotMoveGate, []>;

def move_post_xx    :  SDNode<"QASMISD::MOVE_POST_MXX", SDTCtrlTargetGate, [SDNPSideEffect]>;
def move_post_zz    :  SDNode<"QASMISD::MOVE_POST_MZZ", SDTCtrlTargetGate, [SDNPSideEffect]>;


def qmov          : SDNode<"QASMISD::QMOV", SDTQMov, []>;
def qasm_mov      : SDNode<"QASMISD::MOV", SDTMov, []>;
def qasm_mov1      : SDNode<"QASMISD::MOV", SDTSingleQubitGate, []>;


def fmr           : SDNode<"QASMISD::FMR", SDTFmr, []>;

def qasm_inc     : SDNode<"QASMISD::INC", SDTIncDec, []>;
def qasm_dec     : SDNode<"QASMISD::DEC", SDTIncDec, []>;

def qasm_cmp     : SDNode<"QASMISD::CMP", SDT_QASMCmp,[SDNPOutGlue]>;

def qasm_brcc    : SDNode<"QASMISD::BR_CC", SDT_QASMBrCC,
                            [SDNPHasChain, SDNPInGlue]>;

def qasm_eq        : SDNode<"QASMCC::CondCode::EQ", SDT_QASMBrCC, [SDNPHasChain]>;
def qasm_nq        : SDNode<"QASMCC::CondCode::NE", SDT_QASMBrCC, [SDNPHasChain]>;


def qasm_clr : SDNode<"QASMISD::CLR", SDTIntBinOp>;
def qasm_set : SDNode<"QASMISD::SET", SDTIntBinOp>;
def qasm_ext : SDNode<"QASMISD::EXT", SDTIntBinOp>;
def qasm_extu : SDNode<"QASMISD::EXTU", SDTIntBinOp>;
def qasm_mak : SDNode<"QASMISD::MAK", SDTIntBinOp>;
def qasm_rot : SDNode<"QASMISD::ROT", SDTIntBinOp>;
def qasm_ff1 : SDNode<"QASMISD::FF1", SDTIntBitCountUnaryOp>;
def qasm_ff0 : SDNode<"QASMISD::FF0", SDTIntBitCountUnaryOp>;

def qasm_bb0 : SDNode<"QASMISD::BB0", SDTIntBinOp>;
def qasm_bb1 : SDNode<"QASMISD::BB0", SDTIntBinOp>;
def qasm_bcond : SDNode<"QASMISD::BB0", SDTIntBinOp>;


// Hi16 and Lo16 nodes are used to handle global addresses.
// TODO Name very similar to LO16/HI16
def Hi16 : SDNode<"QASMISD::Hi16", SDTIntUnaryOp>;
def Lo16 : SDNode<"QASMISD::Lo16", SDTIntUnaryOp>;

// ---------------------------------------------------------------------------//
// Operands.
// ---------------------------------------------------------------------------//

class ImmediateAsmOperand<string name> : AsmOperandClass {
  let Name = name;
  let RenderMethod = "addImmOperands";
}

class ImmediateOp<ValueType vt, string asmop> : Operand<vt> {
  let PrintMethod = "print"#asmop#"Operand";
  let DecoderMethod = "decode"#asmop#"Operand";
  let ParserMatchClass = !cast<AsmOperandClass>(asmop);
  let OperandType = "OPERAND_IMMEDIATE";
}

class ImmOpWithPattern<ValueType vt, string asmop, code pred, SDNodeXForm xform,
      SDNode ImmNode = imm> :
  ImmediateOp<vt, asmop>, PatLeaf<(vt ImmNode), pred, xform>;

multiclass Immediate<ValueType vt, code pred, SDNodeXForm xform, string asmop> {
  // def "" : ImmediateOp<vt, asmop>,
  //          PatLeaf<(vt imm), pred, xform>;
  def "" : ImmOpWithPattern<vt, asmop, pred, xform>;

//  def _timm : PatLeaf<(vt timm), pred, xform>;
  def _timm : ImmOpWithPattern<vt, asmop, pred, xform, timm>;
}

// Constructs an asm operand for a PC-relative address.  SIZE says how
// many bits there are.
class PCRelAsmOperand<string size> : ImmediateAsmOperand<"PCRel"#size> {
  let PredicateMethod = "isImm";
  let ParserMethod = "parsePCRel"#size;
}

// Constructs an operand for a PC-relative address with address type VT.
// ASMOP is the associated asm operand.
class PCRelOperand<ValueType vt, AsmOperandClass asmop> : Operand<vt> {
  let PrintMethod = "printPCRelOperand";
  let ParserMatchClass = asmop;
  let OperandType = "OPERAND_PCREL";
}


// Signed and unsigned operands.
def U5Imm : ImmediateAsmOperand<"U5Imm">;
def U5ImmO : ImmediateAsmOperand<"U5ImmO">  {
  let ParserMethod = "parseImmWO";
}
def U10ImmWO : ImmediateAsmOperand<"U10ImmWO"> {
  let ParserMethod = "parseImmWO";
}
def S16Imm : ImmediateAsmOperand<"S16Imm">;
def U16Imm : ImmediateAsmOperand<"U16Imm">;
def U8Imm  : ImmediateAsmOperand<"U8Imm">;

// PC-relative asm operands.
def PCRel16 : PCRelAsmOperand<"16">;
def PCRel26 : PCRelAsmOperand<"26">;
// def PCRel48 : PCRelAsmOperand<"48">;

// PC-relative offsets of a basic block.  The offset is sign-extended
// and shifted left by 2 bits.
def brtarget16 : PCRelOperand<OtherVT, PCRel16> {
  let EncoderMethod = "getPC16Encoding";
  let DecoderMethod = "decodePC16BranchOperand";
}

def brtarget26 : PCRelOperand<OtherVT, PCRel26> {
  let EncoderMethod = "getPC26Encoding";
  let DecoderMethod = "decodePC26BranchOperand";
}

// Extracting immediate operands from nodes.

// Bits 0-15.
def LO16 : SDNodeXForm<imm, [{
  uint32_t Value = N->getZExtValue() & 0x000000000000FFFFULL;
  return CurDAG->getTargetConstant(Value, SDLoc(N), MVT::i32);
}]>;

// Bits 16-31 (counting from the lsb).
def HI16 : SDNodeXForm<imm, [{
  uint64_t Value = (N->getZExtValue() & 0x00000000FFFF0000ULL) >> 16;
  return CurDAG->getTargetConstant(Value, SDLoc(N), MVT::i32);
}]>;


// Immediates for the lower and upper 16 bits of an i32, with the other
// bits of the i32 being zero.
defm imm32lo16 : Immediate<i32, [{
  return (N->getZExtValue() & ~0x000000000000ffffULL) == 0;
}], LO16, "U16Imm">;

defm imm32hi16 : Immediate<i32, [{
  return (N->getZExtValue() & ~0x00000000ffff0000ULL) == 0;
}], HI16, "U16Imm">;

// Immediates for the lower and upper 16 bits of an i32, with the other
// bits of the i32 being one.
defm imm32lo16c : Immediate<i32, [{
  return (uint32_t(~N->getZExtValue()) & ~0x000000000000ffffULL) == 0;
}], LO16, "U16Imm">;

defm imm32hi16c : Immediate<i32, [{
  return (uint32_t(~N->getZExtValue()) & ~0x00000000ffff0000ULL) == 0;
}], HI16, "U16Imm">;

defm imm32zx5 : Immediate<i32, [{
  return (N->getZExtValue() & ~0x000000000000001fULL) == 0;
}], NOOP_SDNodeXForm, "U5Imm">;

defm imm32zx16 : Immediate<i32, [{
  return (N->getZExtValue() & ~0x00000000000000ffULL) == 0;
}], NOOP_SDNodeXForm, "U16Imm">;

defm imm32zx5O : Immediate<i32, [{
  return (N->getZExtValue() & ~0x000000000000001fULL) == 0;
}], NOOP_SDNodeXForm, "U5ImmO">;

defm imm32zx10WO : Immediate<i32, [{
  return (N->getZExtValue() & ~0x00000000000003ffULL) == 0;
}], NOOP_SDNodeXForm, "U10ImmWO">;



// Predicate: Arbitrary 32 bit value.
def uimm32 : PatLeaf<(imm), [{
  uint64_t Val = N->getZExtValue();
  return isUInt<32>(Val) && (Val & 0xffff);
}]>;


def memsrc : Operand<i32> {
  let MIOperandInfo = (ops GPROpnd, i32imm);
  let PrintMethod = "printAddrModeMemSrc";
  // let EncoderMethod = "getMemSrcValue";
}


def memsrc1 : Operand<i32> {
  let MIOperandInfo = (ops GPROpnd, i32imm);
  let PrintMethod = "printAddrModeMemSrc";
  // let EncoderMethod = "getMemSrcValue";
}


// Operand for printing out a condition code.
def cc : Operand<i32> {
  let PrintMethod = "printCCOperand";
  // let EncoderMethod = "getCCOpValue";
}

// - TODO: need to implement new  PrintMethod and EncoderMethod
def jmptarget : Operand<OtherVT> ;


def i32imm_lo : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= 0 && Imm < 65536;
}]>;

def f32imm_lo : Operand<f32>, FPImmLeaf<f32, [{
  return true;
}]>;

def f64imm_lo : Operand<f64>, FPImmLeaf<f64, [{
  return true;
}]>;



//===----------------------------------------------------------------------===//
// Complex Pattern Definitions.
//===----------------------------------------------------------------------===//

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;

//===----------------------------------------------------------------------===//
// Logical Instructions
//===----------------------------------------------------------------------===//

// Multiclass for logical instructions with immediates.
// The pattern for "and" is slightly different.
multiclass LogicImm<bits<2> FuncI, string OpcStr, SDNode OpNode,
                  InstrItinClass itin = NoItinerary> {
  def ri  : F_LI<FuncI, 0b0,
                 (outs GPROpnd:$rd),
                 !if(!eq(OpcStr, "and"),
                   (ins GPROpnd:$rs1, imm32lo16c:$imm16),
                   (ins GPROpnd:$rs1, imm32lo16:$imm16)
                 ),
                 !strconcat(OpcStr, " $rd, $rs1, $imm16"),
                 !if(!eq(OpcStr, "and"),
                   [(set i32:$rd, (OpNode GPROpnd:$rs1, imm32lo16c:$imm16))],
                   [(set i32:$rd, (OpNode GPROpnd:$rs1, imm32lo16:$imm16))]
                 ),
                 itin>;
  def riu  : F_LI<FuncI, 0b1,
                 (outs GPROpnd:$rd),
                 !if(!eq(OpcStr, "and"),
                   (ins GPROpnd:$rs1, imm32hi16c:$imm16),
                   (ins GPROpnd:$rs1, imm32hi16:$imm16)
                 ),
                 !strconcat(OpcStr, ".u $rd, $rs1, $imm16"),
                 !if(!eq(OpcStr, "and"),
                   [(set i32:$rd, (OpNode GPROpnd:$rs1, imm32hi16c:$imm16))],
                   [(set i32:$rd, (OpNode GPROpnd:$rs1, imm32hi16:$imm16))]
                 ),
                 itin>;
}

// Multiclass for logical instructions with triadic registers or immediates.
multiclass Logic<bits<5> FuncR, bits<2> FuncI, string OpcStr, SDNode OpNode,
                 InstrItinClass itin = NoItinerary>
                 : LogicImm<FuncI, OpcStr, OpNode, itin> {
  let isCommutable = 1 in
    def rr  : F_LR<FuncR, 0b0,
                   (outs GPROpnd:$rd), (ins GPROpnd:$rs1, GPROpnd:$rs2),
                   !strconcat(OpcStr, " $rd, $rs1, $rs2"),
                   [(set i32:$rd, (OpNode GPROpnd:$rs1, GPROpnd:$rs2))],
                   itin>;
  def rrc : F_LR<FuncR, 0b1,
                 (outs GPROpnd:$rd), (ins GPROpnd:$rs1, GPROpnd:$rs2),
                 !strconcat(OpcStr, ".c $rd, $rs1, $rs2"),
                 [(set i32:$rd, (OpNode GPROpnd:$rs1, (not GPROpnd:$rs2)))],
                 itin>;
}

defm MASK : LogicImm<0b01, "mask", and>;
defm AND : Logic<0b01000, 0b00, "and", and>;
defm XOR : Logic<0b01010, 0b10, "xor", xor>;
defm OR  : Logic<0b01011, 0b11, "or", or>;

// Pattern for 32 bit constants.
def : Pat<(and GPR:$rs1, uimm32:$imm),
          (ANDri (ANDriu GPR:$rs1, (HI16 i32:$imm)), (LO16 i32:$imm))>;
def : Pat<(or GPR:$rs1, uimm32:$imm),
          (ORri (ORriu GPR:$rs1, (HI16 i32:$imm)), (LO16 i32:$imm))>;
def : Pat<(xor GPR:$rs1, uimm32:$imm),
          (XORri (XORriu GPR:$rs1, (HI16 i32:$imm)), (LO16 i32:$imm))>;

// def SHL8mCLS  : QASMInst<0xD22, (outs  GPROpnd:$ds), (ins memsrc:$dst, imm32zx16:$imm),
//                  "LDIe {$ds, $dst|$dst, $imm}",
//                  [(set GPROpnd:$ds, (shl (load addr:$dst), imm32zx16:$imm))]>;

// Multiclass for bit-field instructions with triadic registers or immediates.
multiclass Bitfield<bits<6> Func, string OpcStr, SDNode OpNode, PatLeaf ImmOp,
                 InstrItinClass itin = NoItinerary> {
  def rr  : F_BR<Func,
                 (outs GPROpnd:$rd), (ins GPROpnd:$rs1, GPROpnd:$rs2),
                 !strconcat(OpcStr, " $rd, $rs1, $rs2"),
                 [(set GPROpnd:$rd, (OpNode GPROpnd:$rs1, GPROpnd:$rs2))],
                 itin>;
  def rwo : F_BI<Func,
                 (outs GPROpnd:$rd), (ins GPROpnd:$rs1, ImmOp:$w5o5),
                 !strconcat(OpcStr, " $rd, $rs1, $w5o5"),
                 [(set GPROpnd:$rd, (OpNode GPROpnd:$rs1, ImmOp:$w5o5))],
                 itin>;
}

defm CLR  : Bitfield<0b100000, "clr", qasm_clr, imm32zx10WO>;
defm SET  : Bitfield<0b100010, "set", qasm_set, imm32zx10WO>;
defm EXT  : Bitfield<0b100100, "ext", qasm_ext, imm32zx10WO>;
defm EXTU : Bitfield<0b100110, "extu", qasm_extu, imm32zx10WO>;
defm MAK  : Bitfield<0b101000, "SHL", qasm_mak, imm32zx16>;
defm ROT  : Bitfield<0b101010, "rot", qasm_rot, imm32zx5O>;

// Pattern for shifts
def : Pat<(sra GPR:$rs1, GPR:$rs2), (EXTrr GPR:$rs1, GPR:$rs2)>;
def : Pat<(srl GPR:$rs1, GPR:$rs2), (EXTUrr GPR:$rs1, GPR:$rs2)>;
def : Pat<(shl GPR:$rs1, GPR:$rs2), (MAKrr GPR:$rs1, GPR:$rs2)>;
def : Pat<(rotr GPR:$rs1, GPR:$rs2), (ROTrr GPR:$rs1, GPR:$rs2)>;
def : Pat<(sra GPR:$rs1, imm32zx5O:$o5), (EXTrwo GPR:$rs1, imm32zx5O:$o5)>;
def : Pat<(srl GPR:$rs1, imm32zx5O:$o5), (EXTUrwo GPR:$rs1, imm32zx5O:$o5)>;
def : Pat<(shl GPR:$rs1, imm32zx5O:$o5), (MAKrwo GPR:$rs1, imm32zx5O:$o5)>;
def : Pat<(rotr GPR:$rs1, imm32zx5O:$o5), (ROTrwo GPR:$rs1, imm32zx5O:$o5)>;

let rs1 = 0 in
class FindBF<bits<6> Func, string OpcStr, SDNode OpNode,
            InstrItinClass itin = NoItinerary> :
  F_BR<Func, (outs GPROpnd:$rd), (ins GPROpnd:$rs2),
             !strconcat(OpcStr, " $rd, $rs2"),
             [(set GPROpnd:$rd, (OpNode GPROpnd:$rs2))],
             itin>;

def FF1rr : FindBF<0b111010, "ff1", qasm_ff1>;
def FF0rr : FindBF<0b111011, "ff0", qasm_ff0>;

// ctlz = 32 - ff1
//def : Pat<(ctlz GPR:$rs1, GPR:$rs2), (SUBri (i32 32), (FF1rr GPR:$rs1, GPR:$rs2))>;

//===----------------------------------------------------------------------===//
// Arithmetic Instructions
//===----------------------------------------------------------------------===//

// Multiclass for Increase and Decrease instructions (MUST BE ABOVE Arith)
multiclass IncDecOp<bits<6> opcode, string opstr, SDNode opnode, InstrItinClass itin = NoItinerary> {
  let Constraints = "$rd = $rs1" in {
  def rr  : F_IRC<opcode, 0b0, 0b0,
                 (outs GPROpnd:$rd), (ins GPROpnd:$rs1, GPROpnd:$rs2),
                 !strconcat(opstr, " $rd, $rs2"),
                 [(set GPROpnd:$rd, (opnode GPROpnd:$rs1, GPROpnd:$rs2))],
                 itin>;

  def ri  : F_II<opcode,
                 (outs GPROpnd:$rd), (ins GPROpnd:$rs1, imm32zx16:$imm16),
                 !strconcat(opstr, " $rd, $imm16"),
                 [(set GPROpnd:$rd, (opnode GPROpnd:$rs1, imm32zx16:$imm16))],
                 itin>;

    
  }

}

defm INC              : IncDecOp<0b011100, "INC", add>;
defm DEC              : IncDecOp<0b011101, "DEC", sub>;

def : Pat<(qasm_dec GPROpnd:$rs1, imm32zx16:$imm16),  (DECri GPROpnd:$rs1, imm32zx16:$imm16)>;
def : Pat<(qasm_dec GPROpnd:$rs1, GPROpnd:$rs2),      (DECrr GPROpnd:$rs1, GPROpnd:$rs2)>;

def : Pat<(qasm_inc GPROpnd:$rs1, imm32zx16:$imm16),  (INCri GPROpnd:$rs1, imm32zx16:$imm16)>;
def : Pat<(qasm_inc GPROpnd:$rs1, GPROpnd:$rs2),      (INCrr GPROpnd:$rs1, GPROpnd:$rs2)>;
// def : Pat<(qasm_inc addr:$rs1, imm32zx16:$imm16),     (INCri GPROpnd:$rs1, imm32zx16:$imm16)>;


// Multiclass for arithmetic instructions with triadic registers or immediates.
multiclass Arith<bits<6> Func, string OpcStr, SDNode OpNode,
                 InstrItinClass itin = NoItinerary> {
  def rr  : F_IRC<Func, 0b0, 0b0,
                 (outs GPROpnd:$rd), (ins GPROpnd:$rs1, GPROpnd:$rs2),
                 !strconcat(OpcStr, " $rd, $rs1, $rs2"),
                 [(set GPROpnd:$rd, (OpNode GPROpnd:$rs1, GPROpnd:$rs2))],
                 itin>;

  def ri  : F_II<Func,
                 (outs GPROpnd:$rd), (ins GPROpnd:$rs1, imm32zx16:$imm16),
                 !strconcat(OpcStr, " $rd, $rs1, $imm16"),
                 [(set GPROpnd:$rd, (OpNode GPROpnd:$rs1, imm32zx16:$imm16))],
                 itin>;
}

let isCommutable = 1 in
defm ADDU : Arith<0b011000, "ADDU", add>;
defm SUBU : Arith<0b011001, "SUBU", sub>;


//===----------------------------------------------------------------------===//
// Mov Immediate Instructions
//===----------------------------------------------------------------------===//


class MOV<bits<16> opcode, string opstr, dag ins, list<dag>pattern>
    : QASMInst<opcode, (outs GPROpnd:$dst), ins,
              !strconcat(opstr, "\t$dst, $imm"), pattern> {
  // bits<4> dst;
  // bits<16> imm;
  // let Inst{15-12} = dst;
  // let Inst{11-0}  = imm{11-0};
  // let Inst{19-16} = imm{15-12};
  // let Inst{20} = 0;
  // let Inst{25} = 1;
}

def MOVLOi16 : MOV<0xC03E, "MOV", (ins i32imm:$imm),
                  [(set i32:$dst, i32imm_lo:$imm)]>;

def MOVHIi16 : MOV<0xC03E, "MOV", (ins GPROpnd:$src1, i32imm:$imm),
                  [(set GPROpnd:$dst, (qasm_mov GPROpnd:$src1, i32imm_lo:$imm))]>;

def MOV_rr :  QASMInst<0xC03E, (outs GPROpnd:$dst), (ins GPROpnd:$src1),
              "MOV\t$dst, $src1", []>;


//===----------------------------------------------------------------------===//
// Control Flow Instructions
//===----------------------------------------------------------------------===//

let isTerminator = 1, isBarrier = 1 in {
  def JMP : F_JMP<0b11000, 0, (outs), (ins GPROpnd:$rs2),
                  "JMP $rs2", [(brind GPROpnd:$rs2)]>;
  let hasDelaySlot = 1 in
    def JMPn : F_JMP<0b11000, 1, (outs), (ins GPROpnd:$rs2),
                     "JMP.n $rs2", [(brind GPROpnd:$rs2)]>;
}

let isCall = 1, isTerminator = 1, isBarrier = 1 in {
  def JSR : F_JMP<0b11001, 0, (outs), (ins GPROpnd:$rs2),
                  "jsr $rs2", []>;
  let hasDelaySlot = 1 in
    def JSRn : F_JMP<0b11001, 1, (outs), (ins GPROpnd:$rs2),
                     "jsr.n $rs2", []>;
}

let isReturn = 1, isTerminator = 1, isBarrier = 1, Uses = [R10] in {
  def RET : Pseudo<(outs), (ins), [(retflag)]>;
}


let isBranch = 1, isTerminator = 1, isBarrier = 1 in {

  def BR : F_BRANCH<0b11000, 0, (outs), (ins brtarget26:$d26), "BRI $d26",
                    [(br bb:$d26)]>;

  let hasDelaySlot = 1 in {
  def BRn : F_BRANCH<0b11000, 1, (outs), (ins brtarget26:$d26), "BRI.n $d26",
                       [(br bb:$d26)]>;}

}

// let isBranch = 1, isCodeGenOnly = 1, isTerminator = 1, Uses = [R1],SchedRW = [WriteJmp] in {
let isBranch = 1, isTerminator = 1, Uses = [R9] in {
    def JCC : F_BCOND<0xC007, (outs), 
              (ins jmptarget:$cond, cc:$imm16), 
              "BR${imm16} $cond",
              [(qasm_brcc bb:$cond, i32imm_lo:$imm16)]>;

}

multiclass CMPOp<bits<16> opcode, string opstr> {
  let Defs = [R9] in {
    def ri : QASMInst<opcode,
                  (outs), (ins GPROpnd:$rd, i32imm:$imm),
                  !strconcat(opstr, " $rd, $imm"),
                  [(qasm_cmp GPROpnd:$rd, imm32lo16c:$imm), (implicit R9)]>; 

    def rr: QASMInst<opcode,
                  (outs), (ins GPROpnd:$rd, GPROpnd:$rs),
                  !strconcat(opstr, " $rd, $rs"),
                  [(qasm_cmp GPROpnd:$rd, GPROpnd:$rs), (implicit R9)]>; 
  }
}

defm CMP : CMPOp<0xC02A, "CMP">;
def : InstAlias<"nop", (ORrr R0, R0, R0)>;




//===----------------------------------------------------------------------===//
// Load/Store Instructions
//===----------------------------------------------------------------------===//

class LoadUnsigned<bits<1> b, string OpcStr,
                 InstrItinClass itin = NoItinerary> :
  F_LU<b, (outs GPROpnd:$rd), (ins GPROpnd:$rs1, imm32zx16:$si16),
       !strconcat(OpcStr, "\t$rd, $rs1, $si16"),
       [],
       itin>;

def LDurih : LoadUnsigned<0b0, "ld.hu">;
def LDurib : LoadUnsigned<0b1, "ld.bu">;


// |  Mode |   Inst   |   Op1   |   Op2   |   Op3   |
// |-------|----------+---------+---------+---------+
// |  (1)  |   LDD    |   R#    |  IDAddr |   N/A   |
// |  (2)  |   LDD    |   R#    |  UDAddr |   N/A   |
// |  (3)  |   LDD    |   R#1   |   R#2   |   N/A   |
multiclass LoadDirectOp<bits<16> opcode, string opstr, PatFrag opnode> {

  def rr : QASMInst<opcode, (outs GPROpnd:$dst), (ins GPROpnd:$src),
                   !strconcat(opstr, "\t$dst, $src"),
                   [(set GPROpnd:$dst, (opnode GPROpnd:$src))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }

  def ri : QASMInst<opcode, (outs GPROpnd:$dst), (ins i32imm:$imm),
                   !strconcat(opstr, "\t$dst, $imm"),
                   [(set GPROpnd:$dst, (opnode i32imm_lo:$imm))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }

  def rqr : QASMInst<opcode, (outs QPROpnd:$dst), (ins QPROpnd:$src),
                   !strconcat(opstr, "\t$dst, $src"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src))]>;

  def rqi : QASMInst<opcode, (outs QPROpnd:$dst), (ins i32imm:$imm),
                   !strconcat(opstr, "\t$dst, $imm"),
                   [(set QPROpnd:$dst, (opnode i32imm_lo:$imm))]>;

  def rq : QASMInst<opcode, (outs QPROpnd:$dst), (ins GPROpnd:$src),
                   !strconcat(opstr, "\t$dst, $src"),
                   [(set QPROpnd:$dst, (opnode GPROpnd:$src))]>;
}

// |  Mode |   Inst   |   Op1   |   Op2   |   Op3   |
// |-------|----------+---------+---------+---------+
// |  (1)  |   LDI    |   R#    |  IDAddr |   Imm   |
// |  (2)  |   LDI    |   R#1   |  IDAddr |   R#2   |
// |  (3)  |   LDI    |   R#    |  UDaddr |   Imm   | 
// |  (4)  |   LDI    |   R#1   |  UDaddr |   R#2   |
// |  (5)  |   LDI    |   R#1   |    R#2  |   Imm   |
// |  (6)  |   LDI    |   R#1   |    R#2  |   R#3   |
multiclass LoadIndirectOp<bits<16> opcode, string opstr, PatFrag opnode> {

  def radd : QASMInst<opcode, (outs GPROpnd:$dst), (ins memsrc:$addr),
                   !strconcat(opstr, "\t$dst, $addr"),
                   [(set GPROpnd:$dst, (opnode addr:$addr))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
  
  def rqadd : QASMInst<opcode, (outs QPROpnd:$dst), (ins memsrc:$addr),
                   !strconcat(opstr, "\t$dst, $addr"),
                   [(set QPROpnd:$dst, (opnode addr:$addr))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
}

// |  Mode |   Inst   |   Op1   |   Op2   |   Op3   |
// |-------|----------+---------+---------+---------+
// |  (1)  |   STD    |  IDAddr |   R#    |   N/A   |
// |  (2)  |   STD    |  UDAddr |   R#    |   N/A   |
// |  (3)  |   STD    |   R#1   |   R#2   |   N/A   |
multiclass StoreDirectOp<bits<16> opcode, string opstr>{
  
  def rr : QASMInst<opcode, (outs), (ins GPROpnd:$src1, GPROpnd:$src2),
                   !strconcat(opstr, "\t$src1, $src2"),
                   [(store GPROpnd:$src1, GPROpnd:$src2)]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }

  def ri : QASMInst<opcode, (outs), (ins GPROpnd:$src1, i32imm:$imm),
                   !strconcat(opstr, "\t$imm, $src1"),
                   [(store GPROpnd:$src1, i32imm_lo:$imm)]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }

  def rqi  : QASMInst<opcode, (outs), (ins QPROpnd:$src1, i32imm:$imm),
                   !strconcat(opstr, "\t$src1, $imm"),
                   [(store QPROpnd:$src1, i32imm_lo:$imm)]>;

  def rqr  : QASMInst<opcode, (outs), (ins QPROpnd:$src1, QPROpnd:$src2),
                   !strconcat(opstr, "\t$src1, $src2"),
                   [(store QPROpnd:$src1, QPROpnd:$src2)]>;
}


// |  Mode |   Inst   |   Op1   |   Op2   |   Op3   |
// |-------|----------+---------+---------+---------+
// |  (1)  |   LDI    |  IDAddr |   Imm   |   R#    |
// |  (2)  |   LDI    |  IDAddr |   R#1   |   R#2   |
// |  (3)  |   LDI    |  UDaddr |   Imm   |   R#    |
// |  (4)  |   LDI    |  UDaddr |   R#1   |   R#2   |
// |  (5)  |   LDI    |    R#1  |   Imm   |   R#2   |
// |  (6)  |   LDI    |    R#1  |   R#2   |   R#3   |
multiclass StoreIndirectOp<bits<16> opcode, string opstr>{
  
  def radd : QASMInst<opcode, (outs), (ins GPROpnd:$val, memsrc:$addr),
                   !strconcat(opstr, "\t$addr, $val"),
                   [(store GPROpnd:$val, addr:$addr)]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }

  def rqadd : QASMInst<opcode, (outs), (ins QPROpnd:$val, memsrc:$addr),
                   !strconcat(opstr, "\t$addr, $val"),
                   [(store QPROpnd:$val, addr:$addr)]>;

}

//===----------------------------------------------------------------------===//
// Quantum Instructions
//===----------------------------------------------------------------------===//

// Inst        Op1 
// <GATE>      Imm 
// <GATE>      QR# 
multiclass QuantumOp<bits<16> opcode, string opstr, SDNode opnode> {

  def addr : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins memsrc:$addr),
                   !strconcat(opstr, " q[$addr];"),
                   [(set QPROpnd:$dst, (opnode addr:$addr))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }

  def rr : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1),
                   !strconcat(opstr, " q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
    let CustomString = !strconcat("HI IM ", opstr, " IMMEDIATE");
  }
  def ri : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins i32imm:$imm16),
                   !strconcat(opstr, "\t$imm16"),
                   [(set QPROpnd:$dst, (opnode i32imm_lo:$imm16))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> imm16;
  }

  def dcom : DecomposeInst<opcode, (outs QPROpnd:$dst),
                   (ins memsrc:$addr),
                   !strconcat(opstr, " q[$dst] -> c[$addr];"),
                   [(set QPROpnd:$dst, (opnode addr:$addr))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;

    let IsShouldDecompose = 1;
    let UnitaryGate = "U...";
  }
  
  // def rii : QASMInst<opcode, (outs i32imm:$dst),
  //                  (ins i32imm:$imm16),
  //                  !strconcat(opstr, "\t$imm16"),
  //                  [(set i32imm:$dst, (opnode i32imm_lo:$imm16))]> {
  //   bits<16> dst;
  //   bits<16> src1;
  //   bits<16> imm16;
  // }
}

multiclass HFlipOp<bits<16> opcode, string opstr, SDNode opnode>{
  let Defs = [QR11] in
  def rr : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, QPROpnd:$src2),
                   !strconcat(opstr, "\t$src1, $src2"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, QPROpnd:$src2))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
}

multiclass SwapOp<bits<16> opcode, string opstr, SDNode opnode>{
  let Constraints = "$src1 = $dst1, $src2 = $dst2" in
  def rr : QASMInst<opcode, (outs QPROpnd:$dst1, QPROpnd:$dst2),
                   (ins QPROpnd:$src1, QPROpnd:$src2),
                   !strconcat(opstr, " q[$dst1], q[$dst2];"),
                   [(set QPROpnd:$dst1, QPROpnd:$dst2, (opnode QPROpnd:$src1, QPROpnd:$src2))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
}


// Inst        Op1
// MEASURE_Z   Imm
// MEASURE_Z   QR#
multiclass MeasurementOp<bits<16> opcode, string opstr, SDNode opnode> {

  def addr : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins memsrc:$addr),
                   !strconcat(opstr, " q[$dst] -> c[$addr];"),
                   [(set QPROpnd:$dst, (opnode addr:$addr))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }

  def ri : QASMInst<opcode, (outs GPROpnd:$dst),
                   (ins QPROpnd:$src1), // src1
                   !strconcat(opstr, " q[$src1] -> c[$dst];"),
                   [(set GPROpnd:$dst, (opnode QPROpnd:$src1))]> {
    // bits<16> dst;
    // bits<16> src1;
  }
  let Constraints = "$dst = $src1" in
  def rr : QASMInst<opcode, 
                   (outs GPROpnd:$dst), (ins QPROpnd:$src1),
                   !strconcat(opstr, "\t$src1"),
                   [(set GPROpnd:$dst, (opnode QPROpnd:$src1))]> {
    // bits<16> dst;
    // bits<16> src1;
    // bits<16> src2;
  }
}

// Inst   Op1   Op2
// FMR    R#    Imm
// FMR    R#    QR#
multiclass FMROp<bits<16> opcode, string opstr, SDNode opnode>{
  def ri : QASMInst<opcode, (outs GPROpnd:$dst),
                   (ins imm32lo16c:$imm16),
                   !strconcat(opstr, "\t$dst, $imm16"),
                   [(set GPROpnd:$dst, (opnode imm32lo16c:$imm16))]> {
    // bits<16> dst;
    // bits<16> src1;
    // bits<16> imm16;
  }
  def rr : QASMInst<opcode, (outs GPROpnd:$dst),
                   (ins QPROpnd:$src1),
                   !strconcat(opstr, "\t$dst, $src1"),
                   [(set GPROpnd:$dst, (opnode QPROpnd:$src1))]> {
    // bits<16> dst;
    // bits<16> src1;
    // bits<16> src2;
  }
}

// Inst   Op1   Op2
// QMOV   QR#   Imm
// QMOV   QR#   R#
multiclass QMovOp<bits<16> opcode, string opstr, SDNode opnode> {
    // let Constraints = "$dst = $imm" in
    def ri : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins i32imm:$imm),
                   !strconcat(opstr, "\t$dst, $imm"),
                   [(set QPROpnd:$dst, (opnode i32imm_lo:$imm))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> imm16;
    }
  
  // let Constraints = "$dst = $src1" in
  def rr : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins GPROpnd:$src1),
                   !strconcat(opstr, "\t$dst, $src1"),
                   [(set QPROpnd:$dst, (opnode GPROpnd:$src1))]>  {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
}


multiclass MergeSplitOp<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst1, $src2 = $dst2" in
  def rr : QASMInst<opcode, (outs QPROpnd:$dst1, QPROpnd:$dst2),
                   (ins QPROpnd:$src1, QPROpnd:$src2),
                   !strconcat(opstr, " q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst1, QPROpnd:$dst2, (opnode QPROpnd:$src1, QPROpnd:$src2))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
  def ri : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                (ins i32imm:$imm1, i32imm:$imm2),
                !strconcat(opstr, " q[$imm1], q[$imm2];"),
                [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode i32imm_lo:$imm1, i32imm_lo:$imm2))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> imm16;
  }
}

multiclass MultiCtrlTargetOp<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst1, $src2 = $dst2, $src3 = $dst3" in

  def rr : QASMInst<opcode, (outs QPROpnd:$dst1, QPROpnd:$dst2, QPROpnd:$dst3),
                   (ins QPROpnd:$src1, QPROpnd:$src2, QPROpnd:$src3),
                   !strconcat(opstr, " q[$src1], q[$src2], q[$src3];"),
                   [(set QPROpnd:$dst1, QPROpnd:$dst2, QPROpnd:$dst3, (opnode QPROpnd:$src1, QPROpnd:$src2, QPROpnd:$src3))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
  let Constraints = "$imm1 = $dst, $imm2 = $dst2, $imm3 = $dst3" in
  def ri : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2, QPROpnd:$dst3),
                (ins i32imm:$imm1, i32imm:$imm2, i32imm:$imm3),
                !strconcat(opstr, " q[$imm1], q[$imm2], q[$imm3];"),
                [(set QPROpnd:$dst, QPROpnd:$dst2, QPROpnd:$dst3, (opnode i32imm_lo:$imm1, i32imm_lo:$imm2, i32imm_lo:$imm3))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> imm16;
  }
}

multiclass RotationOp<bits<16> opcode, string opstr, SDNode opnode> {

    let Constraints = "$src1 = $dst" in 
    def rr : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, FPROpnd:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, FPROpnd:$imm))]>{
      bits<16> dst;
      bits<16> src1;
     
   }
    let Constraints = "$src1 = $dst" in 
    def ri32 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f32imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f32imm_lo:$imm))]>{
      bits<16> dst;
      bits<16> src1;
     
   }
    let Constraints = "$src1 = $dst" in 
    def ri64 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f64imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f64imm_lo:$imm))]>{
      bits<16> dst;
      bits<16> src1;
    }
}

multiclass CtrlRotationOp<bits<16> opcode, string opstr, SDNode opnode> {


    def ri32 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1,QPROpnd:$src2, f32imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode QPROpnd:$src1,QPROpnd:$src2, f32imm_lo:$imm))]>{
      bits<16> dst;
      bits<16> src1;
     
   }

    def ri64 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1,QPROpnd:$src2, f64imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode QPROpnd:$src1,QPROpnd:$src2, f64imm_lo:$imm))]>{
      bits<16> dst;
      bits<16> src1;
    }
}

multiclass QuantumU1Op<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst" in 
  def ri32 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f32imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f32imm_lo:$imm))]>{
    bits<16> dst;
    bits<16> src1;
   }
  let Constraints = "$src1 = $dst" in 
  def ri64 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f64imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f64imm_lo:$imm))]>{
    bits<16> dst;
    bits<16> src1;
  }
}

multiclass QuantumU2Op<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst" in 
  def ri32 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f32imm:$imm, f32imm:$imm2),
                   !strconcat(opstr, "( $imm, $imm2 ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f32imm_lo:$imm, f32imm_lo:$imm2))]>{
    bits<16> dst;
    bits<16> src1;
   }
  let Constraints = "$src1 = $dst" in 
  def ri64 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f64imm:$imm, f64imm:$imm2),
                   !strconcat(opstr, "( $imm, $imm2 ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f64imm_lo:$imm, f64imm_lo:$imm2))]>{
    bits<16> dst;
    bits<16> src1;
  }
}

multiclass QuantumU3Op<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst" in 
  def ri32 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f32imm:$imm, f32imm:$imm2, f32imm:$imm3),
                   !strconcat(opstr, "( $imm, $imm2, $imm3 ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f32imm_lo:$imm, f32imm_lo:$imm2, f32imm_lo:$imm3))]>{
    bits<16> dst;
    bits<16> src1;
   }
  let Constraints = "$src1 = $dst" in 
  def ri64 : QASMInst<opcode, (outs QPROpnd:$dst),
                   (ins QPROpnd:$src1, f64imm:$imm, f64imm:$imm2, f64imm:$imm3),
                   !strconcat(opstr, "( $imm, $imm2, $imm3 ) q[$src1];"),
                   [(set QPROpnd:$dst, (opnode QPROpnd:$src1, f64imm_lo:$imm, f64imm_lo:$imm2, f64imm_lo:$imm3))]>{
    bits<16> dst;
    bits<16> src1;
  }
}

multiclass QuantumCtrlU1Op<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst, $src2 = $dst2" in 
  def ri32 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1,QPROpnd:$src2, f32imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode QPROpnd:$src1, QPROpnd:$src2, f32imm_lo:$imm))]>{
    bits<16> dst;
    bits<16> src1;
   }

  let Constraints = "$src1 = $dst, $src2 = $dst2" in 
  def ri64 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1,QPROpnd:$src2, f64imm:$imm),
                   !strconcat(opstr, "( $imm ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode QPROpnd:$src1, QPROpnd:$src2, f64imm_lo:$imm))]>{
    bits<16> dst;
    bits<16> src1;
  }
}

multiclass QuantumCtrlU2Op<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst, $src2 = $dst2" in 
  def ri32 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1,QPROpnd:$src2, f32imm:$imm, f32imm:$imm2),
                   !strconcat(opstr, "( $imm, $imm2 ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode QPROpnd:$src1, QPROpnd:$src2, f32imm_lo:$imm, f32imm_lo:$imm2))]>{
    bits<16> dst;
    bits<16> src1;
   }
  let Constraints = "$src1 = $dst, $src2 = $dst2" in 
  def ri64 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1,QPROpnd:$src2, f64imm:$imm, f64imm:$imm2),
                   !strconcat(opstr, "( $imm, $imm2 ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode QPROpnd:$src1, QPROpnd:$src2, f64imm_lo:$imm, f64imm_lo:$imm2))]>{
    bits<16> dst;
    bits<16> src1;
  }
}

multiclass QuantumCtrlU3Op<bits<16> opcode, string opstr, SDNode opnode> {
  let Constraints = "$src1 = $dst, $src2 = $dst2" in 
  def ri32 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1,QPROpnd:$src2, f32imm:$imm, f32imm:$imm2, f32imm:$imm3),
                   !strconcat(opstr, "( $imm, $imm2, $imm3 ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, (opnode QPROpnd:$src1, QPROpnd:$src2, f32imm_lo:$imm, f32imm_lo:$imm2, f32imm_lo:$imm3))]>{
    bits<16> dst;
    bits<16> src1;
   }
  let Constraints = "$src1 = $dst, $src2 = $dst2" in 
  def ri64 : QASMInst<opcode, (outs QPROpnd:$dst, QPROpnd:$dst2),
                   (ins QPROpnd:$src1, QPROpnd:$src2, f64imm:$imm, f64imm:$imm2, f64imm:$imm3),
                   !strconcat(opstr, "( $imm, $imm2, $imm3 ) q[$src1], q[$src2];"),
                   [(set QPROpnd:$dst, QPROpnd:$dst2, 
                   (opnode QPROpnd:$src1, QPROpnd:$src2, f64imm_lo:$imm, f64imm_lo:$imm2, f64imm_lo:$imm3))]>{
    bits<16> dst;
    bits<16> src1;
  }
}

multiclass CnotMoveOp<bits<16> opcode, string opstr, SDNode opnode>{
  let Constraints = "$src1 = $dst1, $src2 = $dst2, $src3 = $dst3" in 
  def rr : QASMInst<opcode, (outs QPROpnd:$dst1, QPROpnd:$dst2, QPROpnd:$dst3),
                   (ins QPROpnd:$src1, QPROpnd:$src2, QPROpnd:$src3),
                   !strconcat(opstr, "\t$src1, $src2, $src3"),
                   [(set QPROpnd:$dst1,QPROpnd:$dst2, QPROpnd:$dst3, (opnode QPROpnd:$src1, QPROpnd:$src2, QPROpnd:$src3))]> {
    bits<16> dst;
    bits<16> src1;
    bits<16> src2;
  }
}

defm MERGE_MXX        : MergeSplitOp<0x001E, "MERGE_MXX", merge_xx>;
defm MERGE_MZZ        : MergeSplitOp<0x0022, "MERGE_MZZ", merge_zz>;
defm SPLIT_MXX        : MergeSplitOp<0x0026, "SPLIT_MXX", split_xx>;
defm SPLIT_MZZ        : MergeSplitOp<0x002A, "SPLIT_MZZ", split_zz>;

defm CX_GATE          : MergeSplitOp<0x0033,      "cx",          cx_gate>;
defm CZ_GATE          : MergeSplitOp<0x0033,      "cz",          cz_gate>;
defm CY_GATE          : MergeSplitOp<0x0033,      "cy",          cy_gate>;
defm CH_GATE          : MergeSplitOp<0x0033,      "ch",          ch_gate>;
defm SWAP_GATE        : MergeSplitOp<0x003A,      "swap",        swap_gate>;

defm CRX_GATE         : CtrlRotationOp<0x003E,       "crx",    crx_gate>;
defm CRY_GATE         : CtrlRotationOp<0x003E,       "cry",    cry_gate>;
defm CRZ_GATE         : CtrlRotationOp<0x003E,       "crz",    crz_gate>;

defm CU1_GATE         : QuantumCtrlU1Op<0x003E,       "cu1",    cu1_gate>;
defm CU2_GATE         : QuantumCtrlU2Op<0x003E,       "cu2",    cu2_gate>;
defm CU3_GATE         : QuantumCtrlU3Op<0x003E,       "cu3",    cu3_gate>;

defm CCX_GATE         : MultiCtrlTargetOp<0x003E,       "ccx",    ccx_gate>;
defm CCY_GATE         : MultiCtrlTargetOp<0x003E,       "ccy",    ccy_gate>;
defm CCZ_GATE         : MultiCtrlTargetOp<0x003E,       "ccz",    ccz_gate>;
defm CCH_GATE         : MultiCtrlTargetOp<0x003E,       "cch",    cch_gate>;
defm CSWAP_GATE       : MultiCtrlTargetOp<0x003E,       "cswap",  cswap_gate>;

defm RX_GATE          : RotationOp<0x0006,       "rx",         rx_gate>; 
defm RY_GATE          : RotationOp<0x0006,       "ry",         ry_gate>;
defm RZ_GATE          : RotationOp<0x0006,       "rz",         rz_gate>;
defm P_GATE           : RotationOp<0x0005,       "p",          p_gate>;

defm U1_GATE          : QuantumU1Op<0x0006,       "u1",         u1_gate>;
defm U2_GATE          : QuantumU2Op<0x0006,       "u2",         u2_gate>;
defm U3_GATE          : QuantumU3Op<0x0006,       "u3",         u3_gate>;

defm CNOT_POST_MXX    : CnotMoveOp<0x002F,      "CNOT_POST_MXX",      cnot_post_xx>;
defm CNOT_POST_MZZ    : CnotMoveOp<0x0033,      "CNOT_POST_MZZ",      cnot_post_zz>;

defm MOVE_POST_MXX    : MergeSplitOp<0x0036,    "MOVE_POST_MXX",      move_post_xx>;
defm MOVE_POST_MZZ    : MergeSplitOp<0x003A,    "MOVE_POST_MZZ",      move_post_zz>; 

defm STD              : StoreDirectOp<0xC036,       "STD">;
defm STI              : StoreIndirectOp<0xC03B,     "STI">;

defm LDD              : LoadDirectOp<0xC02E,      "LDD", load>;
defm LDI              : LoadIndirectOp<0xC033,    "LDI", load>;

defm CTRL_GATE        : QuantumOp<0x0006,       "ctrl",       ctrl_gate>;

defm INIT_X_GATE      : QuantumOp<0x0005,       "INIT_X",     init_x_gate>;    
defm INIT_Z_GATE      : QuantumOp<0x0019,       "INIT_Z",     init_z_gate>;
defm X_GATE           : QuantumOp<0x000D,       "x",          x_gate>;
defm Z_GATE           : QuantumOp<0x0011,       "z",          z_gate>;
defm Y_GATE           : QuantumOp<0x0009,       "y",          y_gate>;
defm SX_Gate          : QuantumOp<0x000E,       "sx",         sx_gate>;
defm S_GATE           : QuantumOp<0x0012,       "s",          s_gate>;
defm T_GATE           : QuantumOp<0x0013,       "t",          t_gate>;
defm I_GATE           : QuantumOp<0x0001,       "id",          i_gate>;
defm H_GATE           : QuantumOp<0x003D,       "h",          h_gate>;

defm SDAG_GATE        : QuantumOp<0x0066,       "sdg",        sdag_gate>;
defm TDAG_GATE        : QuantumOp<0x0062,       "tdg",        tdag_gate>;

defm MEASURE_X_GATE   : MeasurementOp<0x0015,   "measure_x",   meas_x_gate>;
defm MEASURE_Z_GATE   : MeasurementOp<0x0011,   "measure",     meas_z_gate>;


let mayLoad = 1 in
defm QMOV             : QMovOp<0x8006,          "QMOV",       qmov>;
defm FMR              : FMROp<0xC042,           "FMR",        fmr>;

defm DEFORM           : QuantumOp<0x0041,       "DEFORM",         deform>;
defm FLIP_EXPAND      : HFlipOp<0x0046,         "FLIP_EXPAND",    flip_expand>;
defm FLIP_CONTRACT    : HFlipOp<0x004A,         "FLIP_CONTRACT",  flip_contract>;
defm FLIP_SHIFT       : HFlipOp<0x004E,         "FLIP_SHIFT",     flip_shift>;


// Gate cancellations "self-canceling"
def : Pat<(qubit (x_gate (qubit (x_gate QPR:$rs1)))),  (qubit QPROpnd:$rs1)>;
def : Pat<(qubit (z_gate (qubit (z_gate QPR:$rs1)))),  (qubit QPROpnd:$rs1)>;
def : Pat<(qubit (y_gate (qubit (y_gate QPR:$rs1)))),  (qubit QPROpnd:$rs1)>;
def : Pat<(qubit (h_gate (qubit (h_gate QPR:$rs1)))),  (qubit QPROpnd:$rs1)>;


def : InstAlias<"LLD $rd, $rs",   (LDIradd GPR:$rd, GPR:$rs, 0)>;

class Insn<bits <4> MajOpc, bit MinOpc> {
  bits<32> insnEncoding;
  let insnEncoding{15-12} = MajOpc;
  let insnEncoding{11} = MinOpc;
}

multiclass RegAndImmInsn<bits <4> opcode> {
  def rr : Insn<opcode, 0>;
  def ri : Insn<opcode, 1>;
}

def SUBNA : Insn<0x00, 0>;
defm ADD : RegAndImmInsn<0x01>;