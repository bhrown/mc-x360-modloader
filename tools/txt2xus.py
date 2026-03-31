#!/usr/bin/env python3
"""
txt2xus.py
Converts a plain text file (one string per line, UTF-8) to a .xus file.

Usage:
    python txt_to_xus.py <input.txt> <output.xus> [--original original.xus]
"""

import sys
import struct

HEADER_SIZE = 12
DEFAULT_HEADER = b"XUIS\x01\x02\x00\x01\xcc\xcc\x00\x00"


def unescape(s):
    # inverse of escape() in xus2txt.py
    result = []
    i = 0
    while i < len(s):
        if s[i] == "\\" and i + 1 < len(s):
            c = s[i + 1]
            if c == "n":
                result.append("\r\n")
                i += 2
            elif c == "r":
                result.append("\r")
                i += 2
            elif c == "\\":
                result.append("\\")
                i += 2
            else:
                result.append(s[i])
                i += 1
        else:
            result.append(s[i])
            i += 1
    return "".join(result)


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input.txt> <output.xus> [--original original.xus]")
        sys.exit(1)

    input_path  = sys.argv[1]
    output_path = sys.argv[2]

    original_header = None
    if "--original" in sys.argv:
        idx = sys.argv.index("--original")
        if idx + 1 < len(sys.argv):
            with open(sys.argv[idx + 1], "rb") as f:
                original_header = bytearray(f.read()[:HEADER_SIZE])

    with open(input_path, "r", encoding="utf-8-sig") as f:
        lines = f.read().splitlines()

    strings = [unescape(line) for line in lines if line != ""]

    header = bytearray(original_header if original_header else DEFAULT_HEADER)

    body = bytearray()
    for s in strings:
        char_count = len(s)
        body += struct.pack(">H", char_count)
        body += s.encode("utf-16-be")

    struct.pack_into(">H", header, 0x0A, len(strings) & 0xFFFF)

    with open(output_path, "wb") as f:
        f.write(bytes(header) + bytes(body))

    print(f"Wrote {len(strings)} strings to '{output_path}'")


if __name__ == "__main__":
    main()
