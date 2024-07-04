/*********************************************************************
 *   EmitSwap.cpp:
 *
 *   Authors:
 *      Sengthai Heng (sengthai37@gmail.com)
 *      Youngsun Han (youngsun@pknu.ac.kr)
 *      Nagyeong Choi (choi2019@pukyong.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/

#include "llvm/Transforms/QCC/EmitSwap.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "swap-ia"

STATISTIC(NumOfSwap, "Number of Swap Gate.");

/**
 * This method will emit Swap gates as pair by ID into into a vector
 *
 * @param Instruction &i LLVM Instruction (Gate Instruction)
 * @param Vector &swaps is the list of swap pair
 */
void EmitSwapPass::analysisSwapIntoList(
    Instruction &i,
    std::vector<std::vector<Instruction *>> &swaps) {

  vector<Instruction *> swap;

  for(int j=0; j < i.getNumOperands()-1; j++){
    Instruction *CtrlInst = dyn_cast<Instruction>(i.getOperand(j));
    swap.push_back(CtrlInst);
  }

  swap.push_back(dyn_cast<Instruction>(&i));
  
  swaps.push_back(swap);
}

/**
 * This method will run the pass
 *
 */
PreservedAnalyses EmitSwapPass::run(Function &F, FunctionAnalysisManager &AM) {

  for (BasicBlock &bb : F) {
  for (Instruction &i : bb) {
      if (isa<SwapGateInst>(i)) {
        analysisSwapIntoList(i, swapList);
      }
    }
  }
  
  NumOfSwap = swapList.size();
  
  // Instruction* firstInstruction = *swapList.begin()->begin();

  return PreservedAnalyses::all();
}