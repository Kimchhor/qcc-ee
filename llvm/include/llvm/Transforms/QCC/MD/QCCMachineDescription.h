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

#ifndef QCC_QCCMACHINEDESCRIPTION
#define QCC_QCCMACHINEDESCRIPTION

#include <iostream>
#include <map>
#include <optional>

#include "llvm/Transforms/QCC/MD/MachineDescription.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QODGAnalysis Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCMachineDescription {

  int sizeOfCroppedTopology;
  int numberOfRegisterQubits;
  int sizeOfToplogy;

  std::vector<Node> dataNodes;
  std::vector<Node> ancillaNodes;
  std::map<int, Node> nodes;

  std::vector<std::vector<int>> connectionAdjMatrix;

  std::map<int, int> physical2VirtualQubitMap;
  std::map<int, int> vritual2physicalQubitMap;
  MachineDescription &MD;

  /**
   * This method will crop (nxn) from (NxN) by the number of register (virtual)
   * qubits.
   *
   * @param N is the size of checkerboard (NxN)
   * @param numberOfRegisterQubit is the number of register (virtual) qubits
   */
  int calulateCropN(int N, int numberOfRegisterQubit);

  /**
   * This method will define the connection of physical qubits.
   */
  void setupConnectionNodeMatrix();

  /**
   * This method will emit the nodes (physical qubits) that will be used.
   */
  void setupCroppedNodes();

  /**
   * This method will init the size of checkerboard toplogy
   */
  void setupSize();

  /**
   * This method will return the number of data virtual qubit
   *
   * @param N is the size of checkerboard (NxN)
   */
  inline int getNumberOfDataQubit(int N);

public:
  using NodePair = std::pair<unsigned, unsigned>;
  using Vector2D = std::vector<std::vector<int>>;
  template <typename T> using PairConnectivity = std::map<NodePair, T>;

  PairConnectivity<Boundary> edges;
  PairConnectivity<unsigned> connectivities;

  Topology topology;

  /**
   * This method will init the QCCMachineDescirption object
   *
   * @param numberOfRegisterQubits the number of virtual qubits
   * @param MD Machine Description
   */
  QCCMachineDescription(int numberOfRegisterQubits, MachineDescription &MD);

  ///////////////////////////////////////////////////////////////////////
  // Member Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will return the boundary from 2 physical qubit.
   *
   * @param rPhysicalQubit the physical qubits in right.
   * @param lPhysicalQubit the physical qubits in left.
   *
   * @return Boundary in { Z, REVERSE_Z, X, REVERSE_X }
   */
  std::optional<Boundary> getEdgeDefinition(int rnID, int lnID);

  std::optional<Boundary> getEdgeDefinition(Node rn, Node ln) {
    return getEdgeDefinition(ln.id, rn.id);
  }

  /**
   * This method will return the connection from 2 physical qubit.
   *
   * @param rPhysicalQubit the physical qubits in right.
   * @param lPhysicalQubit the physical qubits in left.
   *
   * @return binary (0: forward, 1: reverse)
   */
  std::optional<unsigned> getConnectionDefinition(int rnID, int lnID);

  std::optional<unsigned> getConnectionDefinition(Node rn, Node ln) {
    return getConnectionDefinition(ln.id, rn.id);
  }

  /**
   * This method will get the qubit node by the physical qubit ID.
   *
   * @param ID the physical qubits ID.
   *
   * @return Node
   */
  Node *getNodeBy(int ID);

  /** This method is used to get Date Node*/
  inline std::vector<Node> getDataNodes() { return dataNodes; };

  /** This method is used to get Ancilla Node*/
  inline std::vector<Node> getAncillaNodes() { return ancillaNodes; };

  /**
   * This method is used to get Size of toplogy.
   * E.g,. Topology NxN then it will return N
   */
  inline int getSizeOfToplogy() { return sizeOfToplogy; };

  /**
   * This method will return the size of croppted toplogy.
   */
  inline int getSizeOfCroppedTopology() { return sizeOfCroppedTopology; };
  inline int getSizeOfSquareTopology() {
    return (sizeOfToplogy * sizeOfToplogy);
  };

  /**
   * This method will return the physical qubit ID
   * @param virtualQID the virtual Qubit ID
   * @return Physical qubit ID type as int
   */
  inline int getPhysicalQubitBy(int virtualQID) {
    return vritual2physicalQubitMap[virtualQID];
  }

  /**
   * This method will return the virtual qubit ID
   * @param virtualQID the physical Qubit ID
   * @return Virtual qubit ID type as int
   */
  inline int getVirtualQubitBy(int physicalQID) {
    return physical2VirtualQubitMap[physicalQID];
  }

  Vector2D getConnectionAdjMatrix() { return connectionAdjMatrix; };

  /**
   * This method will get the diagnal neightbor of physical qubit.
   *
   * @param PhysicalQubit the physical qubits in right.
   * @return binary (0: forward, 1: reverse)
   */
  std::vector<int> getDiagonalNeighborNodes(int nodeID);

  /**
   * This static method create QCCMachineDescription object
   *
   * @param path the path to machine description json format
   * @param numberOfRegisterQubit the number of register (virtual) qubits
   *
   * @return QCCMachineDescription object
   */
  static QCCMachineDescription &create(std::string path,
                                       int numberOfRegisterQubit);
};

} // namespace qcc

#endif