#!/usr/bin/env python3
# encoding: utf-8

import sys
from checkInlineContents import checkContents

def checkFile(filePath: str) -> int:
    with open(filePath, "r") as f:
        lines: list[str] = f.readlines()

    if len(lines) == 0:
        print(f"{filePath}: empty file", file=sys.stderr)
        return 1

    ans: int = 0

    if lines[0][:20] != "// Problem: https://":
        ans = 1
        print(f"{filePath}:1: missing problem link", file=sys.stderr)

    if len(lines) < 2 or lines[1][:11] != "// Status: ":
        ans = 1
        print(f"{filePath}:2: missing status", file=sys.stderr)


    if checkContents(filePath, True) != 0:
        ans = 1

    return ans

def main() -> int:
    try:
        filePath: str = sys.argv[1]
        return checkFile(filePath)
    except (ValueError, IOError) as err:
        print(str(err), file=sys.stderr)
        print("\t for help read ./test-problems/README.md", file=sys.stderr)
        return 2

if __name__ == "__main__":
    exit(main())
