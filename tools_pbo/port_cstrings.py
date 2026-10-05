#!/usr/bin/env python3
"""Re-apply our string changes to C string definitions by variable name.
port_cstrings.py FILE BASE_REV OUR_REV  (git revs) -- edits FILE in place"""
import re, subprocess, sys
f, base, ours = sys.argv[1:4]
def show(rev):
    return subprocess.run(['git', 'show', f'{rev}:{f}'], capture_output=True, text=True, cwd='/home/claude/pokeemerald').stdout
DEF = re.compile(r'^(\s*(?:static\s+)?const u8 (\w+)\[\]\s*=\s*)(_\(".*"\));\s*$', re.M)
b = {m.group(2): m.group(3) for m in DEF.finditer(show(base))}
o = {m.group(2): m.group(3) for m in DEF.finditer(show(ours))}
cur = open('/home/claude/pokeemerald/' + f).read()
n = 0; missing = []
for name, val in o.items():
    if b.get(name) == val:
        continue
    pat = re.compile(r'^(\s*(?:static\s+)?const u8 ' + re.escape(name) + r'\[\]\s*=\s*)(_\(".*"\)|COMPOUND_STRING\(".*"\));\s*$', re.M)
    m = pat.search(cur)
    if not m:
        missing.append(name); continue
    cur = cur[:m.start(2)] + val + cur[m.end(2):]
    n += 1
open('/home/claude/pokeemerald/' + f, 'w').write(cur)
print(f, 'ported', n, 'missing', missing)
