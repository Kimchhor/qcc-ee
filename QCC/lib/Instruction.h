/*********************************************************************
 *   Instructions.h:
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

#ifndef QCC_QCCINSTRUCTION
#define QCC_QCCINSTRUCTION

#include <ostream>

#include <llvm/Transforms/QCC/NodeGate.h>

#include "Type.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// Instruction Class Declaration
///////////////////////////////////////////////////////////////////////

class Instruction {

private:
  QType *ty0;
  QType *ty1;
  QType *ty2;
  std::string command;

public:
  Instruction(std::string command): command(command) {};
  Instruction(QType *ty0): ty0(ty0) {}
  Instruction(QType *ty0, QType *ty1):ty0(ty0), ty1(ty1) {}
  Instruction(QType *ty0, QType *ty1, QType *ty2): ty0(ty0), ty1(ty1), ty2(ty2) {}

  QType *getType0() { return ty0; }
  QType *getType1() { return ty1; }
  QType *getType2() { return ty2; }

  std::string getCommand() { return command; }

  virtual void write(ostream &o) = 0;
  friend ostream &operator<<(ostream &o, Instruction *inst);
};

} // namespace qcc

#endif