#!/usr/bin/env python3
"""
xus2txt.py
Extracts strings from a .xus file, one per line.
Embedded \r\n within strings are escaped as the literal text \\n.

Usage:
    python xus_extract.py <input.xus> [output.txt]
"""

import sys

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


def escape(s):
    return s.replace("\\", "\\\\").replace("\r\n", "\\n").replace("\r", "\\r").replace("\n", "\\n")


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <input.xus> [output.txt]")
        sys.exit(1)

    input_path  = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else None

    with open(input_path, "rb") as f:
        data = f.read()

    if not data.startswith(b"XUIS"):
        print("WARNING: File does not start with XUIS magic")

    strings = extract_strings(data)
    lines   = [escape(s) for s in strings]
    output  = "\n".join(lines)

    if output_path:
        with open(output_path, "w", encoding="utf-8-sig", newline="\n") as f:
            f.write(output + "\n")
        print(f"Extracted {len(strings)} strings to '{output_path}'")
    else:
        sys.stdout.reconfigure(encoding="utf-8")
        print(output)


if __name__ == "__main__":
    main()
