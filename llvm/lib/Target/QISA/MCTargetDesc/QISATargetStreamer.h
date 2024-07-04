//===- QISATargetStreamer.h - QISA Target Streamer ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_POWERPC_QISATARGETSTREAMER_H
#define LLVM_LIB_TARGET_POWERPC_QISATARGETSTREAMER_H

#include "QISADirectives.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCStreamer.h"

namespace llvm {

class MCExpr;
class MCSymbol;
class MCSymbolELF;
class formatted_raw_ostream;

class QISATargetStreamer : public MCTargetStreamer {
public:

  QISATargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

  virtual void emitISAVersion() = 0;
  virtual void emitHeaderInfo(unsigned base, unsigned size, StringRef type) = 0;\
  virtual void emitLargeCommand(StringRef Text) = 0;
  virtual void emitSymbol(StringRef Text, QISASymbolAttr Attribute = MCSA_OTHER) = 0;

  virtual void emitKernelHeader(std::vector<KernelHeader>) = 0;
  virtual void emitSymbolTable(SymTable ST) = 0;
  virtual void emitDirectiveQmapArea(QMap QM) = 0;
  virtual void emitDirectiveDataArea(std::vector<DataArea> DA) = 0;
};


class QISATargetAsmStreamer final : public QISATargetStreamer {
  formatted_raw_ostream &OS;
public:
  QISATargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);

  void finish() override;

  void emitISAVersion() override;
  void emitHeaderInfo(unsigned base, unsigned size, StringRef type) override;
  void emitLargeCommand(StringRef Text) override;
  void emitSymbol(StringRef Text, QISASymbolAttr Attribute) override;

  void emitKernelHeader(std::vector<KernelHeader>) override; 
  void emitSymbolTable(SymTable ST) override;
  void emitDirectiveQmapArea(QMap QM) override;
  void emitDirectiveDataArea(std::vector<DataArea> DA) override;
};


// class QISATargetELFStreamer final : public QISATargetStreamer {

// };

} // end namespace llvm

#endif // LLVM_LIB_TARGET_POWERPC_QISATARGETSTREAMER_H
