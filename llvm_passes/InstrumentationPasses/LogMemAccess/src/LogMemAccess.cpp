//
// Created by machiry at the beginning of time.
//
// This pass logs all the memory access.

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
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>


using namespace llvm;

#define LOG_FUNC_NAME "log_mem_access"

namespace UMD {


  /***
   * The main pass.
   */
  struct LogMemAccessPass : public ModulePass {
  public:
    static char ID;
    Function *logFunction;

    LogMemAccessPass() : ModulePass(ID) {
      this->logFunction = nullptr;
    }

    ~LogMemAccessPass() {
    }


    Value *createPointerToVoidPtrCast(IRBuilder<> &targetBuilder, Value *pointerOp) {
      return targetBuilder.CreatePointerCast(pointerOp, IntegerType::getInt8PtrTy(pointerOp->getContext()));
    }

    Value *createValueToUnsignedIntCast(IRBuilder<> &targetBuilder, Value *valueOp) {
      if (valueOp->getType()->isPointerTy()) {
        valueOp = targetBuilder.CreatePtrToInt(valueOp, IntegerType::getInt32Ty(valueOp->getContext()));
      }
      return targetBuilder.CreateIntCast(valueOp, IntegerType::getInt32Ty(valueOp->getContext()), true);
    }


    Function *getLogFunction(Module &m) {
      if (this->logFunction == nullptr) {
        // void*, int, int
        Type *parameterTypes[] = {IntegerType::getInt8PtrTy(m.getContext()),
                                  IntegerType::getInt32Ty(m.getContext()),
                                  IntegerType::getInt32Ty(m.getContext())};
        // void (void*, int, int)
        FunctionType *log_function_type = FunctionType::get(IntegerType::getVoidTy(m.getContext()), parameterTypes,
                                                            false);

        // get the reference to function
        Function *func = cast<Function>(m.getOrInsertFunction(LOG_FUNC_NAME, log_function_type));

        this->logFunction = func;
      }
      return this->logFunction;
    }

    bool instrumentLoad(LoadInst *targetInstr) {
      bool retVal = true;

      try {
        dbgs() << "Instrumenting:" << *targetInstr << "\n";
        // set the insertion point to be after the load instruction.
        auto targetInsertPoint = targetInstr->getIterator();
        targetInsertPoint++;
        IRBuilder<> builder(&(*targetInsertPoint));

        // get the log function
        Function *targetLogFunction = this->getLogFunction(*targetInstr->getModule());

        // get the arguments for the function.
        Value *address = targetInstr->getPointerOperand();
        Value *targetValue = targetInstr;

        address = this->createPointerToVoidPtrCast(builder, address);
        targetValue = this->createValueToUnsignedIntCast(builder, targetValue);

        ConstantInt *readFlag = ConstantInt::get(IntegerType::getInt32Ty(targetInstr->getContext()), 0);
        // prepare arguments.
        Value* arguments[] = {address, targetValue, readFlag};
        builder.CreateCall(targetLogFunction, arguments);
      } catch (const std::exception &e) {
        dbgs() << "[?] Error occurred while trying to instrument load instruction:" << e.what() << "\n";
        retVal = false;
      }
      return retVal;
    }

    bool instrumentStore(StoreInst *targetInstr) {
      bool retVal = true;
      try {
        dbgs() << "Instrumenting:" << *targetInstr << "\n";
        // set the insertion point to be before the store instruction.
        auto targetInsertPoint = targetInstr->getIterator();
        IRBuilder<> builder(&(*targetInsertPoint));

        // get the log function
        Function *targetLogFunction = this->getLogFunction(*targetInstr->getModule());

        // get the arguments for the function.
        Value *address = targetInstr->getPointerOperand();
        Value *targetValue = targetInstr->getValueOperand();

        address = this->createPointerToVoidPtrCast(builder, address);
        targetValue = this->createValueToUnsignedIntCast(builder, targetValue);

        ConstantInt *writeFlag = ConstantInt::get(IntegerType::getInt32Ty(targetInstr->getContext()), 1);

        // prepare arguments.
        Value* arguments[] = {address, targetValue, writeFlag};

        builder.CreateCall(targetLogFunction, arguments);
      } catch (const std::exception &e) {
        dbgs() << "[?] Error occurred while trying to instrument store instruction:" << e.what() << "\n";
        retVal = false;
      }
      return retVal;

    }


    bool runOnModule(Module &m) override {
      bool edited = false;
      // module is a collection of functions.
      for (auto &currFunc: m) {
        if(currFunc.getName() == LOG_FUNC_NAME) {
          continue;
        }
        // function is a collection of basic blocks.
        for (auto &currBB: currFunc) {
          // basic block is a collection of instructions.
          for (auto &currIns: currBB) {
            Instruction *currInstrPtr = &currIns;
            // is this a load instruction?
            if (LoadInst *LD = dyn_cast<LoadInst>(currInstrPtr)) {
              edited = instrumentLoad(LD);

            }
            //is this a store instruction.
            if (StoreInst *SI = dyn_cast<StoreInst>(currInstrPtr)) {
              edited = instrumentStore(SI);
            }
          }
        }
      }
      // true indicates that changes have been made to the module.
      return edited;
    }

  };

  char LogMemAccessPass::ID = 0;
  // pass arg, pass desc, cfg_only, analysis only
  static RegisterPass<LogMemAccessPass> x("logm", "Log all memory accesses.", false, false);
}