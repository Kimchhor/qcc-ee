/*
 * File: eqisa.h
 *       version: 0.91
 */

#ifndef _EQISA_H
#define _EQISA_H

#include <stdint.h>

/*
 * Kernel format
 * 0x0000        +------------------------------------+ <------+
 *               |  eQISA spec ver (4bytes)           |        |
 * 0x0004        |------------------------------------|        |
 *               |  QMap base: 0x0050 (4bytes)        |        |
 * 0x0008        |------------------------------------|        |
 *               |  QMap size (4bytes)                |        |
 * 0x000C        |------------------------------------|        |
 *               |  Symbol table base: STB (4bytes)   |        |
 * 0x0010        |------------------------------------|        |
 *               |  Symbol table size (4bytes)        |        |
 * 0x0014        |------------------------------------|        |
 *               |  IData area base: IDB (4bytes)     |        |
 * 0x0018        |------------------------------------|        |
 *               |  IData area size (4bytes)          |    Header
 * 0x001C        |------------------------------------|        |
 *               |  UData area base: UDB (4bytes)     |        |
 * 0x0020        |------------------------------------|        |
 *               |  UData area size (4bytes)          |        |
 * 0x0024        |------------------------------------|        |
 *               |  Code area base: CB (4bytes)       |        |
 * 0x0028        |------------------------------------|        |
 *               |  Code area size (4bytes)           |        |
 * 0x002C        |------------------------------------|        |
 *   :           |                                    |        |
 *   :           |             Unused                 |        |
 *   :           |                                    |        |
 * 0x0050        +------------------------------------+ <------+
 *               |  QMap_entry_count (4bytes)         |        |
 * 0x0050+0x04   |------------------------------------|        |
 *               |  QMap_pq_index#1 (4bytes)          |        |
 * 0x0050+0x08   |------------------------------------|        |
 *               |  QMap_lq_index#1 (4bytes)          |        |
 * 0x0050+0x0C   |------------------------------------|      QMap
 *   :           |              :                     |        |
 *   :           |              :                     |        |
 * 0x0050+n*0x04 |------------------------------------|        |
 *               |  QMap_pq_index#n (4bytes)          |        |
 * 0x0050+n*0x08 |------------------------------------|        |
 *               |  QMap_lq_index#n (4bytes)          |        |
 * STB           +------------------------------------+ <------+
 *               |  STB_entry_count (4bytes)          |        |
 * STB+0x04      |------------------------------------|        |
 *               |  Symbol name (18bytes)             |        |
 * STB+0x1C      |------------------------------------|        |
 *               |  Symbol taddress (2bytes)          |       ST
 * STB+0x1E      |------------------------------------|    (Entry is increased by 24bytes)
 *               |  Symbol caddress (4bytes)          |        |
 * STB+0x22      |------------------------------------|        |
 *   :           |              :                     |        |
 *   :           |              :                     |        |
 * IDB           +------------------------------------+ <------+
 *               |  KRF_entry_count (4bytes)          |        |
 * IDB+0x04      |------------------------------------|        |
 *               |  iData-1 (4bytes)                  |        |
 * IDB+0x08      |------------------------------------|      IDA (increased by 4bytes)
 *               |  iData-2 (4bytes)                  |        |
 * IDB+0x1C      |------------------------------------|        |
 *   :           |              :                     |        |
 *   :           |              :                     |        |
 * UDB           +------------------------------------+ <--+---+
 *               |  KRF[0] (4bytes)                   |    |   |
 * UDB+0x04      |------------------------------------|    |   |
 *   :           |              :                     |   KRF  |
 * UDB+0x04*(n-1)|------------------------------------|    |   |
 *               |  KRF[n-1] (4bytes)                 |    |   |
 * UDB+0x04*n    |------------------------------------| <--+ UDA (increased by 4bytes)
 *               |  uData-1 (4bytes)                  |        |
 * UDB+0x04*(n+1)|------------------------------------|        |
 *               |  uData-2 (4bytes)                  |        |
 * UDB+0x04*(n+2)|------------------------------------|        |
 *   :           |              :                     |        |
 *   :           |              :                     |        |
 * CB            +------------------------------------+ <------+
 *               |  Opcode block-1 (8bytes)           |        |
 * CB+0x08       |------------------------------------|        |
 *  :            |  Opcode block-2                    |        |
 * CB+0x10       |------------------------------------|      CA (increase by 8bytes)
 *   :           |              :                     |        |
 *   :           |              :                     |        |
 * CB+...        |------------------------------------|        |
 *               |  Opcode block-n                    |        |
 *               +------------------------------------+ <------+
 */


/*
 * Current version of Kernel file format
 *   : Main version(8bits) + Subversion(8bits) + Revision(16bits)
 * Current version : v0.9.1
 */
#define KERNEL_VER     ((0x00 <<24) | (0x09 <<16) | (0x0001))

#define KERNEL_MVER    ((KERNEL_VER >> 24) & 0x00FF)
#define KERNEL_SVER    ((KERNEL_VER >> 16) & 0x00FF)
#define KERNEL_RVER    (KERNEL_VER & 0xFFFF)

/* Kernel header */
#define HDR_KVER       0x0000
#define HDR_QM_BASE    0x0004
#define HDR_QM_SIZE    0x0008
#define HDR_ST_BASE    0x000C
#define HDR_ST_SIZE    0x0010
#define HDR_IDA_BASE   0x0014
#define HDR_IDA_SIZE   0x0018
#define HDR_UDA_BASE   0x001C
#define HDR_UDA_SIZE   0x0020
#define HDR_CA_BASE    0x0024
#define HDR_CA_SIZE    0x0028


/*
 * Opcode Block Encoding to represent an Instruction
 *
 * |------------|------------|------------|------------|
 * |   Opcode   |  Operand1  |  Operand2  |  Operand3  |
 * |  (16bits)  |  (16bits)  |  (16bits)  |  (16bits)  |
 * |------------|------------|------------|------------|
 *
 * 1. Opcode encoding structure
 *
 *    |---------------|----------------------------------|
 *    |  Opcode_type  |  Opcode_cmd  |  Opcode_operand#  |
 *    |    (2bits)    |   (12bits)   |      (2bits)      |
 *    |---------------|----------------------------------|
 *
 *     o Opcode_type
 *       00 : Quantum operation basic instructions
 *       01 : Quantum operation directive instructions
 *       10 : Quantum execution control instructions
 *       11 : Classical execution control instructions
 *
 *     o Opcode_cmd
 *
 *     o Opcode_operand#
 *       00 : NA
 *       01 : Operand1 + NA + NA
 *       10 : Operand1 + Operand2 + NA
 *       11 : Operand1 + Operand2 + Operand3
 *
 * 2. Operand encoding structure
 *
 *    |----------------|-----------------|
 *    |  Operand_type  |  Operand_value  |
 *    |    (3bits)     |     (13bits)    |
 *    |----------------|-----------------|
 *
 *    o Operand_type
 *       000 : Unused
 *       001 : Immediate value type
 *       010 : CGPR(Classical General Purpose Register) type
 *       011 : QPR(Quantum Parameter Register) type
 *       100 : Code Address type
 *       101 : IData(Initialized Data) Address type
 *       110 : UData(Uninitialized Data) Address type
 *
 */

/* Opcode Block Management */
#define OPCODE_BLOCK_SIZE      8        /* Opcode block size: 8bytes */

/* Opcode Management */
#define OPCODE_SHIFT           48       /* Opcode position */
#define OPCODE_TYPE_MASK       0xC000   /* Opcode_type Mask */
#define OPCODE_OPNDNUM_MASK    0x0003   /* Opcode_operand# Mask */

#define OPCODE_TYPE_QO         0x0000   /* Quantum operation basic instructions     */
#define OPCODE_TYPE_QD         0x4000   /* Quantum operation directive instructions */
#define OPCODE_TYPE_QC         0x8000   /* Quantum execution control instructions   */
#define OPCODE_TYPE_CC         0xC000   /* Classical execution control instructions */

/* Operand Management */
#define OPERAND_SHIFT1         32       /* Operand1 position */
#define OPERAND_SHIFT2         16       /* Operand2 position */
#define OPERAND_VAL_MASK       0x1FFF   /* Operand_value Mask */
#define OPERAND_TYPE_MASK      0xE000   /* Operand_type Mask */

#define OPERAND_TYPE_NA        0x0000   /* Unused type */
#define OPERAND_TYPE_IMM       0x2000   /* Immediate type */
#define OPERAND_TYPE_CGPR      0x4000   /* CGPR type */
#define OPERAND_TYPE_QPR       0x6000   /* QPR type   */
#define OPERAND_TYPE_CADDR     0x8000   /* Code Address type */
#define OPERAND_TYPE_IDADDR    0xA000   /* IData Address type */
#define OPERAND_TYPE_UDADDR    0xC000   /* UData Address type */

/* Opcode of Quantum operation basic instructions */
#define OP0_INIT_X          (OPCODE_TYPE_QO | (0x0001 << 2) | 0x0001)  /* 0x0005 */
#define OP0_INIT_Z          (OPCODE_TYPE_QO | (0x0002 << 2) | 0x0001)  /* 0x0009 */
#define OP0_X               (OPCODE_TYPE_QO | (0x0003 << 2) | 0x0001)  /* 0x000D */
#define OP0_Z               (OPCODE_TYPE_QO | (0x0004 << 2) | 0x0001)  /* 0x0011 */
#define OP0_MEASURE_X       (OPCODE_TYPE_QO | (0x0005 << 2) | 0x0001)  /* 0x0015 */
#define OP0_MEASURE_Z       (OPCODE_TYPE_QO | (0x0006 << 2) | 0x0001)  /* 0x0019 */
#define OP0_MERGE_MXX       (OPCODE_TYPE_QO | (0x0007 << 2) | 0x0002)  /* 0x001E */
#define OP0_MERGE_MZZ       (OPCODE_TYPE_QO | (0x0008 << 2) | 0x0002)  /* 0x0022 */
#define OP0_SPLIT_MXX       (OPCODE_TYPE_QO | (0x0009 << 2) | 0x0002)  /* 0x0026 */
#define OP0_SPLIT_MZZ       (OPCODE_TYPE_QO | (0x000A << 2) | 0x0002)  /* 0x002A */
#define OP0_CNOT_POST_MXX   (OPCODE_TYPE_QO | (0x000B << 2) | 0x0003)  /* 0x002F */
#define OP0_CNOT_POST_MZZ   (OPCODE_TYPE_QO | (0x000C << 2) | 0x0003)  /* 0x0033 */
#define OP0_MOVE_POST_MXX   (OPCODE_TYPE_QO | (0x000D << 2) | 0x0002)  /* 0x0036 */
#define OP0_MOVE_POST_MZZ   (OPCODE_TYPE_QO | (0x000E << 2) | 0x0002)  /* 0x003A */
#define OP0_HADAMARD        (OPCODE_TYPE_QO | (0x000F << 2) | 0x0001)  /* 0x003D */
#define OP0_DEFORM          (OPCODE_TYPE_QO | (0x0010 << 2) | 0x0001)  /* 0x0041 */
#define OP0_FLIP_EXPAND     (OPCODE_TYPE_QO | (0x0011 << 2) | 0x0002)  /* 0x0046 */
#define OP0_FLIP_CONTRACT   (OPCODE_TYPE_QO | (0x0012 << 2) | 0x0002)  /* 0x004A */
#define OP0_FLIP_SHIFT      (OPCODE_TYPE_QO | (0x0013 << 2) | 0x0002)  /* 0x004E */
#define OP0_INIT_SA         (OPCODE_TYPE_QO | (0x0014 << 2) | 0x0001)  /* 0x0051 */
#define OP0_INIT_TA         (OPCODE_TYPE_QO | (0x0015 << 2) | 0x0001)  /* 0x0055 */
#define OP0_S_POST          (OPCODE_TYPE_QO | (0x0016 << 2) | 0x0002)  /* 0x005A */
#define OP0_T_POST          (OPCODE_TYPE_QO | (0x0017 << 2) | 0x0002)  /* 0x005E */
#define OP0_T_DAG_POST      (OPCODE_TYPE_QO | (0x0018 << 2) | 0x0002)  /* 0x0062 */

/* Opcode of Quantum operation directive instructions */
#define OP1_PARALLEL_BEGIN  (OPCODE_TYPE_QD | (0x0001 << 2) | 0x0000)  /* 0x4004 */
#define OP1_PARALLEL_END    (OPCODE_TYPE_QD | (0x0002 << 2) | 0x0000)  /* 0x4008 */

/* Opcode of Quantum execution control instructions */
#define OP2_QMOV            (OPCODE_TYPE_QC | (0x0001 << 2) | 0x0002)  /* 0x8006 */

/* Opcode of Classical execution control instructions */
#define OP3_BREQ            (OPCODE_TYPE_CC | (0x0001 << 2) | 0x0003)  /* 0xC007 */
#define OP3_BRNE            (OPCODE_TYPE_CC | (0x0002 << 2) | 0x0003)  /* 0xC00B */
#define OP3_BRGT            (OPCODE_TYPE_CC | (0x0003 << 2) | 0x0003)  /* 0xC00F */
#define OP3_BRGE            (OPCODE_TYPE_CC | (0x0004 << 2) | 0x0002)  /* 0xC012 : deprecated */
#define OP3_BRLT            (OPCODE_TYPE_CC | (0x0005 << 2) | 0x0003)  /* 0xC017 */
#define OP3_BRLE            (OPCODE_TYPE_CC | (0x0006 << 2) | 0x0002)  /* 0xC01A : deprecated */
#define OP3_BRI             (OPCODE_TYPE_CC | (0x0007 << 2) | 0x0003)  /* 0xC01F */
#define OP3_INC             (OPCODE_TYPE_CC | (0x0008 << 2) | 0x0002)  /* 0xC022 */
#define OP3_DEC             (OPCODE_TYPE_CC | (0x0009 << 2) | 0x0002)  /* 0xC026 */
#define OP3_CMP             (OPCODE_TYPE_CC | (0x000A << 2) | 0x0002)  /* 0xC02A */
#define OP3_LDD             (OPCODE_TYPE_CC | (0x000B << 2) | 0x0002)  /* 0xC02E */
#define OP3_LDI             (OPCODE_TYPE_CC | (0x000C << 2) | 0x0003)  /* 0xC033 */
#define OP3_STD             (OPCODE_TYPE_CC | (0x000D << 2) | 0x0002)  /* 0xC036 */
#define OP3_STI             (OPCODE_TYPE_CC | (0x000E << 2) | 0x0003)  /* 0xC03B */
#define OP3_MOV             (OPCODE_TYPE_CC | (0x000F << 2) | 0x0002)  /* 0xC03E */
#define OP3_FMR             (OPCODE_TYPE_CC | (0x0010 << 2) | 0x0002)  /* 0xC042 */

#endif /* _EQISA_H */
