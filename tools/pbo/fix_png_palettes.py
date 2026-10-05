#!/usr/bin/env python3
"""The art pack saves every indexed PNG with a padded 256-colour palette. gbagfx emits the whole PLTE for
.gbapal, which would overflow [16]-sized palette arrays, so trim each palette back to the length the
original file in git HEAD had (every pixel index is checked to fit)."""
import subprocess, io, os, sys, glob
from PIL import Image
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import seasons_scan as S
R = S.ROOT
os.chdir(R)
ref = sys.argv[1] if len(sys.argv) > 1 else 'HEAD'
fixed = 0
cands = set()
for f in glob.glob('graphics/seasons/*/**/*.png', recursive=True):
    cands.add(f.split('/', 3)[3])
out = subprocess.run(['git', 'diff', '--name-only', ref, '--', '*.png'], capture_output=True, text=True).stdout.split()
cands |= set(out)
for t in sorted(cands):
    blob = subprocess.run(['git', 'show', f'{ref}:{t}'], capture_output=True).stdout
    if not blob:
        continue
    n = len(Image.open(io.BytesIO(blob)).getpalette() or []) // 3
    for p in [t] + [os.path.join(S.SEASON_DIR, s, t) for s in S.SEASONS]:
        if not os.path.exists(p):
            continue
        im = Image.open(p)
        if im.mode != 'P' or len(im.getpalette()) // 3 <= n:
            continue
        a = np.array(im)
        assert a.max() < n, (p, a.max(), n)
        pal = im.getpalette()[:n * 3]
        out = Image.fromarray(a, 'P')
        out.putpalette(pal)
        bits = 4 if n <= 16 else 8
        out.save(p, bits=bits) if bits == 4 else out.save(p)
        fixed += 1
print('trimmed', fixed, 'png palettes')
