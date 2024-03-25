#!/usr/bin/env bash

for file in ./test-problems/*/*.cpp
do
    g++ -Wall -Wextra -Wfatal-errors -Wconversion -std=c++17 $file || exit 1
    python ./doc/scripts/checkTestProblems.py $file || exit 1
done
