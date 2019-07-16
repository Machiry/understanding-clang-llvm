# LLVM Passes
This folder contains various llvm passes.
## Building
Building all the passes.

    mkdir obj
    cmake ..
    make -j4

All the shared objects for the passes will be present inside the `obj` folder.
## Passes
* [Instrumentation Passes](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/InstrumentationPasses):

    These passes modify the program by adding instrumentation code.
    * [Log memory access](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/InstrumentationPasses/LogMemAccess): This pass logs all the memory reads and writes by inserting call to the function: ``,
    with address and value being written and read.
* [Static Analysis Passes](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/StaticAnalysisPasses):

    These passes perform static analysis on the provided bitcode.
    * [Loop Exiting BB finder](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/StaticAnalysisPasses/GetLoopExitingBBs): This pass identifies all the basic-blocks that control exit to a loop.
    * [Function Identifier Pass](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/StaticAnalysisPasses/FunctionIdentifier): This pass identifies all the functions in the module, also prints all the corresponding function names.
## Using a pass
Using a pass is easy, just load it using `opt` on a bitcode file. 
#### Example:
> Using Function Identifier pass
```
cd obj/StaticAnalysisPasses/FunctionIdentifier
opt -load ./libFunctionIdentifier.so -identfunc <input_bc_file>
```
