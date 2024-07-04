 /*********************************************************************
 *   GateUnroll.cpp:
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

#include "llvm/Transforms/QCC/GateUnroll/GateUnroll.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

using namespace llvm;

#define DEBUG_TYPE "gate-unroll"


static Value* getVariableReference(Value* Op) {
    for (auto &U : Op->uses()) {
        auto LoadStorePointerOperand = getLoadStorePointerOperand(U.getUser());
        if (LoadStorePointerOperand) {
            return LoadStorePointerOperand;
        }
    }
    return getLoadStorePointerOperand(Op);
}

/**
 * This method will run the pass
 *
 */
PreservedAnalyses GateUnrollPass::run(Function &F,
                                         FunctionAnalysisManager &AM) {

    // Collect gates to unroll first
    std::vector<GateInst*> gatesToUnroll;

    for (BasicBlock &BB : F) {
        for (Instruction &I : BB) {
            if (auto *GI = dyn_cast<GateInst>(&I)) {
                if (GI->isShouldDecompose()) {
                    gatesToUnroll.push_back(GI);
                    // Because of the iterator invalidation issue, 
                    // we can't unroll the gate here
                }
            }
        }
    }

    // Unroll the collected gates after the initial iteration
    for (GateInst* GI : gatesToUnroll) {

        const int numOperands = GI->getNumOperands();
        SmallVector<Instruction*> InstOperands;
        SmallVector<Value*> RefQubits;


        for (int OpIdx = 0; OpIdx < numOperands; OpIdx++) {

            // Remove "%3 = ctrl %ctrl0" and replace with its operand instruction
            // e.g: %3 -> %ctrl0
            //      ...
            //      %5 = x q1 %ctrl0, %4, %tar
            if(auto *Inst = dyn_cast<Instruction>(GI->getOperand(OpIdx))) {
                if (isa<CtrlGateInst>(Inst)) {
                    auto *OpInst = dyn_cast<Instruction>(Inst->getOperand(0));
                    GI->setOperand(OpIdx, OpInst);
                    Inst->replaceAllUsesWith(OpInst);
                    Inst->eraseFromParent();
                }
            }

            Instruction *Inst = cast<Instruction>(GI->getOperand(OpIdx));

            InstOperands.push_back(Inst);

            RefQubits.push_back(getVariableReference(GI->getOperand(OpIdx)));
        }


        for (auto Gate: GI->decomposeRules) {
            InstOperands = Gate(InstOperands, dyn_cast<Instruction>(GI));
        }
        
        // for (int i = 0; i < numOperands-1; i++) {
        //     new StoreInst(InstOperands[i], RefQubits[i],/*isVolatile*/ true, GI);
        //     // The last Op is already stored as when in GI
        //     // new StoreInst(dyn_cast<Instruction>(Op2), ADDR_Q2, GI); 
        // }

        // replace and remove the original gate
        GI->replaceAllUsesWith(InstOperands[numOperands-1]);
        GI->eraseFromParent();

    }

  return PreservedAnalyses::all();
}

