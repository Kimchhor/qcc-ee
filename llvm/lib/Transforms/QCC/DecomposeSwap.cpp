/*********************************************************************
 *   DecomposeSwap.cpp:
 *
 *   Authors:
 *      Sengthai Heng (sengthai37@gmail.com)
 *      Youngsun Han  (youngsun@pknu.ac.kr)
 *      Nagyeong Choi (choi2019@pukyong.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/

#include "llvm/Transforms/QCC/DecomposeSwap.h"
#include "llvm/Transforms/QCC/EmitSwap.h"
#include "llvm/Transforms/QCC/MultiGateAnalysis.h"

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

#define DEBUG_TYPE "swap-decompose"

// This use to get Integer type in DecomposeSwapPass(_)
LLVMContext context;

/**
 * This method emits the swap gate as a vector of CNOT gate pairs.
 *
 * @param SwapList &swapList the vector that contain pair of swaps
 */
void DecomposeSwapPass::decompose(SwapList &swapList) {
  for(auto &swapTuple : swapList) {
    
    // Emit the previous instructions
    llvm::Value *qControl0 = swapTuple.front()->getOperand(0);
    llvm::Value *qTarget = swapTuple.back()->getOperand(swapTuple.back()->getNumOperands()-1);

    // extracting allocation information
    llvm::LoadInst *li0 = dyn_cast<LoadInst>(qControl0);
    llvm::LoadInst *li1 = dyn_cast<LoadInst>(qTarget);    
    llvm::Value *address_of_q_load_0 = li0->getOperand(0);
    llvm::Value *address_of_q_load_1 = li1->getOperand(0);

    // Get Type (= q1)
    llvm::Type * type = address_of_q_load_0->getType()->getPointerElementType();

    // Temporary reference points for alignment
    Instruction *temp = CtrlGateInst::Create(qControl0);

    // 1st.cnot
      // CNOT q0, q1
      //   %8 = ctrl q1* %6
      //   %9 = x q1* %7, %8
    Instruction *c0 = CtrlGateInst::Create(qControl0);
    Instruction *c1 = XGateInst::Create(c0, qTarget);

    // Replace swap instruction with the last CNOT q0
    ReplaceInstWithInst(swapTuple[0], c0);

    // Setting a reference point
    ReplaceInstWithInst(swapTuple[1], temp);

    // Start sorting from reference point
    c1->insertBefore(temp);


    // 2nd. load
    llvm::Instruction *cl1 = new llvm::LoadInst(type, address_of_q_load_1, "", temp);
    llvm::Instruction *tl1 = new llvm::LoadInst(type, address_of_q_load_0, "", temp);
    // 3rd. load
    llvm::Instruction *cl2 = new llvm::LoadInst(type, address_of_q_load_0, "", temp);
    llvm::Instruction *tl2 = new llvm::LoadInst(type, address_of_q_load_1, "", temp);


    // 2nd. cnot
      // CNOT q1, q0
      //   %10 = ctrl q1* %9
      //   %11 = x q1* %8, %10
    Instruction *c2 = CtrlGateInst::Create(cl1);
    Instruction *c3 = XGateInst::Create(c2, tl1);
  
    // 3rd. cnot
      // CNOT q0, q1
      //    %12 = ctrl q1* %11
      //    %13 = x q1* %10, %12
    Instruction *c4 = CtrlGateInst::Create(cl2);
    Instruction *c5 = XGateInst::Create(c4, tl2);
    
    c2->insertBefore(cl2);
    c3->insertBefore(cl2);
    c4->insertAfter(tl2);

    // Replace swap instruction with the last CNOT q1
    ReplaceInstWithInst(temp, c5);

    // 1st. store
    llvm::StoreInst * s1 = new StoreInst(c1, address_of_q_load_1, cl1);
    // 2nd. store
    llvm::StoreInst * s2 = new StoreInst(c2, address_of_q_load_1, c3);
    llvm::StoreInst * s3 = new StoreInst(c3, address_of_q_load_0, cl2);
    // 3rd. store
    llvm::StoreInst * s4 = new StoreInst(c4, address_of_q_load_0, c5);
  }
}

/**
 * This method will run the pass
 *
 */
PreservedAnalyses DecomposeSwapPass::run(Function &F,
                                         FunctionAnalysisManager &AM) {

  EmitSwapPass emitSwap;

  // Run EmitSwapPass to get list of swap
  emitSwap.run(F, AM);

  // Run Multi Gate Analysis
  MultiGateAnalysisPass multiGateAnalysis;
  multiGateAnalysis.run(F, AM);

  // Check if there is no swap
  if (emitSwap.swapList.size() == 0) {
    return PreservedAnalyses::none();
  }

  // Decompose Swap
  decompose(emitSwap.swapList);
  
  errs() << "\n[-------Swap Decomposition Successfully-------]\n";

  return PreservedAnalyses::all();
}
