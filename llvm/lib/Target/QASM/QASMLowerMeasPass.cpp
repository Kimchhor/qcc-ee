//===-- QASMLowerMeasPass.cpp - Convert Address to Imm for QASM
//-----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "LowerMeasPass"

#include "QASM.h"
#include "QASMDefine.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "QASMISelLowering.h"

using namespace llvm;

#define DEBUG_TYPE "LowerMeasPass"

namespace {

class QASMLowerMeasPass : public MachineFunctionPass {
MachineRegisterInfo *MRI = nullptr;
const TargetInstrInfo *TII = nullptr;

public:
  static char ID;
  QASMLowerMeasPass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

};

char QASMLowerMeasPass::ID = 1;

} // namespace

bool QASMLowerMeasPass::runOnMachineFunction(MachineFunction &MF) {

  LLVM_DEBUG(dbgs() << "\n********** Convert Address to Imm  **********\n"
                    << "********** Function: " << MF.getName() << '\n');
  MRI = &MF.getRegInfo(); 
  TII = MF.getSubtarget().getInstrInfo();

  for (MachineBasicBlock &MBB : MF) {

   std::vector<MachineInstr*> RemoveMIs;

   for (auto MI = MBB.begin(), ME = MBB.end(); MI != ME; ++MI) {

      if (MI == nullptr) continue;

      unsigned Opcode = MI->getOpcode();
     
      if (Opcode == QASM::MEASURE_Z_GATErr || Opcode == QASM::MEASURE_Z_GATEri ) {

        MachineInstr *NextMI = MI->getNextNode();
        if (!NextMI) continue;
        if (NextMI->getOpcode() != QASM::STDri) continue;

        MachineOperand Op2 = NextMI->getOperand(1);

        int Imm = Op2.getImm();
        unsigned PBitIdx = ((OPERAND_TYPE_UDADDR ^ Imm) / 4) - 1;

        const MCInstrDesc &Desc = TII->get(QASM::MEASURE_Z_GATEaddr);

        BuildMI(MBB, MI, DebugLoc(), Desc).addReg(MI->getOperand(1).getReg()).addImm(PBitIdx);
        
        MachineInstr &MIT = *MI;

       RemoveMIs.push_back(&MIT);

      }

    }

    for (auto MI : RemoveMIs)
        MI->removeFromParent();

  }

  return false;
}


FunctionPass *llvm::createQASMLowerMeasPass() {
  return new QASMLowerMeasPass();
}

static RegisterPass<QASMLowerMeasPass> X("LowerMeasPass",
                                                 "Convert Address to Imm");