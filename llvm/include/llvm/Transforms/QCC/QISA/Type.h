/*********************************************************************
 *   Type.h:
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

#ifndef QCC_QCCTYPE
#define QCC_QCCTYPE

#include <iostream>

namespace qcc {

///////////////////////////////////////////////////////////////////////
// Type Class Declaration
///////////////////////////////////////////////////////////////////////

class QType {

public:
  /** Decribe the Type of parameter */
  enum TypeID {
    /** Immidiate Type */
    IMMTyID = 0,
    /** Classcial General Purpose Register R0  -> R63) */
    CGPRTyID,
    /** Quantum Parameter Register QR0 -> QR63) */
    QPRTyID,
    /** The address type */
    ADDRTyID,
  };

private:
  TypeID ID;
  int value = -1;
  std::string address;
  bool tempory = false;

public:
  QType(TypeID ty, int value) : ID(ty), value(value) {}
  QType(TypeID ty, std::string address) : ID(ty), address(address) {}

  /**
   * Return the type id for the type. This will return one of the TypeID enum
   * elements defined above.
   */
  TypeID getTypeID() const { return ID; }

  /** Return the value of its type */
  int getValue() const { return value; }

  /** Set the value of tempory */
  void setTempory(bool isTempory) { tempory = isTempory; }

  /** True if the this type is tempory used */
  bool isTempory() { return tempory; }

  /** Return the value of its address */
  std::string getAddress() const { return address; }

  /** Return true if this is 'Immidiate'. */
  bool isImmTy() const { return getTypeID() == IMMTyID; }

  /** Return true if this is 'CGPR'. */
  bool isCGPRTy() const { return getTypeID() == CGPRTyID; }

  /** Return true if this is 'QPR'. */
  bool isQPRTy() const { return getTypeID() == QPRTyID; }

  /** Return true if this is 'Address'. */
  bool isADDRTy() const { return getTypeID() == ADDRTyID; }

  /**
   * Write the type
   * e.g,.
   *   R0
   *   QR1
   *   0x12
   */
  void write(ostream &o) {
    switch (getTypeID()) {
    case IMMTyID:
      o << getValue();
      break;
    case CGPRTyID:
      o << "R" << getValue();
      break;
    case QPRTyID:
      o << "QR" << getValue();
      break;
    case ADDRTyID:
      if (getValue() == -1) {
        o << "." << getAddress();
      } else {
        o << std::hex << "0x" << getValue() << std::dec;
      }
      break;
    }
  };

  friend ostream &operator<<(ostream &o, QType *ty) {
    ty->write(o);
    return o;
  };
};

} // namespace qcc

#endif