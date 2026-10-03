#!/usr/bin/env python3
"""PokeBall Orange overworld restyle: Buu's Fury-inspired colour grading + textured grass.
Maps/metatile layouts are untouched; only tileset palettes and grass pixels change.
Always works from pristine copies saved in /home/claude/restyle/orig (idempotent)."""
import os, re, json, glob, shutil, struct, colorsys, math
import numpy as np
from PIL import Image

PE = '/home/claude/pokeemerald'
ORIG = '/home/claude/restyle/orig'
TS = f'{PE}/data/tilesets'

def orig_path(p):
    rel = os.path.relpath(p, PE)
    o = os.path.join(ORIG, rel)
    if not os.path.exists(o):
        os.makedirs(os.path.dirname(o), exist_ok=True)
        shutil.copy2(p, o)
    return o

# ---------------------------------------------------------------- colour grading
GRASS = [(164, 213, 197), (115, 197, 164), (65, 180, 131), (24, 164, 106)]   # light, base, dark, darker
ANCHORS = [
    # grass ramp -> Buu's Fury yellow-greens
    ((164, 213, 197), (168, 208, 56)),
    ((115, 197, 164), (120, 164, 32)),
    ((65, 180, 131), (88, 128, 16)),
    ((24, 164, 106), (48, 80, 8)),
    # tree / bush greens
    ((180, 255, 131), (200, 240, 80)),
    ((131, 197, 98), (112, 168, 24)),
    ((57, 139, 49), (40, 100, 24)),
    ((57, 82, 0), (24, 48, 8)),
    # sand / dirt -> warm orange earth
    ((238, 230, 164), (240, 208, 144)),
    ((222, 205, 131), (224, 176, 112)),
    ((213, 180, 106), (204, 144, 88)),
    ((205, 156, 82), (168, 104, 56)),
    # water: deeper, more saturated
    ((115, 189, 246), (96, 176, 248)),
    ((98, 172, 238), (72, 152, 240)),
    ((98, 164, 222), (64, 136, 224)),
    ((82, 139, 197), (48, 112, 200)),
    ((74, 115, 172), (32, 88, 176)),
    # pinkish cliffs / rocks -> warm sandstone like Buu's Fury canyons
    ((238, 213, 205), (240, 208, 160)),
    ((222, 180, 164), (224, 176, 120)),
    ((189, 148, 139), (192, 136, 88)),
    ((156, 115, 115), (160, 104, 64)),
    ((131, 90, 90), (128, 80, 48)),
    ((98, 65, 82), (88, 56, 40)),
    # navy outline -> near-black ink like Toriyama line art
    ((24, 41, 82), (24, 24, 48)),
    ((41, 49, 90), (40, 40, 64)),
]
A_SRC = np.array([a for a, b in ANCHORS], float)
A_DST = np.array([b for a, b in ANCHORS], float)

def generic(c):
    r, g, b = [v / 255 for v in c]
    h, s, v = colorsys.rgb_to_hsv(r, g, b)
    if 0.2 < h < 0.48 and s > 0.15:        # leftover greens lean yellow like the anime backgrounds
        h = h + (0.25 - h) * 0.35
    elif 0.1 < h <= 0.2 and s < 0.5 and v > 0.3:   # drab olive rock/earth -> warm sandstone
        h = h + (0.08 - h) * 0.6
        s = s * 1.35
    s = min(1.0, s * 1.25 + 0.02)
    v = min(1.0, max(0.0, 0.5 + (v - 0.5) * 1.16))   # punchier, cel-shaded contrast
    if v < 0.42:
        v *= 0.86                                     # deeper shadow tones
    r, g, b = colorsys.hsv_to_rgb(h, s, v)
    return np.array([r, g, b]) * 255

def grade(c):
    c = np.array(c, float)
    d = np.sqrt(((A_SRC - c) ** 2).sum(1))
    w = np.exp(-(d / 26.0) ** 2)
    W = w.max()
    g = generic(c)
    if W < 1e-3:
        out = g
    else:
        delta = (w[:, None] * (A_DST - A_SRC)).sum(0) / w.sum()
        out = g * (1 - W) + (c + delta) * W
    out = np.clip(np.round(out / 8) * 8, 0, 248)   # snap to 15-bit steps
    return tuple(int(v) for v in out)

def read_pal(p):
    L = open(p).read().split('\n')
    return [tuple(int(v) for v in l.split()) for l in L[3:19]]

def write_pal(p, cols):
    with open(p, 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n' + ''.join(f'{r} {g} {b}\n' for r, g, b in cols))

# ---------------------------------------------------------------- grass texture
# 16x16 texture in ramp levels: 0 light, 1 base, 2 dark, 3 darker. Quadrants 0/3 and 1/2 are equal
# because Emerald lays grass out as a 2-tile checkerboard.
TEX_A = [   # grass blades: 0 bright tip, 2 shade, 3 root
    "11011111",
    "10211101",
    "12311021",
    "11111231",
    "11101111",
    "01021111",
    "21231110",
    "31111102",
]
TEX_B = [
    "11110111",
    "11102111",
    "01123111",
    "21111101",
    "31111021",
    "11011231",
    "10211111",
    "12311111",
]
TEX = [np.array([[int(ch) for ch in row] for row in t]) for t in (TEX_A, TEX_B)]

# dirt / sand: scattered pebbles and grit
SAND = [(238, 230, 164), (222, 205, 131), (213, 180, 106), (205, 156, 82)]
DIRT_A = [
    "11111111",
    "11311111",
    "11011121",
    "11111111",
    "12111113",
    "11111101",
    "11131111",
    "11101111",
]
DIRT_B = [
    "11111211",
    "13111111",
    "01111111",
    "11112111",
    "11111131",
    "11211101",
    "11111111",
    "31111211",
]
DIRT = [np.array([[int(ch) for ch in row] for row in t]) for t in (DIRT_A, DIRT_B)]
MATERIALS = [(GRASS, TEX), (SAND, DIRT)]

def main():
    layouts = json.load(open(f'{PE}/data/layouts/layouts.json'))['layouts']
    hdr = open(f'{PE}/src/data/tilesets/headers.h').read()
    # tileset symbol -> folder
    gfx = open(f'{PE}/src/data/tilesets/graphics.h').read() + open(f'{PE}/src/graphics.c').read()
    t2d = {m.group(1): f'{PE}/' + m.group(2) for m in re.finditer(r'const u32 (gTilesetTiles_\w+)\[\] = INCGFX_U32\("(data/tilesets/[^"]+)/tiles\.png"', gfx)}
    sym2dir = {m.group(1): t2d.get(m.group(2)) for m in re.finditer(r'const struct Tileset (gTileset_\w+) =\s*\{.*?\.tiles = (gTilesetTiles_\w+)', hdr, re.S)}
    pairs = set((L['primary_tileset'], L['secondary_tileset']) for L in layouts if 'primary_tileset' in L)

    dirs = sorted(set(glob.glob(f'{TS}/primary/*')) | set(glob.glob(f'{TS}/secondary/*')))
    pal_orig = {d: [read_pal(orig_path(f'{d}/palettes/{i:02d}.pal')) for i in range(16)] for d in dirs if os.path.isdir(f'{d}/palettes')}

    # 1) grade palettes 0-12 of every tileset
    n = 0
    for d, pals in pal_orig.items():
        for i in range(13):
            new = [pals[i][0]] + [grade(c) for c in pals[i][1:]]
            write_pal(f'{d}/palettes/{i:02d}.pal', new); n += 1
    # field-effect palettes that sit on the map (tall grass rustle, etc.)
    for p in ['general_0.pal', 'general_1.pal', 'cut_grass.pal']:
        fp = f'{PE}/graphics/field_effects/palettes/{p}'
        cols = read_pal(orig_path(fp))
        write_pal(fp, [cols[0]] + [grade(c) for c in cols[1:]])

    # 2) grass texture on primary tilesets: find tile usages (palette + quadrant) across all pairs
    def tiles_of(path):
        a = np.array(Image.open(path))
        h, w = a.shape
        return a, [(ty, tx) for ty in range(h // 8) for tx in range(w // 8)]

    usage = {}   # (tileset dir, local tile) -> list of (palette colours, quadrant, flips)
    for prim_sym, sec_sym in pairs:
        pd, sd = sym2dir.get(prim_sym), sym2dir.get(sec_sym)
        if pd is None or sd is None or pd not in pal_orig or sd not in pal_orig:
            continue
        pals = pal_orig[pd][:6] + pal_orig[sd][6:13]
        for d in (pd, sd):
            mt = open(f'{d}/metatiles.bin', 'rb').read()
            for m in range(len(mt) // 16):
                ents = struct.unpack_from('<8H', mt, m * 16)
                for k, e in enumerate(ents):
                    t = e & 0x3FF; p = e >> 12
                    if p >= 13:
                        continue
                    key = (pd, t) if t < 512 else (sd, t - 512)
                    usage.setdefault(key, []).append((pals[p], k % 4, (e >> 10) & 3))

    textured = 0
    for td in sorted(set(k[0] for k in usage)):
        src = orig_path(f'{td}/tiles.png')
        im = Image.open(src); a = np.array(im).copy()
        h, w = a.shape
        ntiles = (h // 8) * (w // 8)
        for t in range(ntiles):
            us = usage.get((td, t))
            if not us:
                continue
            ty, tx = divmod(t, w // 8)
            blk = a[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8]
            for ramp, texpair in MATERIALS:
                # palette indices holding this material's ramp in (nearly) every palette the tile is drawn with
                ramp_idx = []
                for col in ramp:
                    cand = [i for i in range(1, 16) if sum(u[0][i] == col for u in us) >= 0.9 * len(us)]
                    ramp_idx.append(cand[0] if cand else None)
                if ramp_idx[1] is None or ramp_idx[0] is None or ramp_idx[2] is None:
                    continue
                base = blk == ramp_idx[1]
                if base.sum() < 6:
                    continue
                q = us[0][1]
                tex = texpair[0] if q in (0, 3) else texpair[1]
                fl = us[0][2]
                if fl & 1: tex = tex[:, ::-1]
                if fl & 2: tex = tex[::-1]
                lut = [ramp_idx[0], ramp_idx[1], ramp_idx[2], ramp_idx[3] if ramp_idx[3] is not None else ramp_idx[2]]
                newblk = blk.copy()
                for lvl in range(4):
                    newblk[base & (tex == lvl)] = lut[lvl]
                blk = newblk
                textured += 1
            a[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8] = blk
        out = Image.fromarray(a, 'P'); out.putpalette(im.getpalette())
        out.save(f'{td}/tiles.png')
    print(f'graded {n} palettes, textured {textured} grass tiles')

if __name__ == '__main__':
    main()
