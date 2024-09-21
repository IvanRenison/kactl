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

def checkContents(filePath: str, mandatory: bool = False) -> int:
    with open(filePath, "r") as f:
        lines: list[str] = f.readlines()

    ans: int = 0

    contents: list[tuple[int, int]] = []

    inside_content: bool = False
    for i, line in enumerate(lines):
        if "/// content/" in line:
            if inside_content:
                print(f"{filePath}:{i+1}: Content inside content", file=sys.stderr)
                return 1
            else:
                contents.append((i, -1))
                inside_content = True
        elif line[-16:] == "/// END content\n":
            if not inside_content:
                print(f"{filePath}:{i+1}: \"END content\" with out opening", file=sys.stderr)
                return 1
            else:
                contents[-1] = (contents[-1][0], i)
                inside_content = False

    if inside_content:
        print(f"{filePath}:{contents[-1][0]}: Content not closed", file=sys.stderr)
        return 1

    if len(contents) == 0 and mandatory:
        print(f"{filePath}: What content are you testing?", file=sys.stderr)
        return 1

    for start, end in contents:
        fileContent: str = "./" + lines[start].split()[-1]
        # Count amount of tabs in the line
        indentation_count: int = sum(1 for c in lines[start] if c == '\t')
        indentation: str = "\t" * indentation_count
        with open(fileContent, "r") as f:
            content_lines: list[str] = f.readlines()
            content_lines = process_content(content_lines)
            for i, line in enumerate(content_lines):
                if line != '\n':
                    content_lines[i] = indentation + line

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
                elif i == end:
                    j = len(content_lines)
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
        filePath: str = "./test-problems/numerical/NTT-operations-inv.cpp"#sys.argv[1]
        return checkContents(filePath)
    except (ValueError, IOError) as err:
        print(str(err), file=sys.stderr)
        return 2

if __name__ == "__main__":
    exit(main())
