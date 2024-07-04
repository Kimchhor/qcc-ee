/*********************************************************************
 *   MultiGateAnalysis.h:
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

#ifndef LLVM_TRANSFORMS_ANALYSISCNOT_ANALYSISCNOT_H
#define LLVM_TRANSFORMS_ANALYSISCNOT_ANALYSISCNOT_H

#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"

namespace llvm {

///////////////////////////////////////////////////////////////////////
// AnalysisCNOTPass Class Declaration
///////////////////////////////////////////////////////////////////////

class MultiGateAnalysisPass : public PassInfoMixin<MultiGateAnalysisPass> {

public:
  ///////////////////////////////////////////////////////////////////
  // Member Variable Declaration
  ///////////////////////////////////////////////////////////////////

  /** The pair of CXGate instructions type */
  using CnotPair = std::pair<CXGateInst *, CXGateInst *>;

  /** The list of CNOT pair type*/
  using CnotList = std::vector<CnotPair>;

  /** The Toffoli pair composes of 3 CXGates Instructions type*/
  using ToffoliPair = std::tuple<CXGateInst *, CXGateInst *, CXGateInst *>;

  /** The Toffoli List contains ToffoliPair type */
  using ToffoliList = std::vector<ToffoliPair>;

  /** The Swap pair composes of 2 Swap gate instruction type*/
  using SwapPair = std::pair<SwapGateInst *, SwapGateInst *>;

  /** The Swap List contains list of Swap Pair. */
  using SwapList = std::vector<SwapPair>;

  /** The list of pair of cx */
  CnotList cnotList;

  /** The list of turple of cx */
  ToffoliList toffoliList;

  /** The list of pair of swap inst */
  SwapList swapList;

  ///////////////////////////////////////////////////////////////////////
  // Member Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will run the pass
   */
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  /**
   * This method will check if the Toffoli gate detected
   *
   * @param id  Qubit ID of CX0Gate or CX1Gates
   * @return true if this node is toffoli gate
   */
  bool isToffoli(unsigned id);

  /**
   * This method will get the last ID of cnot/toffoli gates
   *
   * @param Function &F Function
   * @param FunctionAnalysisManager &AM Function Analysis Manager
   */
  unsigned int getLastID();
};

} // namespace llvm

#endif