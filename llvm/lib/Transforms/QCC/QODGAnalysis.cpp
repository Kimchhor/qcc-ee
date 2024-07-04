/*********************************************************************
 *   QODGAnalysis.cpp:
 *
 *   Authors:
 *      Sengthai Heng (sengthai37@gmail.com)
 *      Youngsun Han  (youngsun@pknu.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/

#include "llvm/Transforms/QCC/QODGAnalysis.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include <functional>
// inlcude Instruction class
#include "llvm/IR/Instruction.h"

using namespace llvm;

#define DEBUG_TYPE "qodg-ia"

/**
 * This method will generate the Register ID depond on IDs in list
 *
 * @param venctor<NodeGate*> a list of register (classical or qubit).
 * @return a new ID.
 */
int QODGAnalysis::generateRegisterID(vector<NodeGate *> regs) {
  if (!regs.empty()) {
    return regs.back()->qubitIndex[0] + 1;
  }
  return 0;
}

/**
 * This method will create the register node of qubit or classical allocation.
 * it will work on AllocaInst Type only.
 *
 * @param AllocaInst a llvm instruction. e.g,. %1 = alloca (q1 or i1).
 * @param NodeGate a new register node.
 */
void QODGAnalysis::createRegisterNode(AllocaInst *AI, NodeGate &node) {

  // type gate
  Type *val = AI->getAllocatedType();

  if (val->isIntegerTy()) {
    // set type
    node.type = gateType::c_register;
    // set id
    node.qubitIndex.push_back(generateRegisterID(classicalRegisters));
    // push to classicalRegisters
    classicalRegisters.push_back(&node);
  } else if (val->isQubitTy()) {
    // set to root
    node.isRoot = true;
    // set type
    node.type = gateType::q_register;
    // set id
    node.qubitIndex.push_back(generateRegisterID(quantumRegisters));
    // push to quantumRegisters
    quantumRegisters.push_back(&node);
  } else if (val->isArrayTy()) {
    // set to root
    node.isRoot = true;
    // set type
    node.type = gateType::q_register;
    // set id
    node.qubitIndex.push_back(generateRegisterID(quantumRegisters));
    // push to quantumRegisters
    quantumRegisters.push_back(&node);

    // if val is array
      // if element type is qubit
         // create qubit registers
      // if element type is classical
         // create classical registers
  } else {
    errs() << "Error alloca type";
  }
}

/**
 * This method will create new NodeGate object using Instuction and qubitIndex
 *
 * @param Instruction the llvm instruction. e.g,. %2 = h q1* %1.
 * @param unsigned the postion of qubit that gate applied on.
 */
NodeGate *QODGAnalysis::createNode(Instruction &inst, unsigned qubitIndex) {

  NodeGate *node = new NodeGate();

  node->name = StringRef(inst.getOpcodeName()).str();

  // set index
  node->qubitIndex.push_back(qubitIndex);

  // set instruction type
  if (isa<MultiGateInst>(inst)) {
    node->type = gateType::multi;
    errs() << "DEBUG: MultiGateInst Node: " << node << "\n";
  } 
  if (isa<GateInst>(inst)) {
    node->type = gateType::single;
    errs() << "DEBUG: GateInst Node: " << node << "\n";
    // set rotation value
    if (RotationGateInst *RGI = dyn_cast<RotationGateInst>(&inst)) {
      node->rotationValue = RGI->getRotationValue();
      node->type = gateType::rotation;
      errs() << "DEBUG: RotationGateInst Node: " << node << "\n";
    }
  } 
  if (isa<XGateInst>(inst)) {
    node->type = gateType::multi;
    errs() << "DEBUG: XGateInst Node: " << node << "\n";
  }
  if (isa<CtrlGateInst>(inst)) {
    node->type = gateType::multi;
    errs() << "DEBUG: CtrlGateInst Node: " << node << "\n";
  } 
  if (isa<MeasXGateInst>(inst) || isa<MeasYGateInst>(inst) || isa<MeasZGateInst>(inst)) {
    node->type = gateType::measure;
    errs() << "DEBUG: MeasGateInst Node: " << node << "\n";
  }

  // set gate
  node->gates.push_back(&inst);
  return node;
}

/**
 * This method will find the quantum register by virtual ID
 *
 * @param virtualQID a virtual Qubit ID
 * @return Register Node
 */
NodeGate *QODGAnalysis::findRegisterNode(int virtualQID) {
  if (virtualQID < (int)quantumRegisters.size()) {
    auto registerNode = quantumRegisters[virtualQID];
    if (virtualQID == (int)registerNode->qubitIndex[0]) {
      return registerNode;
    } else {
      for (auto node : quantumRegisters) {
        if (virtualQID == (int)node->qubitIndex[0]) {
          return node;
        }
      }
    }
  }
  return nullptr;
}

/**
 * This method will update the previous node if it would contain another
 * instruction. e.g,. if a Node (N) contains CX0 instruction, so it must be
 * constain CX1.
 *
 * @param NodeGate* a previous node.
 * @param Instruction& the another pair of instructions
 * @param unsigned the postion of qubit that gate applied on.
 */
void QODGAnalysis::updateNode(NodeGate *node, Instruction &inst,unsigned qubitIndex) {
  node->qubitIndex.push_back(qubitIndex);
  node->gates.push_back(&inst);
}

/**
 * This method will find the Node by register ID in before specific index ID
 *
 * @param beforeIndexNode the index node
 * @param byRegisterID The register ID we want to find
 * @return NodeGate* the node
 * @return int the index of gates in node
 */
std::tuple<NodeGate *, unsigned>
QODGAnalysis::findNodeBefore(int beforeIndexNode, int byRegisterID) {

  // assert(beforeIndexNode <  QODNodes.size());

  for (int i = beforeIndexNode - 1; i >= 0; i--) {

    auto &node = QODNodes[i];

    for (unsigned indexGate = 0; indexGate < node->qubitIndex.size();
         indexGate++) {
      if ((int)node->qubitIndex[indexGate] == byRegisterID &&
          node->type != c_register) {
        return std::make_tuple(node, indexGate);
      }
    }
  }

  return std::make_tuple(nullptr, 0);
}

/**
 * This method will find the Node by register ID in after specific index ID
 *
 * @param afterIndexNode the index node
 * @param byRegisterID The register ID we want to find
 * @return NodeGate* the node
 * @return int the index of gates in node
 */
std::tuple<NodeGate *, unsigned> QODGAnalysis::findNodeAfter(int afterIndexNode, int byRegisterID) {

  // assert(afterIndexNode <  QODNodes.size());

  for (unsigned i = afterIndexNode + 1; i < QODNodes.size(); i++) {

    auto &node = QODNodes[i];

    for (unsigned indexGate = 0; indexGate < node->qubitIndex.size();
         indexGate++) {
      if ((int)node->qubitIndex[indexGate] == byRegisterID &&
          node->type != c_register) {
        return std::make_tuple(node, indexGate);
      }
    }
  }

  return std::make_tuple(nullptr, 0);
}

/**
 * This method will filter the nodes to get only operate gates
 *
 * @return NodeGates that has only operate gates
 */
vector<NodeGate *> QODGAnalysis::getOpNodes() {
  std::vector<NodeGate *> opNodes;
  std::copy_if(QODNodes.begin(), QODNodes.end(), std::back_inserter(opNodes),
               [](NodeGate *node) {
                 return node->type != gateType::c_register &&
                        node->type != q_register;
               });
  return opNodes;
}

/**
 * This method will compute the lienar schedule (Depth) of circuit
 *
 * @return 2D vector of node, size of this 1D array is the depth
 */
std::vector<std::vector<NodeGate *>> QODGAnalysis::getLinearScheduleDepth() {
  std::vector<std::vector<NodeGate *>> linearScheduleNode;
  std::set<int> qubitIndexs;
  std::vector<NodeGate *> subListNodes;
  bool debug = false;

  for (auto node : QODNodes) {

    if (node->type == gateType::q_register || node->type == c_register)
      continue;

    if (debug) {
      cout << "GATE NAME : " << node->getOperationName();
      cout << "   : [";
      for (auto i : node->qubitIndex) {
        cout << i << ",";
      }
      cout << "]" << endl;
    }

    bool shouldInsert = true;

    // check each of qubit in node
    for (auto qubitIndex : node->qubitIndex) {
      auto ret = qubitIndexs.insert(qubitIndex);

      if (!ret.second) {
        shouldInsert = false;
        break;
      }
    }

    // Insert node to that vector
    subListNodes.push_back(node);

    if (!shouldInsert) {

      // remove the node that just added
      subListNodes.pop_back();

      // add sub list nodes to linearScheduleNode
      linearScheduleNode.push_back(subListNodes);

      // clear subListNodes
      subListNodes.clear();

      // clear qubitIndexs
      qubitIndexs.clear();

      // save the last state of the node
      qubitIndexs =
          std::set<int>(node->qubitIndex.begin(), node->qubitIndex.end());
      subListNodes.push_back(node);
    }
  }

  // add the rest of nodes to linearScheduleNodes
  linearScheduleNode.push_back(subListNodes);
  subListNodes.clear();
  qubitIndexs.clear();

  return linearScheduleNode;
}

/**
 * This method will compute global live interval of each quantum register node.
 *
 * @return all of quantum registers each of which has the interval value
 */
QODGAnalysis::LiveInterval QODGAnalysis::getGlobalLiveInterval() {

  int timeInterval = 0;

  LiveInterval liveInterval;

  // prepare live interval
  for (auto registerNode : quantumRegisters)
    liveInterval[registerNode] = std::make_pair(-1, -1);

  for (auto node : QODNodes) {

    // skip if it is register
    if (node->type == gateType::q_register ||
        node->type == gateType::c_register)
      continue;

    for (auto qubitIndex : node->qubitIndex) {
      auto registerNode = findRegisterNode(qubitIndex);

      if (liveInterval[registerNode].first < 0) {
        liveInterval[registerNode].first = timeInterval;
      }

      liveInterval[registerNode].second = timeInterval;
    }

    timeInterval++;
  }

  return liveInterval;
}

/**
 * This method will compute live interval and assign the physical register.
 * This algorithm is inspired by Linear-scan register allocation.
 *
 * @param numberOfPhysicalRegisters the number of register in real device
 * @return IntervalRegLocRichInfo the map of NodeGate as key
 * and IntervalRegisterLocInfo as value.
 */
QODGAnalysis::IntervalRegLocRichInfo
QODGAnalysis::getScanIntervalRegLoc(unsigned numberOfPhysicalRegisters,
                                    unsigned mergeRegister,
                                    unsigned ancillaRegister) {

  std::vector<int> registers;

  // set the register in descending
  for (unsigned i = numberOfPhysicalRegisters - 2; i > 0; i--) {
    // 2 registers are locked for merged and ancilla qubit.
    if (i == mergeRegister || i == ancillaRegister) {
      continue;
    }
    registers.push_back(i - 1);
  }

  // get global live interval
  auto liveIntervals = getGlobalLiveInterval();

  IntervalRegLocRichInfo expireRegisterIntervalRichInfo;
  IntervalRegLocRichInfo registerIntervalRichInfo;

  // filter QODNodes only instructions
  std::vector<NodeGate *> instructionNodes = getOpNodes();

  for (int interval = 0; interval < (int)instructionNodes.size(); interval++) {

    // for (auto intervalInfo: liveIntervals){
    for (auto it = liveIntervals.begin(); it != liveIntervals.end();) {
      std::pair<int, int> stepInterval = it->second;

      if (stepInterval.first <= interval && interval <= stepInterval.second) {

        // insert this intervalInfo to registerIntervalRichInfo
        auto irli = IntervalRegisterLocInfo();
        irli.liveInterval =
            std::make_pair(stepInterval.first, stepInterval.second);

        if (!registers.empty()) {
          // move the physical regiter to the intervalInfo
          irli.physicalRegister = registers.back();

          registers.pop_back();

          // set Merge register and ancilla register to used interval
          irli.mergePhysicalRegister = mergeRegister;
          irli.ancillaPhysicalRegister = ancillaRegister;

          // move register to that registerIntervalRichInfo
          registerIntervalRichInfo[it->first] = irli;

        } else {
          // TODO: need to release the register from previous qubit.
          errs() << "The virtual register exceed the physical registers";
        }

        liveIntervals.erase(it++);

      } else {
        ++it;
      }
    }

    // expire old intervals
    for (auto it = registerIntervalRichInfo.begin();
         it != registerIntervalRichInfo.end();) {
      auto endInterval = it->second.liveInterval.second;

      // expire interval
      if (endInterval <= interval) {
        // release interval
        // move the physical register back to registers
        registers.push_back(it->second.physicalRegister);

        // move to expireRegisterInterval
        expireRegisterIntervalRichInfo[it->first] = it->second;

        // remove from registerIntervalRichInfo
        registerIntervalRichInfo.erase(it++);
      } else {
        ++it;
      }
    }
  }

  return expireRegisterIntervalRichInfo;
}

/**
 * This method will print the live interval with register location assignment
 *
 */
void QODGAnalysis::dumpScanIntervalRegLoc(unsigned numberOfPhysicalRegisters,
                                          unsigned mergeRegister,
                                          unsigned ancillaRegister) {

  // filter QODNodes only instructions
  std::vector<NodeGate *> instructionNodes = getOpNodes();

  cout << "\n\n";
  cout << "\t Vitual Register Qubits\n";
  cout << "Line \t";

  for (auto registerNode : quantumRegisters) {
    cout << " " << registerNode->qubitIndex[0] << " ";
  }

  cout << endl;

  auto scanIntervalRegLoc = getScanIntervalRegLoc(
      numberOfPhysicalRegisters, mergeRegister, ancillaRegister);

  for (int timeInterval = 0; timeInterval < (int)instructionNodes.size();
       timeInterval++) {

    cout << "- " << timeInterval << " :\t";

    for (auto registerNode : quantumRegisters) {

      if (scanIntervalRegLoc[registerNode].liveInterval.first <= timeInterval &&
          timeInterval <=
              scanIntervalRegLoc[registerNode].liveInterval.second) {
        cout << " " << scanIntervalRegLoc[registerNode].physicalRegister << " ";
      } else {
        cout << " - ";
      }
    }

    cout << endl;
  }
}

/**
 * This method will print the live interval
 *
 */
void QODGAnalysis::dumpLiveInterval() {

  auto liveInterval = getGlobalLiveInterval();
  int timeInterval = 0;

  cout << "\n\n";
  cout << "\t Vitual Register Qubits\n";
  cout << "Line \t";
  for (auto registerNode : quantumRegisters) {
    cout << " " << registerNode->qubitIndex[0] << " ";
  }
  cout << endl;

  for (auto node : QODNodes) {

    if (node->type == gateType::q_register ||
        node->type == gateType::c_register)
      continue;

    cout << "- " << timeInterval << " :\t";

    for (auto registerNode : quantumRegisters) {

      if (liveInterval[registerNode].first <= timeInterval &&
          timeInterval <= liveInterval[registerNode].second) {
        cout << " | ";
      } else {
        cout << " - ";
      }
    }

    cout << endl;
    timeInterval++;
  }
}

/**
 * This method will insert multi-qubit gate (nodes) before the specific register
 * IDs (virtual QID)
 *
 * @param sRegID the source of multi-qubit gate ID. e.g,. (Control or Target
 * gate)
 * @param dRegID the direction of multi-qubit gate ID. e.g,. (Target or Control
 * gate)
 * @param multiGateID the new virtual ID of multi-qubit gate node, need to
 * generate this.
 * @param beforeNodeIndex the virtual ID that it will insert before
 * @param multiGateType the type of multi-qubit gate. e.g,. CNOT or SWAP
 *
 */
void QODGAnalysis::insertMultiNodeBefore(unsigned sRegID, unsigned dRegID,
                                         int multiGateID, int beforeNdoeIndex,
                                         MultiGateType multiGateType) {

  bool Debug = false;

  auto [startNode0, startID0] = findNodeBefore(beforeNdoeIndex, sRegID);
  auto [startNode1, startID1] = findNodeBefore(beforeNdoeIndex, dRegID);

  Instruction *startInst0 = startNode0->gates[startID0];
  Instruction *startInst1 = startNode1->gates[startID1];

  if (Debug) {
    cout << "Before 0 : " << to_string(sRegID) << endl;
    startInst0->dump();

    cout << "Before 1 : " << to_string(dRegID) << endl;
    startInst1->dump();
  }

  // This use to get Integer type in QODGAnalysis(_)

  Type *i32_type = IntegerType::getInt32Ty(*context);
  Value *gateID = ConstantInt::get(i32_type, multiGateID, true);

  // Insert swap
  // Instruction *c0 = SwapGateInst::Create(startInst0, gateID);
  // Instruction *c1 = SwapGateInst::Create(startInst1, gateID);
  Instruction *c0;
  Instruction *c1;

  switch (multiGateType) {
  case cnot:
    c0 = CX0GateInst::Create(startInst0, gateID);
    c1 = CX1GateInst::Create(startInst1, gateID);
    break;
  case swap:
    c0 = SwapGateInst::Create(startInst0, gateID);
    c1 = SwapGateInst::Create(startInst1, gateID);
    break;
  default:
    break;
  }

  if (Debug) {
    cout << "\n\n NEW INSTRUCTIONS: \n";
    c0->dump();
    c1->dump();
    cout << "\n ------------------- \n \n";
  }

  // Connect the successor Instructions

  auto [endNode0, endID0] = findNodeAfter(beforeNdoeIndex, sRegID);
  auto [endNode1, endID1] = findNodeAfter(beforeNdoeIndex, dRegID);

  Instruction *endInst0 = nullptr;
  Instruction *endInst1 = nullptr;

  if (endNode0) {
    endInst0 = endNode0->gates[endID0];
    endInst0->setOperand(0, c0);
    if (Debug) {
      cout << "END INST 0: \n";
      endInst0->dump();
      cout << endl;
    }
  }

  if (endNode1) {
    endInst1 = endNode1->gates[endID1];
    endInst1->setOperand(0, c1);
    if (Debug) {
      cout << "END INST: 1\n";
      endInst1->dump();
      cout << endl;
    }
  }

  auto currentNode = QODNodes[beforeNdoeIndex];

  // add the to location
  // c1->insertAfter(
  //     (startInst0->comesBefore(startInst1) ? startInst1 : startInst0));
  c1->insertBefore(currentNode->gates[0]);
  c0->insertBefore(c1);

  // Insert new NodeGate
  // - instructions
  // - qubitIndex
  // - successors
  // - predecessors
  NodeGate *node = new NodeGate();
  node->name = "NEW NODE";
  node->type = gateType::multi;
  node->qubitIndex = {sRegID, dRegID};
  node->gates = {c0, c1};
  node->predecessors = {startNode0, startNode1};

  if (endNode0) {
    // cout << "It has successor node 0. \n";
    node->successors.push_back(endNode0);

    // update successor
    if (endID0 < endNode0->predecessors.size()) {
      endNode0->predecessors[endID0] = node;
    } else {
      endNode0->predecessors.push_back(node);
    }
  }

  if (endNode1) {
    // cout << "It has successor node 1. \n";

    node->successors.push_back(endNode1);

    // update predecessors
    if (endID1 < endNode1->predecessors.size()) {
      endNode1->predecessors[endID1] = node;
    } else {
      endNode1->predecessors.push_back(node);
    }
  }

  // Update Predcessors Gate
  // - update predecessors
  if (startID0 < startNode0->successors.size()) {
    startNode0->successors[startID0] = node;
  } else {
    startNode0->successors.push_back(node);
  }

  if (startID1 < startNode1->successors.size()) {
    startNode1->successors[startID1] = node;
  } else {
    startNode1->successors.push_back(node);
  }

  QODNodes.insert(QODNodes.begin() + beforeNdoeIndex, node);
}

/**
 * This method will insert SWAP before the specific register IDs (virtual QID)
 *
 * @param sRegID the source of multi-qubit gate ID. It doesn't matter which one
 * is which.
 * @param dRegID the direction of multi-qubit gate ID. It doesn't matter which
 * one is which.
 * @param multiGateID the new virtual ID of SWAP gate node, need to generate
 * this.
 * @param beforeNodeIndex the virtual ID that it will insert before
 *
 */
void QODGAnalysis::insertSWAPNodeBefore(unsigned sRegID, unsigned dRegID,
                                        int multiGateID, int beforeNdoeID) {
  return insertMultiNodeBefore(sRegID, dRegID, multiGateID, beforeNdoeID, swap);
}

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
void QODGAnalysis::insertCNOTNodeBefore(unsigned sRegID, unsigned dRegID,
                                        int multiGateID, int beforeNdoeID) {
  return insertMultiNodeBefore(sRegID, dRegID, multiGateID, beforeNdoeID, cnot);
}

/**
 * This method will safety delete node including LLVM IR instruction by the node
 * index
 */
void QODGAnalysis::deleteNodeBy(int nodeIndex) {

  auto gateNode = QODNodes[nodeIndex];

  // TODO: delete qubit or classical register
  //        - currently, we can't remove them.

  // TEST
  // cout << "\n\nCurrent Gates\n";
  // for (auto gate: gateNode->gates){
  //   gate->dump();
  // }
  // cout << "----------\n\n";
  // END TEST

  for (unsigned index = 0; index < gateNode->gates.size(); index++) {

    int qubitIndex = gateNode->qubitIndex[index];

    auto [preNode, preGateID] = findNodeBefore(nodeIndex, qubitIndex);
    auto [succNode, succGateID] = findNodeAfter(nodeIndex, qubitIndex);

    auto succInst = (succNode ? succNode->gates[succGateID] : nullptr);
    auto preInst = preNode->gates[preGateID];

    // Update Pre & Succ of its Node
    if (succNode) {
      succNode->name = "succ " + succNode->getOperationName();
      // update predecessor of node
      for (unsigned i = 0; i < succNode->predecessors.size(); i++) {
        if (succNode->predecessors[i] == gateNode) {
          succNode->predecessors[i] = preNode;
          break;
        }
      }
      // Update Successors of Node
      for (unsigned i = 0; i < preNode->successors.size(); i++) {
        if (preNode->successors[i] == gateNode) {
          preNode->successors[i] = succNode;
          break;
        }
      }
      preNode->name = "pre " + preNode->getOperationName();

    } else if (preNode) {
      // Delete dead-node of Predecessor node
      for (unsigned i = 0; i < preNode->successors.size(); i++) {
        if (preNode->successors[i] == gateNode) {
          preNode->successors.erase(preNode->successors.begin() + i);
          break;
        }
      }
    }

    // Update Pre & Succ of its Instruction
    if (succInst) {
      // cout << "Successor of Node with Gate : " << to_string(index) << endl;
      // succInst->dump();
      succInst->setOperand(0, preInst);
    }

    gateNode->gates[index]->eraseFromParent();
  }

  // Remove Node
  QODNodes.erase(QODNodes.begin() + nodeIndex);
}

/**
 * This method will safety delete node including LLVM IR instruction fron
 * QODNodes
 */
void QODGAnalysis::deleteNodeBy(NodeGate *node) {
  for (unsigned i = 0; i < QODNodes.size(); i++) {
    if (QODNodes[i] == node) {
      deleteNodeBy(i);
      return;
    }
  }
}

/**
 * This method will run the pass
 *
 */
PreservedAnalyses QODGAnalysis::run(Function &F, FunctionAnalysisManager &AM) {

  context = &(F.getContext());

  quantumRegisters.clear();
  classicalRegisters.clear();
  QODNodes.clear();

  for (BasicBlock &BB : F) {
    for (Instruction &inst : BB) {
      // Exit early when it return instruction
      if (ReturnInst *Ret = dyn_cast<ReturnInst>(&inst)) {
        (void)Ret;
        break;
      }

      NodeGate *node;
      node = new NodeGate();

      // Registeration Check
      if (AllocaInst *AI = dyn_cast<AllocaInst>(&inst)) {
        // Check AllocaInst type is array or not 
        if (AI->getAllocatedType()->isArrayTy()) {
            // Check AllocaInst type is qubit type
            if (AI->getAllocatedType()->getArrayElementType()->isQubitTy()) {
              errs() << "Qubit Register: " << *AI << "\n";
              auto arrayType = cast<ArrayType>(AI->getAllocatedType());
              for (unsigned i = 0; i < arrayType->getNumElements(); i++) {
                std::vector<Value*> indices;
                Type *Int32Type = Type::getInt32Ty(AI->getContext());

                // First index is zero to go through the pointer
                indices.push_back(ConstantInt::get(Int32Type, 0));
                // Second index is the element index
                indices.push_back(ConstantInt::get(Int32Type, i));

                // Generate the GetElementPtrInst instruction to access the array element
                Value *elementPtr = GetElementPtrInst::CreateInBounds(AI, indices, "", &inst);
                errs() << "Processing Qubit: " << *elementPtr << "\n";
                // continue;
              }
            }
            // Check AllocaInst type is classical integer type
            if (AI->getAllocatedType()->getArrayElementType()->isIntegerTy()) {
              // print AI
              errs() << "Classical Register: " << *AI << "\n";
              auto arrayType = cast<ArrayType>(AI->getAllocatedType());
              for (unsigned i = 0; i < arrayType->getNumElements(); i++) {
                std::vector<Value*> indices;
                Type *Int32Type = Type::getInt32Ty(AI->getContext());

                // First index is zero to go through the pointer
                indices.push_back(ConstantInt::get(Int32Type, 0));
                // Second index is the element index
                indices.push_back(ConstantInt::get(Int32Type, i));

                // Generate the GetElementPtrInst instruction to access the array element
                Value *elementPtr = GetElementPtrInst::CreateInBounds(AI, indices, "", &inst);
                errs() << "Processing Integer: " << *elementPtr << "\n";
                // continue;
              }
            }
        }

        createRegisterNode(AI, *node);

        node->gates.push_back(&inst);
        QODNodes.push_back(node);

        errs() << "DEBUG: Register Node: " << node << "\n";
        continue;
      }

      // if (inst.getOpcode() == Instruction::HGate || 
      //     inst.getOpcode() == Instruction::XGate || 
      //     inst.getOpcode() == Instruction::YGate || 
      //     inst.getOpcode() == Instruction::ZGate || 
      //     inst.getOpcode() == Instruction::CX0Gate || 
      //     inst.getOpcode() == Instruction::SwapGate) {
      //   // Set successor to val
      //   node->predecessors.push_back(QODNodes[QODNodes.size()-1]);
      //   QODNodes[QODNodes.size()-1]->successors.push_back(node);

      //   errs() << "DEBUG: Gate Node: " << node << "\n";
      // }

      Value *val = inst.getOperand(0);

      for (unsigned t = 0; t < QODNodes.size(); t++) {
        for (unsigned i = 0; i < QODNodes[t]->gates.size(); i++) {
          if (val == QODNodes[t]->gates[i]) {

            unsigned qubitIndex = QODNodes[t]->qubitIndex[i];

            NodeGate *node = createNode(inst, qubitIndex);
            bool isNodeExisting = false;

            // This method will decide should it create new node or it exist already in previous node.
            if (auto *muliGateInst = dyn_cast<MultiGateInst>(&inst)) {
              for (auto &tempNode : QODNodes) {
                if (tempNode->gates.empty()) {
                  errs() << "Gates of tempNode is empty.";
                  exit(0);
                }
                if (auto *previousGateInst = dyn_cast<MultiGateInst>(tempNode->gates[0])) {
                  if (muliGateInst->getQubitID() == previousGateInst->getQubitID()) {
                    updateNode(tempNode, inst, qubitIndex);
                    node = tempNode;
                    isNodeExisting = true;
                  }
                }
              }
            }

            // Check if the instruction is a CtrlGateInst using isa
            if (isa<CtrlGateInst>(inst)) {
              // Set successor to val
              node->predecessors.push_back(QODNodes[QODNodes.size()-1]);
              QODNodes[QODNodes.size()-1]->successors.push_back(node);

              errs() << "DEBUG: Multi Gate Node: " << node << "\n";
            }

            // This method will decide should it create new node or it exist already in previous node for single gate node.
            if (inst.getOpcode() == Instruction::HGate || 
                inst.getOpcode() == Instruction::XGate || 
                inst.getOpcode() == Instruction::YGate || 
                inst.getOpcode() == Instruction::ZGate) {
              // Set successor to val
              node->predecessors.push_back(QODNodes[QODNodes.size()-1]);
              QODNodes[QODNodes.size()-1]->successors.push_back(node);

              errs() << "DEBUG: Single Gate Node: " << node << "\n";
            }

            // This method will decide should it create new node or it exist already in previous node for CNOT node.
            if (inst.getOpcode() == Instruction::CX1Gate && inst.getOpcode() == Instruction::XGate) {
              // Set successor to val
              node->predecessors.push_back(QODNodes[QODNodes.size()-1]);
              QODNodes[QODNodes.size()-1]->successors.push_back(node);

              errs() << "DEBUG: Multi Gate Node: " << node << "\n";
            }

            // This method will decide should it create new node or it exist already in previous node for SWAP node.
            if (inst.getOpcode() == Instruction::CX0Gate && inst.getOpcode() == Instruction::SwapGate) {
              // Set successor to val
              node->predecessors.push_back(QODNodes[QODNodes.size()-1]);
              QODNodes[QODNodes.size()-1]->successors.push_back(node);

              errs() << "DEBUG: Multi Gate Node: " << node << "\n";
            }

            // This will get the index of classical bit to measure node
            if (inst.getOpcode() == Instruction::MeasXGate || inst.getOpcode() == Instruction::MeasYGate || inst.getOpcode() == Instruction::MeasZGate) {
              if (inst.getNumOperands() < 2) {
                errs() << "The Measure must have 2 operands \n";
                exit(1);
              }

              Value *val2 = inst.getOperand(1);
              node->type = gateType::measure;
              // This will get the index of classical bit to measure node
              for (auto &c_node : classicalRegisters) {
                if (val2 == c_node->gates.back()) {
                  node->bitIndex = c_node->qubitIndex.back();
                  node->gates.push_back(c_node->gates.back());
                  c_node->successors.push_back(node);
                  node->predecessors.push_back(c_node);
                  break;
                }
              }

              // Insert node to TempNode
              if (!isNodeExisting) {
                QODNodes.push_back(node);
              }
            }
          }
        }
      }
    }
  }

  return PreservedAnalyses::all();
}
