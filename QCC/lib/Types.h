/*********************************************************************
 *   Types.h:
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

#ifndef QCC_QCCTYPES
#define QCC_QCCTYPES

#include "Type.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QPRType Class Declaration
///////////////////////////////////////////////////////////////////////

class QPRType : public qcc::QType {

public:
  friend qcc::QType;

  int value = -1;

  QPRType(int value) : qcc::QType(QPRTyID, value), value(value) {}

  static QPRType *create(int value) { return new QPRType(value); }

  static bool classof(const QType *T) { return T->getTypeID() == QPRTyID; }
};

///////////////////////////////////////////////////////////////////////
// CGPRType Class Declaration
///////////////////////////////////////////////////////////////////////

class CGPRType : public qcc::QType {

public:
  friend qcc::QType;

  int value = -1;

  CGPRType(int value) : qcc::QType(CGPRTyID, value), value(value) {}

  static CGPRType *create(int value) { return new CGPRType(value); }

  static bool classof(const QType *T) { return T->getTypeID() == CGPRTyID; }
};

///////////////////////////////////////////////////////////////////////
// ImmType Class Declaration
///////////////////////////////////////////////////////////////////////

class ImmType : public qcc::QType {

public:
  friend qcc::QType;

  int value = -1;

  ImmType(int value) : qcc::QType(IMMTyID, value), value(value) {}

  static ImmType *create(int value) { return new ImmType(value); }

  static bool classof(const QType *T) { return T->getTypeID() == IMMTyID; }
};

///////////////////////////////////////////////////////////////////////
// ADDRTy Class Declaration
///////////////////////////////////////////////////////////////////////

class AddrType : public qcc::QType {

public:
  friend qcc::QType;

  int value = -1;

  AddrType(int value) : qcc::QType(ADDRTyID, value), value(value) {}
  AddrType(std::string address) : qcc::QType(ADDRTyID, address) {}

  static AddrType *create(int value) { return new AddrType(value); }
  static AddrType *create(std::string address) { return new AddrType(address); }

  static bool classof(const QType *T) { return T->getTypeID() == IMMTyID; }
};

} // namespace qcc

#endif