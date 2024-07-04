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

#include "llvm/Transforms/QCC/Layout/QCCLayout.h"
#include "llvm/Transforms/QCC/MD/QCCMachineDescription.h"
#include "llvm/Transforms/QCC/QODGAnalysis.h"

namespace qcc {

///////////////////////////////////////////////////////////////////////
// ObjectFile Class Declaration
///////////////////////////////////////////////////////////////////////

struct SymbolData {

  /** It can be use as address for AddrType */
  std::string name;

  /** The size of data area */
  unsigned size;

  /** The start address */
  unsigned address;

  /** List of physical qubit ID */
  std::vector<unsigned> vals;

  /**
   * This method will write the data area \n
   * e.g:
   * \code {.asm}
   *    ## Data area
   *    0x18:0,2,7,12,14
   *    0x40:0,7,6  
   * \endcode
   */
  void writeData(std::ostream &o) {
    int valSize = vals.size();
    o << "0x" << std::hex << address << ":";
    for (int i = 0; i < valSize; i++) {
      o << std::dec << vals[i];
      if (i < valSize - 1) {
        o << ",";
      }
    }
    o << std::endl;
  }

  /**
   * This method will write the symbol area \n
   * e.g:
   * \code {.asm}
   *    ## Symbol area
   *    .rval: 0x8
   *    .data_qb: 0x18
   *    .data_mxx_cnot: 0x40   
   * \endcode
   */
  void writeSymbol(std::ostream &o) {
    o << "." << name << ": 0x" << std::hex << address;
    o << std::endl;
  }
};

class ObjectFile {

public:
  ObjectFile(QODGAnalysis &QODG, QCCMachineDescription &QMD, QCCLayout *layout);

  // Data area
  std::vector<SymbolData> sysmbolData;

  /**
   * This method will write the data area \n
   * e.g:
   * \code {.asm}
   *    ## Data area
   *    0x18:0,2,7,12,14
   *    0x40:0,7,6  
   * \endcode
   */
  void writeDataArea(std::ostream &o);

  /**
   * This method will write the symbol area \n
   * e.g:
   * \code {.asm}
   *    ## Symbol area
   *    .rval: 0x8
   *    .data_qb: 0x18
   *    .data_mxx_cnot: 0x40   
   * \endcode
   */
  void writeSymbolTable(std::ostream &o);
};

} // namespace qcc

#endif