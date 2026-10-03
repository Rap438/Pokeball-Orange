#!/usr/bin/env python3
"""Rip OAM sprites from a GBA memory dump.
usage: rip.py PREFIX OUTDIR
expects PREFIX.oam (0x400), PREFIX.vram (0x18000 from 0x06000000), PREFIX.pal (0x400), PREFIX.io (0x60)
writes OUTDIR/spr_NN.png (RGBA, index 0 transparent) and OUTDIR/sheet.png
"""
import sys, os, struct
from PIL import Image

pre, outd = sys.argv[1], sys.argv[2]
os.makedirs(outd, exist_ok=True)
oam = open(pre + '.oam', 'rb').read()
vram = open(pre + '.vram', 'rb').read()
pal = open(pre + '.pal', 'rb').read()
io = open(pre + '.io', 'rb').read()
dispcnt = struct.unpack_from('<H', io, 0)[0]
map1d = bool(dispcnt & 0x40)
SIZES = {(0, 0): (8, 8), (0, 1): (16, 16), (0, 2): (32, 32), (0, 3): (64, 64),
         (1, 0): (16, 8), (1, 1): (32, 8), (1, 2): (32, 16), (1, 3): (64, 32),
         (2, 0): (8, 16), (2, 1): (8, 32), (2, 2): (16, 32), (2, 3): (32, 64)}

def color(i):
    c = struct.unpack_from('<H', pal, i * 2)[0]
    return ((c & 31) * 255 // 31, ((c >> 5) & 31) * 255 // 31, ((c >> 10) & 31) * 255 // 31)

def render(idx, raw=False):
    a0, a1, a2 = struct.unpack_from('<HHH', oam, idx * 8)
    mode = (a0 >> 8) & 3
    if mode == 2: return None
    shape = a0 >> 14; size = a1 >> 14
    if shape == 3: return None
    w, h = SIZES[(shape, size)]
    bpp8 = bool(a0 & 0x2000)
    tile = a2 & 0x3FF; palb = a2 >> 12
    hflip = bool(a1 & 0x1000) and not (a0 & 0x100)
    vflip = bool(a1 & 0x2000) and not (a0 & 0x100)
    im = Image.new('RGBA', (w, h), (0, 0, 0, 0))
    tw, th = w // 8, h // 8
    idxs = [[0] * w for _ in range(h)]
    for ty in range(th):
        for tx in range(tw):
            if bpp8:
                t = tile + (ty * tw * 2 + tx * 2 if map1d else ty * 32 + tx * 2)
                base = 0x10000 + (t & 0x3FF) * 32
                for y in range(8):
                    for x in range(8):
                        v = vram[base + y * 8 + x] if base + y * 8 + x < len(vram) else 0
                        idxs[ty * 8 + y][tx * 8 + x] = v
            else:
                t = tile + (ty * tw + tx if map1d else ty * 32 + tx)
                base = 0x10000 + (t & 0x3FF) * 32
                for y in range(8):
                    for x in range(4):
                        b = vram[base + y * 4 + x] if base + y * 4 + x < len(vram) else 0
                        idxs[ty * 8 + y][tx * 8 + x * 2] = (b & 15) + (palb << 4 if b & 15 else 0)
                        idxs[ty * 8 + y][tx * 8 + x * 2 + 1] = (b >> 4) + (palb << 4 if b >> 4 else 0)
    px = im.load()
    for y in range(h):
        for x in range(w):
            v = idxs[y][x]
            if v:
                px[x, y] = color(256 + v) + (255,)
    if hflip: im = im.transpose(Image.FLIP_LEFT_RIGHT)
    if vflip: im = im.transpose(Image.FLIP_TOP_BOTTOM)
    ypos = a0 & 0xFF; xpos = a1 & 0x1FF
    return im, (xpos, ypos, w, h, tile, palb, bpp8, (a0 >> 10) & 3, a1 & 0x1000 > 0)

items = []
for i in range(128):
    r = render(i)
    if r and r[0].getbbox():
        items.append((i, r))
        r[0].save(f'{outd}/spr_{i:03d}.png')
# contact sheet with labels
from PIL import ImageDraw
cell = 72
cols = 10
rows = (len(items) + cols - 1) // cols or 1
S = Image.new('RGBA', (cols * cell, rows * (cell + 10)), (60, 60, 90, 255))
d = ImageDraw.Draw(S)
for k, (i, (im, info)) in enumerate(items):
    x = (k % cols) * cell; y = (k // cols) * (cell + 10)
    sc = max(1, min(cell // im.width, cell // im.height))
    S.alpha_composite(im.resize((im.width * sc, im.height * sc), Image.NEAREST), (x, y + 10))
    d.text((x + 1, y), f'{i} t{info[4]}p{info[5]}', fill=(255, 255, 0, 255))
S = S.resize((S.width * 2, S.height * 2), Image.NEAREST)
S.save(f'{outd}/sheet.png')
with open(f'{outd}/info.txt', 'w') as f:
    for i, (im, info) in items: f.write(f'{i} x{info[0]} y{info[1]} {info[2]}x{info[3]} tile{info[4]} pal{info[5]} 8bpp{info[6]} pri{info[7]} hflip{info[8]}\n')
print(len(items), 'sprites, map1d=', map1d)
