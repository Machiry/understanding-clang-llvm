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
    * [Log memory access](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/InstrumentationPasses/LogMemAccess): This pass logs all the memory reads and writes by inserting call to the function: `log_mem_access`,
    with address and value being written and read. (Try this on [simple_log.c](https://github.com/Machiry/understanding-clang-llvm/blob/master/examples/simple_log.c))
      
        > Usage:

            cd obj/InstrumentationPasses/LogMemAccess
            opt -load ./libLogMemAccess.so -logm <input_bc_file> -o <path_to_output_bc_file>
            
        **Note:**
        To use this pass, make sure that you define the following function in the input source file:
        ```
                void log_mem_access(void *addr, int value, int flag) {
                    if(flag == 0) {
                        printf("Reading value 0x%x from %p\n", value, addr);
                    }
                    if(flag == 1) {
                        printf("Writing value 0x%x to %p\n", value, addr);
                    }
                }
* [Static Analysis Passes](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/StaticAnalysisPasses):

    These passes perform static analysis on the provided bitcode.
    * [Loop Exiting BB finder](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/StaticAnalysisPasses/GetLoopExitingBBs): This pass identifies all the basic-blocks that control exit to a loop.
        > Usage:
        ```
        cd obj/StaticAnalysisPasses/GetLoopExitingBBs
        opt -load ./libGetLoopExitingBBs.so -loopbbs <input_bc_file>
        ```
    
    * [Function Identifier Pass](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/StaticAnalysisPasses/FunctionIdentifier): This pass identifies all the functions in the module, also prints all the corresponding function names.
        > Usage:
        ```
        cd obj/StaticAnalysisPasses/FunctionIdentifier
        opt -load ./libFunctionIdentifier.so -identfunc <input_bc_file>
        ```
     * [StructAccess Identifier Pass](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/StaticAnalysisPasses/StructAccessIdentifier): This pass identifies all accesses to structure elements in all functions in the module, also prints the corresponding information.
        > Usage:
        ```
        cd obj/StaticAnalysisPasses/StructAccessIdentifier
        opt -load ./libStructAccessIdentifier.so -staccess <input_bc_file>
        ```
