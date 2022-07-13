#!/bin/bash

cd build
# make clean
make -j20
cd ..
python3 ./scripts/runner.py
# cd ../test
# ./test.sh
