#!/usr/bin/env sh

rm -rf build ext
mkdir build
# cmake -H. -Bbuild -DCMAKE_CXX_COMPILER=clang++
cmake -H. -Bbuild -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=iwyu -DCMAKE_CXX_COMPILER=clang++ -GNinja
# cmake -H. -Bbuild -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=iwyu -DCMAKE_CXX_CLANG_TIDY=clang-tidy

# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .

# cmake --build build -- VERBOSE=1
# cmake --build build
