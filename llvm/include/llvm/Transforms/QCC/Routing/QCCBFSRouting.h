/*********************************************************************
 *   QCCBFSRouting.h:
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

#ifndef QCC_QCCBFSROUTING
#define QCC_QCCBFSROUTING

#include <vector>

#include "llvm/Transforms/QCC/MultiGateAnalysis.h"

#include "llvm/Transforms/QCC/Layout/QCCLayout.h"
#include "llvm/Transforms/QCC/MD/QCCMachineDescription.h"
#include "llvm/Transforms/QCC/Routing/QCCRouting.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QCCBFSRouting Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCBFSRouting : public QCCRouting {

private:
  QCCLayout *layout;
  QODGAnalysis &qodg;
  QCCMachineDescription &machineDescription;

  int multigateLastID;

  /**
   * This method perform the Breadth First Search (BFS) algorithm
   *
   * @param starter the start of physical qubits
   * @param ender the distination physical qubits\
   *
   * @return vector : a path from stater to ender
   *
   */
  std::vector<int> performBFS(int starter, int ender);

  /**
   * This method perform qubit routhing.
   * 1. find multi gates (CX, SWAP) gate that has far connection ( can't be
   * mapped)
   * 2. apply BFS algorithm
   * 3. iterate the BFS's path
   * 4. each iteration, apply swaps, CX/SWAP, and last reverse swaps
   *
   * Note: in our case we apply reverse swaps first because the instructions
   * gate will be manipulate by FILO (First In Last Out)
   */
  void perform() override;

public:
  /**
   * Initialize the QCCBFSRouting
   *
   * @param layout the abstract layout
   * @param qodg QODG Analysis Pass
   * @param machineDescription QCC MachineDescription
   * @param multigateAnalysis MultiGate Analysis Pass
   *
   */
  QCCBFSRouting(QCCLayout *layout, QODGAnalysis &qodg,
                QCCMachineDescription &machineDescription,
                MultiGateAnalysisPass &multigateAnalysis);
};

} // namespace qcc

#endif