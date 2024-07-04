/*********************************************************************
 *   EmitSwap.cpp:
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

#ifndef LLVM_TRANSFORMS_EMITSWAP_EMITSWAP_H
#define LLVM_TRANSFORMS_EMITSWAP_EMITSWAP_H

#include "llvm/IR/PassManager.h"
#include <vector>

namespace llvm {

///////////////////////////////////////////////////////////////////////
// EmitSwapPass Class Declaration
///////////////////////////////////////////////////////////////////////

class EmitSwapPass : public PassInfoMixin<EmitSwapPass> {

public:
  ///////////////////////////////////////////////////////////////////
  // Member Variable Declaration
  ///////////////////////////////////////////////////////////////////

  /** A swap list of pair of Instruction */
  std::vector<std::vector<Instruction *>> swapList;
  

  ///////////////////////////////////////////////////////////////////
  // Member Function Declaration
  ///////////////////////////////////////////////////////////////////

  /**
   * This method will run the pass
   */
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  /**
   * This method will emit Swap gates as pair by ID into into a vector
   *
   * @param i LLVM Instruction (Gate Instruction)
   * @param swaps the list of swap pair
   */
  void analysisSwapIntoList(
      Instruction &i,
      std::vector<std::vector<Instruction *>> &swaps);
      
};

} // namespace llvm

#endif