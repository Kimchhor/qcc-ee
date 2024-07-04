/*********************************************************************
 *   QCCSABREQubitMapping.cpp:
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

#include "llvm/Transforms/QCC/QubitMapping/QCCSABREQubitMapping.h"
// #include <vector>
// #include <unordered_map>
// #include <algorithm>
// #include <iostream>

///////////////////////////////////////////////////////////////////////
// QCCSABREQubitMapping Class Declaration
///////////////////////////////////////////////////////////////////////
namespace qcc {
    


// class QuantumGate {
// public:
//     std::vector<int> getQubits() const {
//         // Return qubit indices involved in this gate
//         return {};
//     }
// };

// class QuantumCircuit {
// public:
//     std::vector<QuantumGate> gates;

//     void addGate(const QuantumGate& gate) {
//         gates.push_back(gate);
//     }

//     std::vector<int> getQubits() const {
//         return {};
//     }

//     const std::vector<QuantumGate>& getInstructions() const {
//         return gates;
//     }
// };

// using Matrix = std::vector<std::vector<double>>; // Simplified matrix type for distance calculations
// using Mapping = std::unordered_map<int, int>; // Logical to physical qubit mapping

// class DirectedGraph {
// public:
//     using Node = QuantumGate;

//     std::unordered_map<Node, std::vector<Node>> adjList;

//     void addEdge(const Node& src, const Node& dest) {
//         adjList[src].push_back(dest);
//     }

//     std::vector<Node> getSuccessors(const Node& node) const {
//         if (adjList.find(node) != adjList.end()) {
//             return adjList.at(node);
//         }
//         return {};
//     }

//     std::vector<Node> getNodesWithNoIncomingEdges() const {
//         std::unordered_map<Node, int> inDegree;
//         for (const auto& p : adjList) {
//             for (const auto& dest : p.second) {
//                 inDegree[dest]++;
//             }
//         }

//         std::vector<Node> result;
//         for (const auto& p : adjList) {
//             if (inDegree[p.first] == 0) {
//                 result.push_back(p.first);
//             }
//         }
//         return result;
//     }
// };

// double calculateDistance(const QuantumGate& gate, const Matrix& distanceMatrix, const Mapping& mapping) {
//     auto qubits = gate.getQubits();
//     return distanceMatrix[mapping.at(qubits[0])][mapping.at(qubits[1])];
// }

// std::vector<QuantumGate> createExtendedSuccessorSet(const std::vector<QuantumGate>& F, const DirectedGraph& dag) {
//     std::vector<QuantumGate> E;
//     for (const auto& gate : F) {
//         auto successors = dag.getSuccessors(gate);
//         for (const auto& successor : successors) {
//             if (E.size() <= 20) {
//                 E.push_back(successor);
//             } else {
//                 break;
//             }
//         }
//     }
//     return E;
// }

// double heuristicFunction(const std::vector<QuantumGate>& F, const DirectedGraph& circuitDAG, const Mapping& initialMapping, const Matrix& distanceMatrix, const QuantumGate& swapGate, const std::vector<double>& decayParameter) {
//     auto E = createExtendedSuccessorSet(F, circuitDAG);
//     auto swapQubits = swapGate.getQubits();
//     double maxDecay = std::max(decayParameter[swapQubits[0]], decayParameter[swapQubits[1]]);
//     double fDistance = 0, eDistance = 0;
//     double W = 0.5;
//     int sizeF = F.size(), sizeE = E.size();

//     for (const auto& gate : F) {
//         fDistance += calculateDistance(gate, distanceMatrix, initialMapping);
//     }
//     for (const auto& gate : E) {
//         eDistance += calculateDistance(gate, distanceMatrix, initialMapping);
//     }

//     fDistance /= sizeF;
//     eDistance = W * (eDistance / sizeE);
//     double H = maxDecay * (fDistance + eDistance);

//     return H;
// }

// int main() {
//     QuantumCircuit circuit;
//     // Populate circuit with gates

//     DirectedGraph circuitDAG;
//     // Build the DAG from the circuit

//     Matrix distanceMatrix;
//     // Initialize distance matrix

//     Mapping initialMapping;
//     // Initialize mapping

//     std::vector<double> decayParameter;
//     // Initialize decay parameters

//     QuantumGate swapGate;
//     // Define a swap gate

//     auto F = circuitDAG.getNodesWithNoIncomingEdges();
//     double heuristicScore = heuristicFunction(F, circuitDAG, initialMapping, distanceMatrix, swapGate, decayParameter);

//     std::cout << "Heuristic score: " << heuristicScore << std::endl;

//     return 0;
// }
/**
 * This method init the QCCBFSQubitMapping object
 *
 * @param machineDescription QCC MachineDescription.
 * @param qodg the QODG Analysis pass
 * @param multiGateAnalysis the MultiGat eAnalysis Pass
 *
 */
QCCSABREQubitMapping::QCCSABREQubitMapping(
    QCCMachineDescription &machineDescription, QODGAnalysis &qodg,
    MultiGateAnalysisPass &multiGateAnalysis)
    : machineDescription(machineDescription), qodg(qodg),
      multiGateAnalysis(multiGateAnalysis) {}

/**
 * This method performs the Trivial layout then BFS routhing method
 *
 */
void QCCSABREQubitMapping::perform() {

  qcc::QCCTrivialLayout layout(machineDescription, qodg);
  layout.run();

  qcc::QCCBFSRouting routing(&layout, qodg, machineDescription, multiGateAnalysis);
  routing.run();

  errs() << "\n[-------SABRE Qubit Mapping Successfully-------]\n\n";
}

} // namespace qcc