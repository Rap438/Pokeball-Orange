#!/usr/bin/env python3
"""Generate SSJ3 Goku overworld frames by growing a long golden mane onto SSJ frames."""
import sys
sys.path.insert(0, '/home/claude/tools')
import numpy as np
from build_ow import *

def is_gold(px):
    r, g, b, a = [px[..., i].astype(int) for i in range(4)]
    yy = np.arange(px.shape[0])[:, None] * np.ones(px.shape[1], int)[None]
    return (a > 0) & (r > 200) & (g > 200) & (b < 150) & (yy < 18)

def gold_tones(fr):
    m = is_gold(fr)
    cols = fr[m][:, :3].astype(int)
    lum = cols.sum(1)
    order = np.argsort(lum)
    mid = np.array([248, 248, 72]); light = np.array([248, 248, 200]); dark = np.array([216, 168, 56])
    outline = np.array([168, 120, 56])
    return outline, dark, mid, light

def put(fr, x, y, c):
    if 0 <= x < fr.shape[1] and 0 <= y < fr.shape[0]:
        fr[y, x, :3] = c; fr[y, x, 3] = 255

def mane(fr, role, tones):
    """role: 'F', 'B', 'S' (S = facing left)."""
    fr = fr.copy()
    outline, dark, mid, light = tones
    gold = is_gold(fr)
    ys, xs = np.where(gold)
    if len(ys) == 0: return fr
    hair_bottom = ys.max()
    body = (fr[..., 3] > 0) & ~gold
    H, W = fr.shape[:2]
    end = min(H - 6, hair_bottom + 12)
    cx = int(round(xs.mean()))
    hw_top = (xs.max() - xs.min()) // 2 + 1
    shape = np.zeros((H, W), bool)
    for y in range(hair_bottom - 2, end + 1):
        t = (y - (hair_bottom - 2)) / max(1, end - hair_bottom + 2)
        half = int(round(hw_top * (1 - 0.55 * t)))
        if role == 'S':
            # mane flows behind (to the right when facing left)
            x0 = cx - 1; x1 = cx + half + 1
            if y > hair_bottom + 3: x0 = cx + 1
        elif role == 'F':
            x0 = cx - half - 2; x1 = cx + half + 2
        else:
            x0 = cx - half; x1 = cx + half
        # jagged bottom
        if y == end and (y % 2 == 0): x0 += 1; x1 -= 1
        for x in range(max(0, x0), min(W, x1 + 1)):
            shape[y, x] = True
    new = fr.copy()
    for y in range(H):
        for x in range(W):
            if not shape[y, x]: continue
            if role == 'F' and body[y, x]:
                continue  # body in front of mane
            if role != 'F' or fr[y, x, 3] == 0:
                # strand shading
                edge = not (shape[y, max(0, x - 1)] and shape[y, min(W - 1, x + 1)]) or (y + 1 < H and not shape[y + 1, x])
                if edge: c = outline
                elif (x + (y // 3)) % 3 == 0: c = dark
                elif (x + (y // 3)) % 3 == 1: c = mid
                else: c = light
                new[y, x, :3] = c; new[y, x, 3] = 255
    return new

def ssj3_frames(spec):
    base = sheet_frames(spec)
    tones = gold_tones(base[0])
    roles = ['F', 'B', 'S', 'F', 'F', 'B', 'B', 'S', 'S']
    return [mane(f, r, tones) for f, r in zip(base, roles)]
