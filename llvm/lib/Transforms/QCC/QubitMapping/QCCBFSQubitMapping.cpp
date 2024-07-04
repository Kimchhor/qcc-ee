/*********************************************************************
 *   QCCBFSQubitMapping.cpp:
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

#include "llvm/Transforms/QCC/QubitMapping/QCCBFSQubitMapping.h"

///////////////////////////////////////////////////////////////////////
// QCCBFSQubitMapping Class Declaration
///////////////////////////////////////////////////////////////////////
namespace qcc {

/**
 * This method init the QCCBFSQubitMapping object
 *
 * @param machineDescription QCC MachineDescription.
 * @param qodg the QODG Analysis pass
 * @param multiGateAnalysis the MultiGat eAnalysis Pass
 *
 */
QCCBFSQubitMapping::QCCBFSQubitMapping(
    QCCMachineDescription &machineDescription, QODGAnalysis &qodg,
    MultiGateAnalysisPass &multiGateAnalysis)
    : machineDescription(machineDescription), qodg(qodg),
      multiGateAnalysis(multiGateAnalysis) {}

/**
 * This method performs the Trivial layout then BFS routhing method
 *
 */
void QCCBFSQubitMapping::perform() {

  qcc::QCCTrivialLayout layout(machineDescription, qodg);
  layout.run();

  qcc::QCCBFSRouting routing(&layout, qodg, machineDescription, multiGateAnalysis);
  routing.run();

  errs() << "\n[-------BFS Qubit Mapping Successfully-------]\n\n";
}

} // namespace qcc