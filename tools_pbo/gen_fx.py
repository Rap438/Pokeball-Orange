#!/usr/bin/env python3
"""Procedural pixel art for PokeBall Orange field effects: ki blast, Kamehameha beam, charge glow, impact, SSJ aura, sparks."""
import numpy as np, math, os
from PIL import Image

PE = '/home/claude/pokeemerald'
OUT = f'{PE}/graphics/dbz'
os.makedirs(OUT, exist_ok=True)

PAL = [(0, 0, 0), (255, 255, 255), (255, 255, 200), (248, 232, 64), (248, 160, 32),
       (208, 248, 255), (96, 208, 248), (40, 136, 248), (24, 64, 200), (200, 136, 24),
       (255, 224, 120), (168, 96, 16), (160, 240, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0)]

def blank(w=16, h=16): return np.zeros((h, w), np.uint8)

def disc(img, cx, cy, r, c):
    h, w = img.shape
    for y in range(h):
        for x in range(w):
            if (x + .5 - cx) ** 2 + (y + .5 - cy) ** 2 <= r * r:
                img[y, x] = c

def ki_ball(phase):
    im = blank()
    disc(im, 8, 8, 6 + phase * 0.6, 4)
    disc(im, 8, 8, 4.6 + phase * 0.5, 3)
    disc(im, 8, 8, 3.2, 2)
    disc(im, 8, 8, 1.8, 1)
    return im

def beam_h(phase):
    im = blank()
    for x in range(16):
        wob = 1 if (x // 4 + phase) % 2 else 0
        for y in range(16):
            d = abs(y - 7.5)
            if d < 1.6: im[y, x] = 1
            elif d < 3.0: im[y, x] = 5
            elif d < 4.6 + wob * 0.6: im[y, x] = 6
            elif d < 6.0 + wob: im[y, x] = 7
    return im

def head(phase):
    im = blank()
    disc(im, 8, 8, 7.6 if phase else 7.0, 8)
    disc(im, 8, 8, 6.4, 7)
    disc(im, 8, 8, 5.0, 6)
    disc(im, 8, 8, 3.6, 5)
    disc(im, 8, 8, 2.2, 1)
    return im

def impact(phase):
    im = blank()
    r = 4 + phase * 3
    for a in range(0, 360, 30):
        for t in range(int(r) - 2, int(r) + 2):
            x = int(8 + math.cos(math.radians(a + phase * 15)) * t)
            y = int(8 + math.sin(math.radians(a + phase * 15)) * t)
            if 0 <= x < 16 and 0 <= y < 16: im[y, x] = 1 if t < r else 3
    disc(im, 8, 8, 3 - phase, 2)
    return im

def charge(phase):
    im = blank()
    disc(im, 8, 8, 4 + phase, 7)
    disc(im, 8, 8, 3 + phase * 0.5, 6)
    disc(im, 8, 8, 1.6, 1)
    for k in range(4):
        a = math.radians(k * 90 + 45 + phase * 30)
        x = int(8 + math.cos(a) * (6 + phase)); y = int(8 + math.sin(a) * (6 + phase))
        if 0 <= x < 16 and 0 <= y < 16: im[y, x] = 5
    return im

def spark(phase):
    im = blank()
    pts = [(3, 1), (7, 5), (5, 8), (10, 12), (8, 15)] if phase == 0 else [(12, 1), (8, 5), (11, 8), (5, 12), (7, 15)]
    for (x0, y0), (x1, y1) in zip(pts, pts[1:]):
        n = max(abs(x1 - x0), abs(y1 - y0))
        for i in range(n + 1):
            x = round(x0 + (x1 - x0) * i / n); y = round(y0 + (y1 - y0) * i / n)
            im[y, x] = 1
            if x + 1 < 16 and im[y, x + 1] == 0: im[y, x + 1] = 12
    return im

frames = [ki_ball(0), ki_ball(1), beam_h(0), beam_h(1), beam_h(0).T.copy(), beam_h(1).T.copy(),
          head(0), head(1), impact(0), impact(1), charge(0), charge(1), spark(0), spark(1)]

def aura(phase):
    im = blank(32, 32)
    for y in range(32):
        for x in range(32):
            dx = (x + .5 - 16) / 11.0
            dy = (y + .5 - 19) / 15.0
            r = math.sqrt(dx * dx + dy * dy)
            flame = 0.18 * math.sin(x * 1.3 + phase * 2.1) + (0.25 if y < 12 and (x + phase * 3) % 5 < 2 else 0)
            if r < 1.0 + flame:
                if r > 0.86 + flame * 0.5: im[y, x] = 9
                elif r > 0.72 + flame * 0.3: im[y, x] = 3
                elif r > 0.62: im[y, x] = 10
    return im

def save(arrs, path):
    a = np.concatenate(arrs, axis=0)
    im = Image.fromarray(a, 'P')
    flat = []
    for c in PAL: flat += list(c)
    im.putpalette(flat + [0] * (768 - len(flat)))
    im.save(path)

save(frames, f'{OUT}/fx.png')
save([aura(p) for p in range(3)], f'{OUT}/aura.png')
print('fx ok', len(frames))
