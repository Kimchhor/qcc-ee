/*********************************************************************
 *   QCCLayout.cpp:
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

#include "llvm/Transforms/QCC/Layout/QCCLayout.h"

///////////////////////////////////////////////////////////////////////
// QCCLayout Class Declaration
///////////////////////////////////////////////////////////////////////
namespace qcc {

        /**
         * This method will get the physical qubit by virtual qubit
         *
         * @param virtualQubit (int) virtual qubit
         * @return (int) physical qubit
         */
        int QCCLayout::getPhysicalQubitBy(int virtualQubit) {
            return vritual2physicalQubitMap[virtualQubit];
        }
        
        /**
         * This method will get the virtual qubit by physical qubit
         *
         * @param physicalQubit (int) physical qubit
         * @return (int) virtual qubit
         */
        int QCCLayout::getVirtualQubitBy(int physicalQubit) {
            return physical2VirtualQubitMap[physicalQubit];
        }

        /**
         * This method will match the physical to virtual qubit and vice versa.
         *
         * @param physicalQubit (int) physical qubit
         * @param virtualQubit (int) virtual qubit
         */
        void QCCLayout::setBothPhysicalVirtualQubit(int physicalQubit, int virtualQubit){
            physical2VirtualQubitMap[physicalQubit] = virtualQubit;
            vritual2physicalQubitMap[virtualQubit] = physicalQubit;
        }

        /**
         * This method will associates a virtual qubit (int) to each physical qubits 
         * in increasing order. e,g: [4,6,5]. virtual qubit 4, 5 and 6 will be mapped to
         * 0, 1, 2 (index) respectively.
         * 
         * @param physicalQubit (int) physical qubit
         * @param virtualQubit (int) virtual qubit
         */
        void QCCLayout::setLayoutFromList(std::vector<int> listVirtualQubits){
            for (unsigned idx = 0; idx < listVirtualQubits.size(); idx++){
                setBothPhysicalVirtualQubit(idx, listVirtualQubits[idx]);
            }
        }


}