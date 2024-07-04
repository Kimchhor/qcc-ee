/*********************************************************************
 *   Instructions.h:
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

#ifndef QCC_CONSTANTGLOBAL
#define QCC_CONSTANTGLOBAL

#define TAB_STRING "       "
#define SPACE_STRING " "
#define COMMA ","
#define NUMBER_OF_REGISTER 64

/** The Quantum Register ID for merged and splitted instruction */
#define TEMP_QR 9 // Tempory Quantum Register

/** The Quantum Register ID reserved for ancilla qubit */
#define ANCILLA_QR 8 // Ancilla Quantum Register

extern std::string MD_PATH;
extern std::string OUT_DIR;

#endif