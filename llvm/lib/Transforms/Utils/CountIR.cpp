#include "llvm/Transforms/Utils/CountIR.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"


using namespace llvm;

#define DEBUG_TYPE "countir"

STATISTIC(NumOfInst, "Number of CNOT Gate.");

PreservedAnalyses
CountIRPass::run(Function &F, FunctionAnalysisManager &AM){
    for(BasicBlock &BB: F){
       
        for(Instruction &I: BB){
            (void) I;
            if (I.getOpcode() == 80){
                ++NumOfInst;
            }
            
        }
    }

    return PreservedAnalyses::all();
}

