//===-- QISADisassembler.cpp - Disassembler for QISA ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QISA.h"
#include "MCTargetDesc/QISAMCTargetDesc.h"
#include "TargetInfo/QISATargetInfo.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCFixedLenDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "qisa-disassembler"

using DecodeStatus = MCDisassembler::DecodeStatus;

namespace {

class QISADisassembler : public MCDisassembler {
public:
  QISADisassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}
  ~QISADisassembler() override = default;

  DecodeStatus getInstruction(MCInst &instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};

} // end anonymous namespace

static MCDisassembler *createQISADisassembler(const Target &T,
                                              const MCSubtargetInfo &STI,
                                              MCContext &Ctx) {
  return new QISADisassembler(STI, Ctx);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQISADisassembler() {
  // Register the disassembler.
  TargetRegistry::RegisterMCDisassembler(getTheQISATarget(),
                                         createQISADisassembler);
}

static const uint16_t GPRDecoderTable[] = {
    QISA::R0,  QISA::R1,  QISA::R2,  QISA::R3,  QISA::R4,  QISA::R5,
    QISA::R6,  QISA::R7,  QISA::R8,  QISA::R9,  QISA::R10, QISA::R11,
    QISA::R12, QISA::R13, QISA::R14, QISA::R15,

    QISA::R16, QISA::R17, QISA::R18, QISA::R19, QISA::R20, QISA::R21,
    QISA::R22, QISA::R23, QISA::R24, QISA::R25, QISA::R26, QISA::R27,
    QISA::R28, QISA::R29, QISA::R30, QISA::R31,
};

static DecodeStatus DecodeGPRRegisterClass(MCInst &Inst, uint64_t RegNo,
                                           uint64_t Address,
                                           const void *Decoder) {
  if (RegNo > 31)
    return MCDisassembler::Fail;

  unsigned Register = GPRDecoderTable[RegNo];
  Inst.addOperand(MCOperand::createReg(Register));
  return MCDisassembler::Success;
}

template <unsigned N>
static DecodeStatus decodeUImmOperand(MCInst &Inst, uint64_t Imm) {
  if (!isUInt<N>(Imm))
    return MCDisassembler::Fail;
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

static DecodeStatus decodeU8ImmOperand(MCInst &Inst, uint64_t Imm,
                                        uint64_t Address, const void *Decoder) {
  return decodeUImmOperand<8>(Inst, Imm);
} 
static DecodeStatus decodeU5ImmOOperand(MCInst &Inst, uint64_t Imm,
                                        uint64_t Address, const void *Decoder) {
  return decodeUImmOperand<5>(Inst, Imm);
}

static DecodeStatus decodeU10ImmWOOperand(MCInst &Inst, uint64_t Imm,
                                          uint64_t Address,
                                          const void *Decoder) {
  return decodeUImmOperand<10>(Inst, Imm);
}

static DecodeStatus decodeU16ImmOperand(MCInst &Inst, uint64_t Imm,
                                        uint64_t Address, const void *Decoder) {
  return decodeUImmOperand<16>(Inst, Imm);
}

static DecodeStatus decodePC26BranchOperand(MCInst &Inst, uint64_t Imm,
                                            uint64_t Address,
                                            const void *Decoder) {
  if (!isUInt<26>(Imm))
    return MCDisassembler::Fail;
  Inst.addOperand(MCOperand::createImm(SignExtend64<26>(Imm)));
  return MCDisassembler::Success;
}

#include "QISAGenDisassemblerTables.inc"

DecodeStatus QISADisassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                              ArrayRef<uint8_t> Bytes,
                                              uint64_t Address,
                                              raw_ostream &CS) const {
  // Instruction size is always 32 bit.
  if (Bytes.size() < 4) {
    Size = 0;
    return MCDisassembler::Fail;
  }
  Size = 4;

  // Construct the instruction.
  uint32_t Inst = 0;
  for (uint32_t I = 0; I < Size; ++I)
    Inst = (Inst << 8) | Bytes[I];

  return decodeInstruction(DecoderTableQISA32, MI, Inst, Address, this, STI);
}
