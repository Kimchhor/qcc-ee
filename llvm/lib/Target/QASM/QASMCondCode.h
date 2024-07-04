//===-- QASMCondCode.h - QASM Condition Code definition --- -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that QASM uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_QASM_QASMCONDCODE_H
#define LLVM_LIB_TARGET_QASM_QASMCONDCODE_H

namespace llvm {

namespace QASMCC {
enum CondCode : unsigned {
  EQ = 2,  // Equal
  NE = 3,  // Not equal
  GT = 4,  // Signed greater than
  LE = 5,  // Signed less than or equal
  LT = 6,  // Signed less than
  GE = 7,  // Signed greater than or equal
  HI = 8,  // Unsigned greater than
  LS = 9,  // Unsigned less than or equal
  LO = 10, // Unsigned less than
  HS = 11, // Unsigned greater than or equal
  BE = 12, // Byte equal
  NB = 13, // No byte equal
  HE = 14, // Half-word equal
  NH = 15, // No half-word equal
};
} // end namespace QASMCC

} // end namespace llvm

#endif
