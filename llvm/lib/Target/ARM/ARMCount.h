///===----- ARMCount.h - Count size of ARM Code Segment -------------------===//
//
//                   PicoXOM Randomization Measurement Project
//
// This file was written by at the University of Rochester.
// All Right Reserved.
//
//===----------------------------------------------------------------------===//
//
// This pass counts the number of combinations that could be gained by
// randomizing the code.
//
//===----------------------------------------------------------------------===//
//

#ifndef ARMCOUNT
#define ARMCOUNT

#include "llvm/CodeGen/MachineFunctionPass.h"

namespace llvm {
  struct ARMCount : public MachineFunctionPass {
    // Pass Identifier
    static char ID;

    ARMCount();
    virtual StringRef getPassName() const override;
    virtual bool runOnMachineFunction(MachineFunction & F) override;
  };

  FunctionPass * createARMCount(void);
}

#endif
