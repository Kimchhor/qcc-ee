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

#include "llvm/Transforms/QCC/QISA/Instructions.h"

namespace qcc {

void YInst::write(ostream &o) {
  o << "Y" << SPACE_STRING << getType0() << endl;
}

void ZInst::write(ostream &o) {
  o << "Z" << SPACE_STRING << getType0() << endl;
}

void XInst::write(ostream &o) {
  o << "X" << SPACE_STRING << getType0() << endl;
}

void IInst::write(ostream &o) {
  o << "I" << SPACE_STRING << getType0() << endl;
}

void HInst::write(ostream &o) {
  o << "H" << SPACE_STRING << getType0() << endl;
}

void QmovInst::write(ostream &o) {
  o << "QMOV" << SPACE_STRING << getType0();
  o << COMMA << getType1() << endl;
}

void InitZInst::write(ostream &o) {
  o << "INIT_Z" << SPACE_STRING << getType0() << endl;
}

void InitXInst::write(ostream &o) {
  o << "INIT_X" << SPACE_STRING << getType0() << endl;
}

void MeasZInst::write(ostream &o) {
  o << "MEASURE_Z" << SPACE_STRING << getType0() << endl;
}

void MergeSplitMZZInst::write(ostream &o) {
  o << "MERGE_MZZ" << SPACE_STRING << getType0() << COMMA;
  o << getType1() << COMMA << getType2() << endl;
  o << TAB_STRING << "SPLIT_MZZ" << SPACE_STRING << getType0() << endl;
}

void MergeSplitMXXInst::write(ostream &o) {
  o << "MERGE_MXX" << SPACE_STRING << getType0() << COMMA;
  o << getType1() << COMMA << getType2() << endl;
  o << TAB_STRING << "SPLIT_MXX" << SPACE_STRING << getType0() << endl;
}

void MovePostMZZInst::write(ostream &o) {
  o << "MOVE_POST_MZZ" << SPACE_STRING << getType0() << COMMA;
  o << getType1() << endl;
}

void MovePostMXXInst::write(ostream &o) {
  o << "MOVE_POST_MXX" << SPACE_STRING << getType0() << COMMA;
  o << getType1() << endl;
}

void CnotPostMZZInst::write(ostream &o) {
  o << "CNOT_POST_MZZ" << SPACE_STRING << getType0() << COMMA;
  o << getType1() << COMMA << getType2() << endl;
}

void CnotPostMXXInst::write(ostream &o) {
  o << "CNOT_POST_MXX" << SPACE_STRING << getType0() << COMMA;
  o << getType1() << COMMA << getType2() << endl;
}

void FMRInst::write(ostream &o) {
  o << "FMR" << SPACE_STRING << getType0() << COMMA << getType1() << endl;
}

void STDInst::write(ostream &o) {
  o << "STD" << SPACE_STRING << getType0() << COMMA << getType1() << endl;
}

void STIInst::write(ostream &o) {
  o << "STI" << SPACE_STRING << getType0() << COMMA << getType1() << COMMA;
  o << getType2() << endl;
}

void LDDInst::write(ostream &o) {
  o << "LDD" << SPACE_STRING << getType0() << COMMA << getType1() << endl;
}

void LDIInst::write(ostream &o) {
  o << "LDI" << SPACE_STRING << getType0() << COMMA << getType1() << COMMA;
  o << getType2() << endl;
}

void CommandInst::write(ostream &o) {
   o << endl;
   o << TAB_STRING <<"##" << SPACE_STRING <<  getCommand() << endl;
 }
} // namespace qcc
