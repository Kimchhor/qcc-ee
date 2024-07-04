/*********************************************************************
 *   QCCLayout.h:
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

#ifndef QCC_QCCDATALAYOUT
#define QCC_QCCDATALAYOUT

#include <iostream>
#include <map>
#include <vector>

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QCCLayout Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCLayout {

private:
  /** Physical to virtual qubits */
  std::map<int, int> physical2VirtualQubitMap;

  /** Register to logical qubits */
  std::map<int, int> vritual2physicalQubitMap;

public:
  ///////////////////////////////////////////////////////////////////
  // Member Fucntions Declaration
  ///////////////////////////////////////////////////////////////////

  /**
   * This method will get execute the layout
   */
  virtual void run() = 0;

  /**
   * This method will get the physical qubit by virtual qubit
   *
   * @param virtualQubit (int) virtual qubit
   * @return (int) physical qubit
   */
  int getPhysicalQubitBy(int virtualQubit);

  /**
   * This method will get the virtual qubit by physical qubit
   *
   * @param physicalQubit (int) physical qubit
   * @return (int) virtual qubit
   */
  int getVirtualQubitBy(int physicalQubit);

  /**
   * This method will match the physical to virtual qubit and vice versa.
   *
   * @param physicalQubit (int) physical qubit
   * @param virtualQubit (int) virtual qubit
   */
  void setBothPhysicalVirtualQubit(int physicalQubit, int virtualQubit);

  /**
   * This method will associates a virtual qubit (int) to each physical qubits
   * in increasing order. \n  e,g: [4,6,5]. virtual qubit 4, 5 and 6 will be mapped
   * to 0, 1, 2 (index) respectively.
   *
   * @param physicalQubit (int) physical qubit
   * @param virtualQubit (int) virtual qubit
   */
  void setLayoutFromList(std::vector<int> listVirtualQubits);
};

} // namespace qcc

#endif