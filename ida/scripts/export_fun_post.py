# export_fun_post.py
#
# IDA run on latest/default.xex after loading latest/default.pdb
# Compare and map functions prior to and after loading pdb file

import idautils
import idaapi
import json

with open(r"F:\path\to\pre_pdb_snapshot.json", "r") as f:
    pre = {int(k): v for k, v in json.load(f).items()}

post = {}
for func in idautils.Functions():
    name = idaapi.get_func_name(func)
    post[func] = name

pre_addrs = set(pre.keys())
post_addrs = set(post.keys())

new_funcs = post_addrs - pre_addrs
removed_funcs = pre_addrs - post_addrs
renamed = {a for a in pre_addrs & post_addrs if pre[a] != post[a]}

print("Functions before: {}".format(len(pre)))
print("Functions after:  {}".format(len(post)))
print("New functions created: {}".format(len(new_funcs)))
print("Functions removed: {}".format(len(removed_funcs)))
print("Functions renamed: {}".format(len(renamed)))

print("\nSample new functions:")
for addr in list(new_funcs)[:20]:
    print("  {:#010x}  {}".format(addr, post[addr]))

print("\nSample renamed:")
for addr in list(renamed)[:20]:
    print("  {:#010x}  {} -> {}".format(addr, pre[addr], post[addr]))

base = r"F:\path\to\basedir"

with open(base + r"\post_pdb_snapshot.json", "w") as f:
    json.dump({str(k): v for k, v in post.items()}, f)

with open(base + r"\new_funcs.json", "w") as f:
    json.dump({str(addr): post[addr] for addr in new_funcs}, f)

with open(base + r"\removed_funcs.json", "w") as f:
    json.dump({str(addr): pre[addr] for addr in removed_funcs}, f)

with open(base + r"\renamed_funcs.json", "w") as f:
    json.dump({str(addr): {"before": pre[addr], "after": post[addr]} for addr in renamed}, f, indent=2)

print("\nExports saved to {}".format(base))
