/*********************************************************************
 *   MachineDescription.h:
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

#ifndef QCC_MACHINEDESCRIPTION
#define QCC_MACHINEDESCRIPTION

#include "llvm/Transforms/QCC/MD/json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

namespace qcc {

enum class Boundary : int { Z, REVERSE_Z, X, REVERSE_X };

struct Connectivity {
  unsigned id;
  unsigned src;
  unsigned dst;
  std::shared_ptr<unsigned> direction;
  std::shared_ptr<Boundary> boundary;
};

/**
 * Type of qubits
 */
enum class Type : int { D /** Data qubit */, I /** Intermittent qubit */ };

struct Node {
  unsigned id;
  unsigned x;
  unsigned z;
  Type type;

  inline bool operator==(const Node rhs) { return id == rhs.id; }

  /**
   * This method will return true if node is Data qubit
   */
  inline bool isDataQubit() { return (type == Type::D); }

  /**
   * This method will return true if node is Intermittent qubit
   */
  inline bool isAncillaQubit() { return (type == Type::I); }
};

struct Topology {
  std::string description;
  std::string type;
  unsigned x_size;
  unsigned z_size;
  std::vector<Node> nodes;
  std::vector<Connectivity> edges;
  std::vector<Connectivity> connectivities;

  /**
   * This method will get qubits (node) by type of of Node
   *
   * @param nodeType the Type of node such as D (data), I (Intermittent)
   */
  inline std::vector<Node> getNodes(Type nodeType) {
    std::vector<qcc::Node> qubits;
    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(qubits),
                 [nodeType](qcc::Node n) { return (n.type == nodeType); });
    return qubits;
  }

  /**
   * This method will return the Data Qubits (Nodes)
   */
  inline std::vector<Node> getDataNodes() { return getNodes(Type::D); }

  /**
   * This method will return the Ancilla Qubits (Nodes)
   */
  inline std::vector<Node> getAncillaNodes() { return getNodes(Type::I); }
};

struct MachineDescription {
  Topology topology;
};

std::string getBoundaryStr(Boundary boundary);

} // namespace qcc

namespace qcc {

std::string readFileIntoString(const std::string &path);
inline json get_untyped(const json &j, const char *property);
inline json get_untyped(const json &j, std::string property);

template <typename T>
inline std::shared_ptr<T> get_optional(const json &j, const char *property);

template <typename T>
inline std::shared_ptr<T> get_optional(const json &j, std::string property);

void from_json(const json &j, qcc::Connectivity &x);
void to_json(json &j, const qcc::Connectivity &x);

void from_json(const json &j, qcc::Node &x);
void to_json(json &j, const qcc::Node &x);

void from_json(const json &j, qcc::Topology &x);
void to_json(json &j, const qcc::Topology &x);

void from_json(const json &j, qcc::MachineDescription &x);
void to_json(json &j, const qcc::MachineDescription &x);

void from_json(const json &j, qcc::Boundary &x);
void to_json(json &j, const qcc::Boundary &x);

void from_json(const json &j, qcc::Type &x);
void to_json(json &j, const qcc::Type &x);
} // namespace qcc

#endif