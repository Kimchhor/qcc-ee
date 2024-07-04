/*********************************************************************
 *   QCCDefaultLayout.cpp:
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

#include "llvm/Transforms/QCC/Layout/QCCTrivialLayout.h"

///////////////////////////////////////////////////////////////////////
// QCCDefaultLayout Class Declaration
///////////////////////////////////////////////////////////////////////
namespace qcc {

/**
 * Initialization Method
 *
 * @param QCCMachineDescription It can be created by QCCMachineDescription
 * @param QODGAnalysis QOOG Analysis Pass
 */
QCCTrivialLayout::QCCTrivialLayout(QCCMachineDescription &QMD,
                                   QODGAnalysis &QODG)
    : QMD(QMD), QODG(QODG) {}

/**
 * This method set layout by assigning 'n' circuit (virtual) qubits (0, .., n-1)
 * to device qubits in cropped toplogy by left to right, top to bottom in order.
 * e.g,.
 * - we have 4 virtual qubits.
 * - * is the anicilla qubits
 * Checkerboard Toptology
 * -------------
 * | 1 | * | 3 |
 * -------------
 * | * | 5 | * |
 * -------------
 * | 7 | * | 9 |
 * -------------
 * Virtual qubits [0,1,2,3,4] will assigned to [1,3,5,7,9], respectively.
 */
void QCCTrivialLayout::run() {

  auto croppedNodes = QMD.getDataNodes();

  for (unsigned idx = 0; idx < croppedNodes.size(); idx++) {
    setBothPhysicalVirtualQubit(croppedNodes[idx].id, idx);
  }
}

} // namespace qcc