#!/usr/bin/env python3
"""Render each text-mode BG layer of a GBA memory dump separately (full map, scroll ignored), index-0 transparent.
usage: bgrip.py PREFIX OUTDIR"""
import sys, os, struct
from PIL import Image
pre, outd = sys.argv[1], sys.argv[2]
os.makedirs(outd, exist_ok=True)
vram = open(pre + '.vram', 'rb').read()
pal = open(pre + '.pal', 'rb').read()
io = open(pre + '.io', 'rb').read()
disp = struct.unpack_from('<H', io, 0)[0]
print('DISPCNT %04X mode %d' % (disp, disp & 7))
def col(i):
    c = struct.unpack_from('<H', pal, i * 2)[0]
    return ((c & 31) * 255 // 31, ((c >> 5) & 31) * 255 // 31, ((c >> 10) & 31) * 255 // 31, 255)
for n in range(4):
    cnt = struct.unpack_from('<H', io, 8 + 2 * n)[0]
    hs, vs = struct.unpack_from('<HH', io, 0x10 + 4 * n)
    en = bool(disp & (0x100 << n))
    cbase = ((cnt >> 2) & 3) * 0x4000; sbase = ((cnt >> 8) & 31) * 0x800
    bpp8 = bool(cnt & 0x80); sz = cnt >> 14
    mw, mh = [(32, 32), (64, 32), (32, 64), (64, 64)][sz]
    print(f'BG{n} en={en} cnt={cnt:04X} char={cbase:X} screen={sbase:X} 8bpp={bpp8} {mw}x{mh} scroll={hs},{vs} pri={cnt&3}')
    im = Image.new('RGBA', (mw * 8, mh * 8), (0, 0, 0, 0)); px = im.load()
    for my in range(mh):
        for mx in range(mw):
            blk = (mx // 32) + (my // 32) * (2 if mw == 64 else 1)
            e = struct.unpack_from('<H', vram, sbase + blk * 0x800 + ((my % 32) * 32 + (mx % 32)) * 2)[0]
            t = e & 0x3FF; hf = e & 0x400; vf = e & 0x800; pb = e >> 12
            for y in range(8):
                for x in range(8):
                    sx = 7 - x if hf else x; sy = 7 - y if vf else y
                    if bpp8:
                        a = cbase + t * 64 + sy * 8 + sx
                        v = vram[a] if a < 0x10000 else 0
                    else:
                        a = cbase + t * 32 + sy * 4 + sx // 2
                        b = vram[a] if a < 0x10000 else 0
                        v = (b >> 4) if sx & 1 else (b & 15)
                        if v: v += pb * 16
                    if v: px[mx * 8 + x, my * 8 + y] = col(v)
    im.save(f'{outd}/bg{n}.png')
