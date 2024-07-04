//===-- QISATargetInfo.cpp - QISA target implementation -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/QISATargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheQISATarget() {
  static Target TheQISATarget;
  return TheQISATarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQISATargetInfo() {
  RegisterTarget<Triple::qisa, /*HasJIT=*/false> X(getTheQISATarget(), "qisa",
                                                   "eQISA Quantum OS v0.42", "QISA");
}
