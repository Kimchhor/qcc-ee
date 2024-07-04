
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

 
#include "llvm/Transforms/QCC/GateDecomposition/EmitNativeMultiple.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "multiple-ia"

void EmitNativeMultiplePass::analysisMultipleIntoList(Instruction &I, std::vector<Instruction *> &nativeList) {

}

PreservedAnalyses EmitNativeMultiplePass::run(Function &F, FunctionAnalysisManager &AM) {
  for (BasicBlock &bb : F) {
    for (Instruction &i : bb) {
      if (isa<HGateInst>(i)) { // [수정필요] 다중큐비트게이트인지 확인 -> MultiGateAnalysis.h 변경해야할까?
        // [수정필요] 매개변수: 현재 입력된 코드 중 다중큐비트게이트, 다중큐비트게이트 분해 정보
        analysisMultipleIntoList(i, nativeList);
      }
    }
  }

  return PreservedAnalyses::all();
}
