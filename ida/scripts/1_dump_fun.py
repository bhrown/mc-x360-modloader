import idautils
import idc
import json
import os

EXPORT_PATH = r"F:\minecraft-lce\patch\func_export.json"

funcs_to_export = [
    "loadMod",
    "dispatcher",
    "dispatcher_bl",
]

out = {}
for name in funcs_to_export:
    ea = idc.get_name_ea_simple(name)
    if ea == idc.BADADDR:
        # try mangled name search
        for func_ea in idautils.Functions():
            fname = idc.get_func_name(func_ea)
            demangled = idc.demangle_name(fname, idc.get_inf_attr(idc.INF_SHORT_DN))
            if name in (demangled or "") or name in fname:
                ea = func_ea
                break

    if ea == idc.BADADDR:
        print(f"WARNING: {name} not found")
        continue

    func = idaapi.get_func(ea)
    if func is None:
        print(f"WARNING: no function at {ea:08X} for {name}")
        continue

    size  = func.end_ea - func.start_ea
    data  = idc.get_bytes(ea, size)

    out[name] = {
        "ea":    hex(ea),
        "size":  size,
        "bytes": list(data),
    }
    print(f"Exported {name}: 0x{ea:08X} size=0x{size:X}")

with open(EXPORT_PATH, "w") as f:
    json.dump(out, f, indent=2)

print(f"Written to {EXPORT_PATH}")
