#!/bin/bash

cd build
make clean
make -j20
cd ../test
./test.sh
