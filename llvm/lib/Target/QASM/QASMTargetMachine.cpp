//===-- QASMTargetMachine.cpp - Define TargetMachine for QASM ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "QASMTargetMachine.h"
#include "QASM.h"
#include "QASMTargetObjectFile.h"
#include "TargetInfo/QASMTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeQASMTarget() {
  // Register the target.
  RegisterTargetMachine<QASMTargetMachine> X(getTheQASMTarget());
}

namespace {
// TODO: Check.
std::string computeDataLayout(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string Ret;

  // Big endian.
  Ret += "E";

  // Data mangling.
  Ret += DataLayout::getManglingComponent(TT);

  // Pointers are 32 bit.
  Ret += "-p:32:8:32";

  // Make sure that global data has at least 16 bits of alignment by
  // default, so that we can refer to it using LARL.  We don't have any
  // special requirements for stack variables though.
  Ret += "-i1:8:16-i8:8:16";

  // 64-bit integers are naturally aligned.
  Ret += "-i64:64";

  // 128-bit floats are aligned only to 64 bits.
  Ret += "-f128:32";

  // We prefer 16 bits of aligned for all globals; see above.
  Ret += "-a:8:16";

  // Integer registers are 32bits.
  Ret += "-n32";

  return Ret;
}

// TODO: Check.
Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue() || *RM == Reloc::DynamicNoPIC)
    return Reloc::Static;
  return *RM;
}

} // namespace

/// Create an QASM architecture model
QASMTargetMachine::QASMTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     Optional<CodeModel::Model> CM,
                                     CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, FS), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Medium), OL),
      TLOF(std::make_unique<QASMELFTargetObjectFile>()) {
  initAsmInfo();
}

QASMTargetMachine::~QASMTargetMachine() {}

const QASMSubtarget *
QASMTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU = !CPUAttr.hasAttribute(Attribute::None)
                        ? CPUAttr.getValueAsString().str()
                        : TargetCPU;
  std::string FS = !FSAttr.hasAttribute(Attribute::None)
                       ? FSAttr.getValueAsString().str()
                       : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    I = std::make_unique<QASMSubtarget>(TargetTriple, CPU, FS, *this);
  }

  return I.get();
}

namespace {
/// QASM Code Generator Pass Configuration Options.
class QASMPassConfig : public TargetPassConfig {
public:
  QASMPassConfig(QASMTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  QASMTargetMachine &getQASMTargetMachine() const {
    return getTM<QASMTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *QASMTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new QASMPassConfig(*this, PM);
}

bool QASMPassConfig::addInstSelector() {
  addPass(createQASMISelDag(getQASMTargetMachine(), getOptLevel()));
  return false;
}

void QASMPassConfig::addPreEmitPass() {
  // TODO Add pass for div-by-zero check.
  // addPass(createAncillaAllocationPass());
  addPass(createQASMAddr2ImmPass());
  addPass(createDecomposeLogical2NativePass());
  addPass(createQASMLowerMeasPass());
}
