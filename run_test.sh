#!/bin/bash

cd build 
make -j20
cd ../test
./test.sh
