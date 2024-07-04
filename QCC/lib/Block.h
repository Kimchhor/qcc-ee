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

#include "llvm/Transforms/QCC/NodeGate.h"

#include "Instructions.cpp"

using namespace std;

namespace qcc {

///////////////////////////////////////////////////////////////////////
// Block Class Declaration
///////////////////////////////////////////////////////////////////////

class Block {

private:
  using NodeGates = std::vector<NodeGate *>;

  string name; // kernel name

  vector<Instruction *> insts;
  QODGAnalysis &QODG;
  QCCLayout *layout;
  ObjectFile *objectFile;

  std::set<int> initializedQuantumRegisters;
  std::set<int> usedRegisters;

  void setInstructions(NodeGates nodeGates);

  bool insert2set(int val, std::set<int> &Qset);

  bool insertInitializedQuantumRegisters(int physicalRegister);

  bool insertUsedRegisters(int physicalRegister);

  void moveQubit2Reg(std::map<int, int> &qReg2physicalQubit,
                       QPRType *qr, ImmType *imm);

  void moveQubit2Reg(std::map<int, int> &qReg2physicalQubit, QPRType *qr,
                          int value);

  void initZInst(int physicalQubit, QPRType *qr);
  void initXInst(int physicalQubit, QPRType *qr);

  void moveZBoundary(QPRType *qr2, QPRType *qr0, QPRType *qr1 );
  void moveXBoundary(QPRType *qr2, QPRType *qr0, QPRType *qr1 );
  void command(std::string str);
public:
  Block(string name, QODGAnalysis &QODG, QCCLayout *layout, ObjectFile *objectFile);
  Block(QODGAnalysis &QODG, QCCLayout *layout, ObjectFile *objectFile);
  

  void write(ostream &o);
  friend ostream &operator<<(ostream &o, Block *b);
};

} // namespace qcc

#endif