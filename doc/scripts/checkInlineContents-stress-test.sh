#!/usr/bin/env bash

set -e

for file in ./stress-tests/*/*.cpp
do
    python3 ./doc/scripts/checkInlineContents.py $file
done
