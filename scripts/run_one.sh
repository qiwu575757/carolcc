#!/bin/bash

cd build
# make clean
cmake ..
make -j20
cd ..
rm test/*.ir*
rm test/*.s
rm test/*.hir
rm test/*.sy.*

./build/compiler -debug -emit-hir -emit-mir  -O2 $1
# ./build/compiler -emit-hir -emit-mir $1
# cd ../test
# ./test.sh
