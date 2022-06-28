#!/bin/bash
log_file="trivial-autotest-ayame.log"

mkdir -p build/log
rm -rf build/log/$log_file
touch build/log/$log_file

echo -e "\033[32m\033[1m[Compiling ayame...]\033[0m"
javac -encoding UTF-8 $(find . -name "*.java") -cp lib/antlr4-runtime-4.8.jar:lib/argparse4j-0.9.0.jar
echo -e "\033[32m\033[1m[Ayame compiled]\033[0m"