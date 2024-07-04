//===- QASMInstPrinter.cpp - Convert QASM MCInst to assembly syntax -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QASMInstPrinter.h"
#include "QASMInstrInfo.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/ADT/StringExtras.h"
#include "QASMCondCode.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "QASMGenAsmWriter.inc"

void QASMInstPrinter::printOperand(const MCInst *MI, int OpNum,
                                   const MCSubtargetInfo &STI, raw_ostream &O) {
    const MCOperand &MO = MI->getOperand(OpNum);
    if (MO.isReg()) {
      if (!MO.getReg())
          O << '0';
      else
          // O << getRegisterName(MO.getReg());
          O << MO.getReg();
    }else if (MO.isImm()){
      // O << MO.getImm();
      O << "0x";
      O.write_hex(MO.getImm());
    } else if (MO.isSFPImm()) {
      APFloat FVal(APFloat::IEEEsingle(), APInt(32, MO.getSFPImm()));
      bool unused;
      FVal.convert(APFloat::IEEEdouble(), llvm::APFloat::rmNearestTiesToEven, &unused);
      SmallVector<char, 32> Buffer;
      FVal.toString(Buffer);
      O << Buffer;
    } 
    else if (MO.isDFPImm()) {
      APFloat FVal(APFloat::IEEEdouble(), APInt(64, MO.getDFPImm()));
      bool unused;
      FVal.convert(APFloat::IEEEdouble(), llvm::APFloat::rmNearestTiesToEven, &unused);
      SmallVector<char, 64> Buffer;
      FVal.toString(Buffer);
      O << Buffer;
    } 
    else if (MO.isExpr()) {
        MO.getExpr()->print(O, &MAI);
    } else {
        llvm_unreachable("Invalid operand");
    }
}

void QASMInstPrinter::printOperand(const MCOperand &MO, const MCAsmInfo *MAI,
                                   raw_ostream &O) {
  if (MO.isReg()) {
    if (!MO.getReg())
      O << '0';
    else
      O << getRegisterName(MO.getReg());
  } else if (MO.isImm()) {
    // O << MO.getImm();
    // O << "0x";
    O.write_hex(MO.getImm());
  }
  else if (MO.isExpr())
    MO.getExpr()->print(O, MAI);
  else
    llvm_unreachable("Invalid operand");
}

void QASMInstPrinter::printU5ImmOperand(const MCInst *MI, int OpNum,
                                        const MCSubtargetInfo &STI,
                                        raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  O << Value;
}

void QASMInstPrinter::printU5ImmOOperand(const MCInst *MI, int OpNum,
                                         const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  O << "<" << Value << ">";
}

void QASMInstPrinter::printU10ImmWOOperand(const MCInst *MI, int OpNum,
                                           const MCSubtargetInfo &STI,
                                           raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  int64_t Width = (Value >> 5) & 0x1f;
  int64_t Offset = Value & 0x1f;
  O << Width << "<" << Offset << ">";
}

void QASMInstPrinter::printU16ImmOperand(const MCInst *MI, int OpNum,
                                         const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  O << Value; // 1remove hex print
}


void QASMInstPrinter::printU8ImmOperand(const MCInst *MI, int OpNum,
                                         const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  int64_t Value = MI->getOperand(OpNum).getImm();
  // assert(isUInt<N>(Value) && "Invalid uimm argument");
  O << Value;
}

void QASMInstPrinter::printPCRelOperand(const MCInst *MI, uint64_t Address,
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

void QASMInstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                      const MCSubtargetInfo &STI,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case QASMCC::EQ:
  case QASMCC::BE:
  case QASMCC::HE:
   O << "EQ";
   break;
  case QASMCC::NE:
  case QASMCC::NB:
  case QASMCC::NH:
    O << "NE";
   break;
  case QASMCC::GT:
  case QASMCC::HI:
    O << "GT";
   break;
  case QASMCC::GE:
  case QASMCC::HS:
    O << "GE";
   break;
  case QASMCC::LT:
  case QASMCC::LO:
    O << "LT";
   break;
  case QASMCC::LE:
  case QASMCC::LS:
    O << "LE";
  break;
  }

}
void QASMInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,
                                raw_ostream &O) {
                                    // Try to print any aliases first.
  if (!printAliasInstr(MI, Address, STI, O))
    printInstruction(MI, Address, STI, O);
  printAnnotation(O, Annot);
}

void QASMInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << (RegNo - QASM::QR0);
}

// Print a 'memsrc' operand which is a (Register, Offset) pair.
void QASMInstPrinter::printAddrModeMemSrc(const MCInst *MI, unsigned OpNum, const MCSubtargetInfo &STI,
                                         raw_ostream &O) {
  const MCOperand &Op1 = MI->getOperand(OpNum);
  // const MCOperand &Op2 = MI->getOperand(OpNum + 1);
  
  // printRegName(O, Op1.getReg());
  unsigned Offset;
  if (Op1.isImm())
    Offset = Op1.getImm();
  else if (Op1.isReg())
    Offset = Op1.getReg();
  // if (Offset) {
    // O << ", 0x";
    O << (Offset);
  // }
}
