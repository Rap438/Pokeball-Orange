#!/usr/bin/env python3
"""PokeBall Orange: carry the v0.3 trainer renames (pret src/data/trainers.h) into expansion's
trainers.party, and the trainer class renames into battle_main.c. Run from repo root."""
import re, subprocess
def show(rev, path):
    return subprocess.run(['git', 'show', f'{rev}:{path}'], capture_output=True, text=True, check=True).stdout
def names(src):
    out = {}
    for m in re.finditer(r'\[(TRAINER_\w+)\]\s*=\s*\{(.*?)\n    \},', src, re.S):
        n = re.search(r'\.trainerName = _\("(.*?)"\)', m.group(2))
        if n: out[m.group(1)] = n.group(1)
    return out
old = names(show('731ad5b', 'src/data/trainers.h'))
new = names(show('v0.3.0-baseline', 'src/data/trainers.h'))
changed = {k: v for k, v in new.items() if old.get(k) != v}
p = 'src/data/trainers.party'
s = open(p).read()
done = 0
for t, n in changed.items():
    pat = re.compile(r'(=== ' + re.escape(t) + r' ===\nName: )([^\n]*)')
    if pat.search(s):
        s = pat.sub(lambda m: m.group(1) + n, s, count=1); done += 1
    else:
        print('not in party file:', t, n)
open(p, 'w').write(s)
print('trainer names ported:', done, 'of', len(changed))

oldc = show('731ad5b', 'src/data/text/trainer_class_names.h')
newc = show('v0.3.0-baseline', 'src/data/text/trainer_class_names.h')
cls = lambda src: dict(re.findall(r'\[(TRAINER_CLASS_\w+)\] = _\("(.*?)"\)', src))
oc, nc = cls(oldc), cls(newc)
p = 'src/battle_main.c'
s = open(p).read()
for k, v in nc.items():
    if oc.get(k) != v:
        s, c = re.subn(r'(\[' + k + r'\] = \{ _\(")[^"]*("\))', lambda m: m.group(1) + v + m.group(2), s, count=1)
        print('class', k, '->', v, 'ok' if c else 'MISSING')
open(p, 'w').write(s)
