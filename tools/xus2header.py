#!/usr/bin/env python3
"""
xus2header.py
Converts a .xus string table to a C++ header file with preprocessor defines.

Each string gets an identifier: IDS_(FIRST_WORD)_(INDEX)
The define value is the string's index, matching the order in the .xus file.

Usage:
    python xus_to_header.py <input.xus> <output.h>
"""

import sys
import re

HEADER_SIZE = 12


def extract_strings(data):
    strings = []
    pos = HEADER_SIZE
    while pos + 2 <= len(data):
        length = (data[pos] << 8) | data[pos + 1]
        pos += 2
        if length == 0:
            break
        byte_len = length * 2
        if pos + byte_len > len(data):
            break
        s = data[pos:pos + byte_len].decode("utf-16-be", errors="replace")
        strings.append(s)
        pos += byte_len
    return strings


def make_identifier(s, index):
    match = re.match(r'[^a-zA-Z0-9]*([a-zA-Z0-9]+)', s.strip())
    first_word = match.group(1).upper() if match else "STR"
    return f"IDS_{first_word}_{index}"


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input.xus> <output.h>")
        sys.exit(1)

    input_path  = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, "rb") as f:
        data = f.read()

    if not data.startswith(b"XUIS"):
        print("WARNING: File does not start with XUIS magic")

    strings = extract_strings(data)

    guard = output_path.replace(".", "_").replace("/", "_").replace("\\", "_").upper()

    with open(output_path, "w", encoding="utf-8-sig", newline="\n") as f:
        f.write(f"#pragma once\n")
        f.write(f"// Auto-generated from {input_path}\n")
        f.write(f"// {len(strings)} strings\n\n")

        for i, s in enumerate(strings):
            ident   = make_identifier(s, i)
            preview = s.replace("\r", "\\r").replace("\n", "\\n")
            if len(preview) > 60:
                preview = preview[:60] + "..."
            f.write(f"// {preview}\n")
            f.write(f"#define {ident} {i}\n\n")

    print(f"Wrote {len(strings)} defines to '{output_path}'")


if __name__ == "__main__":
    main()
