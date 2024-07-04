//===-- QASM.td - Describe the eQASM Target Machine -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// QASM Subtarget features.
//

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "QASMRegisterInfo.td"
include "QASMCallingConv.td"
include "QASMSchedule.td"
include "QASMInstrFormats.td"
include "QASMInstrInfo.td"

//===----------------------------------------------------------------------===//
// QASM processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"qasm", QASMSchedModel, []>;

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def QASMInstrInfo : InstrInfo;
def QASMAsmParser : AsmParser;
def QASMAsmParserVariant : AsmParserVariant {
  let RegisterPrefix = "%";
}

def QASMAsmWriter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  int PassSubtarget = 1;
  int Variant = 0;
}

def QASM : Target {
  let InstructionSet = QASMInstrInfo;
  let AssemblyParsers  = [QASMAsmParser];
  let AssemblyParserVariants = [QASMAsmParserVariant];
  let AssemblyWriters = [QASMAsmWriter];
  let AllowRegisterRenaming = 1;
}
