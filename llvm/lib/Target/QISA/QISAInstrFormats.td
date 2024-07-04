//===-- QISAInstrFormats.td - QISA Instruction Formats -----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class InstQISA<dag outs, dag ins, string asmstr, list<dag> pattern,
             InstrItinClass itin = NoItinerary>
   : Instruction {
  field bits<64> Inst;
  field bits<64> SoftFail = 0; // Needed by disassembler.

  let Namespace = "QISA";
  let Size = 4;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString   = asmstr;
  let Pattern = pattern;

  let DecoderNamespace = "QISA";

  let Itinerary = itin;
}

class Pseudo<dag outs, dag ins, list<dag> pattern>
  : InstQISA<outs, ins, "", pattern> {
  let isCodeGenOnly       = 1;
  let isPseudo            = 1;
  let hasNoSchedulingInfo = 1;
}

// The formats follow the categories specified in the User Manual.

class Q_L<dag outs, dag ins, string asmstr, list<dag> pattern,
            InstrItinClass itin = NoItinerary> : InstQISA<outs, ins, asmstr, pattern, itin>{
  bits<5> rd;
  bits<5> rs;
  bits<5> rt;
  
  let Inst{25-21} = rs;
  let Inst{20-16} = rt;
  let Inst{15-11} = rd;
}

// Category: Logical.
class F_L<dag outs, dag ins, string asmstr, list<dag> pattern,
          InstrItinClass itin = NoItinerary>
   : InstQISA<outs, ins, asmstr, pattern, itin> {
  bits<5>  rd;
  bits<5>  rs1;
  let Inst{25-21} = rd;
  let Inst{20-16} = rs1;
}

// Format: Logical with 16bit immediate.
class F_LI<bits<2> func, bits<1> upper, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin = NoItinerary>
   : F_L<outs, ins, asmstr, pattern, itin> {
  bits<16> imm16;
  let Inst{31-29} = 0b010;
  let Inst{28-27} = func;
  let Inst{26}    = upper;
  let Inst{15-0}  = imm16;
}

// Format: Logical with triadic register.
class F_LR<bits<5> func, bits<1> comp, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin = NoItinerary>
   : F_L<outs, ins, asmstr, pattern, itin> {
  bits<5>  rs2;
  let Inst{31-26} = 0b111101;
  let Inst{15-11} = func;
  let Inst{10}    = comp;
  let Inst{9-5}   = 0b00000;
  let Inst{4-0}   = rs2;
}

// Category: Bit-fields.
class F_B<bits<6> func, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin = NoItinerary>
   : F_L<outs, ins, asmstr, pattern, itin> {
  let Inst{31-27} = 0b11110;
  let Inst{15-10} = func;
}

// Format: Bit-field with triadic register.
class F_BR<bits<6> func, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin = NoItinerary>
   : F_B<func, outs, ins, asmstr, pattern, itin> {
  bits<5> rs2;
  let Inst{26}  = 0b1;
  let Inst{9-5} = 0b00000;
  let Inst{4-0} = rs2;
}

// Format: Bit-field with 10-bit immediate.
class F_BI<bits<6> func, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin = NoItinerary>
   : F_B<func, outs, ins, asmstr, pattern, itin> {
  bits<10> w5o5;
  let Inst{26}  = 0b0;
  let Inst{9-0} = w5o5;
}

// Category: Integer.
class F_I<dag outs, dag ins, string asmstr, list<dag> pattern,
          InstrItinClass itin = NoItinerary>
   : InstQISA<outs, ins, asmstr, pattern, itin> {
  bits<5>  rd;
  bits<5>  rs1;
  let Inst{25-21} = rd;
  let Inst{20-16} = rs1;
}

// Format: Integer with 16bit immediate.
class F_II<bits<6> func, dag outs, dag ins, string asmstr, list<dag> pattern,
           InstrItinClass itin = NoItinerary>
   : F_I<outs, ins, asmstr, pattern, itin> {
  bits<16> imm16;

  let Inst{31-26} = func;
  let Inst{15-0}  = imm16;
}

// Format: Integer with triadic register and carry.
class F_IRC<bits<6> func, bits<1> carryin, bits<1> carryout, dag outs, dag ins,
            string asmstr, list<dag> pattern, InstrItinClass itin = NoItinerary>
   : F_I<outs, ins, asmstr, pattern, itin> {
  bits<5> rs2;

  let Inst{31-26} = 0b111101;
  let Inst{15-10} = func;
  let Inst{9}     = carryin;
  let Inst{8}     = carryout;
  let Inst{7-5}   = 0b000;
  let Inst{4-0}   = rs2;
}

// Category: Load/Store/Exchange

// Format: Load/Store with word index
class F_LSINDEX<dag outs, dag ins, string asmstr, list<dag> pattern,
                InstrItinClass itin = NoItinerary>
   : InstQISA<outs, ins, asmstr, pattern, itin> {
  bits<5>  rd;
  bits<5>  rs1;
  bits<16> si16;

  let Inst{25-21} = rd;
  let Inst{20-16} = rs1;
  let Inst{15-0}  = si16;
}

// Format: Load/Store
class F_LS<bits<4> func, bits<2> ty,
               dag outs, dag ins, string asmstr, list<dag> pattern,
               InstrItinClass itin = NoItinerary>
   : F_LSINDEX<outs, ins, asmstr, pattern, itin> {
  let Inst{31-28} = func;
  let Inst{27-26} = ty;
}

// Format: Load unsigned
class F_LU<bits<1> b,
               dag outs, dag ins, string asmstr, list<dag> pattern,
               InstrItinClass itin = NoItinerary>
   : F_LSINDEX<outs, ins, asmstr, pattern, itin> {
  let Inst{31-27} = 0b00001;
  let Inst{26}    = b;
}


// Category: Flow control.

// Format: JMP/JSR
class F_JMP<bits<5> func, bits<1> next,
            dag outs, dag ins, string asmstr, list<dag> pattern,
            InstrItinClass itin = NoItinerary>
   : InstQISA<outs, ins, asmstr, pattern, itin> {
  bits<5> rs2;

  let Inst{31-26} = 0b111101;
  let Inst{25-16} = 0b0000000000;
  let Inst{15-11} = func;
  let Inst{10}    = next;
  let Inst{9-5}   = 0b00000;
  let Inst{4-0}   = rs2;
}

// Format: BR/BSR
class F_BRANCH<bits<5> func, bits<1> next,
               dag outs, dag ins, string asmstr, list<dag> pattern,
               InstrItinClass itin = NoItinerary>
   : InstQISA<outs, ins, asmstr, pattern, itin> {
  bits<26> d26;

  let Inst{31-27} = func;
  let Inst{26}    = next;
  let Inst{25-0}  = d26;
}

// Format: BB0/BB1/BCOND
class F_BCOND<bits<16> func,
               dag outs, dag ins, string asmstr, list<dag> pattern>
   : InstQISA<outs, ins, asmstr, pattern> {
  let Inst{63-48} = func;
  let Inst{47-32} = 0b0000000000000000;
  let Inst{31-16} = 0b0000000000000000;
  let Inst{15-0} = 0b0000000000000000;
}

class InstQuantum<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<64> Inst;

  let Namespace = "QISA";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString   = asmstr;
  let Pattern = pattern;
  // let Size = 16;
  let hasSideEffects = 1;
}

// QISA pseudo instructions format
class QISAPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstQuantum<outs, ins, asmstr, pattern> {
  let isPseudo = 1;
}

// OOB0 : Operation Operand that has 0 block
class QISAInst<bits<16> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstQuantum<outs, ins, asmstr, pattern> {
  let Inst{63-48} = opcode;
  let Inst{47-32} = 0b0000000000000000;
  let Inst{31-16} = 0b0000000000000000;
  let Inst{15-0} = 0b0000000000000000;
}

// class MulInst<bits<7> opcode, dag outs, dag ins, string asmstr,
//               list<dag> pattern> : InstQuantum<outs, ins, asmstr, pattern> {
//   let Inst{27-21} = opcode;
//   let Inst{20}    = 0;  // We never update flags
//   let Inst{7-4}   = 0b1001;
// }

// class BranchInst<bits<4> opcode, dag outs, dag ins,
//                  string asmstr, list<dag> pattern>
//     : InstQuantum<outs, ins, asmstr, pattern> {
//   let Inst{27-24} = opcode;
// }
