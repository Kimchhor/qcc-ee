//===-- QASMTargetInfo.cpp - QASM target implementation -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/QASMTargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheQASMTarget() {
  static Target TheQASMTarget;
  return TheQASMTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQASMTargetInfo() {
  RegisterTarget<Triple::qasm, /*HasJIT=*/false> X(getTheQASMTarget(), "qasm",
                                                   "QASM Backend", "QASM");
}
