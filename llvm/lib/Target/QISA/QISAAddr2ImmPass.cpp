//===-- QISAAddr2ImmPass.cpp - Convert Address to Imm for QISA
//-----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "addr2imm"

#include "QISA.h"
#include "QISADefine.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "QISAISelLowering.h"

using namespace llvm;

#define DEBUG_TYPE "addr2imm"

namespace {

class QISAAddr2ImmPass : public MachineFunctionPass {
MachineRegisterInfo *MRI = nullptr;
const TargetInstrInfo *TII = nullptr;

public:
  static char ID;
  QISAAddr2ImmPass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

};

char QISAAddr2ImmPass::ID = 1;

} // namespace

bool QISAAddr2ImmPass::runOnMachineFunction(MachineFunction &MF) {

  LLVM_DEBUG(dbgs() << "\n********** Convert Address to Imm  **********\n"
                    << "********** Function: " << MF.getName() << '\n');
  MRI = &MF.getRegInfo(); 
  TII = MF.getSubtarget().getInstrInfo();

  // QCL_FIXME: data qubits should be dynamic
  // std::vector<unsigned> dataQubit = {4, 0, 2};
  std::vector<unsigned> dataQubit = {0, 4, 2};


  for (MachineBasicBlock &MBB : MF) {

   std::vector<MachineInstr*> RemoveMIs;

   for (auto MI = MBB.begin(), ME = MBB.end(); MI != ME; ++MI) {

      if (MI == nullptr) continue;

      unsigned Opcode = MI->getOpcode();
           
      if (Opcode == QISA::QMOVrr ) {

        MachineInstr *PreMF = MI->getPrevNode();
        
        if (!PreMF) continue;
        if (PreMF->getOpcode() != QISA::LDDri) continue;
        
        MachineOperand Op2 = PreMF->getOperand(1);

        if (!Op2.isImm()) continue;

        int Imm = Op2.getImm();
        unsigned PQubitIdx = ((OPERAND_TYPE_IDADDR ^ Imm) / 4) - 1;

        if (PQubitIdx >= dataQubit.size())
            llvm_unreachable("Using qubit over the 3 qubits");

        int64_t PQubit = dataQubit[PQubitIdx];

        if (!MI->getOperand(0).isReg()) continue;
        
        BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::QMOVri), MI->getOperand(0).getReg())
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


FunctionPass *llvm::createAddr2ImmPass() {
  return new QISAAddr2ImmPass();
}

static RegisterPass<QISAAddr2ImmPass> X("addr2imm",
                                                 "Convert Address to Imm");