//===- QASMDirectives.h - Enums for directives on various targets -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines various enums that represent target-specific directives.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_QASM_QASMDIRECTIVES_H
#define LLVM_QASM_QASMDIRECTIVES_H
#include "QASMDefine.h"
#include "llvm/ADT/SmallString.h"

namespace llvm {

enum QASMSymbolAttr{
    MCSA_IDATA_AREA,
    MCSA_UDATA_AREA,
    MCSA_CODE_AREA,
    MCSA_OTHER,
};

/* Symbol Table Management */
typedef struct kernel_header {
	StringRef sym_name;              /* Symbol name */
	uint32_t sym_addr;          /* Symbol Address */
} KernelHeader;

typedef struct sym_entry {
	StringRef sym_name;         /* Symbol name */
	uint16_t sym_taddr;         /* Operand_type+Data symbol address */
	uint32_t sym_caddr;         /* Code symbol address */
} SymEntry;

typedef struct sym_table {
	uint32_t  symtbl_entry_cnt; /* Symbol entry count */
	std::vector<SymEntry> symtbl_entries;    /* Symbol Table entry */
} SymTable;

/* QMap Management */
typedef struct qmap_entry {
	uint32_t qment_pq_index;   /* Program Qubit Index */
	uint32_t qment_lq_index;   /* Logical Qubit Index */
} QmapEntry;

typedef struct qmap {
	uint32_t   qmap_entry_cnt; /* QMap entry count */
	std::vector<QmapEntry> qmap_entries;    /* QMap entry */
} QMap;

typedef struct data_area {
	StringRef sym_name; 	   /* Symbol name */
	std::vector<uint32_t> sym_addrs;       /* Symbol Address */
} DataArea;

} // end namespace llvm

#endif
