/*********************************************************************
 *   QODGAnalysis.h:
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

#ifndef LLVM_TRANSFORMS_QODGANALYSIS_QODGANALYSIS_H
#define LLVM_TRANSFORMS_QODGANALYSIS_QODGANALYSIS_H

#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/QCC/NodeGate.h"

#include "set"
#include "tuple"

namespace llvm {

///////////////////////////////////////////////////////////////////////
// QODGAnalysis Class Declaration
///////////////////////////////////////////////////////////////////////

class QODGAnalysis : public PassInfoMixin<QODGAnalysis> {

  ///////////////////////////////////////////////////////////////////////
  // Member Private Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will generate the Register ID depond on IDs in list
   *
   * @param venctor<NodeGate*> a list of register (classical or qubit).
   * @return a new ID.
   */
  int generateRegisterID(vector<NodeGate *> regs);

  /**
   * This method will create the register node of qubit or classical allocation.
   * it will work on AllocaInst Type only.
   *
   * @param AllocaInst a llvm instruction. e.g,. %1 = alloca (q1 or i1).
   * @param NodeGate a new register node.
   */
  void createRegisterNode(AllocaInst *AI, NodeGate &node);

  LLVMContext *context;

public:
  /** The type of muli-qubit gate */
  enum MultiGateType {
    /** CNOT gate */
    cnot,
    /** SWAP gate */
    swap
  };

  struct IntervalRegisterLocInfo {
    std::pair<int, int> liveInterval;
    int physicalRegister;
    int ancillaPhysicalRegister;
    int mergePhysicalRegister; // Reserved for CNOT

    IntervalRegisterLocInfo() {
      liveInterval = std::make_pair(-1, -1);
      physicalRegister = -1;
      ancillaPhysicalRegister = -1;
      mergePhysicalRegister = -1;
    }
  };

  vector<NodeGate *> quantumRegisters;
  vector<NodeGate *> classicalRegisters;
  vector<NodeGate *> QODNodes;

  /** The type of dictionary that has NodeGate ptr as key and pair of beginning
   * to end interval of that node as value
   */
  using LiveInterval = std::map<NodeGate *, std::pair<int, int>>;

  /** The tyep of dictionary taht has NodeGate as ptr and
   * IntervalRegisterLocInfo as value.
   */
  using IntervalRegLocRichInfo = std::map<NodeGate *, IntervalRegisterLocInfo>;

  ///////////////////////////////////////////////////////////////////////
  // Member Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will run the pass
   */
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  /**
   * This method will create new NodeGate object using Instuction and qubitIndex
   *
   * @param inst the llvm instruction. e.g,. %2 = h q1* %1.
   * @param qubitIndex the postion of qubit that gate applied on.
   */
  NodeGate *createNode(Instruction &inst, unsigned qubitIndex);

  /**
   * This method will update the previous node if it would contain another
   * instruction. e.g,. if a Node (N) contains CX0 instruction, so it must be
   * constain CX1.
   *
   * @param node a previous node.
   * @param inst the another pair of instructions
   * @param qubitIndex the postion of qubit that gate applied on.
   */
  void updateNode(NodeGate *node, Instruction &inst, unsigned qubitIndex);

  // void insertBefore(NodeGate node, NodeGate newNode);
  // void insertAfter(NodeGate node, NodeGate newNode);
  // void append(NodeGate node);

  /**
   * This method will find the Node by register ID in before specific index ID
   *
   * @param beforeIndexNode the index node
   * @param byRegisterID The register ID we want to find
   * @return NodeGate* the node
   * @return int the index of gates in node
   */
  std::tuple<NodeGate *, unsigned> findNodeBefore(int beforeIndexNode,
                                                  int byRegisterID);

  Instruction *findInstBefore(int beforeIndexNode, int byRegisterID) {
    auto [node, id] = findNodeBefore(beforeIndexNode, byRegisterID);
    if (node) {
      return node->gates[id];
    }
    return nullptr;
  };

  /**
   * This method will find the Node by register ID in after specific index ID
   *
   * @param afterIndexNode the index node
   * @param byRegisterID The register ID we want to find
   * @return NodeGate* the node
   * @return int the index of gates in node
   */
  std::tuple<NodeGate *, unsigned> findNodeAfter(int afterIndexNode,
                                                 int byRegisterID);

  /**
   * This method will find the Node by register ID in after specific index ID
   *
   * @param afterIndexNode the index node
   * @param byRegisterID The register ID we want to find
   * @return NodeGate* the node
   * @return the instruction
   */
  Instruction *findInstAfter(int afterIndexNode, int byRegisterID) {

    auto [node, id] = findNodeAfter(afterIndexNode, byRegisterID);

    if (node) {
      return node->gates[id];
    }
    return nullptr;
  };

  /**
   * This method will find the quantum register by virtual ID
   *
   * @param virtualQID a virtual Qubit ID
   * @return Register Node
   */
  NodeGate *findRegisterNode(int virtualQID);

  /**
   * This method will insert multi-qubit gate (nodes) before the specific
   * register IDs (virtual QID)
   *
   * @param sRegID the source of multi-qubit gate ID. e.g,. (Control or Target
   * gate)
   * @param dRegID the direction of multi-qubit gate ID. e.g,. (Target or
   * Control gate)
   * @param multiGateID the new virtual ID of multi-qubit gate node, need to
   * generate this.
   * @param beforeNodeIndex the virtual ID that it will insert before
   * @param multiGateType the type of multi-qubit gate. e.g,. CNOT or SWAP
   *
   */
  void insertMultiNodeBefore(unsigned sRegID, unsigned dRegID, int multiGateID,
                             int beforeNodeIndex, MultiGateType multiGateType);

  /**
   * This method will insert SWAP before the specific register IDs (virtual QID)
   *
   * @param sRegID the source of multi-qubit gate ID. It doesn't matter which
   * one is which.
   * @param dRegID the direction of multi-qubit gate ID. It doesn't matter which
   * one is which.
   * @param multiGateID the new virtual ID of SWAP gate node, need to generate
   * this.
   * @param beforeNodeIndex the virtual ID that it will insert before
   *
   */
  void insertSWAPNodeBefore(unsigned sRegID, unsigned dRegID, int multiGateID,
                            int beforeNdoeIndex);

  /**
   * This method will insert CNOT before the specific register IDs (virtual QID)
   *
   * @param sRegID the control of multi-qubit gate ID.
   * @param dRegID the target of multi-qubit gate ID.
   * @param multiGateID the new virtual ID of CNOT gate node, need to generate
   * this.
   * @param beforeNodeIndex the virtual ID that it will insert before
   *
   */
  void insertCNOTNodeBefore(unsigned sRegID, unsigned dRegID, int multiGateID,
                            int beforeNdoeIndex);

  /**
   * This method will compute the lienar schedule (Depth) of circuit
   *
   * @return 2D vector of node, size of this 1D array is the depth
   */
  std::vector<std::vector<NodeGate *>> getLinearScheduleDepth();

  /**
   * This method will compute global live interval of each quantum register
   * node.
   *
   * @return all of quantum registers each of which has the interval value
   */
  LiveInterval getGlobalLiveInterval();

  /**
   * This method will print the live interval
   */
  void dumpLiveInterval();

  /**
   * This method will compute live interval and assign the physical register.
   * This algorithm is inspired by Linear-scan register allocation.
   *
   * @param numberOfPhysicalRegisters the number of register in real device
   * @return IntervalRegLocRichInfo the map of NodeGate as key
   * and IntervalRegisterLocInfo as value.
   */
  IntervalRegLocRichInfo
  getScanIntervalRegLoc(unsigned numberOfPhysicalRegisters,
                        unsigned mergeRegister, unsigned ancillaRegister);

  /**
   * This method will print the live interval with register location assignment
   */
  void dumpScanIntervalRegLoc(unsigned numberOfPhysicalRegisters,
                              unsigned mergeRegister, unsigned ancillaRegister);

  /**
   * This method will filter the nodes to get only operate gates
   *
   * @return NodeGates that has only operate gates
   */
  vector<NodeGate *> getOpNodes();

  /**
   * This method will safety delete node including LLVM IR instruction by the
   * node index
   */
  void deleteNodeBy(int nodeIndex);

  /**
   * This method will safety delete node including LLVM IR instruction fron
   * QODNodes
   */
  void deleteNodeBy(NodeGate *nodeGate);
};

} // namespace llvm

#endif