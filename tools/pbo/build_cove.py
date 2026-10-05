#!/usr/bin/env python3
"""PokeBall Orange v0.4 demo area: builds the gTileset_PboCove secondary tileset and repaints
Littleroot Town + Route 101 with it.

Art direction (approved mockup): warm, localized light (campfire, lamps, windows) against cooler
surroundings, grounding shadows, layered vegetation, readable dirt paths, a river with real banks.

GBA techniques used (all stock engine features of pokeemerald-expansion 1.17):
  * baked terrain: paths, river banks, shadows and the fire's light pool are painted per pixel and
    cut into metatiles (periodic textures keep the tile count down)
  * DNS light blending: palette 11 marks the fire colours and a second copy of the grass colours as
    light-blended (11.pla). By day they look like ordinary grass; at night they blend toward warm
    orange instead of darkening, so the campfire throws a dithered pool of light.
  * OBJ_EVENT_GFX_LIGHT_SPRITE halos over the fire and lamps (night only)
  * tileset animation for the flames and the river surface
  * metatile layer types for depth (tent ridge drawn over Goku when he walks behind it)

Run from the repo root:  python3 tools/pbo/build_cove.py
It is idempotent: the tileset is rebuilt from Petalburg every run and the two layouts are rebuilt
from their v0.3 blockdata (kept in tools/pbo/cove_src/).
"""
import json, os, re, shutil, struct, sys
import numpy as np
from PIL import Image

sys.path.insert(0, os.path.dirname(__file__))
import mapkit

ROOT = os.getcwd()
SRC_TS = 'data/tilesets/secondary/petalburg'
DST_TS = 'data/tilesets/secondary/pbo_cove'
KEEP = os.path.join(ROOT, 'tools/pbo/cove_src')

# ----------------------------------------------------------------------------------------------- palettes
G = [(168, 208, 56), (120, 168, 32), (88, 128, 16), (48, 80, 8)]          # General grass (pal 2, idx 12-15)
P7 = [(24, 41, 82)] + G + [
    (216, 176, 112), (184, 140, 88), (136, 96, 56), (88, 56, 32),          # 5-8 dirt hi/mid/lo, bank dark
    (200, 232, 248), (104, 160, 232), (56, 112, 208), (40, 72, 168),       # 9-12 foam, water light/mid/deep
    (152, 152, 168), (88, 88, 112), (40, 32, 24)]                          # 13-14 rock, 15 outline
P11 = [(232, 152, 72)] + G + G + [
    (192, 56, 24), (240, 136, 40), (248, 208, 80), (255, 248, 216),        # 9-12 fire (light-blended)
    (168, 160, 152), (96, 88, 88), (40, 28, 24)]                           # 13-14 stone/metal, 15 dark
P11_LIGHT = [0, 5, 6, 7, 8, 9, 10, 11, 12]
P12 = [(24, 41, 82),
       (232, 220, 176), (200, 180, 136), (152, 128, 96), (56, 40, 40),     # 1-4 canvas hi/mid/lo, opening
       (128, 88, 48),                                                      # 5 rope / pole
       (192, 136, 80), (152, 100, 56), (104, 64, 32), (72, 44, 24),        # 6-9 wood hi/mid/lo, bark dark
       (216, 176, 120),                                                    # 10 log rings
       (120, 120, 136), (64, 64, 80),                                      # 11-12 metal
       (72, 112, 16), (48, 80, 8),                                         # 13-14 (unused greens)
       (40, 28, 20)]                                                       # 15 outline
PAL_TERRAIN, PAL_LIGHT, PAL_PROPS = 7, 11, 12

GRASS_MAP = {12: 1, 13: 2, 14: 3, 15: 4}

BAYER = np.array([[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]) / 16.0


def write_pal(path, cols):
    with open(path, 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n')
        for c in cols:
            f.write('%d %d %d\n' % c)


# ----------------------------------------------------------------------------------------------- textures
GEN = mapkit.Tileset('data/tilesets/primary/general')


def grass_tex():
    t2 = np.vectorize(GRASS_MAP.get)(GEN.tiles[2])
    t3 = np.vectorize(GRASS_MAP.get)(GEN.tiles[3])
    return np.block([[t2, t3], [t3, t2]]).astype(np.uint8)


def periodic_noise(seed, n=16):
    r = np.random.RandomState(seed)
    return r.rand(n, n)


def dirt_tex():
    n = periodic_noise(7)
    t = np.full((16, 16), 6, np.uint8)
    t[n > 0.78] = 5
    t[n < 0.16] = 7
    # a few pebbles
    for (x, y) in ((3, 4), (11, 2), (7, 11), (14, 13)):
        t[y, x] = 8
        t[y - 1, x] = 5
    return t


def water_tex(frame):
    t = np.full((16, 16), 11, np.uint8)
    n = periodic_noise(3)
    t[n < 0.30] = 12
    # ripples: short light dashes that drift right one pixel per frame
    for (x, y, l) in ((1, 2, 4), (9, 5, 3), (4, 9, 5), (12, 12, 3), (6, 14, 2)):
        for k in range(l):
            t[y, (x + k + frame) % 16] = 10
    t[(5 + frame * 3) % 16, (13 + frame) % 16] = 9
    return t


def tile_tex(tex, w, h):
    return np.tile(tex, (h // 16 + 1, w // 16 + 1))[:h, :w]


# ----------------------------------------------------------------------------------------------- canvas helpers
def ellipse_mask(w, h, cx, cy, rx, ry):
    yy, xx = np.mgrid[0:h, 0:w]
    return ((xx + 0.5 - cx) / rx) ** 2 + ((yy + 0.5 - cy) / ry) ** 2 <= 1.0


def line(c, x0, y0, x1, y1, v):
    n = max(abs(x1 - x0), abs(y1 - y0)) + 1
    for i in range(n):
        x = round(x0 + (x1 - x0) * i / max(n - 1, 1))
        y = round(y0 + (y1 - y0) * i / max(n - 1, 1))
        if 0 <= y < c.shape[0] and 0 <= x < c.shape[1]:
            c[y, x] = v


def outline(c, v=15):
    """1px outline around non-zero pixels (on the transparent side)."""
    m = c > 0
    o = np.zeros_like(m)
    o[1:, :] |= m[:-1, :]
    o[:-1, :] |= m[1:, :]
    o[:, 1:] |= m[:, :-1]
    o[:, :-1] |= m[:, 1:]
    c[o & ~m] = v


def poly_mask(w, h, pts):
    yy, xx = np.mgrid[0:h, 0:w]
    px, py = xx + 0.5, yy + 0.5
    inside = np.zeros((h, w), bool)
    n = len(pts)
    for i in range(n):
        x0, y0 = pts[i]
        x1, y1 = pts[(i + 1) % n]
        cond = ((y0 > py) != (y1 > py))
        xint = (x1 - x0) * (py - y0) / ((y1 - y0) if y1 != y0 else 1e-9) + x0
        inside ^= cond & (px < xint)
    return inside


# ----------------------------------------------------------------------------------------------- props (layer 1)
def art_tent():
    """48x48, palette 12. Rows: 0 ridge (walk-behind), 1-2 solid."""
    W = H = 48
    c = np.zeros((H, W), np.uint8)
    outer = poly_mask(W, H, [(24, 5), (45, 44), (3, 44)])
    front = poly_mask(W, H, [(24, 8), (39, 44), (9, 44)])
    c[outer] = 3
    c[front] = 2
    # light from the fire (bottom right) on the right half of the front face
    yy, xx = np.mgrid[0:H, 0:W]
    c[front & (xx > 24) & ((xx + yy) % 5 != 0)] = 1
    door = poly_mask(W, H, [(24, 23), (31, 44), (17, 44)])
    c[door] = 4
    line(c, 24, 23, 17, 44, 1)      # flap edges catch the light
    line(c, 24, 23, 31, 44, 2)
    line(c, 24, 2, 24, 7, 5)        # ridge pole tip
    line(c, 24, 8, 24, 22, 3)       # seam
    for x0, x1 in ((3, 0), (45, 47)):
        line(c, x0, 30, x1, 44, 5)  # guy ropes
    c[44:46, 0:2] = 9
    c[44:46, 46:48] = 9
    c[45, 3:46][c[45, 3:46] == 0] = 0
    outline(c, 15)
    return c


def art_crate():
    c = np.zeros((16, 16), np.uint8)
    c[4:15, 2:14] = 7
    c[4:7, 2:14] = 6                # lid catches light
    for y in (8, 11):
        c[y, 2:14] = 8
    line(c, 3, 7, 12, 14, 8)
    line(c, 12, 7, 3, 14, 8)
    c[4:15, 2] = 8
    c[4:15, 13] = 9
    outline(c, 15)
    return c


def art_log():
    c = np.zeros((16, 32), np.uint8)
    body = ellipse_mask(32, 16, 16, 9.5, 14.5, 4.6)
    c[body] = 7
    c[(body) & (np.mgrid[0:16, 0:32][0] < 8)] = 6
    c[(body) & (np.mgrid[0:16, 0:32][0] > 11)] = 8
    for x in (9, 15, 21, 26):
        c[7:12, x][body[7:12, x]] = 9
    end = ellipse_mask(32, 16, 4.5, 9.5, 3.2, 4.4)
    c[end] = 10
    c[ellipse_mask(32, 16, 4.5, 9.5, 1.6, 2.2)] = 6
    c[9, 4] = 8
    outline(c, 15)
    return c


def art_fire(frame):
    """16x16 layer-1 campfire, palette 11 (9-12 light-blended)."""
    c = np.zeros((16, 16), np.uint8)
    # stone ring
    for (x, y) in ((2, 12), (5, 14), (9, 14), (12, 13), (13, 10), (3, 9)):
        c[ellipse_mask(16, 16, x + 0.5, y + 0.5, 1.8, 1.3)] = 14
        c[y - 1, x] = 13
    # charred logs crossing
    line(c, 4, 12, 11, 9, 15)
    line(c, 4, 9, 11, 12, 15)
    # flames: three tongues that sway with the frame
    sway = [0, 1, 0, -1][frame]
    tall = [0, -1, 1, 0][frame]
    for (bx, h, s) in ((6, 7 + tall, sway), (8, 9 - tall, -sway), (10, 6 + tall, sway)):
        for k in range(h):
            y = 11 - k
            w = max(0, 2 - k // 3)
            x = bx + (s if k > h // 2 else 0)
            col = 9 if k >= h - 2 else (10 if k >= h // 2 else 11)
            c[y, max(0, x - w):x + w + 1] = col
    c[ellipse_mask(16, 16, 8, 10, 2.6, 1.8)] = 11
    c[ellipse_mask(16, 16, 8, 10.5, 1.2, 0.9)] = 12
    # a spark
    sx, sy = [(5, 2), (11, 1), (7, 3), (10, 2)][frame]
    c[sy, sx] = 11
    return c


def art_lamp():
    """16x32: head (palette 11, light-blended glass) on top of a post (palette 12)."""
    head = np.zeros((16, 16), np.uint8)
    head[4:12, 5:11] = 14
    head[5:11, 6:10] = 11
    head[6:9, 7:9] = 12
    head[3, 4:12] = 15
    head[2, 6:10] = 15
    head[12:16, 7:9] = 14
    outline(head, 15)
    post = np.zeros((16, 16), np.uint8)
    post[0:13, 7:9] = 11
    post[0:13, 8] = 12
    post[12:15, 5:11] = 12
    post[12, 5:11] = 11
    outline(post, 15)
    return head, post


# ----------------------------------------------------------------------------------------------- metatile builder
class Builder:
    def __init__(self):
        if os.path.exists(DST_TS):
            shutil.rmtree(DST_TS)
        shutil.copytree(SRC_TS, DST_TS)
        im = Image.open(os.path.join(DST_TS, 'tiles.png'))
        self.png_pal = im.getpalette()
        a = np.array(im)
        self.tiles = []
        for ty in range(a.shape[0] // 8):
            for tx in range(a.shape[1] // 8):
                self.tiles.append(a[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8].copy())
        self.base_tiles = 159   # Petalburg's real count (-num_tiles 159); the rest of the sheet is padding
        self.tiles = self.tiles[:self.base_tiles]
        self.tile_index = {}
        mt = open(os.path.join(DST_TS, 'metatiles.bin'), 'rb').read()
        self.metatiles = [list(struct.unpack('<8H', mt[i:i + 16])) for i in range(0, len(mt), 16)]
        at = open(os.path.join(DST_TS, 'metatile_attributes.bin'), 'rb').read()
        self.attrs = list(struct.unpack('<%dH' % (len(at) // 2), at))
        self.mt_index = {}
        self.named = {}

    def tile(self, arr, pal, unique=False):
        """Add an 8x8 tile (dedupe incl. flips); returns the tile entry bits (index | flips | palette)."""
        arr = np.asarray(arr, np.uint8)
        if not arr.any():
            return 0 | (pal << 12) if False else 0
        if not unique:
            for flip, a in ((0, arr), (0x400, arr[:, ::-1]), (0x800, arr[::-1, :]), (0xC00, arr[::-1, ::-1])):
                k = a.tobytes()
                if k in self.tile_index:
                    return (512 + self.tile_index[k]) | flip | (pal << 12)
        self.tiles.append(arr.copy())
        idx = len(self.tiles) - 1
        if not unique:
            self.tile_index[arr.tobytes()] = idx
        return (512 + idx) | (pal << 12)

    def metatile(self, l0, p0, l1=None, p1=0, behavior=0, layer=0, name=None, unique=False):
        """l0/l1: 16x16 index arrays (l1 may be None). layer: 0 normal, 1 covered, 2 split."""
        e = []
        for (lay, p) in ((l0, p0), (l1, p1)):
            for k in range(4):
                if lay is None:
                    e.append(0)
                    continue
                ox, oy = (k & 1) * 8, (k >> 1) * 8
                e.append(self.tile(lay[oy:oy + 8, ox:ox + 8], p, unique))
        attr = (behavior & 0xFF) | (layer << 12)
        key = (tuple(e), attr)
        if key in self.mt_index and not unique:
            mid = self.mt_index[key]
        else:
            self.metatiles.append(e)
            self.attrs.append(attr)
            mid = 512 + len(self.metatiles) - 1
            self.mt_index[key] = mid
        if name:
            self.named[name] = mid
        return mid

    def save(self):
        n = len(self.tiles)
        assert n <= 512, f'too many tiles: {n}'
        assert len(self.metatiles) <= 512, f'too many metatiles: {len(self.metatiles)}'
        rows = (n + 15) // 16
        a = np.zeros((rows * 8, 128), np.uint8)
        for i, t in enumerate(self.tiles):
            a[(i // 16) * 8:(i // 16) * 8 + 8, (i % 16) * 8:(i % 16) * 8 + 8] = t
        im = Image.fromarray(a, 'P')
        im.putpalette(self.png_pal)
        im.save(os.path.join(DST_TS, 'tiles.png'))
        with open(os.path.join(DST_TS, 'metatiles.bin'), 'wb') as f:
            for e in self.metatiles:
                f.write(struct.pack('<8H', *e))
        with open(os.path.join(DST_TS, 'metatile_attributes.bin'), 'wb') as f:
            f.write(struct.pack('<%dH' % len(self.attrs), *self.attrs))
        write_pal(os.path.join(DST_TS, 'palettes/%02d.pal' % PAL_TERRAIN), P7)
        write_pal(os.path.join(DST_TS, 'palettes/%02d.pal' % PAL_LIGHT), P11)
        write_pal(os.path.join(DST_TS, 'palettes/%02d.pal' % PAL_PROPS), P12)
        with open(os.path.join(DST_TS, 'palettes/%02d.pla' % PAL_LIGHT), 'w') as f:
            f.write('# PokeBall Orange: campfire + lit-grass colours glow at night\n')
            for i in P11_LIGHT:
                f.write('%d\n' % i)
        return n


MB_NORMAL, MB_POND_WATER = 0x00, 0x10
# original metatiles that may be repainted (plain grass and the light grass patches)
BAKEABLE = {0x001, 0x1d0, 0x1d1, 0x1d2, 0x1d8, 0x1d9, 0x1da, 0x1e0, 0x1e1, 0x1e2}
LAYER_NORMAL, LAYER_COVERED = 0, 1
FIRE_TILE_SLOTS = []   # tile indices (in tileset) of the fire's 4 layer-1 tiles, for animation
WATER_TILE_SLOTS = []


# ----------------------------------------------------------------------------------------------- map painting
class MapPaint:
    """Paints a W x H map: per cell either keep the original metatile, or bake terrain."""

    def __init__(self, b, lid, src_bin):
        self.b = b
        self.L = mapkit.Layout(lid)
        self.L.blocks = list(struct.unpack('<%dH' % (len(src_bin) // 2), src_bin))
        self.W, self.H = self.L.w, self.L.h
        pw, ph = self.W * 16, self.H * 16
        self.grass = tile_tex(grass_tex(), pw, ph)
        self.path = np.zeros((ph, pw), bool)
        self.water = np.zeros((ph, pw), bool)
        self.shadow = np.zeros((ph, pw), np.uint8)
        self.lit = np.zeros((ph, pw), bool)
        self.rocks = []
        self.cells = {}          # (x,y) -> 'bake' | 'keep' | ('prop', ...)
        self.props = {}          # (x,y) -> (layer1 16x16, pal, behavior, layer type, collision)
        self.glow_cells = set()

    # -- semantic painting in cell units
    def path_cells(self, cells, r=5.0):
        """Organic dirt path through a list of cells (x,y)."""
        pw, ph = self.W * 16, self.H * 16
        yy, xx = np.mgrid[0:ph, 0:pw]
        n = np.tile(periodic_noise(11, 32), (ph // 32 + 1, pw // 32 + 1))[:ph, :pw]
        m = np.zeros((ph, pw), bool)
        for (x, y) in cells:
            cx, cy = x * 16 + 8, y * 16 + 8
            m |= (np.abs(xx + 0.5 - cx) <= 8 + 1.5 * n) & (np.abs(yy + 0.5 - cy) <= 8 + 1.5 * n)
        # round the outside corners a little
        self.path |= m

    def water_cells(self, cells):
        pw, ph = self.W * 16, self.H * 16
        yy, xx = np.mgrid[0:ph, 0:pw]
        n = np.tile(periodic_noise(5, 32), (ph // 32 + 1, pw // 32 + 1))[:ph, :pw]
        cs = set(cells)
        for (x, y) in cells:
            x0, y0 = x * 16, y * 16
            # inset 3px from sides that border land, so the bank lives inside the water cell
            l = 0 if (x - 1, y) in cs or x == 0 else 3
            r = 16 if (x + 1, y) in cs or x == self.W - 1 else 13
            t = 0 if (x, y - 1) in cs or y == 0 else 5
            bt = 16 if (x, y + 1) in cs or y == self.H - 1 else 13
            sub = (xx >= x0 + l - n * 1.5) & (xx < x0 + r + n * 1.5) & (yy >= y0 + t - n * 1.2) & (yy < y0 + bt + n * 1.2)
            sub &= (xx >= x0) & (xx < x0 + 16) & (yy >= y0) & (yy < y0 + 16)
            self.water |= sub
        for c in cells:
            self.cells[c] = 'bake'

    def glow(self, fx, fy, r0=20, r1=40):
        pw, ph = self.W * 16, self.H * 16
        yy, xx = np.mgrid[0:ph, 0:pw]
        d = np.hypot(xx + 0.5 - fx, (yy + 0.5 - fy) * 1.15)
        t = np.clip((d - r0) / (r1 - r0), 0, 1)
        thr = np.tile(BAYER, (ph // 4 + 1, pw // 4 + 1))[:ph, :pw]
        self.lit |= (d < r0) | ((d < r1) & (thr >= t))
        for y in range(self.H):
            for x in range(self.W):
                if self.lit[y * 16:y * 16 + 16, x * 16:x * 16 + 16].any():
                    self.glow_cells.add((x, y))

    def shadow_ellipse(self, cx, cy, rx, ry, strength=1):
        pw, ph = self.W * 16, self.H * 16
        m = ellipse_mask(pw, ph, cx, cy, rx, ry)
        self.shadow[m] = np.maximum(self.shadow[m], strength)

    def prop(self, x, y, art, pal, collision=1, layer=LAYER_COVERED, behavior=MB_NORMAL, name=None):
        self.props[(x, y)] = (art, pal, behavior, layer, collision, name)
        self.cells[(x, y)] = 'bake'

    def bake(self, x, y):
        self.cells[(x, y)] = 'bake'

    # -- rendering
    def terrain_cell(self, x, y):
        """Returns (16x16 indices, palette, behavior) for the baked ground of a cell."""
        sl = (slice(y * 16, y * 16 + 16), slice(x * 16, x * 16 + 16))
        g = self.grass[sl].copy()
        pth = self.path[sl]
        wat = self.water[sl]
        sh = self.shadow[sl]
        if (x, y) in self.glow_cells and not wat.any() and not pth.any():
            out = g.copy()
            out = np.minimum(out + sh, 4)
            lit = self.lit[sl]
            out[lit] += 4
            return out.astype(np.uint8), PAL_LIGHT, MB_NORMAL
        out = g.copy()
        out = np.minimum(out + sh, 4)
        # dirt path with darker rim and grass tufts nibbling the edge
        if pth.any():
            dt = tile_tex(dirt_tex(), 16, 16)
            out[pth] = dt[pth]
            P = np.pad(self.path, 1, mode='edge')[y * 16:y * 16 + 18, x * 16:x * 16 + 18]
            edge_in = pth & ~(P[:-2, 1:-1] & P[2:, 1:-1] & P[1:-1, :-2] & P[1:-1, 2:])
            out[edge_in] = 7
            edge_out = ~pth & (P[:-2, 1:-1] | P[2:, 1:-1] | P[1:-1, :-2] | P[1:-1, 2:])
            out[edge_out & (out < 3)] = 3
        if wat.any():
            Wp = np.pad(self.water, 4, mode='edge')
            yy0, xx0 = y * 16 + 4, x * 16 + 4
            wt = tile_tex(water_tex(0), 16, 16)
            for py in range(16):
                for px in range(16):
                    gy, gx = yy0 + py, xx0 + px
                    if wat[py, px]:
                        # water: foam/light right at the bank
                        near = not (Wp[gy - 1, gx] and Wp[gy + 1, gx] and Wp[gy, gx - 1] and Wp[gy, gx + 1])
                        near2 = not (Wp[gy - 2, gx] and Wp[gy, gx - 2] and Wp[gy, gx + 2])
                        out[py, px] = 9 if near else (10 if near2 else wt[py, px])
                    else:
                        # land next to water: dirt face where the water lies below (we look down the drop)
                        if Wp[gy + 1, gx]:
                            out[py, px] = 8
                        elif Wp[gy + 2, gx]:
                            out[py, px] = 7
                        elif Wp[gy + 3, gx]:
                            out[py, px] = 15 if out[py, px] == 4 else 6
                        elif Wp[gy - 1, gx] or Wp[gy, gx - 1] or Wp[gy, gx + 1]:
                            out[py, px] = 15
            for (rx, ry, rr) in self.rocks:
                m = ellipse_mask(16, 16, rx - x * 16, ry - y * 16, rr, rr * 0.75)
                out[m] = 14
                m2 = ellipse_mask(16, 16, rx - x * 16 - rr * 0.3, ry - y * 16 - rr * 0.3, rr * 0.5, rr * 0.4)
                out[m2] = 13
        beh = MB_POND_WATER if wat.mean() >= 0.5 else MB_NORMAL
        return out.astype(np.uint8), PAL_TERRAIN, beh

    def apply(self):
        b = self.b
        for y in range(self.H):
            for x in range(self.W):
                c = (x, y)
                forced = self.cells.get(c) == 'bake' or c in self.props
                soft = c in self.glow_cells or self.path[y * 16:y * 16 + 16, x * 16:x * 16 + 16].any() \
                    or self.shadow[y * 16:y * 16 + 16, x * 16:x * 16 + 16].any()
                if not forced and not (soft and self.L.get(x, y)[0] in BAKEABLE):
                    continue
                l0, p0, beh = self.terrain_cell(x, y)
                coll, elev, layer, l1, p1, name = 0, 3, LAYER_NORMAL, None, 0, None
                if beh == MB_POND_WATER:
                    elev = 1
                if c in self.props:
                    l1, p1, pbeh, layer, coll, name = self.props[c]
                    if coll:
                        elev = 0
                if name == 'fire':
                    # the flame tiles are animated, so they get their own slots (layer 0 shared as usual)
                    e0 = [b.tile(l0[(k >> 1) * 8:(k >> 1) * 8 + 8, (k & 1) * 8:(k & 1) * 8 + 8], p0) for k in range(4)]
                    e1 = [b.tile(np.full((8, 8), 15, np.uint8) * 0 + l1[(k >> 1) * 8:(k >> 1) * 8 + 8, (k & 1) * 8:(k & 1) * 8 + 8] | (0 if l1[(k >> 1) * 8:(k >> 1) * 8 + 8, (k & 1) * 8:(k & 1) * 8 + 8].any() else 0), p1, unique=True) for k in range(4)]
                    b.metatiles.append(e0 + e1)
                    b.attrs.append(beh | (layer << 12))
                    mid = 512 + len(b.metatiles) - 1
                    b.named['fire'] = mid
                else:
                    mid = b.metatile(l0, p0, l1, p1, beh, layer, name=name)
                self.L.set(x, y, mid, coll, elev)
        self.L.save()


# ----------------------------------------------------------------------------------------------- the maps
def littleroot(b, src):
    M = MapPaint(b, 'LAYOUT_LITTLEROOT_TOWN', src)
    # river: comes in from Route 101 down the east edge and opens into a pool beside the camp,
    # then slips away under the trees in the south-east corner
    river = [(x, y) for y in range(0, 11) for x in (18, 19)]
    river += [(x, y) for y in range(11, 15) for x in (17, 18, 19)]
    M.water_cells(river)
    M.rocks += [(18 * 16 + 6, 12 * 16 + 9, 3.0), (17 * 16 + 8, 14 * 16 + 3, 2.2), (19 * 16 + 4, 6 * 16 + 8, 2.4)]
    # main road from the north gate to a little plaza, spurs to each door, the lab and the camp
    road = [(x, y) for y in range(0, 9) for x in (10, 11)]
    plaza = [(x, y) for y in range(9, 12) for x in range(7, 15)]
    spurs = [(5, 9), (6, 9)]
    lab = [(10, y) for y in range(12, 18)] + [(x, 17) for x in range(7, 10)]
    camp = [(11, 15)]
    M.path_cells(road + plaza + spurs + lab + camp)
    # campfire, tent, log, crate
    fx, fy = 14, 16
    M.glow(fx * 16 + 8, fy * 16 + 10, 20, 40)
    tent = art_tent()
    for ty in range(3):
        for tx in range(3):
            part = tent[ty * 16:ty * 16 + 16, tx * 16:tx * 16 + 16]
            if ty == 0:
                M.prop(13 + tx, 12, part, PAL_PROPS, collision=0, layer=LAYER_NORMAL)
            else:
                M.prop(13 + tx, 12 + ty, part, PAL_PROPS, collision=1, layer=LAYER_COVERED)
    M.shadow_ellipse(14 * 16 + 10, 15 * 16 + 1, 26, 5)
    M.prop(fx, fy, art_fire(0), PAL_LIGHT, collision=1, name='fire')
    log = art_log()
    M.prop(12, 16, log[:, :16], PAL_PROPS)
    M.prop(13, 16, log[:, 16:], PAL_PROPS)
    M.shadow_ellipse(13 * 16, 17 * 16 + 1, 15, 3)
    M.prop(16, 13, art_crate(), PAL_PROPS)
    M.shadow_ellipse(16 * 16 + 9, 14 * 16 + 1, 8, 2.5)
    # lamps by the plaza
    head, post = art_lamp()
    for (lx, ly) in ((9, 8), (6, 11)):
        M.prop(lx, ly - 1, head, PAL_LIGHT, collision=0, layer=LAYER_NORMAL)
        M.prop(lx, ly, post, PAL_PROPS, collision=1)
        M.shadow_ellipse(lx * 16 + 10, ly * 16 + 15, 6, 2)
    sign_mt = M.L.get(15, 13)[0]
    M.apply()
    # the town sign moves from the camp clearing to the north gate (same metatile)
    M.L.set(12, 2, sign_mt, 1, 0)
    M.L.save()
    return M


def route101(b, src):
    M = MapPaint(b, 'LAYOUT_ROUTE101', src)
    # the old light-grass patches are dropped: the worn path now gives the meadow its structure
    for y in range(M.H):
        for x in range(M.W):
            m, c, e = M.L.get(x, y)
            if m in (0x1d0, 0x1d1, 0x1d2, 0x1d8, 0x1d9, 0x1da, 0x1e0, 0x1e1, 0x1e2):
                M.L.set(x, y, 0x001, c, e)
    river = [(x, y) for y in range(7, 20) for x in (18, 19)] + [(17, 7), (17, 8)]
    M.water_cells(river)
    M.rocks += [(18 * 16 + 9, 9 * 16 + 6, 2.6), (19 * 16 + 3, 15 * 16 + 10, 2.4)]
    # worn path along the way people actually walk: gate -> pocket -> clearing -> (tall grass) -> north gate
    path = [(x, y) for y in range(14, 20) for x in (10, 11)]
    path += [(7, 14), (8, 14), (9, 14), (7, 13), (7, 12), (7, 11), (7, 10), (8, 10), (9, 10), (10, 10)]
    path += [(9, y) for y in range(0, 6)] + [(10, 0), (10, 1), (10, 4), (11, 4)]
    M.path_cells(path)
    M.apply()
    return M


def main():
    os.makedirs(KEEP, exist_ok=True)
    srcs = {}
    for lid, name in (('LAYOUT_LITTLEROOT_TOWN', 'LittlerootTown'), ('LAYOUT_ROUTE101', 'Route101')):
        keep = os.path.join(KEEP, name + '.bin')
        if not os.path.exists(keep):
            shutil.copy(os.path.join(ROOT, 'data/layouts', name, 'map.bin'), keep)
        srcs[lid] = open(keep, 'rb').read()
    b = Builder()
    littleroot(b, srcs['LAYOUT_LITTLEROOT_TOWN'])
    route101(b, srcs['LAYOUT_ROUTE101'])
    # animation slots: the fire metatile's layer-1 tiles, the water interior tiles
    fire = b.metatiles[b.named['fire'] - 512]
    fire_tiles = [(v & 0x3FF) - 512 for v in fire[4:8]]
    wt = water_tex(0)
    water_tiles = []
    for k in range(4):
        ox, oy = (k & 1) * 8, (k >> 1) * 8
        key = wt[oy:oy + 8, ox:ox + 8].astype(np.uint8).tobytes()
        water_tiles.append(b.tile_index.get(key))
    n = b.save()
    # animation frames: fire (4 tiles each frame), water (4 tiles each frame)
    os.makedirs(os.path.join(DST_TS, 'anim/fire'), exist_ok=True)
    os.makedirs(os.path.join(DST_TS, 'anim/water'), exist_ok=True)
    for f in range(4):
        fa = art_fire(f)
        strip = np.zeros((8, 32), np.uint8)
        for k in range(4):
            ox, oy = (k & 1) * 8, (k >> 1) * 8
            strip[:, k * 8:k * 8 + 8] = fa[oy:oy + 8, ox:ox + 8]
        im = Image.fromarray(strip, 'P'); im.putpalette(b.png_pal)
        im.save(os.path.join(DST_TS, 'anim/fire/%d.png' % f))
        wa = water_tex(f)
        strip = np.zeros((8, 32), np.uint8)
        for k in range(4):
            ox, oy = (k & 1) * 8, (k >> 1) * 8
            strip[:, k * 8:k * 8 + 8] = wa[oy:oy + 8, ox:ox + 8]
        im = Image.fromarray(strip, 'P'); im.putpalette(b.png_pal)
        im.save(os.path.join(DST_TS, 'anim/water/%d.png' % f))
    info = {'tiles': n, 'metatiles': len(b.metatiles), 'fire_tiles': fire_tiles, 'water_tiles': water_tiles,
            'named': b.named}
    json.dump(info, open(os.path.join(KEEP, 'cove_info.json'), 'w'), indent=1)
    with open(os.path.join(ROOT, 'src/data/tilesets/pbo_cove_anim.h'), 'w') as f:
        f.write('// generated by tools/pbo/build_cove.py: tile slots animated in gTileset_PboCove\n')
        f.write('#define PBO_COVE_NUM_TILES %d\n' % n)
        f.write('#define PBO_COVE_FIRE_TILE %d\n' % fire_tiles[0])
        assert fire_tiles == list(range(fire_tiles[0], fire_tiles[0] + 4))
        for k, t in enumerate(water_tiles):
            f.write('#define PBO_COVE_WATER_TILE_%d %d\n' % (k, t))
    g = open(os.path.join(ROOT, 'src/data/tilesets/graphics.h')).read()
    g = re.sub(r'(pbo_cove/tiles.png", ".4bpp.fastSmol", "-num_tiles )\d+', lambda m: m.group(1) + str(n), g)
    open(os.path.join(ROOT, 'src/data/tilesets/graphics.h'), 'w').write(g)
    print(json.dumps(info))


if __name__ == '__main__':
    main()
