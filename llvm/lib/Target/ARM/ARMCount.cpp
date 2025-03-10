///===----- ARMCount.cpp - Count size of ARM Code Segment -----------------===//
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

#define DEBUG_TYPE "arm-count"

#include "ARMCount.h"
#include "ARMTargetMachine.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;

STATISTIC(NumFunctions, "Number of Functions");
STATISTIC(NumBasicBlocks, "Number of Basic Blocks");
STATISTIC(CodeSize, "Size of Code Segment in Bytes");
STATISTIC(LeftOver, "Amount of Memory Remaining in Code Segment");

STATISTIC(NumFuncPlaces, "Number of Places to Locate a Function");
STATISTIC(NumBBPlaces, "Number of Places to Locate a Basic Block");
STATISTIC(NumFallThroughs, "Number of Basic Blocks That Fall Through");

// Total code size in bytes
static const unsigned long TotalCodeMemory = 2u * 1024u * 1024u;

char ARMCount::ID = 0;

ARMCount::ARMCount() : MachineFunctionPass(ID) {
  return;
}

StringRef
ARMCount::getPassName() const {
  return "ARM Randomization Measurement Pass";
}

//
// Method: runOnMachineFunction()
//
// Description:
//  This is the entry point for this MachineFunction pass.  This method will
//  add to the total code size the size of the specified MachineFunction.
//
bool
ARMCount::runOnMachineFunction(MachineFunction & F) {
  const TargetInstrInfo * TII = F.getSubtarget().getInstrInfo();
  DebugLoc dl;

  //
  // Increase the number of functions seen in the code.
  //
  if (F.size()) {
    ++NumFunctions;
  }

  //
  // Determine the size of the current machine function.
  //
  unsigned functionSize = 0;
  for (MachineFunction::iterator mbi = F.begin(); mbi != F.end(); ++mbi) {
    //
    // Increase the count of basic blocks.
    //
    ++NumBasicBlocks;

    //
    // Record whether the basic block can fall through to its successor in
    // the control flow graph.
    //
    if (mbi->canFallThrough()) {
      ++NumFallThroughs;
    }

    //
    // Scan through all instructions within this machine basic block and
    // calculate the size of the MachineFunction.
    //
    for (auto mi = mbi->rbegin(); mi != mbi->rend(); ++mi) {
      // Find the size of the current function
      functionSize += TII->getInstSizeInBytes(*mi);
    }
  }

  //
  // Increment the total number of bytes in the code segment by the number of
  // bytes in the current function.
  //
  CodeSize += functionSize;

  //
  // Update the amount of memory left over in the code segment.
  //
  LeftOver = TotalCodeMemory - CodeSize;

  //
  // Update the count of the number of places in which a function can be placed
  // within the code segment.  We assume that instructions (and therefore
  // functions) must be placed on a 16-byte boundary.
  //
  NumFuncPlaces = NumFunctions + (LeftOver / 2u);

  //
  // Update the number of places in which a basic block can be placed if one
  // performed inter-procedural basic block placement.  To account for the
  // fact that a piece of code may no longer fall through to its successor,
  // we decrement the leftover code segment size by the number of additional
  // jumps we would insert (i.e., one 4-byte jump for every fall-through).
  //
  NumBBPlaces = NumBasicBlocks + (LeftOver / 2u) - (NumFallThroughs * 4);
  return false;
}

namespace llvm {
  FunctionPass * createARMCount(void) {
    return new ARMCount();
  }
}
