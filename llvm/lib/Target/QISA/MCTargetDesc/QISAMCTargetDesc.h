//===-- QISAMCTargetDesc.h - QISA target descriptions -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QISA_MCTARGETDESC_QISAMCTARGETDESC_H
#define LLVM_LIB_TARGET_QISA_MCTARGETDESC_QISAMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {

class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class StringRef;
class Target;
class Triple;
class raw_pwrite_stream;
class raw_ostream;

MCCodeEmitter *createQISAMCCodeEmitter(const MCInstrInfo &MCII,
                                       const MCRegisterInfo &MRI,
                                       MCContext &Ctx);

MCAsmBackend *createQISAMCAsmBackend(const Target &T,
                                     const MCSubtargetInfo &STI,
                                     const MCRegisterInfo &MRI,
                                     const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter> createQISAObjectWriter(uint8_t OSABI);
} // end namespace llvm

// Defines symbolic names for QISA registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "QISAGenRegisterInfo.inc"

// Defines symbolic names for the QISA instructions.
#define GET_INSTRINFO_ENUM
#include "QISAGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "QISAGenSubtargetInfo.inc"

#endif
