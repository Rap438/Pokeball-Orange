#!/usr/bin/env python3
"""PboCove draws grass with General's grass ramp (palette 2, colours 12-15) so the camp blends into the
surrounding town. The art pack restyled the two tilesets separately; copy General's seasonal ramp back
into the cove's terrain (7: 1-4) and light (11: 1-4 and 5-8) palettes for every season."""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import seasons_scan as S
os.chdir(S.ROOT)

def path(season, p):
    return p if season == 'summer' else S.variant_path(season, p)

def load(p):
    lines = open(p).read().replace('\r', '').strip().split('\n')
    return lines[:3], [tuple(map(int, l.split())) for l in lines[3:]]

def save(p, head, cols):
    open(p, 'w').write('\r\n'.join(head + ['%d %d %d' % c for c in cols]) + '\r\n')

for s in S.SEASONS:
    _, gen = load(path(s, 'data/tilesets/primary/general/palettes/02.pal'))
    ramp = gen[12:16]
    for pal, slots in (('07', (1,)), ('11', (1, 5))):
        p = path(s, f'data/tilesets/secondary/pbo_cove/palettes/{pal}.pal')
        if s != 'summer' and p == f'data/tilesets/secondary/pbo_cove/palettes/{pal}.pal':
            p = os.path.join(S.SEASON_DIR, s, p)   # create the seasonal copy
            os.makedirs(os.path.dirname(p), exist_ok=True)
            head, cols = load(f'data/tilesets/secondary/pbo_cove/palettes/{pal}.pal')
        else:
            head, cols = load(p)
        for start in slots:
            cols[start:start + 4] = ramp
        save(p, head, cols)
    print(s, ramp)
