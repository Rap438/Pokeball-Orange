#!/usr/bin/env python3
"""Render a tileset pair's metatiles (primary [+ secondary]) to PNG, using JASC palettes."""
import sys, struct, numpy as np
from PIL import Image
PE = '/home/claude/pokeemerald'
def load_pal(path):
    L = open(path).read().split('\n')[3:19]
    return [tuple(int(v) for v in l.split()) for l in L]
def load_tiles(path):
    im = Image.open(path); a = np.array(im)
    h, w = a.shape; t = []
    for ty in range(h // 8):
        for tx in range(w // 8):
            t.append(a[ty*8:ty*8+8, tx*8:tx*8+8] & 15)
    return t
def tileset(kind, name, palroot=None):
    d = f'{PE}/data/tilesets/{kind}/{name}'
    pals = [load_pal(f'{d}/palettes/{i:02d}.pal') for i in range(16)]
    return load_tiles(f'{d}/tiles.png'), pals, open(f'{d}/metatiles.bin', 'rb').read()
def render(prim, sec=None, which='primary', cols=16):
    pt, pp, pm = prim
    tiles = pt + [np.zeros((8, 8), np.uint8)] * (512 - len(pt))
    pals = pp[:6] + (sec[1][6:13] if sec else pp[6:13]) + pp[13:]
    if sec: tiles = tiles + sec[0]
    mt = pm if which == 'primary' else sec[2]
    n = len(mt) // 16
    rows = (n + cols - 1) // cols
    out = np.zeros((rows * 16, cols * 16, 3), np.uint8); out[:] = (255, 0, 255)
    for m in range(n):
        ents = struct.unpack_from('<8H', mt, m * 16)
        my, mx = divmod(m, cols)
        for layer in range(2):
            for q in range(4):
                e = ents[layer * 4 + q]
                t = e & 0x3FF; hf = e >> 10 & 1; vf = e >> 11 & 1; p = e >> 12
                if t >= len(tiles): continue
                a = tiles[t]
                if hf: a = a[:, ::-1]
                if vf: a = a[::-1]
                col = np.array(pals[p], np.uint8)[a]
                y0 = my * 16 + (q // 2) * 8; x0 = mx * 16 + (q % 2) * 8
                reg = out[y0:y0+8, x0:x0+8]
                mask = a != 0 if layer else np.ones((8, 8), bool)
                reg[mask] = col[mask]
    return out
if __name__ == '__main__':
    prim = tileset('primary', sys.argv[1])
    sec = tileset('secondary', sys.argv[2]) if len(sys.argv) > 2 and sys.argv[2] != '-' else None
    which = sys.argv[3] if len(sys.argv) > 3 else 'primary'
    o = render(prim, sec, which)
    Image.fromarray(o).resize((o.shape[1] * 2, o.shape[0] * 2), Image.NEAREST).save(sys.argv[4] if len(sys.argv) > 4 else '/home/claude/restyle/mt.png')
