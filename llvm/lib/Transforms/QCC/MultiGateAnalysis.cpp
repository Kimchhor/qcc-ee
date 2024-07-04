/*********************************************************************
 *   MultiGateAnalysis.cpp:
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

#include "llvm/Transforms/QCC/MultiGateAnalysis.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "multi-gate-ia"

STATISTIC(NumOfCnot, "Number of CNOT Gate.");
STATISTIC(NumOfToffoli, "Number of Toffoli Gate.");

/**
 * This method will check if the Toffoli gate detected
 *
 * @param unsigned int id It is QubitID of CX0Gate or CX1Gates
 */
bool MultiGateAnalysisPass::isToffoli(unsigned id) {

  for (auto toffoli : toffoliList) {
    if (std::get<0>(toffoli)->getQubitID() == id) {
      return true;
    }
  }

  return false;
}

/**
 * This method will get the last ID of cnot/toffoli gates
 *
 * @param Function &F Function
 * @param FunctionAnalysisManager &AM Function Analysis Manager
 */
unsigned int MultiGateAnalysisPass::getLastID() {

  CnotList tempCnots = cnotList;
  ToffoliList tempToffolies = toffoliList;
  SwapList tempSwaps = swapList;

  // sort cnot gates by id to get the last ID
  std::sort(tempCnots.begin(), tempCnots.end(), [](auto &left, auto &right) {
    return left.first->getQubitID() < right.first->getQubitID();
  });

  // sort toffoli gates by id to get the last ID
  std::sort(tempToffolies.begin(), tempToffolies.end(),
            [](auto &left, auto &right) {
              return std::get<0>(left)->getQubitID() <
                     std::get<0>(right)->getQubitID();
            });

  // sort swap gates by id to get the last ID
  std::sort(tempSwaps.begin(), tempSwaps.end(), [](auto &left, auto &right) {
    return left.first->getQubitID() < right.first->getQubitID();
  });

  // check if cnots, toffolies, swap is empty then set id to 0
  unsigned int cnot_id =
      (tempCnots.size() > 0) ? tempCnots.back().first->getQubitID() : 0;
  unsigned int toffoli_id =
      (tempToffolies.size() > 0)
          ? std::get<0>(tempToffolies.back())->getQubitID()
          : 0;
  unsigned int swap_id =
      (tempSwaps.size() > 0) ? swapList.back().first->getQubitID() : 0;

  // find the last id among cnot, toffoli, swap gate
  unsigned int last_id = (toffoli_id >= cnot_id) ? toffoli_id : cnot_id;
  last_id = (last_id >= swap_id) ? last_id : swap_id;

  return last_id;
}

/**
 * This method will run the pass
 *
 */
PreservedAnalyses MultiGateAnalysisPass::run(Function &F,
                                             FunctionAnalysisManager &AM) {

  for (BasicBlock &BB : F) {
    for (Instruction &inst : BB) {

      bool isExist = false;

      if (isa<SwapGateInst>(inst)) {

        for (unsigned idx = 0; idx < swapList.size(); ++idx) {
          auto &swapPair = swapList[idx];

          SwapGateInst *swap = dyn_cast<SwapGateInst>(swapPair.first);
          SwapGateInst *currentSwap = dyn_cast<SwapGateInst>(&inst);

          if (swap->getQubitID() == currentSwap->getQubitID()) {
            if (dyn_cast_or_null<SwapGateInst>(swapPair.second)) {
              swapPair.second = currentSwap;
              break;
            }
            isExist = true;
          }
        }

        if (!isExist) {
          SwapPair tempPair;
          SwapGateInst *currentCX = dyn_cast<SwapGateInst>(&inst);
          tempPair.first = currentCX;
          swapList.push_back(tempPair);
        }
      }

      if (isa<CXGateInst>(inst)) {

        for (unsigned idx = 0; idx < cnotList.size(); ++idx) {

          auto &cnot = cnotList[idx];

          if (isa<CXGateInst>(cnot.first)) {

            // cast Instructions to CXGattInst object
            CXGateInst *CX = dyn_cast<CXGateInst>(cnot.first);
            CXGateInst *currentCX = dyn_cast<CXGateInst>(&inst);

            // compare gate id
            if (CX->getQubitID() == currentCX->getQubitID()) {

              CXGateInst *secondCX = dyn_cast_or_null<CXGateInst>(cnot.second);

              // if the second gate in pair exist, it must be toffoli gate.
              if (secondCX) {

                // toffoli gate
                ToffoliPair toffoli;
                toffoli = std::make_tuple(cnot.first, cnot.second, currentCX);
                toffoliList.push_back(toffoli);

                // remove current pair of cnot in vector because it's toffoli
                // gate
                cnotList.erase(cnotList.begin() + idx);

              } else {

                cnot.second = currentCX;
              }

              isExist = true;
              break;
            }
          }
        }

        // check if this multi-gate not exist; then init pair for it to vector.
        if (!isExist) {
          CnotPair tempPair;
          CXGateInst *currentCX = dyn_cast<CXGateInst>(&inst);
          tempPair.first = currentCX;
          cnotList.push_back(tempPair);
        }
      }
    }
  }

  // count the number of cnot and toffoli gate
  NumOfCnot = cnotList.size();
  NumOfToffoli = toffoliList.size();

  return PreservedAnalyses::all();
}
