#!/bin/bash

cd build
make clean
make -j20
cd ..
python ./scripts/runner.py
# cd ../test
# ./test.sh
