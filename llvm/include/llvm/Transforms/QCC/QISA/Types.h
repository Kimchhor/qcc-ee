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

#include "llvm/Transforms/QCC/QISA/Type.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QPRType Class Declaration
///////////////////////////////////////////////////////////////////////

class QPRType : public qcc::QType {

public:
  friend qcc::QType;

  int value = -1;

  QPRType(int value) : qcc::QType(QPRTyID, value), value(value) {}

  /**
   * Create pointer object
   * @param value  the quantum register ID
   * @return the pointer object of QPRType
   */
  static QPRType *create(int value) { return new QPRType(value); }

  /**
   * This class to check if it is QPRTY type or not
   */
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

  /**
   * Create pointer object
   * @param value  the quantum register ID
   * @return the pointer object of CGPRType
   */
  static CGPRType *create(int value) { return new CGPRType(value); }

  /**
   * This class to check if it is CGPRTyID type or not
   */
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

  /**
   * Create pointer object
   * @param value  the quantum register ID
   * @return the pointer object of IMMTyID
   */
  static ImmType *create(int value) { return new ImmType(value); }

  /**
   * This class to check if it is IMMTyID type or not
   */
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

  /**
   * Create pointer object
   * @param value  the quantum register ID
   * @return the pointer object of IMMTyID
   */
  static AddrType *create(int value) { return new AddrType(value); }

  /**
   * Create pointer object
   * @param address address as text (.data_db)
   * @return the pointer object of IMMTyID
   */
  static AddrType *create(std::string address) { return new AddrType(address); }

  /**
   * This class to check if it is AddrType type or not
   */
  static bool classof(const QType *T) { return T->getTypeID() == ADDRTyID; }
};

} // namespace qcc

#endif