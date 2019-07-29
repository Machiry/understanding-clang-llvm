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
   * This pass detects all direct accesses to struct members.
   */
  struct StructAccessIdentifierPass : public ModulePass {
  public:
    static char ID;

    StructAccessIdentifierPass() : ModulePass(ID) {
    }

    ~StructAccessIdentifierPass() {
    }

    // get the structure type embedded in the current type.
    Type *getStructureAccessType(Type *currType) {
      if(currType->isPointerTy()) {
        PointerType *currPtrType = dyn_cast<PointerType>(currType);
        return this->getStructureAccessType(currPtrType->getPointerElementType());
      }
      return currType;
    }

    // get the operand that represents the field in the provided
    // GEP instruction.
    // getelementptr inbounds %struct.foo, %struct.foo* %obj, i32 0, i32 0
    Value* getFieldOperand(GetElementPtrInst *gepI) {
      unsigned numOp = gepI->getNumOperands();
      // the last operand is the field number.
      return gepI->getOperand(numOp-1);
    }


    bool checkFunction(Function &currFunc) {
      bool hasAccesses = false;
      std::string funcName = "NONAME";
      if(currFunc.hasName()) {
        funcName = currFunc.getName();
      }
      dbgs() << "[+] Checking struct accesses in function:" << funcName << "\n";
      for(auto &currBB: currFunc) {
        for(auto &currIns: currBB) {
          Instruction *currInstrPtr = &currIns;
          // check if this is a GEP instruction.
          if(GetElementPtrInst *targetAccess = dyn_cast<GetElementPtrInst>(currInstrPtr)) {
            Type *accessedType = targetAccess->getPointerOperandType();
            Type *targetAccType = this->getStructureAccessType(accessedType);
            // check we are accessing a struct?
            if(targetAccType->isStructTy()) {
              hasAccesses = true;
              StructType *stType = dyn_cast<StructType>(targetAccType);
              std::string stName = "NONAME";
              // get the name of the struct, if it has one,
              if(stType->hasName()) {
                stName = stType->getName();
              }
              // get the field number.
              Value *fieldOp = getFieldOperand(targetAccess);
              if(ConstantInt *CNum = dyn_cast<ConstantInt>(fieldOp)) {
                unsigned fieldNum = CNum->getValue().getZExtValue();
                // get the type of the field.
                Type *elementType = stType->getStructElementType(fieldNum);
                dbgs() << "[*] Accessing field:" << fieldNum << " of structure:" << stName << " and its a ";
                // if this is a pointer type?
                if(elementType->isPointerTy()) {
                  dbgs() << "pointer type\n";
                } else {
                  dbgs() << "scalar type\n";
                }
              }
            }

          }
        }
      }
      dbgs() << "[+] Finished Checking struct accesses in function:" << funcName << "\n";
      return hasAccesses;
    }


    bool runOnModule(Module &m) override {
      // iterate through all the functions.
      for (auto &currFunc: m) {
        // if this is not a declaration.
        if(!currFunc.isDeclaration()) {
          checkFunction(currFunc);
        }
      }
      // this is just an analysis pass,
      // we do not change the module.
      return false;
    }

  };

  char StructAccessIdentifierPass::ID = 0;
  // pass arg, pass desc, cfg_only, analysis only
  static RegisterPass<StructAccessIdentifierPass> x("staccess",
                                              "Identify all the direct access to struct members.",
                                                    false,
                                                    true);
}