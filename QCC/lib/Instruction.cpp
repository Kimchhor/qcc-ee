/*********************************************************************
 *   Instruction.cpp:
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

#include "Instruction.h"

namespace qcc {

ostream &operator<<(ostream &o, Instruction *inst) {
  inst->write(o);
  return o;
}

} // namespace qcc