#!/usr/bin/env python3
"""Make a Buu-saga Krillin overworld sheet: Goku's frames with a short, close-cropped haircut."""
import sys
sys.path.insert(0, '/home/claude/tools')
import numpy as np
from build_ow import *

def is_hair(px):
    r, g, b, a = [px[..., i].astype(int) for i in range(4)]
    return (a > 0) & (r < 70) & (g < 70) & (b < 70)

def is_skin(px):
    r, g, b, a = [px[..., i].astype(int) for i in range(4)]
    return (a > 0) & (r > 180) & (g > 120) & (b < 170) & (r - b > 60)

def haircut(fr, role):
    fr = fr.copy()
    H, W = fr.shape[:2]
    hair = is_hair(fr); skin = is_skin(fr)
    hy = np.where(hair[:18].any(1))[0]
    if len(hy) == 0: return fr
    # head extent: use skin in rows 6..17 when visible, else hair columns at row 12
    sy, sx = np.where(skin[6:18]); sy = sy + 6
    if role != 'B' and len(sx) > 3:
        cx = (sx.min() + sx.max()) / 2.0; half = (sx.max() - sx.min()) / 2.0 + 1.0
        face_top = sy.min()
    else:
        cols = np.where(hair[12])[0]
        cx = (cols.min() + cols.max()) / 2.0 if len(cols) else W / 2
        half = 4.0; face_top = 9
    if role == 'S':
        half += 0.5
    top = face_top - 4
    cy = face_top + 3
    ry = cy - top
    for y in range(0, 18):
        for x in range(W):
            if not hair[y, x]: continue
            inside = ((x - cx) / (half + 0.5)) ** 2 + ((y - cy) / ry) ** 2 <= 1.0 and y >= top
            if not inside:
                fr[y, x] = 0
    # fill any gaps on top of skull so it reads as a buzz cut
    for y in range(top, cy):
        for x in range(W):
            if fr[y, x, 3] == 0 and ((x - cx) / half) ** 2 + ((y - cy) / ry) ** 2 <= 0.85:
                fr[y, x, :3] = (24, 24, 24); fr[y, x, 3] = 255
    # shift the whole figure down 2px so Krillin reads shorter than Goku (feet are kept)
    out = np.zeros_like(fr)
    out[2:20] = fr[0:18]
    out[20:] = fr[20:]
    return out

def krillin_frames():
    base = sheet_frames(L5(1156))
    roles = ['F', 'B', 'S', 'F', 'F', 'B', 'B', 'S', 'S']
    return [haircut(f, r) for f, r in zip(base, roles)]
