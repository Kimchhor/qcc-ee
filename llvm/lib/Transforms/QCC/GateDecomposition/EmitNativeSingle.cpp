/*********************************************************************
 *   DecomposeToNative.cpp:
 *
 *   Authors:
 *      Nagyeong Choi (choi2019@pukyong.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/


#include "llvm/Transforms/QCC/GateDecomposition/EmitNativeSingle.h"

#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "single-ia"

// void EmitNativeSinglePass::analysisSingleIntoList(Instruction &I) {
// }

PreservedAnalyses EmitNativeSinglePass::run(Function &F, FunctionAnalysisManager &AM) { // [수정필요] 매개변수 추가 필요, 단일큐비트게이트 분해 정보
  // for (BasicBlock &bb : F) {
  //   for (Instruction &i : bb) {
  //     if (isa<>(i)) { // [수정필요] 단일큐비트게이트인지 확인 -> 뭘 수정해야할까??...
  //       // [수정필요] 매개변수: 현재 입력된 코드 중 단일큐비트게이트, 단일큐비트게이트 분해 정보
  //       analysisSingleIntoList(i, );
  //     }
  //   }
  // }

  return PreservedAnalyses::all();
}