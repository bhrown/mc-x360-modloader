# 3_patch_branches.py

# Replace vftable entry for HellDimension_getFogColor with dispatcher
VFTABLE_SLOT = 0x8205D6C8
ida_bytes.patch_dword(VFTABLE_SLOT, addr_map["dispatcher"])
idc.create_insn(VFTABLE_SLOT)
