#include "llvm/Support/InitLLVM.h"
#include <llvm/Analysis/CallGraph.h>
#include <llvm/Analysis/CallPrinter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/ReplaceConstant.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <string>

#include <iostream>
#include <vector>
#include <set>

#include "llvm/IR/PassManager.h"
#include <llvm/Transforms/QCC/EmitSwap.h>

using namespace llvm;
using namespace std;

static ExitOnError ExitOnErr;

int main(int argc, char **argv) {

  cout << endl;

  if (argc < 2) {
    errs() << "Expected an argument - IR file name\n";
    exit(1);
  }
  LLVMContext context;

  cout << "QIR TESTING" << endl << endl;

  // Parse the input LLVM IR file into a module.
  SMDiagnostic Err;
  LLVMContext Context;
  errs() << "FILE FROM: " << argv[1] << "\n";
  parseIRFile(argv[1], Err, Context);
  std::unique_ptr<Module> Mod = (parseIRFile(argv[1], Err, Context));
  if (!Mod) {
    Err.print(argv[0], errs());
    return 1;
  }

  Module *mm = Mod.get();

  set<QubitType*> qubitType;
  set<Value*> valSet;

  vector<IntegerType*> intType;
  for (Function &f : *mm){
    
    for (BasicBlock &bb : f){

      for (Instruction &inst : bb){

        // 31 is alloca instruction code 
        if (inst.getOpcode() == 31){

          Type *y = inst.getType();
          
          PointerType *pty = (PointerType *) y;

          // cout  << "UUUID : " << pty->getElementType()->getTypeID() << endl;
          
          QubitType *qty = (QubitType *) pty->getElementType();
          
          cout << "Ptr: " << qty << endl;
          
        }
        
        if (inst.getOpcode() >= 68  && inst.getOpcode() <= 85){

          // GateInst *XGate = dyn_cast<GateInst>(&inst);
          // QubitType *qty = XGate->getType();
          // qubitType.push_back(qty);

          // inst.dump();
          Value *val = inst.getOperand(0);
          valSet.insert(val);
          // val->dump();

          Type *ty = val->getType();
        
          // PointerType *pty = (PointerType *) ty;
          // QubitType *qty = (QubitType *) pty->getElementType();
          // QubitType *qty = static_cast<QubitType*>(pty->getElementType());
          
          QubitType *qty = static_cast<QubitType*>(ty);

  
          if (qty){
            // cout << endl;
            cout << " - ID : " << qty << endl;
            qubitType.insert(qty);
          }
          
          // cout << inst.getOpcodeName();
          // cout << "\n-------------\n";
          
        }

      }
    }  
  }

  cout << endl;
  // cout << "valSet size: " << valSet.size() << endl;
  cout << "Qubit size: " << qubitType.size() << endl;

  errs() << "verifying... ";
  if (verifyModule(*mm)) {
    errs() << argv[0] << ": Error constructing function!\n";
    return 1;
  }

  errs() << "OK\n";
  errs() << "We just constructed this LLVM module:\n\n---------\n" << *mm;
  return 0;
}