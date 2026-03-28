import idc
import idaapi
import ida_bytes
import ida_funcs
import ida_name
import json

EXPORT_PATH  = r"F:\minecraft-lce\patch\func_export.json"
CAVE_START   = 0x82610A78  # your .cave section address
CAVE_END     = 0x82610CA4  # end of .cave section

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

# Zero remaining
remaining = CAVE_END - cursor
if remaining > 0:
    ida_bytes.patch_bytes(cursor, b"\x00" * remaining)

# Step 1 - delete all existing functions and items in cave range
print("Clearing existing functions in cave range...")
ea = CAVE_START
while ea < CAVE_END:
    func = idaapi.get_func(ea)
    if func:
        ida_funcs.del_func(func.start_ea)
    idc.del_items(ea, idc.DELIT_SIMPLE, 4)
    ea += 4

# Step 2 - redefine code and functions
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

# Step 3 - reanalysis
print("Reanalyzing...")
idc.plan_and_wait(CAVE_START, CAVE_END)
idaapi.auto_wait()

# Patch nop at 0x822EA738 with bl loadMod
PATCH_ADDR  = 0x822EA738
LOAD_MOD_ADDR = addr_map["loadMod"]

offset = LOAD_MOD_ADDR - PATCH_ADDR
li     = ((offset >> 2) & 0xFFFFFF) << 2
bl_insn = 0x48000001 | li

print(f"Patching bl loadMod at 0x{PATCH_ADDR:08X} -> 0x{LOAD_MOD_ADDR:08X} (offset=0x{offset:X})")
ida_bytes.patch_dword(PATCH_ADDR, bl_insn)
idc.create_insn(PATCH_ADDR)

print("Done.")
