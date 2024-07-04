/*********************************************************************
 *   CountCNOT.cpp:
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

#include "llvm/Transforms/QCC/CountCNOT.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h" 

using namespace llvm;

#define DEBUG_TYPE "count-cnot"

STATISTIC(NumOfCnot, "Number of CNOT Gate.");

/**
 * This method will run the pass
 *
 */
PreservedAnalyses CountCNOTPass::run(Function &F, FunctionAnalysisManager &AM){

    for(BasicBlock &BB: F){

        Instruction* lastCtrlGate = nullptr;

        for (Instruction &I : BB) {

            // check if the instruction Op code is 80, it is CNOT Gate.
            // Op code is found in /include/llvm/IR/Instruction.def
            // if (I.getOpcode() == 88){
            //     ++NumOfCnot;
            // }
            
            // Check if the instruction is a CtrlGateInst 
            // E.g. cnot -> q[0], q[1]; 
            if (isa<CtrlGateInst>(I)) {
                lastCtrlGate = &I; // Remember this control gate
                // errs() << "CNOT Gate (CtrlGate): " << I << "\n";     // Print the instruction
            }
            // Check if the instruction is an CtrlGateInst with XGateInst 
            // To exclude swap -> q[0], q[1];
            // E.g. cnot -> q[1], q[0] 
            else if (lastCtrlGate && isa<XGateInst>(I)) {
                lastCtrlGate = nullptr; // Reset after use
                ++NumOfCnot;            // count CNOT instruction increment 
            }
        }
    }

    outs() << "Number of CNOT Gate: "  << NumOfCnot << "\n";
    errs() << "\n[-------CNOT Count Successfully-------]\n\n";

    return PreservedAnalyses::all();
}


