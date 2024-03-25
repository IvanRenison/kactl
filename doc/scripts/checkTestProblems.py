#!/usr/bin/env python3
# encoding: utf-8

import sys

def process_content(content_lines: list[str]) -> list[str]:

    ans: list[str] = []

    in_comment: bool = False # if the line is inside a multiline comment starting with /**
    for line in content_lines:
        if "/**" in line and "*/" not in line:
            in_comment = True
        if in_comment:
            if "*/" in line:
                in_comment = False
            continue

        if "/// exclude-line" in line:
            continue

        if "/// include-line" in line:
            line = line.replace('// ', '', 1)

        if "///" in line:
            line = line.split("///")[0].rstrip() + "\n"

        if line[:9] == "#include ":
            continue

        if len(ans) == 0 and line == "\n":
            continue # skip leading empty lines

        if line != "#pragma once\n":
            ans.append(line)

    return ans


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


    # Find contents

    contents: list[tuple[int, int]] = []

    inside_content: bool = False
    for i, line in enumerate(lines):
        if line[:12] == "/// content/":
            if inside_content:
                print(f"{filePath}:{i+1}: Content inside content", file=sys.stderr)
                return 1
            else:
                contents.append((i, -1))
                inside_content = True
        elif line == "/// END content\n":
            if not inside_content:
                print(f"{filePath}:{i+1}: \"END content\" with out opening", file=sys.stderr)
                return 1
            else:
                contents[-1] = (contents[-1][0], i)
                inside_content = False

    if inside_content:
        print(f"{filePath}:{contents[-1][0]}: Content not closed", file=sys.stderr)
        return 1

    if len(contents) == 0:
        print(f"{filePath}: What content are you testing?", file=sys.stderr)
        return 1

    for start, end in contents:
        fileContent: str = "./" + lines[start][4:-1]
        with open(fileContent, "r") as f:
            content_lines: list[str] = f.readlines()
            content_lines = process_content(content_lines)

        i: int = start + 1
        j: int = 0

        while i < end and j < len(content_lines):
            if "/// START diff" in lines[i]:
                i += 1
                while i < end and "/// END diff" not in lines[i]:
                    if "/// START diff" in lines[i]:
                        ans = 1
                        print(f"{filePath}:{i+1}: Already in diff", file=sys.stderr)
                    i += 1
                i += 1
                if i < end:
                    while j < len(content_lines) and lines[i] != content_lines[j]:
                        j += 1
            else:
                if lines[i] != content_lines[j]:
                    ans = 1
                    print(f"{filePath}:{i+1}: Is not equal", file=sys.stderr)
                i += 1
                j += 1

        if i < end and not j < len(content_lines):
            ans = 1
            print(f"{filePath}:{i+1}: Ends before", file=sys.stderr)

        if not i < end and j < len(content_lines):
            ans = 1
            print(f"{filePath}:{i+1}: Missing lines", file=sys.stderr)

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
