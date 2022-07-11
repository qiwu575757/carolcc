#!/bin/bash

cd build
# make clean
make -j20
cd ..
./build/compiler -debug -emit-hir -emit-mir $1
# cd ../test
# ./test.sh
