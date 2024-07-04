/*********************************************************************
 *   QCCTrivialLayout.h:
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

#ifndef QCC_QCCTRIVIALLAYOUT
#define QCC_QCCTRIVIALLAYOUT

#include "llvm/Transforms/QCC/QODGAnalysis.h"

#include "llvm/Transforms/QCC/Layout/QCCLayout.h"
#include "llvm/Transforms/QCC/MD/QCCMachineDescription.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QCCTrivialLayout Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCTrivialLayout : public QCCLayout {

public:
  ///////////////////////////////////////////////////////////////////
  // Member Attribute Declaration
  ///////////////////////////////////////////////////////////////////

  QCCMachineDescription &QMD;
  QODGAnalysis &QODG;

  ///////////////////////////////////////////////////////////////////
  // Member Attribute Declaration
  ///////////////////////////////////////////////////////////////////

  /**
   * Initialization Method
   *
   * @param[in] QCCMachineDescription It can be created by QCCMachineDescription
   * @param[in] QODGAnalysis QOOG Analysis Pass
   */
  QCCTrivialLayout(QCCMachineDescription &QMD, QODGAnalysis &QODG);

  /**
   * This method set layout by assigning 'n' circuit (virtual) qubits (0, ..,
   * n-1) to device qubits in cropped toplogy by left to right, top to bottom in
   * order. e.g,.
   * - we have 4 virtual qubits.
   * - * is the anicilla qubits
   * Checkerboard Toptology
   * \code {.cpp}
   * -------------
   * | 1 | * | 3 |
   * -------------
   * | * | 5 | * |
   * -------------
   * | 7 | * | 9 |
   * -------------   
   * \endcode
   * Virtual qubits [0,1,2,3,4] will assigned to [1,3,5,7,9], respectively.
   */
  void run() override;
};

} // namespace qcc

#endif