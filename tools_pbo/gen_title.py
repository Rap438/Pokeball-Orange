#!/usr/bin/env python3
"""PokeBall Orange title screen: convert OG's 16:9 art into a 240x160 GBA background
(4bpp tiles, up to 16 palettes x 15 colours, tilemap). PRESS START is painted out of the art
and drawn by the game's own blinking sprite instead."""
import numpy as np, os
from PIL import Image
from sklearn.cluster import KMeans

SRC = '/home/claude/title/src.png'
OUT = '/home/claude/pokeemerald/graphics/title_screen/pbo'
os.makedirs(OUT, exist_ok=True)

im = np.array(Image.open(SRC).convert('RGB')).astype(np.float32)
# paint out "PRESS START" (sits on the sky gradient): interpolate each column between clean rows
y0, y1, x0, x1 = 540, 602, 640, 1030
top, bot = im[y0], im[y1]
for y in range(y0, y1):
    t = (y - y0) / (y1 - y0)
    im[y, x0:x1] = top[x0:x1] * (1 - t) + bot[x0:x1] * t
art = Image.fromarray(im.clip(0, 255).astype(np.uint8))
art = art.crop((86, 0, 1586, 941)).resize((240, 160), Image.LANCZOS)
art.save('/home/claude/title/scaled.png')
px = np.array(art).astype(np.float32)
px = np.round(px / 255 * 31)  # 15-bit colour
H, W = 160, 240
TH, TW = H // 8, W // 8
tiles = px.reshape(TH, 8, TW, 8, 3).transpose(0, 2, 1, 3, 4).reshape(TH * TW, 64, 3)

NPAL, NCOL = 16, 15
rng = np.random.RandomState(7)
# initial grouping: tiles by mean colour + colour spread
feat = np.concatenate([tiles.mean(1), tiles.std(1)], 1)
assign = KMeans(NPAL, n_init=4, random_state=3).fit_predict(feat)

def fit_palettes(assign):
    pals = []
    for p in range(NPAL):
        pts = tiles[assign == p].reshape(-1, 3)
        if len(pts) == 0:
            pts = tiles[rng.randint(len(tiles))]
        uniq = np.unique(pts, axis=0)
        if len(uniq) <= NCOL:
            pal = np.zeros((NCOL, 3)); pal[:len(uniq)] = uniq; pal[len(uniq):] = uniq[0]
        else:
            km = KMeans(NCOL, n_init=2, random_state=1).fit(pts)
            pal = np.round(km.cluster_centers_)
        pals.append(pal.clip(0, 31))
    return np.array(pals)

def tile_err(pals):
    # err[t, p] = sum over pixels of min squared distance to palette p
    d = ((tiles[:, None, :, None, :] - pals[None, :, None, :, :]) ** 2).sum(-1)  # T,P,64,C
    return d.min(-1).sum(-1), d.argmin(-1)

for it in range(8):
    pals = fit_palettes(assign)
    err, _ = tile_err(pals)
    new = err.argmin(1)
    print('iter', it, 'err', err[np.arange(len(tiles)), new].sum())
    if (new == assign).all():
        break
    assign = new
pals = fit_palettes(assign)
err, idx = tile_err(pals)
assign = err.argmin(1)
tidx = idx[np.arange(len(tiles)), assign] + 1  # colour 0 = transparent (unused)

# dedupe tiles with flips
tileset, lookup, tmap = [], {}, []
for t in range(len(tiles)):
    a = tidx[t].reshape(8, 8)
    pal = int(assign[t])
    found = None
    for hf in (0, 1):
        for vf in (0, 1):
            b = a[:, ::-1] if hf else a
            b = b[::-1] if vf else b
            k = (pal, b.tobytes())
            if k in lookup:
                found = (lookup[k], hf, vf); break
        if found: break
    if not found:
        lookup[(pal, a.tobytes())] = len(tileset)
        tileset.append(a)
        found = (len(tileset) - 1, 0, 0)
    n, hf, vf = found
    tmap.append(n | (hf << 10) | (vf << 11) | (pal << 12))
print('tiles', len(tileset))
assert len(tileset) <= 1024

with open(f'{OUT}/title.4bpp', 'wb') as f:
    for a in tileset:
        for row in a:
            for x in range(0, 8, 2):
                f.write(bytes([int(row[x]) | (int(row[x + 1]) << 4)]))
# 32x32 screen (tilemap is 32 wide)
full = np.zeros((32, 32), np.uint16)
full[:TH, :TW] = np.array(tmap, np.uint16).reshape(TH, TW)
full.astype('<u2').tofile(f'{OUT}/title.bin')
with open(f'{OUT}/title.gbapal', 'wb') as f:
    for p in pals:
        cols = [(0, 0, 0)] + [tuple(int(v) for v in c) for c in p]
        for r, g, b in cols:
            v = r | (g << 5) | (b << 10)
            f.write(bytes([v & 0xFF, v >> 8]))
# preview
prev = np.zeros((H, W, 3), np.uint8)
for t in range(len(tiles)):
    ty, tx = divmod(t, TW)
    cols = pals[assign[t]][tidx[t] - 1]
    prev[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8] = (cols.reshape(8, 8, 3) * 255 / 31).astype(np.uint8)
Image.fromarray(prev).save('/home/claude/title/preview.png')
Image.fromarray(prev).resize((720, 480), Image.NEAREST).save('/home/claude/title/preview3x.png')
