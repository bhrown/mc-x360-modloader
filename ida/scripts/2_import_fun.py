# 2_import_fun.py
#
# Path loader functions and calls into program

import idc
import idaapi
import ida_bytes
import ida_funcs
import ida_name
import json

EXPORT_PATH  = r"F:\path\to\func_export.json"
CAVE_START   = 0x82610A78
CAVE_END     = 0x82610CA4

with open(EXPORT_PATH) as f:
    funcs = json.load(f)

cursor = CAVE_START
addr_map = {}

for name, info in funcs.items():
    data = bytes(info["bytes"])
    size = info["size"]
    if cursor % 4:
        cursor += 4 - (cursor % 4)
    ida_bytes.patch_bytes(cursor, data)
    addr_map[name] = cursor
    cursor += size

remaining = CAVE_END - cursor
if remaining > 0:
    ida_bytes.patch_bytes(cursor, b"\x00" * remaining)

print("Clearing existing functions in cave range...")
ea = CAVE_START
while ea < CAVE_END:
    func = idaapi.get_func(ea)
    if func:
        ida_funcs.del_func(func.start_ea)
    idc.del_items(ea, idc.DELIT_SIMPLE, 4)
    ea += 4

print("Defining functions:")
for name, addr in addr_map.items():
    size = funcs[name]["size"]

    ea = addr
    while ea < addr + size:
        length = idc.create_insn(ea)
        ea += length if length > 0 else 4

    if not idc.add_func(addr):
        ida_funcs.add_func(addr, addr + size)

    ida_name.set_name(addr, name, ida_name.SN_NOCHECK | ida_name.SN_FORCE)
    idc.set_func_cmt(addr, "Imported from loader.obj", 1)
    print(f"  {name:<20} 0x{addr:08X} size=0x{size:X}")

print("Reanalyzing...")
idc.plan_and_wait(CAVE_START, CAVE_END)
idaapi.auto_wait()

# patch nop at 0x822EA738 with bl loadMod
PATCH_ADDR  = 0x822EA738
LOAD_MOD_ADDR = addr_map["loadMod"]

# compute bl offset
# bl encoding: bits 0-5 = 0b010010, bits 6-29 = signed offset>>2, bit 30 = AA, bit 31 = LK
offset = LOAD_MOD_ADDR - PATCH_ADDR
li     = ((offset >> 2) & 0xFFFFFF) << 2
bl_insn = 0x48000001 | li

print(f"Patching bl loadMod at 0x{PATCH_ADDR:08X} -> 0x{LOAD_MOD_ADDR:08X} (offset=0x{offset:X})")
ida_bytes.patch_dword(PATCH_ADDR, bl_insn)
idc.create_insn(PATCH_ADDR)

print("Done.")
