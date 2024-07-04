/*********************************************************************
 *   Program.cpp:
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

#include "llvm/Transforms/QCC/QISA/Program.h"

namespace qcc {

Program::Program(string name, std::vector<Block *> blocks, ObjectFile &objFile)
    : name(name), blocks(blocks), objFile(objFile){

    }

void Program::write(ostream &o) {

  objFile.writeDataArea(o);

  // stream blocks here
  for (auto block : blocks) {
    o << block;
  }

  objFile.writeSymbolTable(o);
}

ostream &operator<<(ostream &o, Program *pg) {
  pg->write(o);
  return o;
}

} // namespace qcc
