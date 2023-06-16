# Vecrocks

Vecrocks is a vector database based on [RocksDB](https://github.com/facebook/rocksdb)
and [faiss](https://github.com/facebookresearch/faiss)

## Development

### 0. Environment

1. `cmake`
2. C++ Compiler support `C++17`
3. Your toolchain should support `openmp` for building `faiss` library.

   If you are using macOS, you need to install llvm by `brew install llvm`.

   The clang++ toolchain build in Xcode not support openmp.

### 1. Build

```shell
mkdir build
cd build
cmake ..  # wait for download RocksDB
make -j1 # change the number for parallel compile
```

### 2. Run Test

```shell
# in the build dir
ctest
```

### 3. Try example

```shell
# in the build/example dir
./example
```