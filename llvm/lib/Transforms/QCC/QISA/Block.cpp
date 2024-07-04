/*********************************************************************
 *   Block.h:
 *
 *   Authors:
 *      Sengthai Heng (sengthai37@gmail.com)
 *      Youngsun Han (youngsun@pknu.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/

#include "llvm/Transforms/QCC/QISA/Block.h"

#include <llvm/IR/IRBuilder.h>

namespace qcc {

bool Block::insert2set(int val, std::set<int> &Qset) {
  auto ret = Qset.insert(val);
  // false if it exist
  return ret.second;
}

bool Block::insertInitializedQuantumRegisters(int physicalRegister) {
  return insert2set(physicalRegister, initializedQuantumRegisters);
}


Block::Block(string name, QODGAnalysis &QODG, QCCLayout *layout,
             ObjectFile *objectFile)
    : name(name), QODG(QODG), layout(layout), objectFile(objectFile) {
  setInstructions(QODG.QODNodes);
}

Block::Block(QODGAnalysis &QODG, QCCLayout *layout, ObjectFile *objectFile)
    : QODG(QODG), layout(layout), objectFile(objectFile) {
  name = "kernel_func";
  setInstructions(QODG.QODNodes);
}

void Block::moveQubit2Reg(std::map<int, int> &qReg2physicalQubit, QPRType *qr,
                          ImmType *imm) {

  // check for if it QMOV yet
  bool isRegisterExist = true;
  if (qReg2physicalQubit.count(qr->getValue())) {
    if (qReg2physicalQubit[qr->getValue()] != imm->getValue()) {
      isRegisterExist = false;
    }
  } else {
    isRegisterExist = false;
  }

  if (!isRegisterExist) {
    auto qmov = QmovInst::create(qr, imm);
    insts.push_back(qmov);
    qReg2physicalQubit[qr->getValue()] = imm->getValue();
  }
}

void Block::moveQubit2Reg(std::map<int, int> &qReg2physicalQubit, QPRType *qr,
                          int value) {
  auto imm = ImmType::create(value);
  moveQubit2Reg(qReg2physicalQubit, qr, imm);
}

void Block::initZInst(int physicalQubit, QPRType *qr) {
  if (insertInitializedQuantumRegisters(physicalQubit)) {
    auto initZ = InitZInst::create(qr);
    insts.push_back(initZ);
  }
}

void Block::initXInst(int physicalQubit, QPRType *qr) {
  if (insertInitializedQuantumRegisters(physicalQubit)) {
    auto initX = InitXInst::create(qr);
    insts.push_back(initX);
  }
}

void Block::moveXBoundary(QPRType *qr2, QPRType *qr0, QPRType *qr1) {
  auto INIT_Q1 = InitXInst::create(qr1);

  // MERGE & SPLIT MZZ QR2, QR0 (C), QR1 (AN)
  auto MS0 = MergeSplitMZZInst::create(qr2, qr0, qr1);

  // MOVE POST QRO (C), QR1 (AN)
  auto MOVE_POST = MovePostMZZInst::create(qr0, qr1);

  insts.push_back(INIT_Q1);
  insts.push_back(MS0);
  insts.push_back(MOVE_POST);
}

void Block::moveZBoundary(QPRType *qr2, QPRType *qr0, QPRType *qr1) {
  auto INIT_Q1 = InitZInst::create(qr1);

  // MERGE & SPLIT MXX QR2, QR0 (C), QR1 (AN)
  auto MS0 = MergeSplitMXXInst::create(qr2, qr0, qr1);

  // MOVE POST QRO (C), QR1 (AN)
  auto MOVE_POST = MovePostMXXInst::create(qr0, qr1);

  insts.push_back(INIT_Q1);
  insts.push_back(MS0);
  insts.push_back(MOVE_POST);
}

void Block::command(std::string str) {
  auto command = CommandInst::create(str);
  insts.push_back(command);
}

void Block::setInstructions(NodeGates nodeGates) {

  // set register instructions
  // set init instructions
  // set gates instructions
  auto registerInfo =
      QODG.getScanIntervalRegLoc(NUMBER_OF_REGISTER, TEMP_QR, ANCILLA_QR);

  std::set<int> initializedQuantumRegisters;
  std::vector<int> liveInterval;

  std::map<int, int> qReg2physicalQubit;
  std::map<int, int> qRegister2value;

  // Reserved Mnemonic for storing result form Measurement
  auto rval = objectFile->sysmbolData[0];

  // filter QODNodes only instructions
  auto opNodes = QODG.getOpNodes();

  for (auto node : opNodes) {

    std::vector<QType *> operands;

    for (auto operand : node->qubitIndex) {

      auto physicalQubit = layout->getPhysicalQubitBy(operand);
      auto info = registerInfo[QODG.findRegisterNode(operand)];

      auto qprTy = QPRType::create(info.physicalRegister);
      auto immTy = ImmType::create(physicalQubit);

      operands.push_back(qprTy);

      moveQubit2Reg(qReg2physicalQubit, qprTy, immTy);

      if (node->predecessors.front()->type == q_register &&
          isa<HGateInst>(node->gates[0])) {

        initXInst(physicalQubit, qprTy);

        continue;
      }
      initZInst(physicalQubit, qprTy);
    }

    if (node->type == multi) {
      for (auto operand : node->qubitIndex) {

        auto info = registerInfo[QODG.findRegisterNode(operand)];

        if (info.mergePhysicalRegister != -1) {

          auto mergeQR = QPRType::create(info.mergePhysicalRegister);

          // SWAP gates don't have aniclla object
          if (node->ancilla) {
            auto ancillaQR = QPRType::create(info.ancillaPhysicalRegister);
            auto ancillaImm = ImmType::create(node->ancilla->selectedQID);
            operands.push_back(ancillaQR);
            moveQubit2Reg(qReg2physicalQubit, ancillaQR, ancillaImm);
          }

          operands.push_back(mergeQR);

          break;
        }
      }
    }

    switch (node->gates[0]->getOpcode()) {

    case llvm::Instruction::XGate: {

      QPRType *QPR = dyn_cast<QPRType>(operands[0]);
      auto I = XInst::create(QPR);
      insts.push_back(I);

      break;
    }
    case llvm::Instruction::ZGate: {

      QPRType *QPR = dyn_cast<QPRType>(operands[0]);
      auto I = ZInst::create(QPR);
      insts.push_back(I);

      break;
    }
    case llvm::Instruction::HGate: {

      // check if it is after register, then it applied InitX
      // already, so it doesn't need to apply H anymore.
      if (node->predecessors[0]->type == q_register) {
        break;
      }

      QPRType *QPR = dyn_cast<QPRType>(operands[0]);
      auto I = ZInst::create(QPR);
      insts.push_back(I);

      break;
    }
    case llvm::Instruction::CX0Gate:
    case llvm::Instruction::CX1Gate: {

      auto stabilizer = node->ancilla->getStabilizerSym();

      auto *QRC = dyn_cast<QPRType>(operands[0]);
      auto *QRT = dyn_cast<QPRType>(operands[1]);
      auto *QRI = dyn_cast<QPRType>(operands[2]);
      auto *QRM = dyn_cast<QPRType>(operands[3]);

      Instruction *INIT = nullptr;
      Instruction *MS0 = nullptr;
      Instruction *MS1 = nullptr;
      Instruction *CNOT = nullptr;

      // get physical qubit
      auto QID_C = qReg2physicalQubit[QRC->getValue()];
      auto QID_T = qReg2physicalQubit[QRT->getValue()];

      command("CNOT : " + to_string(QID_C) + ", " + to_string(QID_T));

      if (stabilizer == node->ancilla->X) { // => X

        // init X on QRI
        INIT = InitXInst::create(QRI);
        // merge & split MZZ on QRM, QRC, QRI
        MS0 = MergeSplitMZZInst::create(QRM, QRC, QRI);
        // merge & split MXX on QRM, QRI, QRT
        MS1 = MergeSplitMXXInst::create(QRM, QRI, QRT);
        // move post MZZ on QRC, QRT, QRI
        CNOT = CnotPostMZZInst::create(QRC, QRT, QRI);

      } else if (stabilizer == node->ancilla->Z) { // => Z

        // init z on QRI
        INIT = InitZInst::create(QRI);
        // merge & split MXX on QRM, QRI, QRT
        MS0 = MergeSplitMXXInst::create(QRM, QRI, QRT);
        // merge & split MZZ on QRM, QRC, QRI
        MS1 = MergeSplitMZZInst::create(QRM, QRC, QRI);
        // move post MXX on QRC, QRT, QRI
        CNOT = CnotPostMXXInst::create(QRC, QRT, QRI);
      }

      insts.push_back(INIT);
      insts.push_back(MS0);
      insts.push_back(MS1);
      insts.push_back(CNOT);

      break;
    }
    case llvm::Instruction::SwapGate: {

      auto *QR0 = dyn_cast<QPRType>(operands[0]);
      auto *QR1 = dyn_cast<QPRType>(operands[1]);
      // auto *QRI = dyn_cast<QPRType>(operands[2]);  // Not Using
      auto *QRM = dyn_cast<QPRType>(operands[3]);

      auto QID_C = qReg2physicalQubit[QR0->getValue()];
      auto QID_T = qReg2physicalQubit[QR1->getValue()];

      // emit all 2 ancilla qubits
      auto AID_0 = node->ancilla->selectedQID;
      auto AID_1 = node->ancilla->anotherQID;

      // sort in order to get
      // | q1 | q2 |
      // | q3 | q4 |
      std::vector<int> chainPhysicalQubits = {QID_C, QID_T, AID_0, AID_1};
      std::sort(chainPhysicalQubits.begin(), chainPhysicalQubits.end());

      command("SWAP : " + to_string(QID_C) + ", " + to_string(QID_T));

      auto q0 = chainPhysicalQubits[0];
      auto q1 = chainPhysicalQubits[1];
      auto q2 = chainPhysicalQubits[2];
      auto q3 = chainPhysicalQubits[3];

      // # MOVE OPERATION BY (BY Z-boundary) ----
      command("FIRST MOVE OPERATION BY (BY Z-boundary)");

      //  QMOV QR0, q0 (C)
      moveQubit2Reg(qReg2physicalQubit, QR0, q0);

      //  QMOV QR1, q1 (AN) (the first z-xis )
      moveQubit2Reg(qReg2physicalQubit, QR1, q1);

      //  INIT_Z QR1 (AN)
      // MERGE & SPLIT MXX QR2, QR0 (C), QR1 (AN)
      // MOVE POST QRO (C), QR1 (AN)
      moveZBoundary(QRM, QR0, QR1);

      // # MOVE OPERATION (BY Z Boundary) ----
      command("SECOND MOVE OPERATION BY (BY Z-boundary)");

      // QMOV QRO, q3 (T)
      moveQubit2Reg(qReg2physicalQubit, QR0, q3);

      // QMOV QR1, q2 (AN) (the second z-xis )
      moveQubit2Reg(qReg2physicalQubit, QR1, q2);

      // INIT_Z QR1 (AN)
      // MERGE & SPLIT MXX QR2, QR0, QR1
      // MOVE POST MXX QR0, QR1
      moveZBoundary(QRM, QR0, QR1);

      // # MOVE OPERATION BY (BY X-boundary) ----
      command("THIRD MOVE OPERATION BY (BY X-boundary)");

      // QMOV QR0, q2
      moveQubit2Reg(qReg2physicalQubit, QR0, q2);

      // QMOV QR1, q0
      moveQubit2Reg(qReg2physicalQubit, QR1, q0);

      // INIT X QR1
      // MERGE & SPLIT MZZ QR2, QR0, QR1
      // MOVE POST MZZ QR0, QR1
      moveXBoundary(QRM, QR0, QR1);

      // # MOVE OPERATION BY (BY X-boundary) ----
      command("FOURTH MOVE OPERATION BY (BY X-boundary)");

      // QMOV QR0, q1
      moveQubit2Reg(qReg2physicalQubit, QR0, q1);

      // QMOV QR1, q3
      moveQubit2Reg(qReg2physicalQubit, QR1, q3);

      // INIT X QR1
      // MERGE & SPLIT MZZ QR2, QR0, QR1
      // MOVE POST MZZ QR0, QR1
      moveXBoundary(QRM, QR0, QR1);

      break;
    }
    case llvm::Instruction::MeasZGate: {

      auto *QR = dyn_cast<QPRType>(operands[0]);

      auto QID = layout->getPhysicalQubitBy(node->qubitIndex[0]);
      auto IMM = ImmType::create(QID);

      auto CR = CGPRType::create(0); // should generate auto

      command("MAEASURE : " + to_string(QID));

      // Point the result address
      auto address = rval.vals[node->bitIndex] + rval.address;
      auto ADDR = AddrType::create(address);

      auto MEAS_Z = MeasZInst::create(QR);
      auto FMR = FMRInst::create(CR, IMM);
      auto STD = STDInst::create(ADDR, CR);

      insts.push_back(MEAS_Z);
      insts.push_back(FMR);
      insts.push_back(STD);
      break;
    }
    default:
      break;
    }
  }
}

void Block::write(ostream &o) {

  o << name << ":" << endl;
  // stream instructions
  for (auto inst : insts) {
    o << TAB_STRING << inst;
  }
}

ostream &operator<<(ostream &o, Block *bl) {
  bl->write(o);
  return o;
}
} // namespace qcc
