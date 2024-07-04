//===-- QASM.h - Top-level interface for QASM representation ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// QASM back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QASM_QASM_H
#define LLVM_LIB_TARGET_QASM_QASM_H

#include "MCTargetDesc/QASMMCTargetDesc.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class QASMTargetMachine;
class FunctionPass;

FunctionPass *createQASMISelDag(QASMTargetMachine &TM,
                                CodeGenOpt::Level OptLevel);


FunctionPass *createAncillaAllocationPass();
FunctionPass *createQASMAddr2ImmPass();
FunctionPass *createQASMLowerMeasPass();
FunctionPass *createDecomposeLogical2NativePass();

}// end namespace llvm
#endif
