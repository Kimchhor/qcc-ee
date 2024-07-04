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

#include "Program.h"

namespace qcc {

Program::Program(string name, std::vector<Block *> blocks, ObjectFile &objFile)
    : name(name), objFile(objFile), blocks(blocks) {

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
