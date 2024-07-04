/*********************************************************************
 *   GateUnroll.cpp:
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

#ifndef LLVM_TRANSFORMS_GATEUNROLL_GATEUNROLL_H
#define LLVM_TRANSFORMS_GATEUNROLL_GATEUNROLL_H

#include "llvm/IR/PassManager.h"

namespace llvm {

///////////////////////////////////////////////////////////////////////
// GateUnrollPass Class Declaration
///////////////////////////////////////////////////////////////////////

class GateUnrollPass : public PassInfoMixin<GateUnrollPass> {

public:
  ///////////////////////////////////////////////////////////////////////
  // Member Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will run the pass
   */
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif