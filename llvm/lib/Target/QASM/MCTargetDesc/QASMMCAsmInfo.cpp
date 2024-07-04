//===-- QASMMCAsmInfo.cpp - QASM asm properties ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QASMMCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"

using namespace llvm;

QASMMCAsmInfo::QASMMCAsmInfo(const Triple &TT) {
  // TODO: Check!
  CodePointerSize = 4;
  CalleeSaveStackSlotSize = 4;
  IsLittleEndian = false;
  UseDotAlignForAlignment = true;
  MinInstAlignment = 4;

  CommentString = "//";
  ZeroDirective = "\t.space\t";
  Data64bitsDirective = "\t.quad\t";
  UsesELFSectionDirectiveForBSS = true;
  SupportsDebugInformation = true;
  PrivateGlobalPrefix = "//";
  PrivateLabelPrefix = "//"; 
  LabelSuffix = ":";
  HasDotTypeDotSizeDirective = false;   //  disable : .size ...
  HasSingleParameterDotFile = false;    //  disable : .file ... 
  HasFunctionAlignment = false;         //  disable : .align ...
  HasIdentDirective = false;            //  disable : .ident ...
  HasGlobalLinkage = false;             //  diable  : .globl ...
  ExceptionsType = ExceptionHandling::None;
}

bool QASMMCAsmInfo::shouldOmitSectionDirective(StringRef SectionName) const {
  return true;
}