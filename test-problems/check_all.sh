#!/usr/bin/env bash

for file in ./test-problems/*/*.cpp
do
    g++ -Wall -Wextra -Wfatal-errors -Wconversion -std=c++17 $file
    python test-problems/checks.py $file
done
