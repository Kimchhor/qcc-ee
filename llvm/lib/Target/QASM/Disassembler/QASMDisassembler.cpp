//===-- QASMDisassembler.cpp - Disassembler for QASM ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QASM.h"
#include "MCTargetDesc/QASMMCTargetDesc.h"
#include "TargetInfo/QASMTargetInfo.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCFixedLenDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "qasm-disassembler"

using DecodeStatus = MCDisassembler::DecodeStatus;

namespace {

class QASMDisassembler : public MCDisassembler {
public:
  QASMDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}
  ~QASMDisassembler() override = default;

  DecodeStatus getInstruction(MCInst &instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};

} // end anonymous namespace

static MCDisassembler *createQASMDisassembler(const Target &T,
                                              const MCSubtargetInfo &STI,
                                              MCContext &Ctx) {
  return new QASMDisassembler(STI, Ctx);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQASMDisassembler() {
  // Register the disassembler.
  TargetRegistry::RegisterMCDisassembler(getTheQASMTarget(),
                                         createQASMDisassembler);
}

static const uint16_t GPRDecoderTable[] = {
    QASM::R0,  QASM::R1,  QASM::R2,  QASM::R3,  QASM::R4,  QASM::R5,
    QASM::R6,  QASM::R7,  QASM::R8,  QASM::R9,  QASM::R10, QASM::R11,
    QASM::R12, QASM::R13, QASM::R14, QASM::R15,

    QASM::R16, QASM::R17, QASM::R18, QASM::R19, QASM::R20, QASM::R21,
    QASM::R22, QASM::R23, QASM::R24, QASM::R25, QASM::R26, QASM::R27,
    QASM::R28, QASM::R29, QASM::R30, QASM::R31,
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

#include "QASMGenDisassemblerTables.inc"

DecodeStatus QASMDisassembler::getInstruction(MCInst &MI, uint64_t &Size,
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

  return decodeInstruction(DecoderTableQASM32, MI, Inst, Address, this, STI);
}
