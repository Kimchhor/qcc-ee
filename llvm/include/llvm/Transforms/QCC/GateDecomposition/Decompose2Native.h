/*********************************************************************
 *   Decompose2oNative.h:
 *
 *   Authors:
 *      Nagyeong Choi (choi2019@pukyong.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/

#ifndef LLVM_TRANDSFORMS_DECOMPOSE2NATIVE_H
#define LLVM_TRANDSFORMS_DECOMPOSE2NATIVE_H

#include "llvm/IR/PassManager.h"
// enum Flags :: unsinged {
//     #define GET_DECOMPO_FLAGS;
//     #include "GateDecompositionImf.inc"
// }
// #include "GateDecompositionImf.inc"
// #include <map>

namespace llvm {
    class Decompose2NativePass : public PassInfoMixin<Decompose2NativePass> {
    public:

        // std::map<std::string, std::map<std::string, std::vector<std::vector<std::pair<std::string, std::variant<double, std::string>>>>>> gateDecompos;
        
        // 정보 테이블 변수 정의
        // long double M_THEATA;
        // long double M_PHI;
        // long double M_LAMBDA;


        /**
         * Runs the decomposition to native transformation on the given function.
         *
         * This method is responsible for applying the decomposition to native transformation
         * on the provided function. It takes a reference to the `Function` object and a
         * reference to the `FunctionAnalysisManager` object as parameters.
         *
         * @param F The function to apply the transformation on.
         * @param FAM The function analysis manager.
         * @return The preserved analyses.
         */
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);

        void initGateDecompositions();
        // void applyDecompositionRule(const Gate &gate);

    };
} // namespace llvm

#endif // LLVM_TRANDSFORMS_DECOMPOSE2NATIVE_H