/*********************************************************************
 *   QCCRouting.h:
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

#ifndef QCC_QCCROUTING
#define QCC_QCCROUTING

#include <chrono>

#include "llvm/Transforms/QCC/QODGAnalysis.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QCCRouting Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCRouting {

private:
  double executionTime = 0.0;

  virtual void perform() = 0;

public:
  /**
   * This method will trigger the virtual perform method.
   */
  void run() {
    auto start = std::chrono::high_resolution_clock::now();

    perform();

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = stop - start;

    executionTime = ms_double.count();
  };

  inline double getExecutionTime() { return executionTime; }
};

} // namespace qcc

#endif