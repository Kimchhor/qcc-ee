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

#ifndef QCC_QCCINSTRUCTIONS
#define QCC_QCCINSTRUCTIONS

#include <ostream>

#include "ConstantGlobal.h"
#include "Instruction.cpp"
#include "Types.h"
#include <llvm/Transforms/QCC/NodeGate.h>

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QmovInst Class Declaration
///////////////////////////////////////////////////////////////////////

class QmovInst : public Instruction {

public:
  QmovInst(ImmType *imm) : Instruction(imm) {}
  QmovInst(QPRType *qpr, ImmType *imm) : Instruction(qpr, imm) {}
  QmovInst(QPRType *qpr, CGPRType *cgpr) : Instruction(qpr, cgpr) {}

  static QmovInst *create(ImmType *imm) { return new QmovInst(imm); }
  static QmovInst *create(int value) {
    auto imm = ImmType::create(value);
    return new QmovInst(imm);
  }

  static QmovInst *create(QPRType *qpr, ImmType *imm) {
    return new QmovInst(qpr, imm);
  }
  static QmovInst *create(QPRType *qpr, CGPRType *cgpr) {
    return new QmovInst(qpr, cgpr);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// InitZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class InitZInst : public Instruction {

public:
  InitZInst(QPRType *qpr) : Instruction(qpr) {}

  static InitZInst *create(QPRType *qpr) { return new InitZInst(qpr); }
  static InitZInst *create(int value) {
    auto qpr = QPRType::create(value);
    return new InitZInst(qpr);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// InitXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class InitXInst : public Instruction {

public:
  InitXInst(QPRType *qpr) : Instruction(qpr) {}

  static InitXInst *create(QPRType *qpr) { return new InitXInst(qpr); }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// YInst Class Declaration
///////////////////////////////////////////////////////////////////////

class YInst : public Instruction {

public:
  YInst(QPRType *qpr) : Instruction(qpr) {}

  static YInst *create(QPRType *qpr) { return new YInst(qpr); }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// ZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class ZInst : public Instruction {

public:
  ZInst(QPRType *qpr) : Instruction(qpr) {}

  static ZInst *create(QPRType *qpr) { return new ZInst(qpr); }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// XInst Class Declaration
///////////////////////////////////////////////////////////////////////

class XInst : public Instruction {

public:
  XInst(QPRType *qpr) : Instruction(qpr) {}

  static XInst *create(QPRType *qpr) { return new XInst(qpr); }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// HInst Class Declaration
///////////////////////////////////////////////////////////////////////

class HInst : public Instruction {

public:
  HInst(QPRType *qpr) : Instruction(qpr) {}

  static HInst *create(QPRType *qpr) { return new HInst(qpr); }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// MeasZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MeasZInst : public Instruction {

public:
  MeasZInst(QPRType *qpr) : Instruction(qpr) {}

  static MeasZInst *create(QPRType *qpr) { return new MeasZInst(qpr); }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// MergeSplitMZZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MergeSplitMZZInst : public Instruction {

public:
  MergeSplitMZZInst(QPRType *qrmd, QPRType *qrus, QPRType *qrds)
      : Instruction(qrmd, qrus, qrds) {}

  static MergeSplitMZZInst *create(QPRType *qrmd, QPRType *qrus,
                                   QPRType *qrds) {
    return new MergeSplitMZZInst(qrmd, qrus, qrds);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// MergeSplitMXXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MergeSplitMXXInst : public Instruction {

public:
  MergeSplitMXXInst(QPRType *qrmd, QPRType *qrls, QPRType *qrrs)
      : Instruction(qrmd, qrls, qrrs) {}

  static MergeSplitMXXInst *create(QPRType *qrmd, QPRType *qrls,
                                   QPRType *qrrs) {
    return new MergeSplitMXXInst(qrmd, qrls, qrrs);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// MovePostMZZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MovePostMZZInst : public Instruction {

public:
  MovePostMZZInst(QPRType *qrs, QPRType *qrd) : Instruction(qrs, qrd) {}

  static MovePostMZZInst *create(QPRType *qrs, QPRType *qrd) {
    return new MovePostMZZInst(qrs, qrd);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// MovePostMXXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MovePostMXXInst : public Instruction {

public:
  MovePostMXXInst(QPRType *qrs, QPRType *qrd) : Instruction(qrs, qrd) {}

  static MovePostMXXInst *create(QPRType *qrs, QPRType *qrd) {
    return new MovePostMXXInst(qrs, qrd);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// CnotPostMXXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class CnotPostMXXInst : public Instruction {

public:
  CnotPostMXXInst(QPRType *qrc, QPRType *qrt, QPRType *qri)
      : Instruction(qrc, qrt, qri) {}

  static CnotPostMXXInst *create(QPRType *qrc, QPRType *qrt, QPRType *qri) {
    return new CnotPostMXXInst(qrc, qrt, qri);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// CnotPostMZZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class CnotPostMZZInst : public Instruction {

public:
  CnotPostMZZInst(QPRType *qrc, QPRType *qrt, QPRType *qri)
      : Instruction(qrc, qrt, qri) {}

  static CnotPostMZZInst *create(QPRType *qrc, QPRType *qrt, QPRType *qri) {
    return new CnotPostMZZInst(qrc, qrt, qri);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// FMRInst Class Declaration
///////////////////////////////////////////////////////////////////////

class FMRInst : public Instruction {

public:
  FMRInst(CGPRType *rd, CGPRType *rs) : Instruction(rd, rs) {}
  FMRInst(CGPRType *rd, ImmType *imm) : Instruction(rd, imm) {}

  static FMRInst *create(CGPRType *rd, CGPRType *rs) {
    return new FMRInst(rd, rs);
  }

  static FMRInst *create(CGPRType *rd, ImmType *imm) {
    return new FMRInst(rd, imm);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// STInst Class Declaration
///////////////////////////////////////////////////////////////////////

class STInst : public Instruction {

public:
  STInst(AddrType *addr, CGPRType *rs) : Instruction(addr, rs) {}
  STInst(CGPRType *rd, CGPRType *rs) : Instruction(rd, rs) {}

  static STInst *create(AddrType *addr, CGPRType *rs) {
    return new STInst(addr, rs);
  }

  static STInst *create(CGPRType *rd, CGPRType *rs) {
    return new STInst(rd, rs);
  }

  virtual void write(ostream &o);
};

///////////////////////////////////////////////////////////////////////
// CommandInst Class Declaration
///////////////////////////////////////////////////////////////////////

class CommandInst : public Instruction {
public:
  CommandInst(std::string str) : Instruction(str) {}

  static CommandInst *create(std::string str) { return new CommandInst(str); }

  virtual void write(ostream &o);
};

} // namespace qcc

#endif