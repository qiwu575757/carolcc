#!/bin/bash

cd build
cmake ..
# make clean
make -j20
cd ..
python3 ./scripts/runner.py -O2
# cd ../test
# ./test.sh
