#!/usr/bin/env python3
"""Voxel Hoenn for the Flying Nimbus flight (src/dbz_voxel.c).

Builds a 512x512 heightmap + colour map of Hoenn from the region map art, two texels per region-map pixel:
  - water/land from the art's colours (small water specks inside land, i.e. map markers, become land),
  - land height from distance to the coast, the art's own relief shading and smooth noise,
  - routes (the tan paths) sit a little lower and keep their colour,
  - Mt. Chimney is a peak with a crater, Sootopolis a ring crater, Ever Grande a high plateau,
  - every town/city is a flat plateau with a cluster of roofed blocks,
  - colours get slope lighting from the north-west and snow on the highest peaks.
Colours are quantised to 60 base colours; the palette holds 4 fog levels of each (240 entries) and a
16-colour sky gradient (240-255).

Writes graphics/dbz/voxel/terrain.bin (u16 per texel: height << 8 | base colour) and
src/data/dbz_voxel_data.h (palette, sky, town and landmark positions).
usage: gen_voxel_hoenn.py
"""
import json, os, re, struct
import numpy as np
from PIL import Image
from sklearn.cluster import KMeans

R = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
N = 512                 # world size in texels (wraps)
S = 2                   # texels per region-map pixel
OX, OY = 8, 104         # where region-map pixel (0,0) lands in the world
CURSOR_X_MIN, CURSOR_Y_MIN = 1, 2   # src/region_map.c: map section (0,0) is at grid cell (1,2)
NBASE, NFOG = 60, 4

def rp(p):
    return os.path.join(R, p)

def region_map_rgb():
    im = Image.open(rp('graphics/pokenav/region_map/map.png'))
    a = np.array(im)
    L = open(rp('graphics/pokenav/region_map/map.pal')).read().split('\n')[3:]
    pal = np.array([[int(v) for v in l.split()] for l in L if l.strip()], np.float32)
    tiles = [a[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8] for ty in range(a.shape[0] // 8) for tx in range(a.shape[1] // 8)]
    m = np.frombuffer(open(rp('graphics/pokenav/region_map/map.bin'), 'rb').read(), np.uint8).reshape(64, 64)
    out = np.zeros((19 * 8, 31 * 8), np.uint8)
    for y in range(19):
        for x in range(31):
            if m[y, x] < len(tiles):
                out[y * 8:y * 8 + 8, x * 8:x * 8 + 8] = tiles[m[y, x]]
    # the map's 8bpp indices start at BG palette 7 (src/region_map.c loads map.pal at BG_PLTT_ID(7))
    return pal[np.clip(out.astype(int) - 112, 0, len(pal) - 1)]

def smooth(a, r):
    k = np.ones(2 * r + 1) / (2 * r + 1)
    a = np.apply_along_axis(lambda v: np.convolve(np.pad(v, r, mode='wrap'), k, 'valid'), 0, a)
    return np.apply_along_axis(lambda v: np.convolve(np.pad(v, r, mode='wrap'), k, 'valid'), 1, a)

def noise(seed, scale):
    rng = np.random.default_rng(seed)
    n = N // scale
    g = rng.random((n, n))
    g = np.kron(g, np.ones((scale, scale)))
    return smooth(g, scale // 2)

def dist_transform(mask):
    """chamfer distance (in texels) from every True cell to the nearest False cell"""
    INF = 10 ** 6
    d = np.where(mask, INF, 0).astype(np.int64)
    H, W = d.shape
    for y in range(H):
        for x in range(W):
            if d[y, x]:
                best = d[y, x]
                if y: best = min(best, d[y - 1, x] + 2)
                if x: best = min(best, d[y, x - 1] + 2)
                if y and x: best = min(best, d[y - 1, x - 1] + 3)
                if y and x < W - 1: best = min(best, d[y - 1, x + 1] + 3)
                d[y, x] = best
    for y in range(H - 1, -1, -1):
        for x in range(W - 1, -1, -1):
            if d[y, x]:
                best = d[y, x]
                if y < H - 1: best = min(best, d[y + 1, x] + 2)
                if x < W - 1: best = min(best, d[y, x + 1] + 2)
                if y < H - 1 and x < W - 1: best = min(best, d[y + 1, x + 1] + 3)
                if y < H - 1 and x: best = min(best, d[y + 1, x - 1] + 3)
                d[y, x] = best
    return d / 2.0

def mapsecs():
    j = json.load(open(rp('src/data/region_map/region_map_sections.json')))
    secs = j['map_sections'] if isinstance(j, dict) else j
    out = {}
    for s in secs:
        if s['id'] == 'MAPSEC_PALLET_TOWN':
            break   # Hoenn's sections come first; Kanto's and Sevii's use the same grid coordinates
        if 'x' in s and 'name' in s:
            out[s['id']] = s
    return out

def sec_center(s):
    """world texel centre of a map section"""
    px = (s['x'] + CURSOR_X_MIN + s.get('width', 1) / 2.0) * 8
    py = (s['y'] + CURSOR_Y_MIN + s.get('height', 1) / 2.0) * 8
    return OX + px * S, OY + py * S

def main():
    rgb = region_map_rgb()                              # 152 x 248 x 3
    r, g, b = rgb[..., 0], rgb[..., 1], rgb[..., 2]
    water = (b > g + 4) & (b > r + 30)
    tan = (r > 150) & (g > 100) & (r > b + 40)
    # map markers drawn in blue on land: specks of "water" walled in by land become land
    from collections import deque
    seen = np.zeros_like(water)
    H0, W0 = water.shape
    for y in range(H0):
        for x in range(W0):
            if water[y, x] and not seen[y, x]:
                comp, q = [], deque([(y, x)]); seen[y, x] = True
                while q:
                    cy, cx = q.popleft(); comp.append((cy, cx))
                    for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                        ny, nx = cy + dy, cx + dx
                        if 0 <= ny < H0 and 0 <= nx < W0 and water[ny, nx] and not seen[ny, nx]:
                            seen[ny, nx] = True; q.append((ny, nx))
                if len(comp) < 40:
                    for cy, cx in comp:
                        water[cy, cx] = False
    # upscale into the world
    up = lambda a: np.kron(a, np.ones((S, S)))
    land = np.zeros((N, N), bool)
    land[OY:OY + H0 * S, OX:OX + W0 * S] = up(~water).astype(bool)
    route = np.zeros((N, N), bool)
    route[OY:OY + H0 * S, OX:OX + W0 * S] = up(tan & ~water).astype(bool)
    art = np.zeros((N, N, 3), np.float32)
    art[...] = (64, 136, 176)
    art[OY:OY + H0 * S, OX:OX + W0 * S] = np.repeat(np.repeat(rgb, S, 0), S, 1)
    relief = np.zeros((N, N), np.float32)
    lum = rgb.mean(-1)
    relief[OY:OY + H0 * S, OX:OX + W0 * S] = up(np.where(~water & ~tan, lum, 0))

    d = dist_transform(land)
    n1, n2 = noise(1, 32), noise(2, 8)
    h = np.where(land, 4 + np.minimum(d, 44) * 0.8 + (relief - 40) * 0.25 + n1 * 18 * np.clip(d / 12, 0, 1) + n2 * 4, 0)
    h = np.where(route & land, np.maximum(6, h * 0.7), h)
    h = smooth(h, 3)
    h = np.where(land, np.maximum(h, 4), 0)

    secs = mapsecs()
    yy, xx = np.mgrid[0:N, 0:N]
    def bump(sec, radius, height, crater=0):
        if sec not in secs:
            return
        cx, cy = sec_center(secs[sec])
        rr = np.hypot(xx - cx, yy - cy)
        cone = np.clip(1 - rr / radius, 0, 1) ** 1.3 * height
        if crater:
            cone -= np.clip(1 - rr / crater, 0, 1) * height * 0.35
        h[...] = np.maximum(h, np.where(land | (rr < radius * 0.5), cone, h))
    bump('MAPSEC_MT_CHIMNEY', 46, 150, crater=7)
    bump('MAPSEC_METEOR_FALLS', 30, 90)
    bump('MAPSEC_MT_PYRE', 18, 70)
    bump('MAPSEC_SOOTOPOLIS_CITY', 22, 55, crater=15)

    # land colours: meadow -> forest -> rock by height with a little noise; sand at the shore; routes keep the map's tan
    n3 = noise(4, 4)
    t = np.clip((h - 15) / 115 + (n3 - 0.5) * 0.3, 0, 1)[..., None]
    meadow, forest, rock = np.array([112, 176, 72]), np.array([48, 128, 64]), np.array([136, 124, 104])
    landc = np.where(t < 0.5, meadow * (1 - t * 2) + forest * (t * 2), forest * (2 - t * 2) + rock * (t * 2 - 1))
    sand = (d < 3)[..., None]
    landc = np.where(sand, np.array([224, 208, 152]), landc)
    roadc = np.array([216, 176, 112])
    colour = np.where(land[..., None], np.where(route[..., None], roadc, landc), art)
    rng = np.random.default_rng(7)
    towns = []
    for sid, s in secs.items():
        if not re.search(r'_(TOWN|CITY)$', sid) or s.get('x', 99) > 27 or s.get('y', 99) > 14:
            continue
        cx, cy = sec_center(s)
        w = s.get('width', 1) * 8 * S; hh = s.get('height', 1) * 8 * S
        x0, y0 = int(cx - w / 2), int(cy - hh / 2)
        x1, y1 = x0 + w, y0 + hh
        base = max(8.0, float(np.median(h[y0:y1, x0:x1])))
        if sid == 'MAPSEC_EVER_GRANDE_CITY':
            base = max(base, 50.0)
        # level the town softly into its surroundings
        rr = np.hypot((xx - cx) / (w / 2 + 6), (yy - cy) / (hh / 2 + 6))
        wgt = np.clip(1.6 - rr * 1.2, 0, 1)
        h[...] = h * (1 - wgt) + base * wgt
        colour[y0:y1, x0:x1] = (200, 184, 136)
        for _ in range(max(3, (w * hh) // 40)):
            bw, bh = rng.integers(2, 4), rng.integers(2, 4)
            bx, by = rng.integers(x0, max(x0 + 1, x1 - bw)), rng.integers(y0, max(y0 + 1, y1 - bh))
            roof = [(208, 64, 48), (56, 104, 200), (232, 168, 48), (200, 200, 208)][rng.integers(0, 4)]
            h[by:by + bh, bx:bx + bw] = base + rng.integers(2, 5)
            colour[by:by + bh, bx:bx + bw] = roof
        towns.append((sid, int(cx), int(cy), int(base)))

    h = np.clip(h, 0, 250)
    # lighting from the north-west, snow on peaks, foam/shallows near coasts
    dx = h - np.roll(np.roll(h, 1, 0), 1, 1)
    light = np.clip(1.0 + dx * 0.035, 0.55, 1.35)
    col = colour * light[..., None]
    snow = np.clip((h - 140) / 30, 0, 1)[..., None]
    col = col * (1 - snow) + np.array([236, 240, 248]) * snow
    wat = ~land
    dw = dist_transform(wat)
    shallow = np.clip(1 - dw / 10, 0, 1)[..., None] * wat[..., None]
    sea = np.array([40, 112, 176]) * (1 - shallow) + np.array([96, 184, 208]) * shallow
    ripple = (noise(3, 4) - 0.5) * 30
    sea = sea + ripple[..., None]
    col = np.where(wat[..., None], sea, col)
    col = np.clip(col, 0, 255)

    # quantise
    flat = col.reshape(-1, 3)
    km = KMeans(NBASE, n_init=1, random_state=1).fit(flat[::37])
    cols = np.clip(np.round(km.cluster_centers_), 0, 255)
    idx = np.zeros(len(flat), np.uint8)
    for s0 in range(0, len(flat), 65536):
        dd = ((flat[s0:s0 + 65536, None, :] - cols[None]) ** 2).sum(-1)
        idx[s0:s0 + 65536] = dd.argmin(1)
    idx = idx.reshape(N, N)

    os.makedirs(rp('graphics/dbz/voxel'), exist_ok=True)
    hb = h.astype(np.uint16)
    with open(rp('graphics/dbz/voxel/terrain.bin'), 'wb') as f:
        f.write(((hb << 8) | idx.astype(np.uint16)).astype('<u2').tobytes())

    horizon = np.array([200, 224, 240])
    def gba(c):
        c = np.clip(np.round(np.array(c) / 8), 0, 31).astype(int)
        return c[0] | (c[1] << 5) | (c[2] << 10)
    pal = []
    for f in range(NFOG):
        t = [0.0, 0.12, 0.3, 0.55][f]
        for c in cols:
            pal.append(gba(c * (1 - t) + horizon * t))
    sky = [gba(np.array([56, 104, 200]) * (1 - k / 15) + horizon * (k / 15)) for k in range(16)]
    pal += sky
    out = ['// generated by tools/pbo/gen_voxel_hoenn.py - do not edit', '',
           f'#define VOXEL_WORLD_SIZE {N}', f'#define VOXEL_NUM_BASE_COLOURS {NBASE}', '#define VOXEL_SKY_FIRST 240', '',
           'static const u16 sVoxelPalette[256] = {' + ', '.join(f'0x{v:04X}' for v in pal) + '};', '',
           '// world texel = region map pixel * %d + (%d, %d); region map cell = 8 pixels; map section (0,0) = cell (%d, %d)'
           % (S, OX, OY, CURSOR_X_MIN, CURSOR_Y_MIN),
           f'#define VOXEL_SCALE {S}', f'#define VOXEL_ORIGIN_X {OX}', f'#define VOXEL_ORIGIN_Y {OY}',
           f'#define VOXEL_CURSOR_X_MIN {CURSOR_X_MIN}', f'#define VOXEL_CURSOR_Y_MIN {CURSOR_Y_MIN}', '']
    open(rp('src/data/dbz_voxel_data.h'), 'w').write('\n'.join(out) + '\n')
    print('towns', len(towns), 'max height', int(h.max()))

if __name__ == '__main__':
    main()
