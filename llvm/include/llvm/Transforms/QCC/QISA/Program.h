/*********************************************************************
 *   QCCPROGRAM.h:
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

#ifndef QCC_QCCPROGRAM
#define QCC_QCCPROGRAM

#include <iostream>
#include <ostream>
#include <vector>

#include "llvm/Transforms/QCC/QISA/Block.h"
#include "llvm/Transforms/QCC/QISA/ObjectFile.h"

using namespace std;

namespace qcc {

///////////////////////////////////////////////////////////////////////
// Type Class Declaration
///////////////////////////////////////////////////////////////////////

/**
 * This class will generate the QISA code using the blocks and object file.
 */
class Program {

private:
  string name;
  std::vector<Block *> blocks;
  ObjectFile &objFile;

public:
  Program(string name, std::vector<Block *> blocks, ObjectFile &objFile);

  void write(ostream &o);

  friend ostream &operator<<(ostream &o, Program *pg);

  /** Get the name of program */
  string getName() { return name; }
};

} // namespace qcc

#endif