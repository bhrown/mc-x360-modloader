# import_fun_renamed.py
#
# IDA run on latest/default.xex (no pdb file) after generating renamed_funcs.json and
# reloading latest/default.xex from scratch.

import idaapi
import json

with open(r"F:\path\to\renamed_funcs.json", "r") as f:
    renamed = json.load(f)

count = 0
skipped = 0
for addr_str, names in renamed.items():
    addr = int(addr_str)
    name = names["after"]
    if name and not name.startswith('sub_'):
        if idaapi.set_name(addr, name, idaapi.SN_FORCE | idaapi.SN_NOWARN):
            count += 1
        else:
            skipped += 1

print("Renamed: {}  Skipped: {}".format(count, skipped))
