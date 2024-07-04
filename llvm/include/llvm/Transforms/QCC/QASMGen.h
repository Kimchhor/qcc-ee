/*********************************************************************
 *   QASMGen.h:
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

#ifndef LLVM_TRANSFORMS_QASMGEN_QASMGEN_H
#define LLVM_TRANSFORMS_QASMGEN_QASMGEN_H

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/QCC/QODGAnalysis.h"

namespace llvm {

///////////////////////////////////////////////////////////////////////
// QASMGen Class Declaration
///////////////////////////////////////////////////////////////////////

class QASMGen : public PassInfoMixin<QASMGen> {

  ///////////////////////////////////////////////////////////////////////
  // Member Private Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will generate the qasm filename depend on
   * source_filename in IR or filename of IR
   *
   * @param string sourceFileName is the name that set in LLVM IR
   * @return string filename with .qasm extension
   */
  string getFileName(string sourceFileName);

  /**
   * This method will generate a node of an allocation
   *
   * @param unsigned numberOfRegisters is the amount of registers.
   * e.g,.: if q[3],so it is 3.
   * @param string instruction name. e.g,. qregs or cregs.
   * @param string variable name. e.g,. q or c.
   * @return string a QASM allocation. e.g,. qregs q[n] or cregs c[n];
   */
  string getQASMAllocation(unsigned numberOfRegisters, string instruction,
                           string name);

  /**
   * This method will generate a node a an qubit register (allocation)
   *
   * @param unsigned numberOfRegisters is the amount of qubit registers.
   * e.g,.: if qreg q[3],so it is 3.
   * @return string a QASM qubit register. e.g,. qregs q[n]
   */
  string getQASMQubitAllocation(unsigned numberOfRegisters);

  /**
   * This method will generate a node a an classical register (allocation)
   *
   * @param unsigned numberOfRegisters is the amount of classical registers.
   * e.g,.: if creg c[2],so it is 2.
   * @return string a QASM classical register. e.g,. cregs c[n]
   */
  string getQASMClassicalAllocation(unsigned numberOfRegisters);

  /**
   * This method will return the header of QASM file format
   *
   * @return string a header file of QASM
   */
  string getQASMHeader();

public:
  ///////////////////////////////////////////////////////////////////////
  // Member Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will generate the QASM instruction using NodeGate object
   *
   * @param node contains llvm instructions and dependcy qubit indexs
   * @return string a QASM instruction. e.g,. h q[1];
   */
  string getQASMInstruction(NodeGate &node);

  /**
   * This method will run the pass
   */
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif