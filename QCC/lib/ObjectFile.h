/*********************************************************************
 *   ObjectFile.h:
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

#ifndef QCC_QCCOBJECTFILE
#define QCC_QCCOBJECTFILE

#include <iostream>
#include <vector>

#include <llvm/Transforms/QCC/Layout/QCCLayout.h>
#include <llvm/Transforms/QCC/MD/QCCMachineDescription.h>
#include <llvm/Transforms/QCC/QODGAnalysis.h>

namespace qcc {

///////////////////////////////////////////////////////////////////////
// ObjectFile Class Declaration
///////////////////////////////////////////////////////////////////////

struct SymbolData {

  std::string name;
  unsigned size;
  unsigned address;
  std::vector<unsigned> vals;

  void writeData(std::ostream &o) {

    o << "0x" << std::hex << address << ":";
    for (int i = 0; i < vals.size(); i++) {
      o << std::dec << vals[i];
      if (i < vals.size() - 1) {
        o << ",";
      }
    }
    o << std::endl;
  }

  void writeSymbol(std::ostream &o) {
    o << "." << name << ": 0x" << std::hex << address;
    o << std::endl;
  }
};

class ObjectFile {

public:
  ObjectFile(QODGAnalysis &QODG, QCCMachineDescription &QMD,
                QCCLayout *layout);

  // Data area
  std::vector<SymbolData> sysmbolData;

  void writeDataArea(std::ostream &o);

  void writeSymbolTable(std::ostream &o);
};

} // namespace qcc

#endif