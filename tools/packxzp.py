#!/usr/bin/env python3
"""
patch_xex_media.py
Replaces a named resource section in an unencrypted, uncompressed XEX2 file.
Uses xex1tool for VA->file offset mapping and resource info parsing.

Usage:
    python patch_xex_media.py <input.xex> <new_resource> <output.xex> [--name media] [--xex1tool path]
"""

import sys
import re
import struct
import argparse
import subprocess


def run_xex1tool(xex1tool, args):
    result = subprocess.run(
        [xex1tool] + args,
        capture_output=True, text=True
    )
    return result.stdout + result.stderr


def get_resource_info(xex1tool, xex_path, name):
    """Parse xex1tool -l output to find VA and size for a named resource."""
    output = run_xex1tool(xex1tool, ["-l", xex_path])
    # look for lines like:
    #     0) 82A30000 - 8459AFEE : media
    pattern = re.compile(
        r"(\d+\))\s+([0-9A-Fa-f]+)\s+-\s+([0-9A-Fa-f]+)\s*:\s*(\S+)"
    )
    for match in pattern.finditer(output):
        res_name = match.group(4)
        if res_name == name:
            va_start = int(match.group(2), 16)
            va_end   = int(match.group(3), 16)
            size     = va_end - va_start + 1
            return va_start, size

    in_resources = False
    for line in output.splitlines():
        if "Resources" in line:
            in_resources = True
        if in_resources:
            print(f"  {line}")
        if in_resources and line.strip() == "":
            break

    return None, None


def get_file_offset(xex1tool, xex_path, va):
    """Use xex1tool --address to convert VA to file offset."""
    output = run_xex1tool(xex1tool, ["--address", hex(va), xex_path])
    # output looks like:
    #   File Offset:  0x9AE000
    match = re.search(r"File Offset:\s*(0x[0-9A-Fa-f]+)", output)
    if match:
        return int(match.group(1), 16)
    print("xex1tool --address output:")
    print(output)
    return None


def find_size_field_offset(data, va, old_size):
    """
    Find the file offset of the VirtualSize field in the XEX section table
    for this resource, so we can patch it.
    Uses XEX_HEADER_SECTION_TABLE = 0x000002FF.
    """
    def be32(off):
        return struct.unpack_from(">I", data, off)[0]

    opt_count = be32(0x14)
    for i in range(opt_count):
        hdr    = 0x18 + i * 8
        opt_id = be32(hdr)
        opt_val = be32(hdr + 4)

        if opt_id != 0x000002FF:  # XEX_HEADER_SECTION_TABLE
            continue

        block_off   = opt_val
        block_size  = be32(block_off)
        entry_count = (block_size - 4) // 16

        for j in range(entry_count):
            e       = block_off + 4 + j * 16
            ent_va  = be32(e + 8)
            ent_sz  = be32(e + 12)
            if ent_va == va:
                return e + 12  # offset of VirtualSize field

    return None


def patch_xex(input_path, new_resource_path, output_path, section_name, xex1tool):
    with open(input_path, "rb") as f:
        data = bytearray(f.read())

    with open(new_resource_path, "rb") as f:
        new_blob = f.read()

    # get VA and size from xex1tool -l
    va, old_size = get_resource_info(xex1tool, input_path, section_name)
    if va is None:
        print(f"ERROR: Resource '{section_name}' not found in xex1tool -l output")
        sys.exit(1)

    # get file offset from xex1tool --address
    file_off = get_file_offset(xex1tool, input_path, va)
    if file_off is None:
        print("ERROR: xex1tool --address failed to return a file offset")
        sys.exit(1)

    new_size = len(new_blob)
    print(f"Resource '{section_name}':")
    print(f"  VA:          0x{va:08X}")
    print(f"  File offset: 0x{file_off:08X}")
    print(f"  Old size:    0x{old_size:08X}  ({old_size:,} bytes)")
    print(f"  New size:    0x{new_size:08X}  ({new_size:,} bytes)")
    print(f"  Delta:       {new_size - old_size:+,} bytes")

    # find the VirtualSize field in the XEX section table to patch
    size_field_off = find_size_field_offset(data, va, old_size)
    if size_field_off is None:
        print("WARNING: Could not find section table size field to patch")
    else:
        print(f"  Size field:  0x{size_field_off:08X}")

    # splice new blob in place of old, padding to original size
    # so all subsequent section file offsets remain valid
    if new_size > old_size:
        print(f"  WARNING: new resource is larger than old ({new_size} > {old_size})")
        print(f"           Subsequent sections will shift - this may break the XEX.")
        padded_blob = bytearray(new_blob)
    else:
        padding = old_size - new_size
        padded_blob = bytearray(new_blob) + bytearray(padding)
        print(f"  Padded to original size with {padding:,} zero bytes")

    result = (bytearray(data[:file_off])
              + padded_blob
              + bytearray(data[file_off + old_size:]))

    # patch VirtualSize in XEX section table
    if size_field_off is not None:
        struct.pack_into(">I", result, size_field_off, new_size)
        print(f"  Patched VirtualSize at 0x{size_field_off:08X}")

    with open(output_path, "wb") as f:
        f.write(result)

    print(f"\nWrote '{output_path}'  ({len(result):,} bytes)")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_xex")
    parser.add_argument("new_resource")
    parser.add_argument("output_xex")
    parser.add_argument("--name",     default="media")
    parser.add_argument("--xex1tool", default="xex1tool")
    args = parser.parse_args()

    patch_xex(args.input_xex, args.new_resource, args.output_xex,
              args.name, args.xex1tool)


if __name__ == "__main__":
    main()
