//===-- QISAMCTargetDesc.cpp - QISA target descriptions -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QISAMCTargetDesc.h"
#include "QISAInstPrinter.h"
#include "QISAMCAsmInfo.h"
#include "QISATargetStreamer.h"

#include "TargetInfo/QISATargetInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "QISAGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "QISAGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "QISAGenRegisterInfo.inc"

static MCAsmInfo *createQISAMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new QISAMCAsmInfo(TT);
  return MAI;
}

static MCInstrInfo *createQISAMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitQISAMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createQISAMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitQISAMCRegisterInfo(X, QISA::R31);  // Return address Register
  InitQISAMCRegisterInfo(X, QISA::QR31); // Return address Register
  return X;
}

static MCSubtargetInfo *createQISAMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  return createQISAMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstPrinter *createQISAMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new QISAInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *createQSIAAsmTargetStreamer(MCStreamer &S,
                                                      formatted_raw_ostream &OS,
                                                      MCInstPrinter *InstPrint,
                                                      bool isVerboseAsm) {
  return new QISATargetAsmStreamer(S, OS);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQISATargetMC() {
  // Register the MCAsmInfo.
  TargetRegistry::RegisterMCAsmInfo(getTheQISATarget(), createQISAMCAsmInfo);

  // Register the MCCodeEmitter.
  TargetRegistry::RegisterMCCodeEmitter(getTheQISATarget(),
                                        createQISAMCCodeEmitter);

  // Register the MCInstrInfo.
  TargetRegistry::RegisterMCInstrInfo(getTheQISATarget(),
                                      createQISAMCInstrInfo);
  // Register the MCRegisterInfo.
  TargetRegistry::RegisterMCRegInfo(getTheQISATarget(),
                                    createQISAMCRegisterInfo);

  // Register the MCSubtargetInfo.
  TargetRegistry::RegisterMCSubtargetInfo(getTheQISATarget(),
                                          createQISAMCSubtargetInfo);
  // Register the MCAsmBackend.
  TargetRegistry::RegisterMCAsmBackend(getTheQISATarget(),
                                       createQISAMCAsmBackend);
  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getTheQISATarget(),
                                        createQISAMCInstPrinter);
  // Register the QSIAAsmTargetStreamer
  TargetRegistry::RegisterAsmTargetStreamer(getTheQISATarget(),
                                            createQSIAAsmTargetStreamer);
}
