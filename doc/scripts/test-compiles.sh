#!/bin/bash
FILE="$1"
TMPDIR="$2"
echo "
#include \"$PWD/content/contest/template.cpp\"
#include \"$PWD/$FILE\"
" >"$TMPDIR/temp.cpp"
g++ -Wall -Wextra -Wfatal-errors -Wconversion -std=c++20 "$TMPDIR/temp.cpp" && rm -f "$TMPDIR/temp.cpp" "$TMPDIR/a.out"
