//====-- QASMMCAsmInfo.h - QASM asm properties ---------------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QASM_MCTARGETDESC_QASMMCASMINFO_H
#define LLVM_LIB_TARGET_QASM_MCTARGETDESC_QASMMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class Triple;

class QASMMCAsmInfo : public MCAsmInfoELF {
public:
  explicit QASMMCAsmInfo(const Triple &TT);

    /// Return true if the .section directive should be omitted when
  /// emitting \p SectionName.  For example:
  ///
  /// shouldOmitSectionDirective(".text")
  ///
  /// returns false => .section .text,#alloc,#execinstr
  /// returns true  => .text
  bool shouldOmitSectionDirective(StringRef SectionName) const override;
};

} // end namespace llvm

#endif
