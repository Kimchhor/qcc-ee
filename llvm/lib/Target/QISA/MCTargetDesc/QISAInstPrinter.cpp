//===- QISAInstPrinter.cpp - Convert QISA MCInst to assembly syntax -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QISAInstPrinter.h"
#include "QISAInstrInfo.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "QISACondCode.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "QISAGenAsmWriter.inc"

void QISAInstPrinter::printOperand(const MCInst *MI, int OpNum,
                                   const MCSubtargetInfo &STI, raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(OpNum);
  if (MO.isReg()) {
    if (!MO.getReg())
      O << '0';
    else
      O  << getRegisterName(MO.getReg());
  } else if (MO.isImm()){
    // O << MO.getImm();
    O << "0x";
    O.write_hex(MO.getImm());
  }
  else if (MO.isExpr())
    MO.getExpr()->print(O, &MAI);
  else
    llvm_unreachable("Invalid operand");
}

void QISAInstPrinter::printOperand(const MCOperand &MO, const MCAsmInfo *MAI,
                                   raw_ostream &O) {
  if (MO.isReg()) {
    if (!MO.getReg())
      O << '0';
    else
      O << getRegisterName(MO.getReg());
  } else if (MO.isImm()) {
    // O << MO.getImm();
    O << "0x";
    O.write_hex(MO.getImm());
  }
  else if (MO.isExpr())
    MO.getExpr()->print(O, MAI);
  else
    llvm_unreachable("Invalid operand");
}

void QISAInstPrinter::printU5ImmOperand(const MCInst *MI, int OpNum,
                                        const MCSubtargetInfo &STI,
                                        raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  O << Value;
}

void QISAInstPrinter::printU5ImmOOperand(const MCInst *MI, int OpNum,
                                         const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  O << "<" << Value << ">";
}

void QISAInstPrinter::printU10ImmWOOperand(const MCInst *MI, int OpNum,
                                           const MCSubtargetInfo &STI,
                                           raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  int64_t Width = (Value >> 5) & 0x1f;
  int64_t Offset = Value & 0x1f;
  O << Width << "<" << Offset << ">";
}

void QISAInstPrinter::printU16ImmOperand(const MCInst *MI, int OpNum,
                                         const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
   O << "0x";
   O.write_hex(Value);
}


void QISAInstPrinter::printU8ImmOperand(const MCInst *MI, int OpNum,
                                         const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  O << Value;
}

void QISAInstPrinter::printPCRelOperand(const MCInst *MI, uint64_t Address,
                                        int OpNum, const MCSubtargetInfo &STI,
                                        raw_ostream &O) {
  // TODO
  const MCOperand &MO = MI->getOperand(OpNum);
  if (MO.isImm()) {
    O << "0x";
    O.write_hex(MO.getImm());
  } else
    MO.getExpr()->print(O, &MAI);
}

void QISAInstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                      const MCSubtargetInfo &STI,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case QISACC::EQ:
  case QISACC::BE:
  case QISACC::HE:
   O << "EQ";
   break;
  case QISACC::NE:
  case QISACC::NB:
  case QISACC::NH:
    O << "NE";
   break;
  case QISACC::GT:
  case QISACC::HI:
    O << "GT";
   break;
  case QISACC::GE:
  case QISACC::HS:
    O << "GE";
   break;
  case QISACC::LT:
  case QISACC::LO:
    O << "LT";
   break;
  case QISACC::LE:
  case QISACC::LS:
    O << "LE";
  break;
  }
}
void QISAInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,
                                raw_ostream &O) {
                                    // Try to print any aliases first.
  if (!printAliasInstr(MI, Address, STI, O))
    printInstruction(MI, Address, STI, O);
  printAnnotation(O, Annot);
}

void QISAInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << StringRef(getRegisterName(RegNo)).upper();
}

// Print a 'memsrc' operand which is a (Register, Offset) pair.
void QISAInstPrinter::printAddrModeMemSrc(const MCInst *MI, unsigned OpNum, const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  const MCOperand &Op1 = MI->getOperand(OpNum);
  const MCOperand &Op2 = MI->getOperand(OpNum + 1);
  
  printRegName(O, Op1.getReg());

  unsigned Offset = Op2.getImm();
  // if (Offset) {
    O << ", 0x";
    O.write_hex(Offset);
  // }
}
