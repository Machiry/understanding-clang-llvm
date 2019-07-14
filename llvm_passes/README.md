# LLVM Passes
This folder contains various llvm passes.
## Building
Building all the passes.

    mkdir obj
    cmake ..
    make -j4

All the shared objects for the passes will be present inside the `obj` folder.
## Passes
* [Function Identifier Pass](https://github.com/Machiry/understanding-clang-llvm/tree/master/llvm_passes/FunctionIdentifier): This pass identifies all the functions in the module, also prints all the corresponding function names.
## Using a pass
Using a pass is easy, just load it using `opt` on a bitcode file. 
#### Example:
> Using Function Identifier pass
```
cd obj/FunctionIdentifier
opt -load ./libFunctionIdentifier.so -identfunc <input_bc_file>
```
