//===-- QASMMCAsmBackend.cpp - QASM assembler backend ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//#include "MCTargetDesc/QASMMCFixups.h"
#include "MCTargetDesc/QASMMCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"

using namespace llvm;

namespace {
class QASMMCAsmBackend : public MCAsmBackend {
  uint8_t OSABI;

public:
  QASMMCAsmBackend(uint8_t osABI) : MCAsmBackend(support::big), OSABI(osABI) {}

  // Override MCAsmBackend
  unsigned getNumFixupKinds() const override {
    return 0; // QASM::NumTargetFixupKinds;
  }
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;
  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;
  bool mayNeedRelaxation(const MCInst &Inst,
                         const MCSubtargetInfo &STI) const override {
    return false;
  }
  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *Fragment,
                            const MCAsmLayout &Layout) const override {
    return false;
  }
  bool writeNopData(raw_ostream &OS, uint64_t Count) const override;
  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    return createQASMObjectWriter(OSABI);
  }
};
} // end anonymous namespace

const MCFixupKindInfo &
QASMMCAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  return MCAsmBackend::getFixupKindInfo(Kind);
}

void QASMMCAsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                  const MCValue &Target,
                                  MutableArrayRef<char> Data, uint64_t Value,
                                  bool IsResolved,
                                  const MCSubtargetInfo *STI) const {}

bool QASMMCAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count) const {
  for (uint64_t I = 0; I != Count; ++I)
    OS << '\x7';
  return true;
}

MCAsmBackend *llvm::createQASMMCAsmBackend(const Target &T,
                                           const MCSubtargetInfo &STI,
                                           const MCRegisterInfo &MRI,
                                           const MCTargetOptions &Options) {
  uint8_t OSABI =
      MCELFObjectTargetWriter::getOSABI(STI.getTargetTriple().getOS());
  return new QASMMCAsmBackend(OSABI);
}