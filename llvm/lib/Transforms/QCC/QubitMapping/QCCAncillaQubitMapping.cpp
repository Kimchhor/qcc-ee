/*********************************************************************
 *   QCCAncillaQubitMapping.cpp:
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

#include "llvm/Transforms/QCC/QubitMapping/QCCAncillaQubitMapping.h"

///////////////////////////////////////////////////////////////////////
// QCCAncillaQubitMapping Class Declaration
///////////////////////////////////////////////////////////////////////
namespace qcc {

/**
 * This method init the QCCBFSQubitMapping object
 *
 * @param machineDescription QCC MachineDescription.
 * @param qodg the QODG Analysis pass
 * @param layout the QCC Layout
 *
 */
QCCAncillaQubitMapping::QCCAncillaQubitMapping(
    QCCMachineDescription &machineDescription, QODGAnalysis &qodg,
    QCCLayout *layout)
    : machineDescription(machineDescription), qodg(qodg), layout(layout) {}

/**
 * This method is used to compute min or max based on binaryCode
 *
 * @param a int it must be number
 * @param b int it must be number
 * @param binaryCode The navigatoer to use min or max
 * @return int the result from min or max
 *
 */
int QCCAncillaQubitMapping::performMinMaxBinary(int a, int b, int binaryCode) {
  return binaryCode ? max(a, b) : min(a, b);
}

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
std::pair<int, int> QCCAncillaQubitMapping::findAnicillaAxis(qcc::Node rNode,
                                                             qcc::Node lNode,
                                                             int cateCode,
                                                             int connCode) {
  auto x = performMinMaxBinary(rNode.x, lNode.x, connCode);
  auto z = performMinMaxBinary(rNode.z, lNode.z, cateCode ^ connCode);
  return std::make_pair(x, z);
}

/**
 * This method performs the Trivial layout then BFS routhing method
 *
 */
void QCCAncillaQubitMapping::perform() {

  for (auto node : qodg.QODNodes) {

    if (node->type == multi) { // check on multi-qubit (CNOT and SWAP)
      // Get physical qubit
      auto cQID = layout->getPhysicalQubitBy(node->qubitIndex[0]);
      auto tQID = layout->getPhysicalQubitBy(node->qubitIndex[1]);

      // find the axis by QID
      // get node containing PQID
      auto cNode = machineDescription.getNodeBy(cQID);
      auto tNode = machineDescription.getNodeBy(tQID);

      // find position
      if (cNode && tNode) {
        // compute the category code
        auto sub_x = (int)cNode->x - (int)tNode->x;
        auto sub_z = (int)cNode->z - (int)tNode->z;
        int categoryCode = std::abs(sub_x + sub_z) / 2;

        // get the connection
        auto connPtr = machineDescription.getConnectionDefinition(cQID, tQID);
        if (!connPtr.has_value()) {
          cout << "There is no conenction information about " << cQID << " - "
               << tQID << endl;
        }
        int connectionCode = *connPtr;

        // find ancilla's axis
        auto [aX, aZ] =
            findAnicillaAxis(*cNode, *tNode, categoryCode, connectionCode);
        auto [bX, bZ] =
            findAnicillaAxis(*cNode, *tNode, categoryCode, !connectionCode);

        // NodesID = (N x z) + x
        auto selectedAncillaID =
            (machineDescription.getSizeOfToplogy() * aZ) + aX;
        auto anotherAncillaID =
            (machineDescription.getSizeOfToplogy() * bZ) + bX;

        // getting stabilizer 1 odd X, 0 even Z,
        int stabilizer = (aX % 2) ? 0 : 1; // 0 is X, 1 is Z

        // Set Ancilla to NodeGate
        node->ancilla = new NodeGate::AncillaQubit(
            selectedAncillaID, anotherAncillaID, stabilizer, categoryCode);
      }
    }
  }
}

} // namespace qcc