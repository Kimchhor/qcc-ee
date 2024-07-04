/*********************************************************************
 *   QCCBFSRouting.cpp:
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

#include <llvm/Transforms/QCC/Routing/QCCBFSRouting.h>

///////////////////////////////////////////////////////////////////////
// QCCBFSRouting Class Declaration
///////////////////////////////////////////////////////////////////////
namespace qcc {

/**
 * Initialize the QCCBFSRouting
 *
 * @param layout the abstract layout
 * @param qodg QODG Analysis Pass
 * @param machineDescription QCC MachineDescription
 * @param multigateAnalysis MultiGate Analysis Pass
 *
 */
QCCBFSRouting::QCCBFSRouting(QCCLayout *layout, QODGAnalysis &qodg,
                             QCCMachineDescription &machineDescription,
                             MultiGateAnalysisPass &multigateAnalysis)
    : layout(layout), qodg(qodg), machineDescription(machineDescription) {

  multigateLastID = multigateAnalysis.getLastID();
}

/**
 * This method perform the Breadth First Search (BFS) algorithm
 *
 * @param starter the start of physical qubits
 * @param ender the distination physical qubits\
 *
 * @return vector : a path from stater to ender
 *
 */
std::vector<int> QCCBFSRouting::performBFS(int starter, int ender) {
  std::vector<bool> marked;
  list<list<int>> queue;
  list<int> path;
  std::vector<int> empty;

  marked.assign(machineDescription.getSizeOfSquareTopology(), false);
  queue.push_back({starter});

  while (!queue.empty()) {

    path = queue.front();
    queue.pop_front();

    int vertex = path.back();

    if (vertex == ender) {

      std::vector pathVector(path.begin(), path.end());

      return pathVector;
    } else if (!marked[vertex]) {

      for (int currentNeighbor :
           machineDescription.getDiagonalNeighborNodes(vertex)) {
        list<int> newPath = path;
        newPath.push_back(currentNeighbor);

        queue.push_back(newPath);

        if (currentNeighbor == ender) {
          std::vector pathVector(newPath.begin(), newPath.end());
          return pathVector;
        }
      }

      marked[vertex] = true;
    }
  }

  return empty;
}

/**
 * This method perform qubit routing.
 * 1. find multi gates (CX, SWAP) gate that has far connection ( can't be
 * mapped)
 * 2. apply BFS algorithm
 * 3. iterate the BFS's path
 * 4. each iteration, apply swaps, CX/SWAP, and last reverse swaps
 *
 * Note: in our case we apply reverse swaps first because the instructions gate
 * will be manipulate by FILO (First In Last Out)
 */
void QCCBFSRouting::perform() {

  bool Debug = false;

  multigateLastID++;

  // 1. find multi gates (CX, SWAP) gate
  for (unsigned idxNode = 0; idxNode < qodg.QODNodes.size(); idxNode++) {

    auto node = qodg.QODNodes[idxNode];

    if (isa<MultiGateInst>(node->gates[0])) {

      assert(node->gates.size() >= 2);

      int RID0 = node->qubitIndex[0];
      int RID1 = node->qubitIndex[1];

      auto TID0 = layout->getPhysicalQubitBy(RID0);
      auto TID1 = layout->getPhysicalQubitBy(RID1);

      optional<int> result =
          machineDescription.getConnectionDefinition(TID0, TID1);

      //
      // if the gates can't be mapped
      //
      if (!result) {
        if (Debug) {
          cout << "\n\nCNOT(" << to_string(TID0) << ", " << to_string(TID1)
               << ")\n";
          node->gates[0]->dump();
          node->gates[1]->dump();

          cout << "--------------\n";
        }

        //
        // 2. apply BFS algorithm
        //
        vector<int> pathVector = performBFS(TID0, TID1);

        // Calculate:
        // - SWAP path
        // - CNOT path
        // - Reverse path
        vector<int> swapPathVector(pathVector.begin(), pathVector.end() - 1);
        std::reverse(swapPathVector.begin(), swapPathVector.end());
        vector<int> cnotPathVector(pathVector.end() - 2, pathVector.end());

        // Apply Reverse SWAP PATH
        if (Debug) {
          cout << "\n - Reverse SWAP PATH \n";
        }

        // insert reverse swaps
        for (int idx = swapPathVector.size() - 1; idx > 0; idx--) {
          if (idx - 1 < 0) {
            break;
          }

          auto sRegID = layout->getVirtualQubitBy(swapPathVector[idx]);
          auto dRegID = layout->getVirtualQubitBy(swapPathVector[idx - 1]);

          qodg.insertSWAPNodeBefore(sRegID, dRegID, multigateLastID, idxNode);
          multigateLastID++;

          if (Debug) {
            std::cout << to_string(swapPathVector[idx]) << " -> "
                      << to_string(swapPathVector[idx - 1]) << endl;
          }
        }

        // inert CNOT or SWAP
        if (Debug) {
          cout << "\n - CNOT PATH \n";
          std::cout << to_string(cnotPathVector[0]) << " -> "
                    << to_string(cnotPathVector[1]) << endl;
        }

        auto sRegID = layout->getVirtualQubitBy(cnotPathVector[0]);
        auto dRegID = layout->getVirtualQubitBy(cnotPathVector[1]);

        if (isa<SwapGateInst>(node->gates[0])) {
          qodg.insertSWAPNodeBefore(sRegID, dRegID, multigateLastID, idxNode);
        } else {
          qodg.insertCNOTNodeBefore(sRegID, dRegID, multigateLastID, idxNode);
        }
        multigateLastID++;

        // insert swaps
        if (Debug) {
          cout << "\n - SWAP PATH \n";
        }
        for (unsigned idx = 0; idx < swapPathVector.size(); idx++) {
          if (idx + 1 >= swapPathVector.size()) {
            break;
          }
          auto sRegID = layout->getVirtualQubitBy(swapPathVector[idx]);
          auto dRegID = layout->getVirtualQubitBy(swapPathVector[idx + 1]);

          qodg.insertSWAPNodeBefore(sRegID, dRegID, multigateLastID, idxNode);
          multigateLastID++;

          if (Debug) {
            std::cout << to_string(swapPathVector[idx]) << " -> "
                      << to_string(swapPathVector[idx + 1]) << endl;
          }
        }

        // delete the old CX/SWAP
        qodg.deleteNodeBy(node);

        if (Debug) {
          cout << "";
          cout << "||||||||||||||||||" << endl;
        }
      }
    }
  }
}

} // namespace qcc