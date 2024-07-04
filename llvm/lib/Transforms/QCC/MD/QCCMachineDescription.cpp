/*********************************************************************
 *   QCCMachineDescription.h:
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

#include "llvm/Transforms/QCC/MD/QCCMachineDescription.h"

///////////////////////////////////////////////////////////////////////
// QCCMachineDescription Class Declaration
///////////////////////////////////////////////////////////////////////
namespace qcc {

/**
 * This method will init the QCCMachineDescirption object
 *
 * @param numberOfRegisterQubits the number of virtual qubits
 * @param MD Machine Description
 */
QCCMachineDescription::QCCMachineDescription(int numberOfRegisterQubits,
                                             MachineDescription &MD)
    : numberOfRegisterQubits(numberOfRegisterQubits), MD(MD) {
  for (auto edge : MD.topology.edges) {
    edges[{edge.src, edge.dst}] = *(edge.boundary);
  }

  for (auto con : MD.topology.connectivities) {
    connectivities[{con.src, con.dst}] = *(con.direction);
  }

  if (MD.topology.z_size != MD.topology.x_size)
    assert("Only Square toplogy is supported");

  if (getNumberOfDataQubit(MD.topology.x_size) < numberOfRegisterQubits) {
    assert("The number of qubits are overflow");
  }

  // init the size of checkerboard toplogy
  setupSize();

  // emit the nodes (physical qubits) that will be used.
  setupCroppedNodes();

  // define the connection of physical qubits
  setupConnectionNodeMatrix();
}

/**
 * This method will init the size of checkerboard toplogy
 */
void QCCMachineDescription::setupSize() {
  sizeOfToplogy = MD.topology.x_size;
  sizeOfCroppedTopology = calulateCropN(sizeOfToplogy, numberOfRegisterQubits);
}

/**
 * This method will emit the nodes (physical qubits) that will be used.
 */
void QCCMachineDescription::setupCroppedNodes() {

  int registerID = 0;

  for (unsigned i = 0; i < (unsigned)sizeOfCroppedTopology; i++) {
    for (unsigned j = 0; j < (unsigned)sizeOfCroppedTopology; j++) {
      for (auto node : MD.topology.nodes) {
        if (node.x == j && node.z == i) {

          nodes[node.id] = node;

          // if x + y is even, it's data qubit
          if ((node.x + node.z) % 2 == 0) {
            // node.id is Topology ID
            // registerID is Register ID

            physical2VirtualQubitMap[node.id] = registerID;
            vritual2physicalQubitMap[registerID] = node.id;

            registerID++;
            dataNodes.push_back(node);
          } else {
            // if x + y is odd, it's ancilla qubit
            ancillaNodes.push_back(node);
          }
        }
      }
    }
  }
}

/**
 * This method will define the connection of physical qubits.
 */
void QCCMachineDescription::setupConnectionNodeMatrix() {

  int sizeOf2N = MD.topology.x_size * MD.topology.z_size;

  std::vector<int> row;
  row.assign(sizeOf2N, 0);
  connectionAdjMatrix.assign(sizeOf2N, row);

  std::vector<std::tuple<int, int>> diagonalDirections;
  diagonalDirections.push_back({-1, -1});
  diagonalDirections.push_back({-1, 1});
  diagonalDirections.push_back({1, -1});
  diagonalDirections.push_back({1, 1});

  for (auto currentNode : dataNodes) {
    for (auto adj : diagonalDirections) {
      int x = currentNode.x + std::get<0>(adj);
      int z = currentNode.z + std::get<1>(adj);

      // check invalid axis
      if (x < 0 || z < 0)
        continue;

      // check x and y must be in Cropped toplogy
      if (x >= sizeOfCroppedTopology || z >= sizeOfCroppedTopology)
        continue;

      int comparephysicalQubit = (sizeOfToplogy * z) + x;

      std::optional<unsigned> direction =
          getConnectionDefinition(currentNode.id, comparephysicalQubit);

      if (direction) {
        connectionAdjMatrix[currentNode.id][comparephysicalQubit] = 1;
      }
    }
  }
}

/**
 * This method will return the number of data virtual qubit
 *
 * @param N is the size of checkerboard (NxN)
 */
inline int QCCMachineDescription::getNumberOfDataQubit(int N) {
  // The algorithm refers to ETRI's doc v0.2
  return ((((N * N) + (N % 2 == 1)) / 2));
}

/**
 * This method will crop (nxn) from (NxN) by the number of register (virtual)
 * qubits.
 *
 * @param N is the size of checkerboard (NxN)
 * @param numberOfRegisterQubit is the number of register (virtual) qubits
 */
int QCCMachineDescription::calulateCropN(int N, int numberOfRegisterQubit) {
  for (int i = 1; i < N + 1; i++) {
    if (getNumberOfDataQubit(i) >= numberOfRegisterQubit) {
      return i;
    }
  }
  return 0;
}

/**
 * This method will return the boundary from 2 physical qubit.
 *
 * @param rPhysicalQubit the physical qubits in right.
 * @param lPhysicalQubit the physical qubits in left.
 *
 * @return Boundary in { Z, REVERSE_Z, X, REVERSE_X }
 */
std::optional<Boundary>
QCCMachineDescription::getEdgeDefinition(int rPhysicalQubit,
                                         int lPhysicalQubit) {
  if (edges.count({rPhysicalQubit, lPhysicalQubit}) > 0) {
    return edges[{rPhysicalQubit, lPhysicalQubit}];
  }
  return std::nullopt;
}

/**
 * This method will return the connection from 2 physical qubit.
 *
 * @param rPhysicalQubit the physical qubits in right.
 * @param lPhysicalQubit the physical qubits in left.
 *
 * @return binary (0: forward, 1: reverse)
 */
std::optional<unsigned>
QCCMachineDescription::getConnectionDefinition(int rPhysicalQubit,
                                               int lPhysicalQubit) {
  if (connectivities.count({rPhysicalQubit, lPhysicalQubit}) > 0) {
    return connectivities[{rPhysicalQubit, lPhysicalQubit}];
  }
  return std::nullopt;
}

/**
 * This method will get the diagnal neightbor of physical qubit.
 *
 * @param PhysicalQubit the physical qubits in right.
 *
 * @return binary (0: forward, 1: reverse)
 */
std::vector<int>
QCCMachineDescription::getDiagonalNeighborNodes(int physicalQubit) {

  std::vector<int> neighbors;

  for (unsigned i = 0; i < connectionAdjMatrix[physicalQubit].size(); i++) {
    if (connectionAdjMatrix[physicalQubit][i] == 1) {
      neighbors.push_back(i);
    }
  }

  return neighbors;
}

/**
 * This method will get the qubit node by the physical qubit ID.
 *
 * @param ID the physical qubits ID.
 *
 * @return Node
 */
Node *QCCMachineDescription::getNodeBy(int ID) {
  if (nodes.count(ID) > 0) {
    return &nodes[ID];
  }
  return nullptr;
}

/**
 * This static method create QCCMachineDescription object
 *
 * @param path the path to machine description json format
 * @param numberOfRegisterQubit the number of register (virtual) qubits
 *
 * @return QCCMachineDescription object
 */
QCCMachineDescription &
QCCMachineDescription::create(std::string path, int numberOfRegisterQubit) {

  // Parse Marchine Description
  std::string jsonString = qcc::readFileIntoString(path);

  qcc::MachineDescription MD;
  json jj = json::parse(jsonString);
  qcc::from_json(jj, MD);

  QCCMachineDescription *QMD =
      new QCCMachineDescription(numberOfRegisterQubit, MD);

  return *QMD;
}

} // namespace qcc
