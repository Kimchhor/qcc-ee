/*********************************************************************
 *   QCCQubitMapping.h:
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

#ifndef QCC_QCCQUBITMAPPING
#define QCC_QCCQUBITMAPPING

#include <chrono>
#include <iostream>

namespace qcc {

///////////////////////////////////////////////////////////////////////
// QCC_QCCQUBITMAPPING Class Declaration
///////////////////////////////////////////////////////////////////////

class QCCQubitMapping {

private:
  double executionTime = 0.0;
  virtual void perform() = 0;

public:
  /**
   * This method will triger the virtual perform() function.
   */
  void execute() {
    auto start = std::chrono::high_resolution_clock::now();

    perform();

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = stop - start;

    executionTime = ms_double.count();
  }

  /** Return the execution time. */
  inline double getExecutionTime() { return executionTime; }
};

} // namespace qcc

#endif