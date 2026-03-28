# 3_patch_branches.py

import idc
import ida_bytes

def patch(branch_addr, target):
    # Compute new bl/b instruction
    # Check if original is bl (bit 0 = 1) or b (bit 0 = 0)
    orig = ida_bytes.get_dword(branch_addr)
    is_bl = orig & 1

    offset  = target - branch_addr
    li      = ((offset >> 2) & 0xFFFFFF) << 2
    new_insn = 0x48000000 | li | is_bl

    ida_bytes.patch_dword(branch_addr, new_insn)
    idc.create_insn(branch_addr)
    print(f"Patched branch at 0x{branch_addr:08X} -> 0x{target:08X}")

patch(0x8261077C, addr_map["dispatcher"]) # Vec3_newTemp_double_double_double in HellDimension::getFogColor
#patch(0x825ACA90, addr_map["dispatcher_bl"]) # Vec3_newTemp_double_double_double in Dimension::getFogColor
