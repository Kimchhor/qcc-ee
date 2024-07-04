#include "llvm/Support/InitLLVM.h"
#include <llvm/Analysis/CallGraph.h>
#include <llvm/Analysis/CallPrinter.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/ReplaceConstant.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include <iostream>
using namespace llvm;
using namespace std;

static ExitOnError ExitOnErr;


int main(int argc, char **argv) {

  cout << endl;

  if (argc < 2) {
    errs() << "Expected an argument - IR file name\n";
    exit(1);
  }

  cout << " \n-------------- PLAY GROUND -------------" << endl << endl;

  // Parse the input LLVM IR file into a module.
  SMDiagnostic Err;
  LLVMContext Context;
  errs() << "FILE FROM: " << argv[1] << "\n\n";
  parseIRFile(argv[1], Err, Context);
  std::unique_ptr<Module> Mod = (parseIRFile(argv[1], Err, Context));
  if (!Mod) {
    Err.print(argv[0], errs());
    return 1;
  }

  Module *M = Mod.get();

  // -------------- PLAY GROUND -------------//






 // -------------- END PLAY GROUND -------------//

  if (verifyModule(*M)) {
    errs() << argv[0] << ": Error constructing function!\n";
    return 1;
  }

  errs() << "-------------- OK ---------------------\n";

  return 0;
}
