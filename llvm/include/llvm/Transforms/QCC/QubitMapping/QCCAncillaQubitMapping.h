/*********************************************************************
 *   QCCAncillaQubitMapping.h:
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

#ifndef QCC_QCCANCILLAQUBITMAPPING
#define QCC_QCCANCILLAQUBITMAPPING

#include <cstdlib>

#include "llvm/Transforms/QCC/QODGAnalysis.h"

#include "llvm/Transforms/QCC/Layout/QCCLayout.h"
#include "llvm/Transforms/QCC/MD/QCCMachineDescription.h"
#include "llvm/Transforms/QCC/QubitMapping/QCCQubitMapping.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QCCAncillaQubitMapping Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCAncillaQubitMapping : public QCCQubitMapping {

private:
  QCCMachineDescription &machineDescription;
  QODGAnalysis &qodg;
  QCCLayout *layout;

  /**
   * This method performs the Trivial layout then BFS routhing method
   *
   */
  void perform() override;

public:
  /**
   * This method init the QCCBFSQubitMapping object
   *
   * @param machineDescription QCC MachineDescription.
   * @param qodg the QODG Analysis pass
   * @param layout the QCC Layout
   *
   */
  QCCAncillaQubitMapping(QCCMachineDescription &machineDescription,
                         QODGAnalysis &qodg, QCCLayout *layout);

  /**
   * This method is used to compute min or max based on binaryCode
   *
   * @param a int it must be number
   * @param b int it must be number
   * @param binaryCode The navigatoer to use min or max
   * @return int the result from min or max
   *
   */
  int performMinMaxBinary(int a, int b, int binaryCode);

  /**
   * This method is used to find the ancilla's axis
   *
   * @param rNode the Node (Physical Qubit) in the right side
   * @param lNode the Node (Physical Qubit) in the left side
   * @param cateCode int the category code
   * @param connCode the connection code (forward (0) or reverse (1))
   * @return pair of int the x and z of anicilla
   *
   */
  std::pair<int, int> findAnicillaAxis(qcc::Node rNode, qcc::Node lNode,
                                       int cateCode, int connCode);
};

} // namespace qcc

#endif