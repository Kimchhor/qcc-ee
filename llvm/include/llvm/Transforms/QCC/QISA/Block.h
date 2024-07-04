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

#ifndef QCC_QCCOTYPE
#define QCC_QCCOTYPE

#include <iostream>
#include <ostream>
#include <vector>

#include "llvm/Transforms/QCC/Layout/QCCLayout.h"
#include "llvm/Transforms/QCC/NodeGate.h"
#include "llvm/Transforms/QCC/QISA/ObjectFile.h"
#include "llvm/Transforms/QCC/QODGAnalysis.h"

#include "llvm/Transforms/QCC/QISA/Instructions.h"

using namespace std;

namespace qcc {

///////////////////////////////////////////////////////////////////////
// Block Class Declaration
///////////////////////////////////////////////////////////////////////

class Block {

private:
  /** A list of NodeGates */
  using NodeGates = std::vector<NodeGate *>;

  /** Kernel name */
  string name;

  /** this vector collect all the instruction in blocks. */
  vector<Instruction *> insts;

  QODGAnalysis &QODG;
  QCCLayout *layout;
  ObjectFile *objectFile;

  std::set<int> initializedQuantumRegisters;
  std::set<int> usedRegisters;

  /**
   * This is the main method of Block. It will navigate the instruction
   * into specific class of instruction such as X inst, it will init XInst
   * class. However, this method also dynamictly init the Z, X instruction and
   * perform QMOV instruction.
   *
   * @param nodeGates the list of nodeGate
   */
  void setInstructions(NodeGates nodeGates);

  /**
   * This help method will insert qubit ID into set.
   * If it exist, it'll return false.
   *
   * @param val the qubit ID
   * @param Qset the set that store Qubit ID
   *
   * @return false if val is exist, otherwise is true then the val
   * will insert into the set
   */
  bool insert2set(int val, std::set<int> &Qset);

  /**
   * This method will return true if that qubit ID has not yet initialiized.
   *
   * @param physicalRegister the Physical register qubit ID
   *
   * @return false if it has been initialized.
   */
  bool insertInitializedQuantumRegisters(int physicalRegister);

  /**
   * This method will insert \a QMOV into \a insts list.
   *
   * @param qReg2physicalQubit the dictionary that has quantum resgister as key
   * and physical qubit ID as value.
   * @param qr the quantum parameter type
   * @param imm the immidiate Type
   */
  void moveQubit2Reg(std::map<int, int> &qReg2physicalQubit, QPRType *qr,
                     ImmType *imm);
  /**
   * This method will insert \a QMOV into \a insts lis
   *
   * @param qReg2physicalQubit the dictionary that has quantum resgister as key
   * and physical qubit ID as value.
   * @param qr the quantum parameter type
   * @param value the quantum register ID
   */
  void moveQubit2Reg(std::map<int, int> &qReg2physicalQubit, QPRType *qr,
                     int value);

  /**
   * This method will insert \a InitZ into \a insts list
   *
   * @param physicalQubit the physical qubit ID
   * @param qr the quantum parameter type
   */
  void initZInst(int physicalQubit, QPRType *qr);

  /**
   * This method will insert \a InitX into \a insts list
   *
   * @param physicalQubit the physical qubit ID
   * @param qr the quantum parameter type
   */
  void initXInst(int physicalQubit, QPRType *qr);

  /**
   * This method will insert \a InitZInst , \a MergeSplitMXXInst inst, and
   * \a MovePostMXXInst into \a insts list.
   *
   * @param pr2 the QR for merge qubit
   * @param pr0 the QR for source qubit
   * @param pr1 the QR for target qubit
   */
  void moveZBoundary(QPRType *qr2, QPRType *qr0, QPRType *qr1);

  /**
   * This method will insert \a InitXInst , \a MergeSplitMZZInst inst, and
   * \a MovePostMZZInst into \a insts list.
   *
   * @param pr2 the QR for merge qubit
   * @param pr0 the QR for source qubit
   * @param pr1 the QR for target qubit
   */
  void moveXBoundary(QPRType *qr2, QPRType *qr0, QPRType *qr1);

  /**
   * This method will be instert \a CommandInst into \a insts list.
   *
   * @param str the message text.
   */
  void command(std::string str);

public:
  Block(string name, QODGAnalysis &QODG, QCCLayout *layout,
        ObjectFile *objectFile);
  Block(QODGAnalysis &QODG, QCCLayout *layout, ObjectFile *objectFile);

  /**
   * This method will write all instructions in blocks
   * @param o the ostream object
   */
  void write(ostream &o);

  /**
   * This method will return all instructions in blocks
   * @return list of instructions
   */
  vector<Instruction *> getInstructions() {return insts;}

  friend ostream &operator<<(ostream &o, Block *b);
};

} // namespace qcc

#endif