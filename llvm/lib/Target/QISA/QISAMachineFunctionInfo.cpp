//===-- QISAMachineFunctionInfo.cpp - QISA private data ----*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//


#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"

#include "QISAMachineFunctionInfo.h"

using namespace llvm;

void QISAMachineFunctionInfo::getDataSegmentRegsiterSize(MachineFunction &MF) {

    MachineFrameInfo &MFI = MF.getFrameInfo();

    // number of registers
    unsigned numReg = MFI.getNumObjects();

    for (unsigned i = 0; i < numReg; i++){

        const llvm::AllocaInst* inst = MFI.getObjectAllocation(i);

        if (inst){
            if (inst->getAllocatedType()->isQubitTy()){
                this->IDSRegisterSize++;
                this->dataRegisterType.push_back(IDS);
            // if the register is array type
            }else if (inst->getAllocatedType()->isArrayTy()){

                ArrayType *arrTy = dyn_cast<ArrayType>(inst->getAllocatedType());

                if (arrTy->getElementType()->isIntegerTy()){
                    this->UDSRegisterSize += arrTy->getNumElements();
                    this->dataRegisterType.push_back(UDS);

                }
                else if (arrTy->getElementType()->isQubitTy()){
                    this->IDSRegisterSize += arrTy->getNumElements();                    
                    this->dataRegisterType.push_back(IDS);
                }
                
            } else{
                this->UDSRegisterSize++;
                this->dataRegisterType.push_back(UDS);
            }
        }
    }

}
