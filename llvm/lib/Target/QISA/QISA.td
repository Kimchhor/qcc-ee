//===-- QISA.td - Describe the eQISA Target Machine -------*- tablegen -*-===//
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
// QISA Subtarget features.
//

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "QISARegisterInfo.td"
include "QISACallingConv.td"
include "QISASchedule.td"
include "QISAInstrFormats.td"
include "QISAInstrInfo.td"

//===----------------------------------------------------------------------===//
// QISA processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"qisa", QISASchedModel, []>;

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def QISAInstrInfo : InstrInfo;
def QISAAsmParser : AsmParser;
def QISAAsmParserVariant : AsmParserVariant {
  let RegisterPrefix = "%";
}

def QISAAsmWriter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  int PassSubtarget = 1;
  int Variant = 0;
}

def QISA : Target {
  let InstructionSet = QISAInstrInfo;
  let AssemblyParsers  = [QISAAsmParser];
  let AssemblyParserVariants = [QISAAsmParserVariant];
  let AssemblyWriters = [QISAAsmWriter];
  let AllowRegisterRenaming = 1;
}
