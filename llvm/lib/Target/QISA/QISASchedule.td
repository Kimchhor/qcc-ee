//===-- QISASchedule.td - QISA Scheduling Model ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

def QISASchedModel : SchedMachineModel {
  // The M88110 can issue 2 instructsions.
  let IssueWidth = 2;

  // In-order CPU.
  let MicroOpBufferSize = 0;

  //let LoadLatency = ?;
  //let MispredictPenalty = ?;

  // Not finished yet.
  let CompleteModel = 0;
  let NoModel = 1;
}

def ALU : ProcResource<2> {let BufferSize = 0; }
def BF :  ProcResource<1> {let BufferSize = 0; }
//def LS : ProcResource<1> { let BufferSize = 0; }

def ReadJmp   : SchedRead;
def ReadALU   : SchedRead;
def ReadBF    : SchedRead;
def WriteJmp  : SchedWrite;    // Jump
def WriteALU  : SchedWrite;
def WriteBF   : SchedWrite;
//def WriteLS   : SchedWrite;


let SchedModel = QISASchedModel in {
  def : WriteRes<WriteALU, [ALU]>   { let Latency = 1; }
  def : WriteRes<WriteBF, [BF]>     { let Latency = 1; }
  def : WriteRes<WriteJmp, [BF]>     { let Latency = 1; }
  def : ReadAdvance<ReadJmp, 0>;
}
