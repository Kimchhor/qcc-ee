/*********************************************************************
 *   QASMGen.cpp:
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

#include "llvm/Transforms/QCC/QASMGen.h"
#include "fstream"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/QCC/MultiGateAnalysis.h"

#include "llvm/Support/CommandLine.h"

#include "llvm/Transforms/QCC/MD/QCCMachineDescription.h"
#include "llvm/Transforms/QCC/QubitMapping/QCCBFSQubitMapping.h"
#include "llvm/Transforms/QCC/QubitMapping/QCCSABREQubitMapping.h"

#include "llvm/Transforms/QCC/QISA/ConstantGlobal.h"

using namespace llvm;

using json = nlohmann::json;

#define DEBUG_TYPE "qasm-gen"

enum QubitMappingType { NONE, BFS };

std::string MD_PATH;
std::string OUT_DIR;

static cl::opt<std::string, true>
    MachindDescriptionPath("md", cl::desc("Input Machine Description"),
                           cl::Hidden, cl::location(MD_PATH));

static cl::opt<std::string, true>
    OutputDir("dir", cl::desc("Output file"),
                           cl::Hidden, cl::location(OUT_DIR));

cl::opt<QubitMappingType> QubitMappingMethod(
    "mapping", cl::desc("Choose Qubit Mapping Method:"),
    cl::init(QubitMappingType::NONE),
    cl::values(clEnumVal(NONE, "Enable default optimizations"),
               clEnumVal(BFS, "Enable expensive optimizations")));

static cl::opt<std::string>
    RoutingMethod("routing", cl::init(""),
                  cl::desc("Input Qubit Mapping Method"));
static cl::opt<std::string>
    LayoutMethod("layout", cl::init(""),
                 cl::desc("Input Qubit Mapping Method"));

/**
 * This method will generate the qasm filename depend on
 * source_filename in IR or filename of IR
 *
 * @param string sourceFileName is the name that set in LLVM IR
 * @return string filename with .qasm extension
 */
string QASMGen::getFileName(string sourceFileName) {
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
  fileName = fileName.substr(0, pos) + ".qasm";
  return fileName;
}

/**
 * This method will generate a node of an allocation
 *
 * @param unsigned numberOfRegisters is the amount of registers.
 * e.g,.: if q[3],so it is 3.
 * @param string instruction name. e.g,. qregs or cregs.
 * @param string variable name. e.g,. q or c.
 * @return string a QASM allocation. e.g,. qregs q[n] or cregs c[n];
 */
string QASMGen::getQASMAllocation(unsigned numberOfRegisters,
                                  string instruction, string name) {
  return instruction + " " + name + "[" + to_string(numberOfRegisters) + "];\n";
}

/**
 * This method will generate a node a an qubit register (allocation)
 *
 * @param unsigned numberOfRegisters is the amount of qubit registers.
 * e.g,.: if qreg q[3],so it is 3.
 * @return string a QASM qubit register. e.g,. qregs q[n]
 */
string QASMGen::getQASMQubitAllocation(unsigned numberOfRegisters) {
  return getQASMAllocation(numberOfRegisters, "qreg", "q");
}

/**
 * This method will generate a node a an classical register (allocation)
 *
 * @param unsigned numberOfRegisters is the amount of classical registers.
 * e.g,.: if creg c[2],so it is 2.
 * @return string a QASM classical register. e.g,. cregs c[n]
 */
string QASMGen::getQASMClassicalAllocation(unsigned numberOfRegisters) {
  return getQASMAllocation(numberOfRegisters, "creg", "c");
}

/**
 * This method will return the header of QASM file format
 *
 * @return string a header file of QASM
 */
string QASMGen::getQASMHeader() {
  return "OPENQASM 2.0;\ninclude \"qelib1.inc\";\n";
}

/**
 * This method will generate the QASM instruction using NodeGate object
 *
 * @param NodeGate a node contains llvm instructions and dependcy qubit indexs
 * @return string a QASM instruction. e.g,. h q[1];
 */
string QASMGen::getQASMInstruction(NodeGate &node) {

  // Gates in node must be not empty.
  if (node.gates.empty()) {
    errs() << "getQASMInstruction: gates is empty.\n";
    return "";
  }

  // quantum and classical register will print on another function
  if (node.type == gateType::c_register || node.type == gateType::q_register) {
    return "";
  }

  llvm::Instruction &inst = *node.gates[0];

  // qasm not support I gate
  if (IGateInst *II = dyn_cast<IGateInst>(&inst)) {
    (void)*II;
    return "";
  }

  // get operation name: x, y, z ... swap, cx, cxx
  string str = node.getOperationName();

  // rotation value for rx, ry, rz
  if (node.type == gateType::rotation) {
    str += "(" + to_string(node.rotationValue) + ")";
  }

  // measure : measure q[n] -> c[n];
  if (node.type == gateType::measure) {

    str += " q[" + to_string(node.qubitIndex[0]) + "] -> ";
    str += "c[" + to_string(node.bitIndex) + "];\n";
  } else {

    if (isa<MultiGateInst>(inst)) {

      // swap the cx1 gate to the last, to make the target gate
      // is at the last index
      for (unsigned idx = 0; idx < node.gates.size(); idx++) {
        if (isa<CX1GateInst>(node.gates[idx])) {
          std::swap(node.qubitIndex[idx], node.qubitIndex.back());
          std::swap(node.gates[idx], node.gates.back());
        }
      }
    }

    // other gate: gate q[n], q[n];
    for (auto &idx : node.qubitIndex) {
      str += " q[" + to_string(idx) + "]";
      str += (&idx != &node.qubitIndex.back()) ? "," : ";\n";
    }
  }

  return str;
}

/**
 * This method will run the pass
 *
 */
PreservedAnalyses QASMGen::run(Function &F, FunctionAnalysisManager &AM) {

  // Using QODG Anslysis Pass
  QODGAnalysis QODG;
  QODG.run(F, AM);

  if (MD_PATH != "") {

    qcc::QCCMachineDescription QMD = qcc::QCCMachineDescription::create(
        MD_PATH, QODG.quantumRegisters.size());

    switch (QubitMappingMethod) {
    case NONE:

      // TODO: Check routing and layout

      break;
    case BFS:

      MultiGateAnalysisPass MGA;
      MGA.run(F, AM);

      // qcc::QCCBFSQubitMapping bfsQubitMapping(QMD, QODG, MGA);
      // bfsQubitMapping.execute();
      qcc::QCCSABREQubitMapping sabreQubitMapping(QMD, QODG, MGA);
      sabreQubitMapping.execute();

      break;
    }
  }

  // Generate filename depend on source_filename in IR or filename of IR
  string fileName = getFileName(F.getParent()->getSourceFileName());

  if (OUT_DIR != "") {
    if (OUT_DIR.back() != '/'){
      OUT_DIR = OUT_DIR + '/';
    }
    fileName = OUT_DIR + fileName;
  }
    
  std::ofstream file(fileName);

  string content = "";

  // Header: OPENQASM 2.0 ...
  content += getQASMHeader();

  // Register Qubit: qreg
  if (QODG.quantumRegisters.size() > 0)
      content += getQASMQubitAllocation(QODG.quantumRegisters.size());

  // Register Classical Bit: creg
  if (QODG.classicalRegisters.size() > 0)
      content += getQASMClassicalAllocation(QODG.classicalRegisters.size());

  // Instruction: x, y, z, h, ..., cx, cxx, swap
  for (auto node : QODG.QODNodes) {
    content += getQASMInstruction(*node);
  }

  file << content;
  cout << content;
  // Close file
  file.close();

  return PreservedAnalyses::all();
}
