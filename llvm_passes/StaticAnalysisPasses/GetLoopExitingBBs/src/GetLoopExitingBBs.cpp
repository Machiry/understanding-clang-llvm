//
// Created by machiry at the beginning of time.
//
// This pass gets the information about basic-blocks that control
// the exit from a loop
//
#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <iostream>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Support/Debug.h>
#include <llvm/Analysis/CFGPrinter.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/IR/DebugInfoMetadata.h>


using namespace llvm;

namespace UMD {


  /***
   * The main pass.
   */
  struct GetLoopExitingBBs : public ModulePass {
  public:
    static char ID;

    GetLoopExitingBBs() : ModulePass(ID) {
    }

    ~GetLoopExitingBBs() {
    }


    bool runOnModule(Module &m) override {
      // iterate through all the functions.
      for (auto &currFunc: m) {
        // does this function have body?
        if(!currFunc.isDeclaration()) {
          // get information about loops.
          auto &loopAnalysis = getAnalysis<LoopInfoWrapperPass>(currFunc);
          LoopInfo &loopInfo = loopAnalysis.getLoopInfo();
          // iterate over all the loops
          for (auto *lobj: loopInfo.getLoopsInPreorder()) {
            SmallVector<BasicBlock *, 32> exitBBs;
            exitBBs.clear();
            // get the exit basic blocks.
            lobj->getExitingBlocks(exitBBs);

            // get the loop exit condition.
            for (auto *bb: exitBBs) {
              Instruction *exitInstr = bb->getTerminator();
              const DebugLoc &DL = exitInstr->getDebugLoc();
              DILocation *di = DL.get();

              dbgs() << *exitInstr << " at " << DL.getLine() << " " << di->getFilename() << "\n";
            }

          }
        }
      }

      for (auto &currFunc: m) {
        // get the function type.
        FunctionType *currFuncType = currFunc.getFunctionType();
        // check the type of each parameter.
        for(unsigned i=0; i< currFuncType->getNumParams(); i++) {
          if(currFuncType->getParamType(i)->isPointerTy()) {
            dbgs() << "Function:" << currFunc.getName() << " has a pointer parameter.\n";
            continue;
          }
        }
      }
      // this is just an analysis pass,
      // we do not change the module.
      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<LoopInfoWrapperPass>();
    }

  };

  char GetLoopExitingBBs::ID = 0;
  // pass arg, pass desc, cfg_only, analysis only
  static RegisterPass<GetLoopExitingBBs> x("loopbbs", "Static analysis pass to get the instructions "
                                                      "that control the exit from a loop.", false, true);
}