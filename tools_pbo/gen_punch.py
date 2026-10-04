#!/usr/bin/env python3
"""Goku's overworld-fight attack frames (32x32) from Buu's Fury's fighter sprites.
Frames per form, in this order (side frames face west; east is h-flipped in game):
  0 down windup, 1 down punch, 2 up windup, 3 up punch, 4 side windup, 5 side punch,
  6 down kick-windup, 7 down kick, 8 up kick-windup, 9 up kick, 10 side kick-windup, 11 side kick
SSJ forms recolour the black hair to gold (Buu's Fury only has the base set in these poses)."""
import sys, pickle, os
sys.path.insert(0, '/home/claude/tools')
import numpy as np
from build_ow import RGB, quantize, save_indexed, save_jasc

PE = '/home/claude/pokeemerald'
OUT = f'{PE}/graphics/dbz/punch'
os.makedirs(OUT, exist_ok=True)
D = pickle.load(open('/home/claude/buu/f1024.pk', 'rb'))
KS = sorted(D)
FRAMES = [1171, 1173, 1176, 1177, 1180, 1181, 1160, 1162, 1164, 1166, 1192, 1193]

HAIR = 1
GOLD = {
    'goku_ssj':  ((248, 240, 88), (232, 184, 40), (176, 112, 16)),
    'goku_ssj2': ((248, 248, 120), (240, 200, 56), (184, 120, 24)),
    'goku_ssj3': ((248, 232, 96), (224, 176, 48), (168, 104, 16)),
}

def rgba(i, hair=None):
    f = D[KS[i]]
    im = np.zeros((32, 32, 4), np.uint8)
    im[..., :3] = RGB[f]
    im[..., 3] = (f > 0) * 255
    if hair:
        from scipy import ndimage
        lab, n = ndimage.label(f == HAIR)
        h = np.zeros_like(f, bool)
        for k in range(1, n + 1):
            if (lab == k).sum() >= 10:       # eyes / small dark details stay dark
                h |= lab == k
        dist = ndimage.distance_transform_cdt(np.pad(h, 1), metric='taxicab')[1:-1, 1:-1]
        yy, xx = np.mgrid[0:32, 0:32]
        streak = ((xx - yy) % 5 == 0) | ((xx + yy) % 7 == 0)
        im[h & (dist >= 3), :3] = hair[0]
        im[h & (dist >= 3) & streak, :3] = hair[1]
        im[h & (dist == 2), :3] = hair[1]
        im[h & (dist == 1), :3] = hair[2]
    return im

SETS = [('goku', None, FRAMES)] + [(n, h, FRAMES) for n, h in GOLD.items()]
SETS.append(('vegito', None, [f - 566 for f in FRAMES]))   # Super Vegito: same layout in Buu's Fury, 566 frames earlier
for name, hair, frames in SETS:
    frs = [rgba(i, hair) for i in frames]
    idx, pal = quantize(frs)
    save_indexed(idx, pal, f'{OUT}/{name}.png', horizontal=False)
    save_jasc(pal, f'{OUT}/{name}.pal')
    print(name, 'ok')
