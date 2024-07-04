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

#ifndef LLVM_TRANDSFORMS_EMITNATIVEMULTIPLE_H
#define LLVM_TRANDSFORMS_EMITNATIVEMULTIPLE_H

#include "llvm/IR/PassManager.h"
#include <map>

namespace llvm {
    class EmitNativeMultiplePass : public PassInfoMixin<EmitNativeMultiplePass> {
    public:

        /// 다음 선언들이 필요한지 모르겠어.
        // 다중큐비트게이트 분해 정보 테이블 변수 정의 (이름, (분해정보)) | 전역변수로 선언
        // extern std::map<std::string, std::map<std::string, std::vector<std::vector<std::pair<std::string, std::variant<double, std::string>>>>>> gateDecompos;
        
        // // 정보 테이블에서 쓰이는 변수 정의 (M_THEATA, M_PHI, M_LAMBDA) | 전역변수로 선언
        // extern long double M_THEATA;
        // extern long double M_PHI;
        // extern long double M_LAMBDA;

        std::vector<Instruction *> nativeList;

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
        void analysisMultipleIntoList(Instruction &I, std::vector<Instruction *> &nativeList);
    };
}

#endif // LLVM_TRANDSFORMS_EMITNATIVEMULTIPLE_H