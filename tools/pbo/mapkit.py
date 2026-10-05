#!/usr/bin/env python3
"""PokeBall Orange map kit: load layouts + tilesets, render maps/metatile sheets, edit blockdata.

    python3 tools/pbo/mapkit.py render LAYOUT_ID out.png [--grid] [--ids] [--coll] [--events MAP_NAME]
    python3 tools/pbo/mapkit.py sheet PRIMARY_DIR SECONDARY_DIR out.png [--from N --to M]

Library use: Layout(id).get(x, y) / set(x, y, metatile, collision, elevation) / save()
Run from the repo root.
"""
import json, os, struct, sys
import numpy as np
from PIL import Image, ImageDraw

ROOT = os.getcwd()
NUM_PRIMARY_TILES = 512
NUM_PRIMARY_METATILES = 512


def tileset_dir(symbol):
    """gTileset_Petalburg -> data/tilesets/secondary/petalburg (via the tiles INCGFX path)."""
    hdr = open(os.path.join(ROOT, 'src/data/tilesets/headers.h')).read()
    i = hdr.index('const struct Tileset ' + symbol + ' =')
    block = hdr[i:hdr.index('};', i)]
    tiles_sym = block.split('.tiles = ')[1].split(',')[0].strip()
    gfx = open(os.path.join(ROOT, 'src/data/tilesets/graphics.h')).read() + open(os.path.join(ROOT, 'src/graphics.c')).read()
    j = gfx.index(tiles_sym + '[]')
    line = gfx[j:gfx.index(';', j)]
    return os.path.dirname(line.split('"')[1])


def load_pal(path):
    lines = open(path).read().replace('\r', '').split('\n')
    n = int(lines[2])
    return [tuple(int(v) for v in l.split()[:3]) for l in lines[3:3 + n]]


class Tileset:
    def __init__(self, d):
        self.dir = d
        im = Image.open(os.path.join(ROOT, d, 'tiles.png'))
        a = np.array(im)
        if a.ndim == 3:
            raise SystemExit(f'{d}/tiles.png is not indexed')
        self.tiles = []
        h, w = a.shape
        for ty in range(h // 8):
            for tx in range(w // 8):
                self.tiles.append(a[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8] & 15)
        mt = open(os.path.join(ROOT, d, 'metatiles.bin'), 'rb').read()
        self.metatiles = [struct.unpack('<8H', mt[i:i + 16]) for i in range(0, len(mt), 16)]
        at = open(os.path.join(ROOT, d, 'metatile_attributes.bin'), 'rb').read()
        self.attrs = list(struct.unpack('<%dH' % (len(at) // 2), at))
        self.pals = []
        for i in range(16):
            p = os.path.join(ROOT, d, 'palettes', '%02d.pal' % i)
            self.pals.append(load_pal(p) if os.path.exists(p) else [(255, 0, 255)] * 16)


class Pair:
    """A primary+secondary tileset pair as the game sees it."""
    def __init__(self, prim_dir, sec_dir):
        self.p = Tileset(prim_dir)
        self.s = Tileset(sec_dir)
        self.pals = self.p.pals[:6] + self.s.pals[6:13] + [[(0, 0, 0)] * 16] * 3
        self._cache = {}

    def tile(self, idx):
        if idx < NUM_PRIMARY_TILES:
            return self.p.tiles[idx] if idx < len(self.p.tiles) else np.zeros((8, 8), np.uint8)
        idx -= NUM_PRIMARY_TILES
        return self.s.tiles[idx] if idx < len(self.s.tiles) else np.zeros((8, 8), np.uint8)

    def metatile_entries(self, mid):
        if mid < NUM_PRIMARY_METATILES:
            return self.p.metatiles[mid] if mid < len(self.p.metatiles) else (0,) * 8
        mid -= NUM_PRIMARY_METATILES
        return self.s.metatiles[mid] if mid < len(self.s.metatiles) else (0,) * 8

    def attr(self, mid):
        if mid < NUM_PRIMARY_METATILES:
            return self.p.attrs[mid] if mid < len(self.p.attrs) else 0
        mid -= NUM_PRIMARY_METATILES
        return self.s.attrs[mid] if mid < len(self.s.attrs) else 0

    def render_metatile(self, mid):
        if mid in self._cache:
            return self._cache[mid]
        out = np.zeros((16, 16, 3), np.uint8)
        e = self.metatile_entries(mid)
        bg = self.pals[0][0]
        out[:, :] = bg
        for layer in range(2):
            for k in range(4):
                v = e[layer * 4 + k]
                t = self.tile(v & 0x3FF)
                if v & 0x400: t = t[:, ::-1]
                if v & 0x800: t = t[::-1, :]
                pal = self.pals[(v >> 12) & 15]
                ox, oy = (k & 1) * 8, (k >> 1) * 8
                for yy in range(8):
                    for xx in range(8):
                        c = t[yy, xx]
                        if c or layer == 0 and False:
                            out[oy + yy, ox + xx] = pal[c]
                        elif layer == 0 and not c:
                            pass
        self._cache[mid] = out
        return out


def layouts():
    return {l['id']: l for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts'] if 'id' in l}


class Layout:
    def __init__(self, lid):
        self.info = layouts()[lid]
        self.w, self.h = self.info['width'], self.info['height']
        self.path = os.path.join(ROOT, self.info['blockdata_filepath'])
        d = open(self.path, 'rb').read()
        self.blocks = list(struct.unpack('<%dH' % (len(d) // 2), d))
        self.pair = Pair(tileset_dir(self.info['primary_tileset']), tileset_dir(self.info['secondary_tileset']))

    def get(self, x, y):
        v = self.blocks[y * self.w + x]
        return v & 0x3FF, (v >> 10) & 3, v >> 12

    def set(self, x, y, mid, coll=None, elev=None):
        om, oc, oe = self.get(x, y)
        coll = oc if coll is None else coll
        elev = oe if elev is None else elev
        self.blocks[y * self.w + x] = (mid & 0x3FF) | (coll << 10) | (elev << 12)

    def save(self):
        open(self.path, 'wb').write(struct.pack('<%dH' % len(self.blocks), *self.blocks))

    def render(self, grid=False, ids=False, coll=False, scale=1):
        img = np.zeros((self.h * 16, self.w * 16, 3), np.uint8)
        for y in range(self.h):
            for x in range(self.w):
                img[y * 16:y * 16 + 16, x * 16:x * 16 + 16] = self.pair.render_metatile(self.get(x, y)[0])
        im = Image.fromarray(img)
        if scale != 1:
            im = im.resize((im.width * scale, im.height * scale), Image.NEAREST)
        d = ImageDraw.Draw(im)
        s = 16 * scale
        for y in range(self.h):
            for x in range(self.w):
                m, c, e = self.get(x, y)
                if coll and c:
                    d.rectangle((x * s, y * s, x * s + s - 1, y * s + s - 1), outline=(255, 0, 0))
                if ids:
                    d.text((x * s + 1, y * s + 1), '%x' % m, fill=(255, 255, 0))
                if grid:
                    d.rectangle((x * s, y * s, x * s + s, y * s + s), outline=(0, 0, 0))
        return im


def sheet(pair, lo, hi, cols=16, scale=2):
    n = hi - lo
    rows = (n + cols - 1) // cols
    im = Image.new('RGB', (cols * 18 * scale, rows * 18 * scale), (40, 40, 60))
    d = ImageDraw.Draw(im)
    for k in range(n):
        m = lo + k
        t = Image.fromarray(pair.render_metatile(m)).resize((16 * scale, 16 * scale), Image.NEAREST)
        x, y = (k % cols) * 18 * scale, (k // cols) * 18 * scale
        im.paste(t, (x, y))
        d.text((x + 1, y + 1), '%x' % m, fill=(255, 255, 0))
    return im


def draw_events(im, map_name, scale=1):
    mj = json.load(open(os.path.join(ROOT, 'data/maps', map_name, 'map.json')))
    d = ImageDraw.Draw(im)
    s = 16 * scale
    for o in mj.get('object_events', []):
        d.rectangle((o['x'] * s + 2, o['y'] * s + 2, o['x'] * s + s - 3, o['y'] * s + s - 3), outline=(0, 255, 255), width=2)
    for o in mj.get('warp_events', []):
        d.rectangle((o['x'] * s + 1, o['y'] * s + 1, o['x'] * s + s - 2, o['y'] * s + s - 2), outline=(255, 0, 255), width=2)
    for o in mj.get('coord_events', []):
        d.rectangle((o['x'] * s + 4, o['y'] * s + 4, o['x'] * s + s - 5, o['y'] * s + s - 5), outline=(255, 128, 0))
    for o in mj.get('bg_events', []):
        d.ellipse((o['x'] * s + 4, o['y'] * s + 4, o['x'] * s + s - 5, o['y'] * s + s - 5), outline=(0, 255, 0), width=2)
    return im


if __name__ == '__main__':
    a = sys.argv[1:]
    if a[0] == 'render':
        L = Layout(a[1])
        scale = 2 if '--x2' in a else 1
        im = L.render(grid='--grid' in a, ids='--ids' in a, coll='--coll' in a, scale=scale)
        if '--events' in a:
            draw_events(im, a[a.index('--events') + 1], scale)
        im.save(a[2])
    elif a[0] == 'sheet':
        pair = Pair(a[1], a[2])
        lo = int(a[a.index('--from') + 1], 0) if '--from' in a else 0
        hi = int(a[a.index('--to') + 1], 0) if '--to' in a else 1024
        sheet(pair, lo, hi).save(a[3])
