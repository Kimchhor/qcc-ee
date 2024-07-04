/*********************************************************************
 *   QISAGen.cpp:
 *
 *   Authors:
 *      Sengthai Heng (sengthai37@gmail.com)
 *      Youngsun Han (youngsun@pknu.ac.kr)
 *
 *
 *   Copyright (c) 2021
 *
 *   High Performance Computing Laboratory (hpcl.pknu.ac.kr)
 **********************************************************************/

#ifndef LLVM_TRANSFORMS_QISAGEN_QISAGEN_H
#define LLVM_TRANSFORMS_QISAGEN_QISAGEN_H

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"

#include "llvm/Transforms/QCC/QODGAnalysis.h"

#include "llvm/Transforms/QCC/QubitMapping/QCCAncillaQubitMapping.h"
#include "llvm/Transforms/QCC/QubitMapping/QCCBFSQubitMapping.h"

#include "llvm/Transforms/QCC/QISA/ObjectFile.h"
#include "llvm/Transforms/QCC/QISA/Program.h"

namespace llvm {

///////////////////////////////////////////////////////////////////////
// DecomposeSwapPass Class Declaration
///////////////////////////////////////////////////////////////////////

class QISAGen : public PassInfoMixin<QISAGen> {

public:
  ///////////////////////////////////////////////////////////////////////
  // Member Function Definition
  ///////////////////////////////////////////////////////////////////////

  /**
   * This method will run the pass
   *
   */
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  /**
   * This method will generate the qasm filename depend on
   * source_filename in IR or filename of IR
   *
   * @param sourceFileName the name that set in LLVM IR
   * @return string filename with .qasm extension
   */
  string getFileName(string sourceFileName);
};
} // namespace llvm

#endif