#!/usr/bin/env python3
"""Import the four-season art pack: summer becomes the base tree, the other seasons keep only the files
that differ from summer under graphics/seasons/<season>/<source path>.
usage: import_seasons.py ART_PACK_DIR
FRLG-only battle views are skipped (this game never visits those maps)."""
import os, sys, json, shutil, hashlib, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import seasons_scan as S

pack = sys.argv[1]
man = json.load(open(os.path.join(pack, 'asset_manifest.json')))
by = {}
for e in man['assets']:
    by.setdefault(e['target_path'], {})[e['season']] = e

summer_views = open(os.path.join(pack, 'overlays/summer/src/data/dbz_battle_views.h')).read()
keep_views = set(re.findall(r'\{&gTileset_General, &gTileset_(\w+),', summer_views))
print('battle views kept:', len(keep_views))

def wanted(t):
    if t.startswith('graphics/dbz/battle_views/'):
        return os.path.splitext(os.path.basename(t))[0] in keep_views
    return True

copied = variants = 0
for t, d in sorted(by.items()):
    if not wanted(t):
        continue
    src = os.path.join(pack, d['summer']['source_path'])
    dst = os.path.join(S.ROOT, t)
    if t != 'src/data/dbz_battle_views.h':
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copyfile(src, dst)
        copied += 1
    for s in S.SEASONS:
        if s == S.BASE or d[s]['sha256'] == d['summer']['sha256'] and t != 'src/data/dbz_battle_views.h':
            continue
        vdst = os.path.join(S.ROOT, S.SEASON_DIR, s, t)
        os.makedirs(os.path.dirname(vdst), exist_ok=True)
        shutil.copyfile(os.path.join(pack, d[s]['source_path']), vdst)
        variants += 1
# the summer battle view header is kept as generator input next to the other seasons
vdst = os.path.join(S.ROOT, S.SEASON_DIR, 'summer', 'src/data/dbz_battle_views.h')
os.makedirs(os.path.dirname(vdst), exist_ok=True)
shutil.copyfile(os.path.join(pack, 'overlays/summer/src/data/dbz_battle_views.h'), vdst)
print('copied', copied, 'base files;', variants, 'seasonal variant files')
