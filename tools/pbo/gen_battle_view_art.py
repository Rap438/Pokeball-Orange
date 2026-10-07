#!/usr/bin/env python3
"""Rebuild the location battle views (src/dbz_battle_bg.c) for every season from the current tilesets.

The art pack shipped these as prebuilt .4bpp/.gbapal files, which .gitignore drops, so a fresh clone
could not build. This regenerates them the same way tools_pbo/gen_battle_views.py did in v0.3, but:
  - per season, using graphics/seasons/<season>/ overrides of the tileset tiles and palettes;
  - written as source files the repo keeps (indexed .png + JASC .pal; make converts them);
  - only for the views listed in graphics/seasons/summer/src/data/dbz_battle_views.h.

Writes graphics/dbz/battle_views/<View>.{png,pal} (summer), graphics/seasons/<s>/graphics/dbz/battle_views/
for seasons whose art differs, the per-season headers, and then src/data/dbz_battle_views.h via gen_seasons.
usage: gen_battle_view_art.py
"""
import json, os, re, struct, subprocess, sys
import numpy as np
from PIL import Image
from sklearn.cluster import KMeans

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import seasons_scan as S

R = S.ROOT
NPAL, NCOL = 6, 15
VIEW_DIR = 'graphics/dbz/battle_views'
GFX = os.path.join(R, 'tools/gbagfx/gbagfx')

def rp(p):
    return os.path.join(R, p)

def load_pal(path):
    L = open(rp(path)).read().replace('\r', '').split('\n')[3:19]
    return np.array([[int(v) for v in l.split()] for l in L], np.float32)

def load_tiles(path):
    a = np.array(Image.open(rp(path)))
    h, w = a.shape
    return [a[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8] & 15 for ty in range(h // 8) for tx in range(w // 8)]

def grade_for_battle(rgb):
    # soften a touch so the Pokemon pop in front: lower contrast + slight haze
    return rgb * 0.82 + 255 * 0.08

# ---------------------------------------------------------------- inputs
base_head = open(rp(f'{S.SEASON_DIR}/summer/src/data/dbz_battle_views.h')).read()
VIEWS = re.findall(r'\{&gTileset_General, &gTileset_(\w+),', base_head)

hdr = open(rp('src/data/tilesets/headers.h')).read()
gfx = open(rp('src/data/tilesets/graphics.h')).read() + open(rp('src/graphics.c')).read()
t2d = {m.group(1): m.group(2) for m in re.finditer(r'const u32 (gTilesetTiles_\w+)\[\] = INCGFX_U32\("(data/tilesets/[^"]+)/tiles\.png"', gfx)}
sym2dir = {m.group(1): t2d[m.group(2)] for m in re.finditer(r'const struct Tileset (gTileset_\w+) =\s*\{.*?\.tiles = (gTilesetTiles_\w+)', hdr, re.S) if m.group(2) in t2d}

layouts = [L for L in json.load(open(rp('data/layouts/layouts.json')))['layouts'] if 'id' in L]

def used_metatiles(ssym):
    s = set()
    for L in layouts:
        if L['primary_tileset'] != 'gTileset_General' or L['secondary_tileset'] != ssym:
            continue
        for key in ('blockdata_filepath', 'border_filepath'):
            blk = open(rp(L[key]), 'rb').read()
            for i in range(0, len(blk), 2):
                s.add(struct.unpack_from('<H', blk, i)[0] & 0x3FF)
    return sorted(s)

# ---------------------------------------------------------------- one view
def build_view(name, season):
    pd, sd = sym2dir['gTileset_General'], sym2dir[f'gTileset_{name}']
    v = lambda p: S.variant_path(season, p)
    ptiles, stiles = load_tiles(v(f'{pd}/tiles.png')), load_tiles(v(f'{sd}/tiles.png'))
    tiles = ptiles + [np.zeros((8, 8), np.uint8)] * (512 - len(ptiles)) + stiles
    pals = [load_pal(v(f'{pd}/palettes/{i:02d}.pal')) for i in range(6)] + \
           [load_pal(v(f'{sd}/palettes/{i:02d}.pal')) for i in range(6, 13)]
    pmt = open(rp(v(f'{pd}/metatiles.bin')), 'rb').read()
    smt = open(rp(v(f'{sd}/metatiles.bin')), 'rb').read()
    used = used_metatiles(f'gTileset_{name}')
    qtiles = []
    for mid in used:
        mt, local = (pmt, mid) if mid < 512 else (smt, mid - 512)
        ents = struct.unpack_from('<8H', mt, local * 16) if local * 16 + 16 <= len(mt) else [0] * 8
        for q in range(4):
            out = np.zeros((8, 8, 3), np.float32)
            for layer in range(2):
                e = ents[layer * 4 + q]
                t, p = e & 0x3FF, e >> 12
                if t >= len(tiles) or p >= 13:
                    continue
                a = tiles[t]
                if e >> 10 & 1: a = a[:, ::-1]
                if e >> 11 & 1: a = a[::-1]
                col = pals[p][a]
                if layer == 0:
                    out = col.copy()
                else:
                    m = a != 0
                    out[m] = col[m]
            qtiles.append(grade_for_battle(out))
    T5 = np.round(np.array(qtiles).reshape(len(qtiles), 64, 3) / 255 * 31)
    feat = np.concatenate([T5.mean(1), T5.std(1)], 1)
    assign = KMeans(NPAL, n_init=3, random_state=2).fit_predict(feat)
    for _ in range(5):
        palsq = []
        for p in range(NPAL):
            pts = T5[assign == p].reshape(-1, 3)
            if len(pts) == 0:
                pts = T5[0]
            u = np.unique(pts, axis=0)
            if len(u) <= NCOL:
                pal = np.zeros((NCOL, 3)); pal[:len(u)] = u; pal[len(u):] = u[0]
            else:
                pal = np.round(KMeans(NCOL, n_init=1, random_state=1).fit(pts[::max(1, len(pts) // 20000)]).cluster_centers_)
            palsq.append(np.clip(pal, 0, 31))
        palsq = np.array(palsq)
        errs = np.zeros((len(T5), NPAL)); idxs = np.zeros((len(T5), NPAL, 64), np.uint8)
        for s0 in range(0, len(T5), 512):
            d = ((T5[s0:s0 + 512, None, :, None, :] - palsq[None, :, None, :, :]) ** 2).sum(-1)
            errs[s0:s0 + 512] = d.min(-1).sum(-1); idxs[s0:s0 + 512] = d.argmin(-1)
        new = errs.argmin(1)
        if (new == assign).all():
            break
        assign = new
    tidx = idxs[np.arange(len(T5)), assign] + 1
    # dedupe (no flips: the game copies tiles straight into VRAM)
    pool, lookup, entries = [], {}, []
    for k in range(len(T5)):
        a = tidx[k].reshape(8, 8).astype(np.uint8)
        key = (int(assign[k]), a.tobytes())
        if key not in lookup:
            lookup[key] = len(pool); pool.append(a)
        entries.append(lookup[key] | (int(assign[k]) << 12))
    mtab = [0xFFFF] * (1024 * 4)
    for n, mid in enumerate(used):
        for q in range(4):
            mtab[mid * 4 + q] = entries[n * 4 + q]
    dark, light = [], []
    for p in palsq:
        full = np.concatenate([[[0, 0, 0]], p])
        for ci in range(16):
            c = full[ci]
            dt, lt = c * 0.62, np.minimum(31, c * 1.25 + 3)
            dark.append(0 if ci == 0 else 1 + int(((p - dt) ** 2).sum(1).argmin()))
            light.append(0 if ci == 0 else 1 + int(((p - lt) ** 2).sum(1).argmin()))
    return pool, palsq, mtab, dark, light

def write_art(dirpath, name, pool, palsq):
    os.makedirs(rp(dirpath), exist_ok=True)
    per_row = 16
    rows = (len(pool) + per_row - 1) // per_row
    img = np.zeros((rows * 8, per_row * 8), np.uint8)
    for i, a in enumerate(pool):
        img[(i // per_row) * 8:(i // per_row) * 8 + 8, (i % per_row) * 8:(i % per_row) * 8 + 8] = a
    im = Image.fromarray(img, 'P')
    flat = []
    for c in [(0, 0, 0)] + [tuple(int(x) * 8 for x in c) for c in palsq[0]]:
        flat += list(c)
    im.putpalette(flat + [0] * (768 - len(flat)))
    im.save(rp(f'{dirpath}/{name}.png'), bits=4)
    with open(rp(f'{dirpath}/{name}.pal'), 'w', newline='\r\n') as f:
        f.write(f'JASC-PAL\n0100\n{NPAL * 16}\n')
        for p in palsq:
            for c in [(0, 0, 0)] + [tuple(int(x) for x in c) for c in p]:
                f.write(' '.join(str(x * 8) for x in c) + '\n')
    # the generator compares built outputs, so produce them now (make would regenerate them anyway)
    subprocess.run([GFX, rp(f'{dirpath}/{name}.png'), rp(f'{dirpath}/{name}.4bpp')], check=True)
    subprocess.run([GFX, rp(f'{dirpath}/{name}.pal'), rp(f'{dirpath}/{name}.gbapal')], check=True)

def main():
    results = {}
    for season in [S.BASE] + [t for t in S.SEASONS if t != S.BASE]:
        out = ['// generated by tools/pbo/gen_battle_view_art.py', '']
        table = []
        for name in VIEWS:
            pool, palsq, mtab, dark, light = build_view(name, season)
            results[(season, name)] = (pool, palsq)
            same = season != S.BASE and len(pool) == len(results[(S.BASE, name)][0]) and \
                all((a == b).all() for a, b in zip(pool, results[(S.BASE, name)][0])) and \
                (palsq == results[(S.BASE, name)][1]).all()
            d = VIEW_DIR if season == S.BASE else f'{S.SEASON_DIR}/{season}/{VIEW_DIR}'
            stale = [rp(f'{d}/{name}.{e}') for e in ('png', 'pal', '4bpp', 'gbapal')]
            if same:
                for f in stale:
                    if os.path.exists(f):
                        os.remove(f)
            else:
                write_art(d, name, pool, palsq)
            out.append(f'static const u32 sBV_Tiles_{name}[] = INCBIN_U32("{VIEW_DIR}/{name}.4bpp");')
            out.append(f'static const u16 sBV_Pal_{name}[] = INCBIN_U16("{VIEW_DIR}/{name}.gbapal");')
            out.append(f'static const u16 sBV_Meta_{name}[] = {{' + ','.join(map(str, mtab)) + '};')
            out.append(f'static const u8 sBV_Dark_{name}[] = {{' + ','.join(map(str, dark)) + '};')
            out.append(f'static const u8 sBV_Light_{name}[] = {{' + ','.join(map(str, light)) + '};')
            table.append(f'    {{&gTileset_General, &gTileset_{name}, sBV_Tiles_{name}, sBV_Pal_{name}, sBV_Meta_{name}, sBV_Dark_{name}, sBV_Light_{name}}},')
            print(f'{season:6} {name}: {len(pool)} tiles' + (' (same as summer)' if same else ''))
        out += ['', 'static const struct DbzBattleView sBattleViews[] = {'] + table + ['};']
        open(rp(f'{S.SEASON_DIR}/{season}/src/data/dbz_battle_views.h'), 'w').write('\n'.join(out) + '\n')
    import gen_seasons  # noqa: E402  (regenerates src/data/dbz_battle_views.h and the other season tables)

if __name__ == '__main__':
    main()
