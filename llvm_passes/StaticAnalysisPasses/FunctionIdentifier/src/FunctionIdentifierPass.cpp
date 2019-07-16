//
// Created by machiry at the beginning of time.
//

#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <iostream>
#include <llvm/Support/Debug.h>
#include <llvm/IR/Module.h>


using namespace llvm;

namespace UMD {


  /***
   * This pass lists all the function in the provided module.
   */
  struct FunctionIdentifierPass : public ModulePass {
  public:
    static char ID;

    FunctionIdentifierPass() : ModulePass(ID) {
    }

    ~FunctionIdentifierPass() {
    }


    bool runOnModule(Module &m) override {
      // iterate through all the functions.
      for (auto &currFunc: m) {
        if(currFunc.hasName()) {
          dbgs() << "[+] Function Name:" << currFunc.getName() << ", Has Body:" << !currFunc.isDeclaration() << "\n";
        }
      }
      // this is just an analysis pass,
      // we do not change the module.
      return false;
    }

  };

  char FunctionIdentifierPass::ID = 0;
  // pass arg, pass desc, cfg_only, analysis only
  static RegisterPass<FunctionIdentifierPass> x("identfunc", "Identify all the functions in the module.", false, true);
}