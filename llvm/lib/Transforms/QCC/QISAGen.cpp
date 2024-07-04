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

#include "llvm/Transforms/QCC/QISAGen.h"
#include "llvm/Transforms/QCC/QISA/ConstantGlobal.h"

using namespace llvm;

#define DEBUG_TYPE "qisa-gen"

/**
 * This method will run the pass
 *
 */
PreservedAnalyses QISAGen::run(Function &F, FunctionAnalysisManager &AM) {

  assert((MD_PATH != "" ) && "No Machine Description Path");  

  // Using Multi gate analysis
  MultiGateAnalysisPass MGA;
  MGA.run(F, AM);

  // Using QODG Anslysis Pass
  QODGAnalysis QODG;
  QODG.run(F, AM);

    auto QMD =
    qcc::QCCMachineDescription::create(MD_PATH, QODG.quantumRegisters.size());

    // set up layout
    qcc::QCCTrivialLayout layout(QMD, QODG);
    layout.run();

    // perform qubit mapping
    qcc::QCCBFSQubitMapping QM(QMD, QODG, MGA);
    QM.execute();
    // qcc::QCCSABREQubitMapping QM(QMD, QODG, MGA);
    // QM.execute();

    // Select Ancilla
    qcc::QCCAncillaQubitMapping AQM(QMD, QODG, &layout);
    AQM.execute();

    // Object File Format
    qcc::ObjectFile kernel(QODG, QMD, &layout);

    // Kernel
    qcc::Block block(QODG, &layout, &kernel);

    vector<qcc::Block *> blocks;
    blocks.push_back(&block);

    // initail Program
    qcc::Program program("main", blocks, kernel);

    // Generate filename depend on source_filename in IR or filename of IR
    string fileName = getFileName(F.getParent()->getSourceFileName());


    if (OUT_DIR != "") {
        if (OUT_DIR.back() != '/'){
          OUT_DIR = OUT_DIR + '/';
        }
        fileName = OUT_DIR + fileName;
    }
    
    std::ofstream file(fileName);
    

    file << &program;

    cout << &program;

    file.close();

  return PreservedAnalyses::all();
}

/**
 * This method will generate the qasm filename depend on
 * source_filename in IR or filename of IR
 *
 * @param string sourceFileName is the name that set in LLVM IR
 * @return string filename with .qasm extension
 */
string QISAGen::getFileName(string sourceFileName) {
  string fileName = sourceFileName;
  string delimiter = "/";
  size_t pos = 0;
  std::string token;

  // find the "/" and split it, then take only the last token
  while ((pos = fileName.find(delimiter)) != std::string::npos) {
    token = fileName.substr(0, pos);
    fileName.erase(0, pos + delimiter.length());
  }

  // change the file extension to .qasm
  pos = fileName.find_last_of(".");
  fileName = fileName.substr(0, pos) + ".qisa";
  return fileName;
}