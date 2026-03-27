# import_bindiff.py

import idaapi
import idc
import sqlite3
import ida_hexrays
import json
import idautils
from collections import defaultdict

db_path = r"F:\path\to\diff.BinDiff"

db = sqlite3.connect(db_path)
cursor = db.cursor()

cursor.execute("""
    SELECT address1, name2, similarity
    FROM function
    WHERE similarity >= 0.75
    AND name2 NOT LIKE 'sub_%'
    AND name2 NOT LIKE 'nullsub_%'
    AND name2 NOT LIKE 'j_%'
    AND name2 != ''
""")
rows = cursor.fetchall()
print("Function matches to import: {}".format(len(rows)))
count = 0
skipped = 0
for address, name, similarity in rows:
    if idaapi.set_name(address, name, idaapi.SN_FORCE | idaapi.SN_NOWARN):
        count += 1
    else:
        skipped += 1
print("Done. Renamed: {}  Skipped: {}".format(count, skipped))
