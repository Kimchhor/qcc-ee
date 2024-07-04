/*********************************************************************
 *   Decompose2Native.cpp:
 *
 *   Authors:
 *      Nagyeong Choi (choi2019@pukyong.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/
#include <iostream>
#include "llvm/Transforms/QCC/GateDecomposition/Decompose2Native.h"
#include "llvm/Transforms/QCC/GateDecomposition/EmitNativeMultiple.h"
#include "llvm/Transforms/QCC/GateDecomposition/EmitNativeSingle.h"
// #include "llvm/Transforms/QCC/MultiGateAnalysis.h"


#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

#define DEBUG_TYPE "decompose2native"

#ifndef GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRINFO_CTOR_DTOR

// This use to get Integer type in DecomposeSwapPass(_)
// LLVMContext context;

/**
 * @brief Initializes the gate decompositions.
 * 
 * This function creates a `gateDecompos` map with the decompositions of various single-qubit and multi-qubit gates.
 * The decomposition is represented as a nested vector of strings, each string representing a gate and its parameters.
 * 
 * @note The `gateDecompos` map is modified by this function.
 */

void Decompose2NativePass::initGateDecompositions(){
}

PreservedAnalyses Decompose2NativePass::run(Function &F,
                                         FunctionAnalysisManager &AM) {

  initGateDecompositions();
  
  EmitNativeSinglePass emitNativeSingle;
  EmitNativeMultiplePass emitNativeMultiple;

  emitNativeSingle.run(F, AM); 
  emitNativeMultiple.run(F, AM);


  // // GateDPGenInstrInfo 객체 ?��?��
  // GateDPGenInstrInfo gateDPGenInstrInfo();

  // // sayHello() ?��?�� ?���??
  // gateDPGenInstrInfo.sayHello();

  std::cout << "Decomposition2Native.cpp" << std::endl;
  return PreservedAnalyses::all();
}

#endif