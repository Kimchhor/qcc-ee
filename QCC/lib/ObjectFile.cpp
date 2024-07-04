/*********************************************************************
 *   QCCKernelFile.cpp:
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

#include "./ObjectFile.h"

namespace qcc {

ObjectFile::ObjectFile(QODGAnalysis &QODG, QCCMachineDescription &QMD,
                             QCCLayout *layout) {

  int CONTINUATION_BYTE = 0x8;

  auto rval = qcc::SymbolData();
  rval.name = "rval";

  auto data_qb = qcc::SymbolData();
  data_qb.name = "data_qb";

  auto data_mxx_cnot = qcc::SymbolData();
  data_mxx_cnot.name = "data_mxx_cnot";

  auto data_mxx_swap = qcc::SymbolData();
  data_mxx_swap.name = "data_mxx_swap";

  // data_db
  for (auto node : QMD.getDataNodes()) {
    data_qb.size += CONTINUATION_BYTE;
    data_qb.vals.push_back(node.id);
  }

  for (auto node : QODG.QODNodes) {

    // rval
    if (node->type == gateType::measure) {
      // the value of rval is as address
      rval.vals.push_back(node->bitIndex * CONTINUATION_BYTE);
      rval.size += CONTINUATION_BYTE;
      continue;
    }

    // data_mxx_cnot
    if (isa<CXGateInst>(node->gates[0])) {

      auto cQID = layout->getPhysicalQubitBy(node->qubitIndex[0]);
      auto tQID = layout->getPhysicalQubitBy(node->qubitIndex[1]);
      auto ancillaQID = node->ancilla->selectedQID;

      data_mxx_cnot.vals.push_back(cQID);
      data_mxx_cnot.vals.push_back(tQID);
      data_mxx_cnot.vals.push_back(ancillaQID);

      // we add 3 values (cQID, tQID, ancillaQID), then the size increase 3
      // times
      data_mxx_cnot.size += CONTINUATION_BYTE * 3;
      continue;
    }

    // TODO: IT BE NEED 2 MORE ANCILLA QUBITS
    // data_mxx_swap
    if (isa<SwapGateInst>(node->gates[0])) {
      auto cQID = layout->getPhysicalQubitBy(node->qubitIndex[0]);
      auto tQID = layout->getPhysicalQubitBy(node->qubitIndex[1]);

      data_mxx_swap.vals.push_back(cQID);
      data_mxx_swap.vals.push_back(tQID);

      // because of adding 2 values (cQID, tQID), then the size increase 2 times
      data_mxx_swap.size += CONTINUATION_BYTE * 2;
      continue;
    }
  }

  if (rval.size > 0)
    sysmbolData.push_back(rval);
  
  if (data_qb.size > 0)
    sysmbolData.push_back(data_qb);

  if (data_mxx_cnot.size > 0)
    sysmbolData.push_back(data_mxx_cnot);

  if (data_mxx_swap.size > 0)
    sysmbolData.push_back(data_mxx_swap);

  for (unsigned int i = 0; i < sysmbolData.size(); i++) {

    int address = 0x8;

    if (i > 0) {
      address = sysmbolData[i - 1].address +
                sysmbolData[i - 1].size;
    }
    sysmbolData[i].address = address;
  }
}

void ObjectFile::writeDataArea(std::ostream &o) {
  o << "## Data area" << endl;
  for (auto ss : sysmbolData) {
    if (ss.name == "rval") continue;
    ss.writeData(o);
  }
}

void ObjectFile::writeSymbolTable(std::ostream &o) {
   o << "## Symbol area" << endl;
  for (auto ss : sysmbolData) {
    ss.writeSymbol(o);
  }
}

} // namespace qcc
