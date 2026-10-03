#!/usr/bin/env python3
"""PokeBall Orange key items: Flying Nimbus, Dragon Radar, the seven Dragon Balls (icons + data)."""
import math, os, re
import numpy as np
from PIL import Image
PE = '/home/claude/pokeemerald'
ICON = f'{PE}/graphics/items/icons'; PALD = f'{PE}/graphics/items/icon_palettes'

def save_icon(name, rgba):
    from build_ow import quantize, save_jasc
    idx, pal = quantize([rgba])
    im = Image.fromarray(idx[0], 'P'); flat = []
    for c in pal: flat += list(c)
    im.putpalette(flat + [0] * (768 - len(flat))); im.save(f'{ICON}/{name}.png')
    save_jasc(pal, f'{PALD}/{name}.pal')

def ball(stars):
    im = np.zeros((24, 24, 4), np.uint8)
    cx, cy, r = 11.5, 12.0, 9.6
    for y in range(24):
        for x in range(24):
            d = math.hypot(x + .5 - cx, y + .5 - cy)
            if d <= r:
                # shading: light from top-left
                l = ((x + .5 - cx) * -0.6 + (y + .5 - cy) * -0.8) / r
                if d > r - 1.1: c = (160, 72, 0)
                elif l > 0.55: c = (255, 232, 160)
                elif l > 0.15: c = (255, 184, 48)
                elif l > -0.35: c = (248, 144, 16)
                else: c = (208, 104, 8)
                im[y, x, :3] = c; im[y, x, 3] = 255
    # stars
    pos = {1: [(12, 13)], 2: [(9, 13), (15, 13)], 3: [(12, 9), (9, 15), (15, 15)],
           4: [(9, 10), (15, 10), (9, 16), (15, 16)], 5: [(12, 8), (8, 11), (16, 11), (9, 16), (15, 16)],
           6: [(9, 8), (15, 8), (7, 13), (17, 13), (9, 18), (15, 18)],
           7: [(12, 7), (8, 10), (16, 10), (12, 13), (8, 16), (16, 16), (12, 19)]}[stars]
    for sx, sy in pos:
        for dx, dy in [(0, 0), (-1, 0), (1, 0), (0, -1), (0, 1)]:
            im[sy + dy, sx + dx, :3] = (200, 16, 16)
        im[sy, sx, :3] = (232, 40, 32)
    im[6, 8, :3] = (255, 255, 255); im[7, 7, :3] = (255, 255, 255)
    return im

def radar():
    im = np.zeros((24, 24, 4), np.uint8)
    cx, cy = 12, 13
    for y in range(24):
        for x in range(24):
            d = math.hypot(x + .5 - cx, y + .5 - cy)
            if d <= 10:
                c = (72, 72, 80) if d > 9 else (232, 232, 240)
                if d <= 7.2:
                    c = (24, 120, 48) if d > 6.4 else (64, 200, 96)
                    if (x - 4) % 4 == 0 or (y - 5) % 4 == 0: c = (40, 160, 72)
                im[y, x, :3] = c; im[y, x, 3] = 255
    # button on top
    for x in range(10, 15):
        for y in range(1, 4):
            im[y, x, :3] = (72, 72, 80) if y == 1 or x in (10, 14) else (200, 200, 208); im[y, x, 3] = 255
    for (x, y) in [(15, 10), (9, 15), (14, 16)]:
        im[y, x, :3] = (248, 200, 32); im[y, x + 1, :3] = (248, 120, 16)
    im[13, 12, :3] = (232, 32, 32)
    return im

def nimbus_icon():
    big = np.array(Image.open(f'{PE}/graphics/field_effects/pics/bird.png').convert('RGBA'))
    ys, xs = np.where(big[..., 3] > 0)
    big = big[ys.min():ys.max() + 1, xs.min():xs.max() + 1]
    sm = np.array(Image.fromarray(big).resize((22, max(6, int(22 * big.shape[0] / big.shape[1]))), Image.NEAREST))
    im = np.zeros((24, 24, 4), np.uint8)
    oy = (24 - sm.shape[0]) // 2
    im[oy:oy + sm.shape[0], 1:23] = sm
    return im

if __name__ == '__main__':
    import sys; sys.path.insert(0, '/home/claude/tools')
    save_icon('nimbus', nimbus_icon())
    save_icon('dragon_radar', radar())
    for n in range(1, 8):
        save_icon(f'dragon_ball_{n}', ball(n))
    print('icons ok')
