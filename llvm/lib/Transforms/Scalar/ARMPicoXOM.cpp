//===- ARMPicoXOM.cpp - ARM XOM Enabler -----------------------------------===//
//
// This file was written by at the University of Rochester.
// All Rights Reserved.
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of a pass that forces functions to be
// generated as execute-only code.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Attributes.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar/ARMPicoXOM.h"

#include <string>

using namespace llvm;

static cl::opt<bool>
EnableXO("arm-picoxom-xo", cl::Hidden,
         cl::desc("Enable ARM PicoXOM XO transformation"),
         cl::init(true));

static cl::opt<bool>
EnableAlign("arm-picoxom-align", cl::Hidden,
            cl::desc("Enable ARM PicoXOM alignment"),
            cl::init(true));

char ARMPicoXOMPass::ID = 0;

ARMPicoXOMPass::ARMPicoXOMPass()
    : ModulePass(ID) {
}

StringRef
ARMPicoXOMPass::getPassName() const {
  return "ARM XOM Enabler Pass";
}

bool
ARMPicoXOMPass::runOnModule(Module & M) {
  if (skipModule(M)) {
    return false;
  }

  // Only for ARM targets
  StringRef TT = M.getTargetTriple();
  if (!TT.startswith("arm") && !TT.startswith("thumb")) {
    return false;
  }

  const std::string TargetFeatures = "target-features";
  const std::string FSExecOnly = "+execute-only";

  for (Function & F : M) {
    Attribute FSAttr = F.getFnAttribute(TargetFeatures);
    std::string FS = FSAttr.getValueAsString();

    // Move the function to section .xom.
    if (EnableAlign) {
      F.setSection(".xom");
    }

    if (FS.find(FSExecOnly) != std::string::npos) {
      // Nothing to be done
      continue;
    }

    // Add the execute-only feature
    if (EnableXO) {
      if (FS.empty()) {
        FS += FSExecOnly;
      } else {
        FS += "," + FSExecOnly;
      }
      F.addFnAttr(TargetFeatures, FS);
    }
  }

  return true;
}

ModulePass * llvm::createARMPicoXOMPass(void) {
  return new ARMPicoXOMPass();
}
