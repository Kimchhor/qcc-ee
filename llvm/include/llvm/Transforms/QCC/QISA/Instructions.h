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

#include "llvm/Transforms/QCC/NodeGate.h"
#include "llvm/Transforms/QCC/QISA/ConstantGlobal.h"
#include "llvm/Transforms/QCC/QISA/Instruction.h"
#include "llvm/Transforms/QCC/QISA/Types.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QmovInst Class Declaration
///////////////////////////////////////////////////////////////////////

class QmovInst : public Instruction {

public:
  QmovInst(ImmType *imm) : Instruction(imm) {}
  QmovInst(QPRType *qpr, ImmType *imm) : Instruction(qpr, imm) {}
  QmovInst(QPRType *qpr, CGPRType *cgpr) : Instruction(qpr, cgpr) {}

  /**
   * Create pointer object
   * @param imm the ImmType
   * @return the pointer object of QmovInst
   */
  static QmovInst *create(ImmType *imm) { return new QmovInst(imm); }

  /**
   * Create pointer object
   * @param value the quantum register ID
   * @return the pointer object of QmovInst
   */
  static QmovInst *create(int value) {
    auto imm = ImmType::create(value);
    return new QmovInst(imm);
  }

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @param imm the ImmType
   * @return the pointer object of QmovInst
   */
  static QmovInst *create(QPRType *qpr, ImmType *imm) {
    return new QmovInst(qpr, imm);
  }

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @param cgpr the classcial-general-purpose-register type
   * @return the pointer object of QmovInst
   */
  static QmovInst *create(QPRType *qpr, CGPRType *cgpr) {
    return new QmovInst(qpr, cgpr);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * QMOV QR0, 0
   * QMOV QR0, R0
   * \endcode
   * 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// InitZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class InitZInst : public Instruction {

public:
  InitZInst(QPRType *qpr) : Instruction(qpr) {}
  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of InitZInst
   */
  static InitZInst *create(QPRType *qpr) { return new InitZInst(qpr); }

  /**
   * Create pointer object
   * @param imm the ImmType
   * @return the pointer object of InitZInst
   */
  static InitZInst *create(int value) {
    auto qpr = QPRType::create(value);
    return new InitZInst(qpr);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * INIT_Z QR0
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// InitXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class InitXInst : public Instruction {

public:
  InitXInst(QPRType *qpr) : Instruction(qpr) {}

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of InitZInst
   */
  static InitXInst *create(QPRType *qpr) { return new InitXInst(qpr); }

  /**
   * Create pointer object
   * @param imm the ImmType
   * @return the pointer object of InitXInst
   */
  static InitXInst *create(int value) {
    auto qpr = QPRType::create(value);
    return new InitXInst(qpr);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * INIT_X QR0
   * \endcode 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// YInst Class Declaration
///////////////////////////////////////////////////////////////////////

class YInst : public Instruction {

public:
  YInst(QPRType *qpr) : Instruction(qpr) {}

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of YInst
   */
  static YInst *create(QPRType *qpr) { return new YInst(qpr); }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  Y QR0
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// ZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class ZInst : public Instruction {

public:
  ZInst(QPRType *qpr) : Instruction(qpr) {}

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of ZInst
   */
  static ZInst *create(QPRType *qpr) { return new ZInst(qpr); }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * Z QR0
   * \endcode
   * 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// XInst Class Declaration
///////////////////////////////////////////////////////////////////////

class XInst : public Instruction {

public:
  XInst(QPRType *qpr) : Instruction(qpr) {}

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of XInst
   */
  static XInst *create(QPRType *qpr) { return new XInst(qpr); }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * X QR0
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// IInst Class Declaration
///////////////////////////////////////////////////////////////////////

class IInst : public Instruction {

public:
  IInst(QPRType *qpr) : Instruction(qpr) {}

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of IInst
   */
  static IInst *create(QPRType *qpr) { return new IInst(qpr); }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * I QR0
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// HInst Class Declaration
///////////////////////////////////////////////////////////////////////

class HInst : public Instruction {

public:
  HInst(QPRType *qpr) : Instruction(qpr) {}

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of HInst
   */
  static HInst *create(QPRType *qpr) { return new HInst(qpr); }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  H QR0
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// MeasZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MeasZInst : public Instruction {

public:
  MeasZInst(QPRType *qpr) : Instruction(qpr) {}

  /**
   * Create pointer object
   * @param qpr the quantum-parameter-register type
   * @return the pointer object of MeasZInst
   */
  static MeasZInst *create(QPRType *qpr) { return new MeasZInst(qpr); }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  MEASURE_Z QR0
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// MergeSplitMZZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MergeSplitMZZInst : public Instruction {

public:
  MergeSplitMZZInst(QPRType *qrmd, QPRType *qrus, QPRType *qrds)
      : Instruction(qrmd, qrus, qrds) {}

  /**
   * Create pointer object
   * @param qrmd the QPR type for immidiate qubit
   * @param qrus the QPR type for source qubit
   * @param qrds the QPR type for distance (target) qubit
   *
   * @return the pointer object of MergeSplitMZZInst
   */
  static MergeSplitMZZInst *create(QPRType *qrmd, QPRType *qrus,
                                   QPRType *qrds) {
    return new MergeSplitMZZInst(qrmd, qrus, qrds);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * MERGE_MZZ QR9,QR8,QR1
   * SPLIT_MZZ QR9
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// MergeSplitMXXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MergeSplitMXXInst : public Instruction {

public:
  MergeSplitMXXInst(QPRType *qrmd, QPRType *qrls, QPRType *qrrs)
      : Instruction(qrmd, qrls, qrrs) {}

  /**
   * Create pointer object
   * @param qrmd the QPR type for immidiate qubit
   * @param qrls the QPR type for source qubit
   * @param qrrs the QPR type for distance (target) qubit
   *
   * @return the pointer object of MergeSplitMXXInst
   */
  static MergeSplitMXXInst *create(QPRType *qrmd, QPRType *qrls,
                                   QPRType *qrrs) {
    return new MergeSplitMXXInst(qrmd, qrls, qrrs);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * MERGE_MXX QR9,QR8,QR1
   * SPLIT_MXX QR9
   * \endcode
   * 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// MovePostMZZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MovePostMZZInst : public Instruction {

public:
  MovePostMZZInst(QPRType *qrs, QPRType *qrd) : Instruction(qrs, qrd) {}

  /**
   * Create pointer object
   * @param qrs the QPR type for source qubit
   * @param qrd the QPR type for distance (target) qubit
   *
   * @return the pointer object of MovePostMZZInst
   */
  static MovePostMZZInst *create(QPRType *qrs, QPRType *qrd) {
    return new MovePostMZZInst(qrs, qrd);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  MOVE_POST_MZZ QR0, QR1
   * \endcode 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// MovePostMXXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class MovePostMXXInst : public Instruction {

public:
  MovePostMXXInst(QPRType *qrs, QPRType *qrd) : Instruction(qrs, qrd) {}

  /**
   * Create pointer object
   * @param qrs the QPR type for source qubit
   * @param qrd the QPR type for distance (target) qubit
   *
   * @return the pointer object of MovePostMXXInst
   */
  static MovePostMXXInst *create(QPRType *qrs, QPRType *qrd) {
    return new MovePostMXXInst(qrs, qrd);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  MOVE_POST_MXX QR0, QR1
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// CnotPostMXXInst Class Declaration
///////////////////////////////////////////////////////////////////////

class CnotPostMXXInst : public Instruction {

public:
  CnotPostMXXInst(QPRType *qrc, QPRType *qrt, QPRType *qri)
      : Instruction(qrc, qrt, qri) {}

  /**
   * Create pointer object
   * @param qrc the QPR type for source qubit
   * @param qrt the QPR type for target qubit
   * @param qri the QPR type for aniclla qubit
   *
   * @return the pointer object of CnotPostMXXInst
   */
  static CnotPostMXXInst *create(QPRType *qrc, QPRType *qrt, QPRType *qri) {
    return new CnotPostMXXInst(qrc, qrt, qri);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  CNOT_POST_MXX QR0,QR1,QR8
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// CnotPostMZZInst Class Declaration
///////////////////////////////////////////////////////////////////////

class CnotPostMZZInst : public Instruction {

public:
  CnotPostMZZInst(QPRType *qrc, QPRType *qrt, QPRType *qri)
      : Instruction(qrc, qrt, qri) {}

  /**
   * Create pointer object
   * @param qrc the QPR type for source qubit
   * @param qrt the QPR type for target qubit
   * @param qri the QPR type for aniclla qubit
   *
   * @return the pointer object of CnotPostMZZInst
   */
  static CnotPostMZZInst *create(QPRType *qrc, QPRType *qrt, QPRType *qri) {
    return new CnotPostMZZInst(qrc, qrt, qri);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  CNOT_POST_MZZ QR0,QR1,QR8
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// FMRInst Class Declaration
///////////////////////////////////////////////////////////////////////

class FMRInst : public Instruction {

public:
  FMRInst(CGPRType *rd, CGPRType *rs) : Instruction(rd, rs) {}
  FMRInst(CGPRType *rd, ImmType *imm) : Instruction(rd, imm) {}

  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param rs the CGPR type for source qubit
   *
   * @return the pointer object of FMRInst
   */
  static FMRInst *create(CGPRType *rd, CGPRType *rs) {
    return new FMRInst(rd, rs);
  }

  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param rs the Imm type for source qubit
   *
   * @return the pointer object of FMRInst
   */
  static FMRInst *create(CGPRType *rd, ImmType *imm) {
    return new FMRInst(rd, imm);
  }

  /**
   * write the instruction. \n
   * e.g,. 
   * \code {.asm}
   * FMR R1, 0
   * FMR R1, R0
   * \endcode
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// STDInst Class Declaration
///////////////////////////////////////////////////////////////////////

class STDInst : public Instruction {

public:
  STDInst(AddrType *addr, CGPRType *rs) : Instruction(addr, rs) {}
  STDInst(CGPRType *rd, CGPRType *rs) : Instruction(rd, rs) {}

  /**
   * Create pointer object
   * @param addr the Address type for distance qubit
   * @param rs the CGPR type for source qubit
   @
   * @return the pointer object of STDInst
   */
  static STDInst *create(AddrType *addr, CGPRType *rs) {
    return new STDInst(addr, rs);
  }

  /**
   * Create pointer object
   * @param rd the CGPR type for source qubit
   * @param rs the CGPR type for source qubit
   *
   * @return the pointer object of STDInst
   */
  static STDInst *create(CGPRType *rd, CGPRType *rs) {
    return new STDInst(rd, rs);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  FMR R0, 7
   *  STD 0x8, R0 
   * \endcode
   * 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// STIInst Class Declaration
///////////////////////////////////////////////////////////////////////

class STIInst : public Instruction {

public:
  STIInst(CGPRType *rd, ImmType *imm, CGPRType *rs) : Instruction(rd, imm,rs) {}
  STIInst(CGPRType *rd, CGPRType *rs, CGPRType *rss) : Instruction(rd, rs, rss) {}

  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param imm the Imm type for source qubit
   * @param rs the CGPR type for source qubit
   *
   * @return the pointer object of STDInst
   */
  static STIInst *create(CGPRType *rd, ImmType *imm, CGPRType *rs) {
    return new STIInst(rd, imm, rs);
  }

  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param rs the CGPR type for source qubit
   * @param rss the CGPR type for source qubit
   *
   * @return the pointer object of STIInst
   */
  static STIInst *create(CGPRType *rd, CGPRType *rs, CGPRType *rss) {
    return new STIInst(rd, rs, rss);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * FMR R0, 7 
   * STI R0, 7, R0
   * \endcode
   * 
   */
  void write(ostream &o) override;
};

///////


///////////////////////////////////////////////////////////////////////
// LDDInst Class Declaration
///////////////////////////////////////////////////////////////////////

class LDDInst : public Instruction {

public:
  LDDInst(CGPRType *rs, AddrType *addr) : Instruction(rs, addr) {}
  LDDInst(CGPRType *rd, CGPRType *rs) : Instruction(rd, rs) {}

  /**
   * Create pointer object
   * @param rs the CGPR type for distance qubit
   * @param addr the Address type  for source qubit
   *
   * @return the pointer object of LDDInst
   */
  static LDDInst *create(CGPRType *rs, AddrType *addr) {
    return new LDDInst(rs, addr);
  }

  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param rs the CGPR type for source qubit
   *
   * @return the pointer object of LDDInst
   */
  static LDDInst *create(CGPRType *rd, CGPRType *rs) {
    return new LDDInst(rd, rs);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   * LDD R0, 0x8
   * QMOV QR0, R0  
   * \endcode
   * 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// LDIInst Class Declaration
///////////////////////////////////////////////////////////////////////

class LDIInst : public Instruction {

public:
  LDIInst(CGPRType *rd, CGPRType *rs, ImmType *imm) : Instruction(rd, rs, imm) {}
  LDIInst(CGPRType *rd, CGPRType *rs, AddrType *addr) : Instruction(rd, rs, addr) {}
  LDIInst(CGPRType *rd, CGPRType *rs, CGPRType *rss) : Instruction(rd, rs, rss) {}

  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param rs the CGPR type for source qubit
   * @param imm the Imm type for source qubit
   *
   * @return the pointer object of STDInst
   */
  static LDIInst *create(CGPRType *rd, CGPRType *rs, ImmType *imm) {
    return new LDIInst(rd, rs, imm);
  }
  
  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param rs the CGPR type for source qubit
   * @param addr the Address type for source qubit
   *
   * @return the pointer object of LDIInst
   */
  static LDIInst *create(CGPRType *rd, CGPRType *rs, AddrType *addr) {
    return new LDIInst(rd, rs, addr);
  }

  /**
   * Create pointer object
   * @param rd the CGPR type for distance qubit
   * @param rs the CGPR type for source qubit
   * @param rss the CGPR type for source qubit
   *
   * @return the pointer object of LDIInst
   */
  static LDIInst *create(CGPRType *rd, CGPRType *rs, CGPRType *rss) {
    return new LDIInst(rd, rs, rss);
  }

  /**
   * write the instruction. \n
   * e.g,.
   * \code {.asm}
   *  LDI R0, R0, 7
   *  QMOV QR0, R1
   * \endcode
   * 
   */
  void write(ostream &o) override;
};

///////////////////////////////////////////////////////////////////////
// CommandInst Class Declaration
///////////////////////////////////////////////////////////////////////

class CommandInst : public Instruction {
public:
  CommandInst(std::string str) : Instruction(str) {}

  /**
   * Create pointer object
   * @param str The message text
   *
   * @return the pointer object of CommandInst
   */
  static CommandInst *create(std::string str) { return new CommandInst(str); }

  /**
   * write the instruction. \n 
   * e.g,.
   * \code {.asm}
   *  # text-message
   * \endcode
   */
  void write(ostream &o) override;
};

} // namespace qcc

#endif