/*********************************************************************
 *   QCCBFSQubitMapping.h:
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

#ifndef QCC_QCCBFSQUBITMAPPING
#define QCC_QCCBFSQUBITMAPPING

#include "llvm/Transforms/QCC/MultiGateAnalysis.h"
#include "llvm/Transforms/QCC/QODGAnalysis.h"

#include "llvm/Transforms/QCC/Layout/QCCTrivialLayout.h"
#include "llvm/Transforms/QCC/QubitMapping/QCCQubitMapping.h"
#include "llvm/Transforms/QCC/Routing/QCCBFSRouting.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QCCBFSQubitMapping Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCBFSQubitMapping : public QCCQubitMapping {

private:
  QCCMachineDescription &machineDescription;
  QODGAnalysis &qodg;
  MultiGateAnalysisPass &multiGateAnalysis;

  /**
   * This method performs the Trivial layout then BFS routhing method
   */
  void perform() override;

public:
  /**
   * This method init the QCCBFSQubitMapping object
   *
   * @param machineDescription QCC MachineDescription.
   * @param qodg the QODG Analysis pass
   * @param multiGateAnalysis the MultiGat eAnalysis Pass
   *
   */
  QCCBFSQubitMapping(QCCMachineDescription &machineDescription,
                     QODGAnalysis &qodg,
                     MultiGateAnalysisPass &multiGateAnalysis);
};

} // namespace qcc

#endif