//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     MachineDescription data = nlohmann::json::parse(jsonString);

// #pragma once

#include "llvm/Transforms/QCC/MD/MachineDescription.h"

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER

namespace nlohmann {
template <typename T> struct adl_serializer<std::shared_ptr<T>> {
  static void to_json(json &j, const std::shared_ptr<T> &opt) {
    if (!opt)
      j = nullptr;
    else
      j = *opt;
  }

  static std::shared_ptr<T> from_json(const json &j) {
    if (j.is_null())
      return std::unique_ptr<T>();
    else
      return std::unique_ptr<T>(new T(j.get<T>()));
  }
};
} // namespace nlohmann
#endif

namespace qcc {

void from_json(const json &j, qcc::Connectivity &x) {
  x.id = j.at("id").get<unsigned>();
  x.src = j.at("src").get<unsigned>();
  x.dst = j.at("dst").get<unsigned>();
  x.direction = qcc::get_optional<unsigned>(j, "direction");
  x.boundary = qcc::get_optional<qcc::Boundary>(j, "boundary");
}

void to_json(json &j, const qcc::Connectivity &x) {
  j = json::object();
  j["id"] = x.id;
  j["src"] = x.src;
  j["dst"] = x.dst;
  j["direction"] = x.direction;
  j["boundary"] = x.boundary;
}

void from_json(const json &j, qcc::Node &x) {
  x.id = j.at("id").get<unsigned>();
  x.x = j.at("x").get<unsigned>();
  x.z = j.at("z").get<unsigned>();
  x.type = j.at("type").get<qcc::Type>();
}

void to_json(json &j, const qcc::Node &x) {
  j = json::object();
  j["id"] = x.id;
  j["x"] = x.x;
  j["z"] = x.z;
  j["type"] = x.type;
}

void from_json(const json &j, qcc::Topology &x) {
  x.description = j.at("description").get<std::string>();
  x.type = j.at("Type").get<std::string>();
  x.x_size = j.at("x_size").get<unsigned>();
  x.z_size = j.at("z_size").get<unsigned>();
  x.nodes = j.at("nodes").get<std::vector<qcc::Node>>();
  x.edges = j.at("edges").get<std::vector<qcc::Connectivity>>();
  x.connectivities =
      j.at("connectivities").get<std::vector<qcc::Connectivity>>();
}

void to_json(json &j, const qcc::Topology &x) {
  j = json::object();
  j["description"] = x.description;
  j["Type"] = x.type;
  j["x_size"] = x.x_size;
  j["z_size"] = x.z_size;
  j["nodes"] = x.nodes;
  j["edges"] = x.edges;
  j["connectivities"] = x.connectivities;
}

void from_json(const json &j, qcc::MachineDescription &x) {
  x.topology = j.at("topology").get<qcc::Topology>();
}

void to_json(json &j, const qcc::MachineDescription &x) {
  j = json::object();
  j["topology"] = x.topology;
}

void from_json(const json &j, qcc::Boundary &x) {
  if (j == "-x")
    x = qcc::Boundary::REVERSE_X;
  else if (j == "-z")
    x = qcc::Boundary::REVERSE_Z;
  else if (j == "+x")
    x = qcc::Boundary::X;
  else if (j == "+z")
    x = qcc::Boundary::Z;
  else
    std::cout << "Input JSON does not conform to schema";
}

void to_json(json &j, const qcc::Boundary &x) {
  switch (x) {
  case qcc::Boundary::REVERSE_X:
    j = "-x";
    break;
  case qcc::Boundary::REVERSE_Z:
    j = "-z";
    break;
  case qcc::Boundary::X:
    j = "+x";
    break;
  case qcc::Boundary::Z:
    j = "+z";
    break;
  default:
    std::cout << "This should not happen";
  }
}

void from_json(const json &j, qcc::Type &x) {
  if (j == "d")
    x = qcc::Type::D;
  else if (j == "i")
    x = qcc::Type::I;
  else
    std::cout << "Input JSON does not conform to schema";
}

void to_json(json &j, const qcc::Type &x) {
  switch (x) {
  case qcc::Type::D:
    j = "d";
    break;
  case qcc::Type::I:
    j = "i";
    break;
  default:
    std::cout << "This should not happen";
  }
}

} // namespace qcc

namespace qcc {
using nlohmann::json;

std::string readFileIntoString(const std::string &path) {
  std::ifstream input_file(path);
  if (!input_file.is_open()) {
    std::cerr << "Could not open the file - '" << path << "'" << std::endl;
    exit(EXIT_FAILURE);
  }
  return std::string((std::istreambuf_iterator<char>(input_file)),
                     std::istreambuf_iterator<char>());
}

inline json get_untyped(const json &j, const char *property) {
  if (j.find(property) != j.end()) {
    return j.at(property).get<json>();
  }
  return json();
}

inline json get_untyped(const json &j, std::string property) {
  return get_untyped(j, property.data());
}

template <typename T>
inline std::shared_ptr<T> get_optional(const json &j, const char *property) {
  if (j.find(property) != j.end()) {
    return j.at(property).get<std::shared_ptr<T>>();
  }
  return std::shared_ptr<T>();
}

template <typename T>
inline std::shared_ptr<T> get_optional(const json &j, std::string property) {
  return get_optional<T>(j, property.data());
}

std::string getBoundaryStr(Boundary boundary) {
  switch (boundary) {
  case Boundary::REVERSE_X:
    return "-X";
  case Boundary::REVERSE_Z:
    return "-Z";
  case Boundary::X:
    return "X";
  case Boundary::Z:
    return "Z";
  default:
    return "";
  }
}

} // namespace qcc
