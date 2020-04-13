//===- ARMPicoXOM.h - ARM XOM Enabler -------------------------------------===//
//
// This file was written by at the University of Rochester.
// All Rights Reserved.
//
//===----------------------------------------------------------------------===//
//
// This file contains the definition of a pass that forces functions to be
// generated as execute-only code.
//
//===----------------------------------------------------------------------===//


#ifndef LLVM_TRANSFORMS_SCALAR_ARMPICOXOM_H
#define LLVM_TRANSFORMS_SCALAR_ARMPICOXOM_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

namespace llvm {

class ARMPicoXOMPass : public ModulePass {
public:
  // Pass identifier variable
  static char ID;

  ARMPicoXOMPass();

  virtual StringRef getPassName() const override;

  virtual bool runOnModule(Module & M) override;
};

ModulePass * createARMPicoXOMPass(void);
}

#endif
