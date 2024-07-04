//===-- QISA.h - Top-level interface for QISA representation ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// QISA back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QISA_QISA_H
#define LLVM_LIB_TARGET_QISA_QISA_H

#include "MCTargetDesc/QISAMCTargetDesc.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class QISATargetMachine;
class FunctionPass;

FunctionPass *createQISAISelDag(QISATargetMachine &TM,
                                CodeGenOpt::Level OptLevel);


FunctionPass *createAncillaAllocationPass();
FunctionPass *createAddr2ImmPass();

}// end namespace llvm
#endif
