#!/bin/bash

cd build
# make clean
make -j20
cd ..
rm test/*.ir*
./build/compiler -debug -emit-hir -emit-mir $1
# ./build/compiler -emit-hir -emit-mir $1
# cd ../test
# ./test.sh
