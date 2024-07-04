//===-- QASMAddr2NativePass.cpp - Convert Address to Imm for QASM
//-----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <iostream>

#define DEBUG_TYPE "decompose2native"

#include "QASM.h"
#include "QASMDefine.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "QASMISelLowering.h"

#include "llvm/Support/CommandLine.h"

using namespace llvm;

static cl::opt<std::string> TargetDevice(
    "target-device",
    cl::desc("Target device for Decomposition to Native"),
    cl::init("zx"),
    cl::Required
);

namespace {

class DecomposeLogical2NativePass : public MachineFunctionPass {
MachineRegisterInfo *MRI = nullptr;
const TargetInstrInfo *TII = nullptr;

public:
  static char ID;
  DecomposeLogical2NativePass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;
};

char DecomposeLogical2NativePass::ID = 2;
}

bool DecomposeLogical2NativePass::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << "\n********** Convert Logical to Native  **********\n");
  MRI = &MF.getRegInfo(); 
  TII = MF.getSubtarget().getInstrInfo();

  

  if (TargetDevice == "zx") {
    errs() << "Decompose Logical to Native for ZX\n";
    errs() << MF.getSubtarget().getInstrInfo();
    
  } else if (TargetDevice == "ibm") {
    errs() << "Decompose Logical to Native for IBM\n";
  } else {
    errs() << "Unknown target device\n";
  }
  return false;
}

FunctionPass *llvm::createDecomposeLogical2NativePass() {
  return new DecomposeLogical2NativePass();
}

static RegisterPass<DecomposeLogical2NativePass> X("decompose2native",
                                                 "Convert Logical to Native");