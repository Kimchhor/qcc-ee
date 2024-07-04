#include <iostream>
#include <map>

#include <list>
#include <optional>
#include <tuple>
#include <vector>

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

#include <llvm/Transforms/QCC/QODGAnalysis.h>

#include <llvm/Transforms/QCC/MultiGateAnalysis.h>
#include <llvm/Transforms/QCC/NodeGate.h>
#include <llvm/Transforms/QCC/QASMGen.h>

#include "llvm/Transforms/QCC/QubitMapping/QCCBFSQubitMapping.h"

#include "llvm/Transforms/QCC/QubitMapping/QCCAncillaQubitMapping.h"

#include <algorithm>
#include <cstdlib>
#include <map>

#include <set>

// using namespace std;

#include "llvm/Transforms/QCC/QISA/ObjectFile.h"
#include "llvm/Transforms/QCC/QISA/Program.h"

int main(int argc, char **argv) {

  cout << endl;

  if (argc < 2) {
    cout << "Expected an argument - IR file name\n";
    exit(1);
  }

  // Parse the input LLVM IR file into a module.
  SMDiagnostic Err;
  LLVMContext Context;
  errs() << "FILE FROM: " << argv[2] << "\n\n";
  parseIRFile(argv[1], Err, Context);
  std::unique_ptr<Module> Mod = (parseIRFile(argv[2], Err, Context));
  if (!Mod) {
    Err.print(argv[0], errs());
    return 1;
  }

  Module *M = Mod.get();
  M->dump();

  QODGAnalysis QODG;
  QASMGen gen;
  MultiGateAnalysisPass MGA;

  FunctionAnalysisManager FAM;

  for (auto &Fn : *M) {
    MGA.run(Fn, FAM);
    QODG.run(Fn, FAM);
    gen.run(Fn, FAM);
  }

  int numberOfRegisterQubit = QODG.quantumRegisters.size();

  // Parse Marchine Description
  string jsonString = qcc::readFileIntoString(argv[1]);
  qcc::MachineDescription MD;
  json jj = json::parse(jsonString);
  qcc::from_json(jj, MD);

  // QCCMachineDescription
  qcc::QCCMachineDescription QMD(numberOfRegisterQubit, MD);
  for (auto n : QODG.QODNodes) {
    n->name = n->getOperationName();
  }

  // set up layout
  qcc::QCCTrivialLayout layout(QMD, QODG);
  layout.run();

  // perform qubit mapping
  qcc::QCCBFSQubitMapping QM(QMD, QODG, MGA);
  QM.execute();

  // Select Ancilla
  qcc::QCCAncillaQubitMapping AQM(QMD, QODG, &layout);
  AQM.execute();
  
  // print ancilla qubit
  for (auto node : QODG.QODNodes) {

    if (isa<CXGateInst>(node->gates[0])) {
      auto cQID = layout.getPhysicalQubitBy(node->qubitIndex[0]);
      auto tQID = layout.getPhysicalQubitBy(node->qubitIndex[1]);

      cout << "\nControl QID = " << cQID << endl;
      cout << "Target QID = " << tQID << endl;
      cout << "Ancilla QID = " << node->ancilla->selectedQID << endl;
      cout << "Stabilizer = "
           << ((node->ancilla->stabilizer) ? "Z: |0〉 " : "X: |+〉") << endl;
      cout << "---------------------------------------------\n";
    }
  }

  cout << "\n\n\n----------------------------------\n\n\n";


  qcc::ObjectFile kernel(QODG, QMD, &layout);


  qcc::Block block(QODG, &layout, &kernel);

  vector<qcc::Block*> blocks;
  blocks.push_back(&block);

  qcc::Program program("main", blocks, kernel);
  
  cout << &program;
  

  cout << "\n\n\n----------------------------------\n\n\n";

  QODG.dumpLiveInterval();
  QODG.dumpScanIntervalRegLoc(NUMBER_OF_REGISTER, TEMP_QR, ANCILLA_QR);

  cout << "\n-------------------\n";

  // M->dump();

  // QASMGen gen1;
  // for (auto &f : *M) {
  //   gen1.run(f, FAM);
  // }

  cout << "\n-------------- OK ---------------------\n\n";
  cout << "\n________________________________________\n\n";
  return 0;
}
