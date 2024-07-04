//===-- QISATargetStreamer.cpp - Mips Target Streamer Methods -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides QISA specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "QISATargetStreamer.h"
#include "QISADefine.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;
using namespace std;

//===----------------------------------------------------------------------===//
// QISATargetStreamer
//===----------------------------------------------------------------------===//

// Implement QISATargetStreamer here

//===----------------------------------------------------------------------===//
// QISATargetAsmStreamer
//===----------------------------------------------------------------------===//

QISATargetAsmStreamer::QISATargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS)
    : QISATargetStreamer(S), OS(OS) { }


// A hook for emitting stuff at the end.
// We use it for emitting the accumulated PAL metadata as directives.
// The PAL metadata is reset after it is emitted.
void QISATargetAsmStreamer::finish() {

}

void QISATargetAsmStreamer::emitISAVersion(){
    OS.write_hex(KERNEL_VER);
    Streamer.emitRawComment(" Current eQISA version", true);
}

void QISATargetAsmStreamer::emitHeaderInfo(unsigned base, unsigned size, StringRef type){
    OS << format_hex(base, 10, true)  <<  "\t # " << type << " base" << "\n";
    OS << format_hex(size, 10, true)  <<  "\t # " << type << " size" << "\n";
}

void QISATargetAsmStreamer::emitLargeCommand(StringRef Text){
    OS << "\t ######### " << Text << " #########"  << "\n";
}

void QISATargetAsmStreamer::emitSymbol(StringRef Text, QISASymbolAttr Attribute){
    switch (Attribute)
    {
    case MCSA_CODE_AREA:  OS << ".ca_"; break;
    case MCSA_IDATA_AREA:  OS << ".ida_"; break;
    case MCSA_UDATA_AREA:  OS << ".uda_"; break;
    case MCSA_OTHER:       OS << "."; break;
    default:
        break;
    }
    OS << Text << ":\n";
}


void QISATargetAsmStreamer::emitKernelHeader(vector<KernelHeader> KH) {
    emitLargeCommand("Header");
    for(auto header: KH){
        OS << format_hex(header.sym_addr, 10, true) << "\t # " << header.sym_name << "\n";
    }
    emitLargeCommand("End Header");
}  


void QISATargetAsmStreamer::emitSymbolTable(SymTable ST) {
    emitLargeCommand("Symbol table");
    OS << format_hex(ST.symtbl_entry_cnt, 10, true) << "\n";
    for (auto sym : ST.symtbl_entries){
        OS << "." << sym.sym_name << "\t"; 
        OS << format_hex(sym.sym_taddr, 10, true) << "\t";
        OS << format_hex(sym.sym_caddr, 10, true) << "\n";
    }
     emitLargeCommand("End Symbol table");
}

void QISATargetAsmStreamer::emitDirectiveQmapArea(QMap QM) {
    emitLargeCommand("QMap");
    OS << format_hex(QM.qmap_entry_cnt, 10, true) << "\n";
    for (auto qmap : QM.qmap_entries){
        OS <<  format_hex(qmap.qment_pq_index, 10, true) << "\n";
        OS <<  format_hex(qmap.qment_lq_index, 10, true)<< "\n"; 
    }
    emitLargeCommand("End QMap");
}

void QISATargetAsmStreamer::emitDirectiveDataArea(vector<DataArea> DA) {
    emitLargeCommand("Data area");
    for (auto data_area : DA){
        OS << "." << data_area.sym_name << "\t";
        for (unsigned i = 0; i < data_area.sym_addrs.size(); i++){
            if(i != 0)
                OS << ", ";
             OS << format_hex(data_area.sym_addrs[i], 10, true);
        }
        OS << "\n";
    }
    emitLargeCommand("End Data area");
}