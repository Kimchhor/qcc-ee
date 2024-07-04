/*********************************************************************
 *   NodeGate.cpp:
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

#ifndef QCC_NODEGATE
#define QCC_NODEGATE

#include <iostream>
#include <llvm/IR/Instruction.h>
#include <vector>

using namespace std;
using namespace llvm;

/** Describes the type of node */
enum gateType {
  /** Single-qubit gates: X and H */
  single,
  /** Multi-qubit gates: CX and SWAP */
  multi,
  /** For the gate that has rotation value: RX, RY, RZ */
  rotation,
  /** Quantum Register gates */
  q_register,
  /** Classical Register gates */
  c_register,
  /** Measurement operater */
  measure
};

///////////////////////////////////////////////////////////////////////
// NodeGate Class Declaration
///////////////////////////////////////////////////////////////////////

class NodeGate {

public:
  /**
   * Describes the ancilla qubits for Multi-qubit gates only (CX, SWAP)
   */
  struct AncillaQubit {

    /** The QID that selected for CNOT gates ancilla */
    int selectedQID = -1;

    /** The QID that selected for CNOT gates ancilla */
    int anotherQID = -1;

    /** if its value is 0, it is X Stabilizer.
     * Otherwise, it is Z Stabilzer
     */
    int stabilizer = -1; // 0 is X ,  1 is Z

    /**
     * Category code describes the patter of gate such as: \n
     * e.g: if categoryCode is 1 of CX(q1, q4) or SWAP(q1, q4) then:
     * \code {.cpp}
     * | q1 | a2 |
     * | a3 | q4 |     
     * \endcode
     * 
     * otherwise, if it is 0 of CX(q2, q3) or SWAP(q2, q3) then:
     * \code {.cpp}
     * | a1 | q2 |
     * | q3 | a4 |
     * \endcode
     * 
     */
    int categoryCode = -1; // 1 = [ \ ], 0 = [ / ]
    /** Constructor to create AncillaQubit Object */
    AncillaQubit(int selectedQID, int anotherQID, int stabilizer,
                 int categoryCode)
        : selectedQID(selectedQID), anotherQID(anotherQID),
          stabilizer(stabilizer), categoryCode(categoryCode) {}

    /** Describes the stabilizer */
    enum StabilizerSymbol {
      /** Stabilzer X is represented by 0 */
      X = 0,
      /** Stabilzer Y is represented by 0 */
      Z = 1
    };

    StabilizerSymbol getStabilizerSym() { return StabilizerSymbol(stabilizer); }
  };

  NodeGate() {
    bitIndex = 0;
    rotationValue = 0.0;
  }; // constructor

  ~NodeGate() {
    gates.clear();
    successors.clear();
    predecessors.clear();
    delete ancilla;
  };

  ///////////////////////////////////////////////////////////////////////
  // Member Properties Definition
  ///////////////////////////////////////////////////////////////////////

  /** The operation name */
  std::string name;

  /**
   * virtual qubits which that node apply on
   *
   * \note the position of qubit that gate applied on.
   * it must be symmetric to gates.
   *
   */
  vector<unsigned> qubitIndex; // virtual qubits

  /** bit index for measure gates and classical register.
   * For measure gate, it tells which classical register that store
   * the result from measurment.
   */
  unsigned bitIndex;

  /** The type of node: single, multi, rotate , q_register, c_register or
   * measure */
  gateType type;

  /** If the node is multi, aniclla will contain info of ancilla */
  AncillaQubit *ancilla = nullptr;

  /** it's true, if the node is qubit registers. */
  bool isRoot = false;

  /** the rotation value for Rx, Ry, Rz gates. */
  double rotationValue;

  /** it contains gate(s) for nodes
   * CNOT gate node will contains CX0 and CX1,
   * while H gate contains only one instruction.
   */
  vector<llvm::Instruction *> gates;

  /** List of the sccessors of node */
  vector<NodeGate *> successors;

  /** List of the predecessors of node */
  vector<NodeGate *> predecessors;

  ///////////////////////////////////////////////////////////////////////
  // Member Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method is used to compare two NodeGate objects by gates
   *
   * @return true if gates in nodes is equal
   */
  bool operator==(const NodeGate &nodeGate) const {
    return std::equal(this->gates.begin(), this->gates.end(),
                      nodeGate.gates.begin());
  }

  /**
   * This method will return a name of instruction.
   *
   * @return name of instruction. \n e.g,. x, y, z, ..., cx, ccx, swap.
   */
  string getOperationName() {
    if (gates.empty())
      return "";

    llvm::Instruction &inst = *gates[0];

    switch (inst.getOpcode()) {
    case Instruction::CtrlGate:
      if (gates.size() == 3) {
        return "ccx";
      }
      return "cx";
      break;
    default:
      return inst.getOpcodeName();
      break;
    }
  }
};
#endif