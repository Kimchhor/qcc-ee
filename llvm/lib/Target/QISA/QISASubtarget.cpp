//===-- QISASubtarget.cpp - QISA Subtarget Information ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the QISA specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "QISASubtarget.h"
#include "QISA.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "qisa-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "QISAGenSubtargetInfo.inc"

void QISASubtarget::anchor() {}

QISASubtarget::QISASubtarget(const Triple &TT, const std::string &CPU,
                             const std::string &FS, const TargetMachine &TM)
    : QISAGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS), TargetTriple(TT),
      InstrInfo(*this), TLInfo(TM, *this), FrameLowering() {}
