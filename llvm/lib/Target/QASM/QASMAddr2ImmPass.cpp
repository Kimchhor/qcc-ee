//===-- QASMAddr2ImmPass.cpp - Convert Address to Imm for QASM
//-----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "addr2imm-qasm"

#include "QASM.h"
#include "QASMDefine.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "QASMISelLowering.h"

using namespace llvm;

#define DEBUG_TYPE "addr2imm-qasm"

namespace {

class QASMAddr2ImmPass : public MachineFunctionPass {
MachineRegisterInfo *MRI = nullptr;
const TargetInstrInfo *TII = nullptr;

public:
  static char ID;
  QASMAddr2ImmPass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

};

char QASMAddr2ImmPass::ID = 1;

} // namespace

bool QASMAddr2ImmPass::runOnMachineFunction(MachineFunction &MF) {

  LLVM_DEBUG(dbgs() << "\n********** Convert Address to Imm  **********\n"
                    << "********** Function: " << MF.getName() << '\n');
  MRI = &MF.getRegInfo(); 
  TII = MF.getSubtarget().getInstrInfo();

  for (MachineBasicBlock &MBB : MF) {

   std::vector<MachineInstr*> RemoveMIs;

   for (auto MI = MBB.begin(), ME = MBB.end(); MI != ME; ++MI) {

      if (MI == nullptr) continue;

      unsigned Opcode = MI->getOpcode();
      
      if (Opcode == QASM::QMOVrr ) {

        MachineInstr *PreMF = MI->getPrevNode();
        
        if (!PreMF) continue;
        if (PreMF->getOpcode() != QASM::LDDri) continue;
        
        MachineOperand Op2 = PreMF->getOperand(1);

        if (!Op2.isImm()) continue;

        int Imm = Op2.getImm();
        unsigned PQubitIdx = ((OPERAND_TYPE_IDADDR ^ Imm) / 4) - 1;

        int64_t PQubit = PQubitIdx;

        if (!MI->getOperand(0).isReg()) continue;
        
        BuildMI(MBB, MI, DebugLoc(), TII->get(QASM::QMOVri), MI->getOperand(0).getReg())
            .addImm(PQubit);

        MachineInstr &MIT = *MI;
        MachineInstr *PMIT = PreMF;

       RemoveMIs.push_back(&MIT);
       RemoveMIs.push_back(PMIT);
      }


    }

    for (auto MI : RemoveMIs)
        MI->removeFromParent();

  }

  return false;
}


FunctionPass *llvm::createQASMAddr2ImmPass() {
  return new QASMAddr2ImmPass();
}

static RegisterPass<QASMAddr2ImmPass> X("addr2imm-qasm",
                                                 "Convert Address to Imm");