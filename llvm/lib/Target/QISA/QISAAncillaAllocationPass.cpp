//===-- QISAAncillaAllocationPass.cpp - Ancilla Allocation for QISA
//-----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "ancilla-alloc"

#include "QISA.h"
#include "QISADefine.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "QISAISelLowering.h"

using namespace llvm;

#define DEBUG_TYPE "ancilla-alloc"

namespace {

class QISAAncillaAllocationPass : public MachineFunctionPass {
MachineRegisterInfo *MRI = nullptr;
const TargetInstrInfo *TII = nullptr;
std::vector<MachineInstr*> removedInstrs;

public:
  static char ID;
  QISAAncillaAllocationPass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  void handleHGateAncilla(MachineInstr &MI);
  void handleSWAPGateAncilla(MachineInstr &MI);
  void expandCNOTGate(MachineInstr &MI);

  unsigned selectHGateAncilla(MachineInstr &MI);

};

char QISAAncillaAllocationPass::ID = 0;

} // namespace

bool QISAAncillaAllocationPass::runOnMachineFunction(MachineFunction &MF) {

  LLVM_DEBUG(dbgs() << "********** Ancilla Allocation  **********\n"
                    << "********** Function: " << MF.getName() << '\n');
  MRI = &MF.getRegInfo(); 
  TII = MF.getSubtarget().getInstrInfo();

  for (MachineBasicBlock &MBB : MF) {
   for (auto MI = MBB.begin(), ME = MBB.end(); MI != ME; ++MI) {
      if (MI->isDebugValue())
            continue;
      unsigned Opcode = MI->getOpcode();
      switch (Opcode) {
      case QISA::H_GATEri:
      case QISA::H_GATErr:
      // case QISA::FLIP_EXPANDrr:
        handleHGateAncilla(*MI);
        break;
      case QISA::CNOT_POST_MXXrr:
        expandCNOTGate(*MI);
        break;
      case QISA::SWAP_GATErr:
        handleSWAPGateAncilla(*MI);
        break;
      default:
        break;
      }
    }
  }

  for (auto MI : removedInstrs)
    MI->removeFromParent();

  return false;
}

unsigned QISAAncillaAllocationPass::selectHGateAncilla(MachineInstr &MI) {
  
  // QCL_FIXME: data and ancilla qubits should be dynamic

  std::vector<unsigned> dataQubit = {0, 4, 2};
  
  //support only the right 2 qubits  
  std::vector<unsigned> ancillaQubit = {1, 5, 1};
  

  MachineInstr * PMI = MI.getPrevNode();

  Register reg = MI.getOperand(0).getReg();

  jump:

  if (!PMI) return 1;

  if (PMI->getOperand(0).getReg() != reg) {
        PMI = PMI->getPrevNode();
        goto jump;
  }

  switch (PMI->getOpcode())
  {
  case QISA::QMOVri:{
    MachineOperand Op1 = PMI->getOperand(1);
    if (Op1.isImm()){
      int64_t PQubit = Op1.getImm();
      
      if (PQubit > dataQubit.size())
        llvm_unreachable("Using qubit over the 3 qubits");
      
      /// H gate apply on next right data qubit
      int64_t AQubit = dataQubit[PQubit] + 1;

      // check if Ancilla qubit is supported
      if (std::find(ancillaQubit.begin(),ancillaQubit.end(),AQubit) != ancillaQubit.end()) {
       // supported 
       return AQubit; 
      }else{
        llvm_unreachable("Cannot allocate aniclla for H gate");
      }
      errs() << "";
    }
    break;
  }
  case QISA::QMOVrr:{
    auto *LD = PMI->getPrevNode();
    if (!LD)
      llvm_unreachable("Fail H gate ancilla selection");
    
    MachineOperand Op1 = LD->getOperand(1);
    if (Op1.isImm()){
      int64_t addr = Op1.getImm();
      unsigned PQubit = ((OPERAND_TYPE_IDADDR ^ addr) / 4)-1;
      
      if (PQubit > dataQubit.size())
        llvm_unreachable("Using qubit over the 3 qubits");
      
      /// H gate apply on next right data qubit
      int64_t AQubit = dataQubit[PQubit] + 1;

      // check if Ancilla qubit is supported
      if (std::find(ancillaQubit.begin(),ancillaQubit.end(),AQubit) != ancillaQubit.end()) {
       // supported 
       return AQubit; 
      }else{
        llvm_unreachable("Cannot allocate aniclla for H gate");
      }
      errs() << "";
    }
    break;
  }
  default:
    PMI = PMI->getPrevNode();
    goto jump;
    break;
  }
  
  
  return 3;
}

void QISAAncillaAllocationPass::handleSWAPGateAncilla(MachineInstr &MI) {

  MachineOperand Op1 = MI.getOperand(0);
  MachineOperand Op2 = MI.getOperand(1);

  MachineBasicBlock &MBB = *MI.getParent();

  // ILQ1
  // ILQ2
  Register QR1 = Op1.getReg();
  Register QR2 = Op2.getReg();
  Register ILQ1 = QISA::QR11; // 1
  Register ILQ2 = QISA::QR12; // 3

  // QMOV	QR11, 0x1
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::QMOVri), ILQ1).addImm(1);

  // QMOV	QR12, 0x3
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::QMOVri), ILQ2).addImm(3);

  // INIT_Z(ILQ1);
  // MERGE_MXX(ILQ1, QR1);
	// SPLIT_MXX(ILQ1, QR1);
  // MOVE_POST_MXX(ILQ1, QR1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::INIT_Z_GATErr), ILQ1).addReg(ILQ1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MERGE_MXXrr), ILQ1).addReg(ILQ1).addReg(ILQ1).addReg(QR1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::SPLIT_MXXrr), ILQ1).addReg(ILQ1).addReg(ILQ1).addReg(QR1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MOVE_POST_MXXrr), ILQ1).addReg(ILQ1).addReg(ILQ1).addReg(QR1);

  // INIT_Z(ILQ2);
	// MERGE_MXX(ILQ2, QR2);
	// SPLIT_MXX(ILQ2, QR2);
	// MOVE_POST_MXX(ILQ2, QR2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::INIT_Z_GATErr), ILQ2).addReg(ILQ2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MERGE_MXXrr), ILQ2).addReg(ILQ2).addReg(ILQ2).addReg(QR2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::SPLIT_MXXrr), ILQ2).addReg(ILQ2).addReg(ILQ2).addReg(QR2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MOVE_POST_MXXrr), ILQ2).addReg(ILQ2).addReg(ILQ2).addReg(QR2);

	// // INIT_X(QR1);
	// // MERGE_MZZ(QR1, ILQ2);
	// // SPLIT_MZZ(QR1, ILQ2);
	// // MOVE_POST_MZZ(QR1, ILQ2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::INIT_X_GATErr), QR1).addReg(QR1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MERGE_MZZrr), QR1).addReg(QR1).addReg(QR1).addReg(ILQ2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::SPLIT_MZZrr), QR1).addReg(QR1).addReg(QR1).addReg(ILQ2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MOVE_POST_MZZrr), QR1).addReg(QR1).addReg(QR1).addReg(ILQ2);

	// // INIT_X(QR2);
	// // MERGE_MZZ(QR2, ILQ1);
	// // SPLIT_MZZ(QR2, ILQ1);
	// // MOVE_POST_MZZ(QR2, ILQ1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::INIT_X_GATErr), QR2).addReg(QR2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MERGE_MZZrr), QR2).addReg(QR2).addReg(QR2).addReg(ILQ1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::SPLIT_MZZrr), QR2).addReg(QR2).addReg(QR2).addReg(ILQ1);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MOVE_POST_MZZrr), QR2).addReg(QR2).addReg(QR2).addReg(ILQ1);


  removedInstrs.push_back(&MI);
}

void QISAAncillaAllocationPass::handleHGateAncilla(MachineInstr &MI){
	
  
  // QMOV	QR10, 0x3
	// HADAMARD	QR0               <- MI
	// FLIP_EXPAND	QR10, QR0
	// FLIP_CONTRACT	QR0, QR10
	// FLIP_SHIFT	QR0, QR10

  MachineOperand Op1 = MI.getOperand(0);

  if (!Op1.isReg())
    return;

  // Select H gate ancilla
  int64_t AncillaQubit = selectHGateAncilla(MI);

  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction *MF = MBB.getParent();

  Register QR1 = Op1.getReg();
  Register QRT = QISA::QR10;

  // QMOV	QRT, 0xc
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::QMOVri), QRT)
      .addImm(AncillaQubit);

  // FLIP_SHIFT QR1, QRT
  MachineInstr *FShift = MBB.getParent()->CreateMachineInstr(TII->get(QISA::FLIP_SHIFTrr), DebugLoc());
  MachineInstrBuilder MIB(*MF, FShift);
  MIB.addReg(QRT).addReg(QRT).addReg(QR1);

  MachineBasicBlock::iterator I = MI;
  MBB.insertAfter(I, FShift);

  // FLIP_CONTRACT	QR1, QRT
  MachineInstrBuilder FContract = BuildMI(MBB, FShift, DebugLoc(), TII->get(QISA::FLIP_CONTRACTrr), QRT)
    .addReg(QRT)
    .addReg(QR1);


  // FLIP_EXPAND	QR0, QR1 
  MachineInstrBuilder FExpand = BuildMI(MBB, FContract.getInstr(), DebugLoc(), TII->get(QISA::FLIP_EXPANDrr), QRT)
      .addReg(QRT)
      .addReg(QR1);  

  // DEFORM	QR1
  BuildMI(MBB, FExpand.getInstr(), DebugLoc(), TII->get(QISA::DEFORMrr), QR1)
    .addReg(QR1);

}


void QISAAncillaAllocationPass::expandCNOTGate(MachineInstr &MI){
  MachineOperand Op1 = MI.getOperand(0);
  MachineOperand Op2 = MI.getOperand(1);
  MachineOperand Op3 = MI.getOperand(2);

  MachineBasicBlock &MBB = *MI.getParent();

  Register QR1 = Op1.getReg(); // CONTROL
  Register QR2 = Op2.getReg(); // TARGET
  Register QRT = Op3.getReg(); // ANCILLA

  // INIT_Z (QRT)
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::INIT_Z_GATErr), QRT).addReg(QRT);

  // MERGE_MXX(QRT, QR2)
  // SPLIT_MXX(QRT, QR2)
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MERGE_MXXrr), QRT).addReg(QRT).addReg(QRT).addReg(QR2);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::SPLIT_MXXrr), QRT).addReg(QRT).addReg(QRT).addReg(QR2);

  // MERGE_MZZ(QR1, QRT)
  // SPLIT_MZZ(QR1, QRT)
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::MERGE_MZZrr), QR1).addReg(QR1).addReg(QR1).addReg(QRT);
  BuildMI(MBB, MI, DebugLoc(), TII->get(QISA::SPLIT_MZZrr), QR1).addReg(QR1).addReg(QR1).addReg(QRT);

}

FunctionPass *llvm::createAncillaAllocationPass() {
  return new QISAAncillaAllocationPass();
}

static RegisterPass<QISAAncillaAllocationPass> X("ancilla-alloc",
                                                 "Ancilla Allocation");