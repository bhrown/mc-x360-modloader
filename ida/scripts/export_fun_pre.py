# export_fun_pre.py
#
# IDA run on latest/default.xex before loading latest/default.pdb

import idautils
import idaapi
import json

snapshot = {}
for func in idautils.Functions():
    name = idaapi.get_func_name(func)
    snapshot[func] = name

with open(r"F:\path\to\pre_pdb_snapshot.json", "w") as f:
    json.dump({str(k): v for k, v in snapshot.items()}, f)

print("Snapshot saved: {} functions".format(len(snapshot)))
