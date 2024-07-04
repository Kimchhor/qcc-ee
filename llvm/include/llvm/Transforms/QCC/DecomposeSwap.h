/*********************************************************************
 *   DecomposeSwap.cpp:
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

#ifndef LLVM_TRANSFORMS_DECOMPOSESWAP_DECOMPOSESWAP_H
#define LLVM_TRANSFORMS_DECOMPOSESWAP_DECOMPOSESWAP_H

#include "llvm/IR/PassManager.h"
#include <vector>

namespace llvm {

///////////////////////////////////////////////////////////////////////
// DecomposeSwapPass Class Declaration
///////////////////////////////////////////////////////////////////////

class DecomposeSwapPass : public PassInfoMixin<DecomposeSwapPass> {

public:
  ///////////////////////////////////////////////////////////////////
  // Alise Declaration
  ///////////////////////////////////////////////////////////////////

  /** A list that contains pairs of Instruction of llvm */
  using SwapList = std::vector<std::vector<Instruction *>>;

  ///////////////////////////////////////////////////////////////////
  // Member Fucntions Declaration
  ///////////////////////////////////////////////////////////////////

  /**
   * This method will run the pass
   */
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  /**
   * This method will emit Swap gates as pair by ID into into a vector
   *
   * @param swapList  the vector that contain pair of swaps
   * @param lastMultiGateID the last ID of multi gate (cnot,
   * toffoli)
   */
  void decompose(SwapList &swapList);
};

} // namespace llvm

#endif