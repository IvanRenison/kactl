#!/usr/bin/env bash

set -e

for file in ./test-problems/*/*.cpp
do
    g++ -Wall -Wextra -Wfatal-errors -Wconversion -std=c++17 $file
    python3 ./doc/scripts/checkTestProblems.py $file
done
