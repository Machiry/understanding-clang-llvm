# understanding-clang-llvm
Repository for understanding and writing LLVM passes.

## Building and installing Clang/LLVM
> This needs ~40GB disk space and compilation time of ~90min.

1) Download [LLVM-8.0.0](http://llvm.org/releases/8.0.0/llvm-8.0.0.src.tar.xz), [clang-8.0.0](http://llvm.org/releases/8.0.0/cfe-8.0.0.src.tar.xz)

2) Unzip the LLVM and Clang source files
    ```
    tar xf llvm-8.0.0.src.tar.xz
    tar xf cfe-8.0.0.src.tar.xz
    mv cfe-8.0.0.src llvm-8.0.0.src/tools/clang
    ```

3) Create your target build folder and make
    ```
    mkdir llvm-8.0.0.obj
    cd llvm-8.0.0.obj
    cmake -DCMAKE_BUILD_TYPE=Debug ../llvm-8.0.0.src (or add "-DCMAKE_BUILD_TYPE:STRING=Release" for releae version)
    make -j8  
    ```

4) Add paths for LLVM and Clang
    ```
    export LLVM_SRC=your_path_to_llvm-8.0.0.src
    export LLVM_OBJ=your_path_to_llvm-8.0.0.obj
    export LLVM_DIR=your_path_to_llvm-8.0.0.obj
    export PATH=$LLVM_DIR/bin:$PATH
    ```

## Converting `.c` file to bitcode file
LLVM passes run on bitcode file. Here, we explain how to convert a `C` file to bitcode file.
```
clang -c -emit-llvm <path_to_c_file> -o <path_to_output_bitcode_file>
```
Example:
```
cd examples
clang -c -emit-llvm simple.c -o simple.bc
```
