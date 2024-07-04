//===-- QASMMCTargetDesc.cpp - QASM target descriptions -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QASMMCTargetDesc.h"
#include "QASMInstPrinter.h"
#include "QASMMCAsmInfo.h"
#include "QASMTargetStreamer.h"

#include "TargetInfo/QASMTargetInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "QASMGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "QASMGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "QASMGenRegisterInfo.inc"

static MCAsmInfo *createQASMMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new QASMMCAsmInfo(TT);
  return MAI;
}

static MCInstrInfo *createQASMMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitQASMMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createQASMMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitQASMMCRegisterInfo(X, QASM::R31);  // Return address Register
  InitQASMMCRegisterInfo(X, QASM::QR31); // Return address Register
  return X;
}

static MCSubtargetInfo *createQASMMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  return createQASMMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstPrinter *createQASMMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new QASMInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *createQSIAAsmTargetStreamer(MCStreamer &S,
                                                      formatted_raw_ostream &OS,
                                                      MCInstPrinter *InstPrint,
                                                      bool isVerboseAsm) {
  return new QASMTargetAsmStreamer(S, OS);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQASMTargetMC() {
  // Register the MCAsmInfo.
  TargetRegistry::RegisterMCAsmInfo(getTheQASMTarget(), createQASMMCAsmInfo);

  // Register the MCCodeEmitter.
  TargetRegistry::RegisterMCCodeEmitter(getTheQASMTarget(),
                                        createQASMMCCodeEmitter);

  // Register the MCInstrInfo.
  TargetRegistry::RegisterMCInstrInfo(getTheQASMTarget(),
                                      createQASMMCInstrInfo);
  // Register the MCRegisterInfo.
  TargetRegistry::RegisterMCRegInfo(getTheQASMTarget(),
                                    createQASMMCRegisterInfo);

  // Register the MCSubtargetInfo.
  TargetRegistry::RegisterMCSubtargetInfo(getTheQASMTarget(),
                                          createQASMMCSubtargetInfo);
  // Register the MCAsmBackend.
  TargetRegistry::RegisterMCAsmBackend(getTheQASMTarget(),
                                       createQASMMCAsmBackend);
  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getTheQASMTarget(),
                                        createQASMMCInstPrinter);
  // Register the QSIAAsmTargetStreamer
  TargetRegistry::RegisterAsmTargetStreamer(getTheQASMTarget(),
                                            createQSIAAsmTargetStreamer);
}
