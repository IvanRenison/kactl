#!/usr/bin/env python3
# encoding: utf-8

from os import path
import os
import sys
import getopt

def calcLineLength(line: str) -> int:
    """
        Returns the length of the line without the newline character
        taking into acount preprocessing
    """
    if "/// exclude-line" in line:
        return 0
    if "/// include-line" in line:
        line = line.replace('// ', '', 1)

    if "///" in line:
        line = line.split("///")[0].rstrip() + "\n"

    tabs: int = line.count("\t")
    return len(line) + tabs - 1

def checkFile(file: str, tabs: bool, lineLength: int) -> int:
    ans = 0
    with open(file, "r") as f:
        lines = f.readlines()
    end: bool = True # if the last statement is closed
    in_comment: bool = False # if the line is inside a multiline comment starting with /**
    for i, line in enumerate(lines):
        if "/*" in line and "*/" not in line:
            in_comment = True
        if in_comment:
            if "*/" in line:
                in_comment = False
            continue
        if line in ["", "\n"]:
            continue
        if tabs:
            j: int = 0
            while j < len(line) and line[j] == "\t":
                j += 1
            if end and j + 1 < len(line) and line[j] == " " and line[j + 1] == " ":
                print(f"{file}: {i + 1} has spaces as indentation", file=sys.stderr)
                ans = 1
            endChar: str = line[-1] if line[-1] != "\n" else line[-2]
            end = endChar in [";", "{", "}"]
        if calcLineLength(line) > lineLength:
            print(f"{file}: {i + 1} line is to long", file=sys.stderr)
            ans = 1
    return ans

def checkDirectory(directory: str, tabs: bool, lineLength: int) -> int:
    ans: int = 0
    for file in os.listdir(directory):
        filePath: str = f"{directory}{os.sep}{file}"
        if path.isdir(filePath):
            if checkDirectory(filePath, tabs, lineLength) != 0:
                ans = 1
        elif file.endswith(".cpp") or file.endswith(".h"):
            if checkFile(filePath, tabs, lineLength) != 0:
                ans = 1
    return ans

def main() -> int:
    try:
        opts, _ = getopt.getopt(sys.argv[1:], "h:t:l:p", ["help", "tabs", "lineLength=", "path="])
        tabs: bool = False
        lineLength: int = 10**10
        filePath: str = "."
        for opt, arg in opts:
            if opt in ("-h", "--help"):
                print("Usage: styleCheck.py [-t] [-l <length>] [-d <path>]")
                print("\t-t, --tabs\t\tcheck for tabs")
                print("\t-l, --lineLength\tcheck for line length")
                print("\t-d, --path\t\tfile or directory to check")
                return 0
            elif opt in ("-t", "--tabs"):
                tabs = True
            elif opt in ("-l", "--lineLength"):
                lineLength = int(arg)
            elif opt in ("-p", "--path"):
                filePath = arg
        if not path.exists(filePath):
            print(f"{filePath} does not exist", file=sys.stderr)
            return 1
        if path.isfile(filePath):
            return checkFile(filePath, tabs, lineLength)
        else:
            return checkDirectory(filePath, tabs, lineLength)

    except (ValueError, getopt.GetoptError, IOError) as err:
        print(str(err), file=sys.stderr)
        print("\t for help use --help", file=sys.stderr)
        return 2


if __name__ == "__main__":
    exit(main())
""