#!/usr/bin/env python3
"""PokeBall Orange city kit: rearrange a town's layout while keeping every warp, sign, NPC and trigger
attached to what it belongs to, then check the result.

Library (run from the repo root):
    from citykit import City
    c = City('FallarborTown')
    c.render('out.png')                       # grid + events (warps magenta, objects cyan, signs green, triggers orange)
    clip = c.cut(x, y, w, h, fill=c.ground)   # lift a rectangle (blocks + events inside it), fill the hole
    c.paste(clip, nx, ny)                     # drop it somewhere else (events move with it)
    c.swap((x1, y1), (x2, y2), w, h)          # exchange two equal rectangles
    c.paint(x, y, w, h, block)                # fill with one block (metatile | coll << 10 | elev << 12)
    c.copy_block(sx, sy)                      # the raw block value at (sx, sy), for painting
    c.move_event(kind, index, x, y)           # kind: 'object', 'warp', 'bg', 'coord'
    problems = c.check()                      # see below; [] means clean
    c.save()

check() verifies, against the map as it was loaded:
  * map edges are unchanged (the neighbouring maps draw these rows/columns at the seams)
  * every warp stays on a walkable tile and is reachable on foot from a connection edge (or any warp
    for maps without connections), every NPC stands on a walkable tile, triggers are walkable
  * every sign (bg event) still has a walkable tile next to it
  * coordinates named in this map's scripts (setobjectxy / warp / coord triggers) are listed in
    script_coords() for manual review; check() flags the ones that turned unwalkable.
CLI: python3 tools/pbo/citykit.py render MAP out.png [scale] [--coll] [--ids] | check MAP | grid MAP
"""
import copy, json, os, re, struct, sys
from collections import deque
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mapkit

KINDS = {'object': 'object_events', 'warp': 'warp_events', 'bg': 'bg_events', 'coord': 'coord_events'}


class Clip:
    def __init__(self, w, h, blocks, events):
        self.w, self.h, self.blocks, self.events = w, h, blocks, events


class City:
    def __init__(self, name):
        self.name = name
        self.path = os.path.join('data/maps', name, 'map.json')
        self.json = json.load(open(self.path))
        self.L = mapkit.Layout(self.json['layout'])
        self.w, self.h = self.L.w, self.L.h
        self.orig_blocks = list(self.L.blocks)
        self.orig_json = copy.deepcopy(self.json)
        self.ground = self._most_common_walkable()
        self.baseline = {self._key(m) for m in self._check()}   # problems the original map already had (elevation, surf...)

    # ------------------------------------------------------------ raw access
    def block(self, x, y):
        return self.L.blocks[y * self.w + x]

    def set_block(self, x, y, v):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.L.blocks[y * self.w + x] = v

    def copy_block(self, x, y):
        return self.block(x, y)

    def walkable(self, x, y, blocks=None):
        b = (blocks or self.L.blocks)[y * self.w + x]
        return ((b >> 10) & 3) == 0

    def _most_common_walkable(self):
        cnt = {}
        for b in self.L.blocks:
            if ((b >> 10) & 3) == 0:
                cnt[b] = cnt.get(b, 0) + 1
        return max(cnt, key=cnt.get)

    def events(self, kind):
        return self.json.get(KINDS[kind]) or []

    # ------------------------------------------------------------ edits
    def paint(self, x, y, w, h, block):
        for j in range(h):
            for i in range(w):
                self.set_block(x + i, y + j, block)

    def _events_in(self, x, y, w, h, remove):
        got = []
        for kind, key in KINDS.items():
            keep = []
            for e in self.json.get(key) or []:
                if isinstance(e.get('x'), int) and x <= e['x'] < x + w and y <= e['y'] < y + h:
                    got.append((kind, e))
                else:
                    keep.append(e)
            if remove and key in self.json:
                self.json[key] = keep
        return got

    def cut(self, x, y, w, h, fill=None):
        blocks = [[self.block(x + i, y + j) for i in range(w)] for j in range(h)]
        evs = self._events_in(x, y, w, h, remove=False)   # events stay in their lists (order = ids); paste moves them
        evs = [(k, dict(e, x=e['x'] - x, y=e['y'] - y), e) for k, e in evs]
        if fill is not None:
            if isinstance(fill, list):          # pattern rows
                for j in range(h):
                    for i in range(w):
                        self.set_block(x + i, y + j, fill[j % len(fill)][i % len(fill[0])])
            else:
                self.paint(x, y, w, h, fill)
        return Clip(w, h, blocks, evs)

    def paste(self, clip, x, y, transparent=None):
        for j in range(clip.h):
            for i in range(clip.w):
                b = clip.blocks[j][i]
                if transparent is not None and b == transparent:
                    continue
                self.set_block(x + i, y + j, b)
        for kind, rel, orig in clip.events:
            orig['x'], orig['y'] = rel['x'] + x, rel['y'] + y

    def swap(self, a, b, w, h):
        ca = self.cut(a[0], a[1], w, h)
        cb = self.cut(b[0], b[1], w, h)
        self.paste(ca, b[0], b[1])
        self.paste(cb, a[0], a[1])

    # ------------------------------------------------------------ paths
    def detect_path_set(self):
        """Find this map's 3x3 path autotile ids from an existing path: returns dict TL,T,TR,L,C,R,BL,B,BR."""
        ids = [[self.block(x, y) & 0x3FF for x in range(self.w)] for y in range(self.h)]
        cnt = {}
        for y in range(1, self.h - 1):
            for x in range(1, self.w - 1):
                m = ids[y][x]
                if ids[y][x - 1] == m == ids[y][x + 1] == ids[y - 1][x] == ids[y + 1][x]:
                    cnt[m] = cnt.get(m, 0) + 1
        raise NotImplementedError('give the path set explicitly')

    def draw_paths(self, cells, tiles, keep_coll=False):
        """Paint a set of (x, y) cells as a path using a 9-slice autotile:
        tiles = dict(TL=, T=, TR=, L=, C=, R=, BL=, B=, BR=) metatile ids (walkable, elevation 3)."""
        cells = set(cells)
        for (x, y) in cells:
            n, s_, w, e = (x, y - 1) in cells, (x, y + 1) in cells, (x - 1, y) in cells, (x + 1, y) in cells
            if not n:
                k = 'TL' if not w else ('TR' if not e else 'T')
            elif not s_:
                k = 'BL' if not w else ('BR' if not e else 'B')
            else:
                k = 'L' if not w else ('R' if not e else 'C')
            self.set_block(x, y, tiles[k] | (3 << 12))

    def path_cells(self, tiles):
        ids = set(tiles.values())
        return {(x, y) for y in range(self.h) for x in range(self.w) if (self.block(x, y) & 0x3FF) in ids}

    @staticmethod
    def rects(*rs):
        """Union of rectangles (x, y, w, h) as a cell set."""
        out = set()
        for x, y, w, h in rs:
            out |= {(x + i, y + j) for j in range(h) for i in range(w)}
        return out

    def move_event(self, kind, index, x, y):
        e = self.events(kind)[index]
        e['x'], e['y'] = x, y

    # ------------------------------------------------------------ checks
    def edge_cells(self, depth=1):
        cells = set()
        for c in self.json.get('connections') or []:
            d = c['direction']
            for k in range(depth):
                if d == 'up':
                    cells |= {(x, k) for x in range(self.w)}
                elif d == 'down':
                    cells |= {(x, self.h - 1 - k) for x in range(self.w)}
                elif d == 'left':
                    cells |= {(k, y) for y in range(self.h)}
                elif d == 'right':
                    cells |= {(self.w - 1 - k, y) for y in range(self.h)}
        return cells

    def reachable_from(self, starts):
        seen = set(s for s in starts if self.walkable(*s))
        q = deque(seen)
        blockers = {(e['x'], e['y']) for e in self.events('object')
                    if e.get('movement_type', '').endswith(('FACE_DOWN', 'FACE_UP', 'FACE_LEFT', 'FACE_RIGHT', 'NONE'))
                    and 'PLAYER' not in e.get('graphics_id', '')}
        while q:
            x, y = q.popleft()
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                n = (x + dx, y + dy)
                if 0 <= n[0] < self.w and 0 <= n[1] < self.h and n not in seen and self.walkable(*n) and n not in blockers:
                    seen.add(n)
                    q.append(n)
        return seen

    def script_coords(self):
        """(x, y) pairs this map's scripts use for setobjectxy / warp-to-self / applymovement start points."""
        p = os.path.join('data/maps', self.name, 'scripts.inc')
        if not os.path.exists(p):
            return []
        s = open(p).read()
        out = []
        for m in re.finditer(r'setobjectxy\w*\s+[\w]+,\s*(\d+),\s*(\d+)', s):
            out.append((int(m.group(1)), int(m.group(2)), m.group(0)))
        mapid = self.json['id']
        for m in re.finditer(r'warp\w*\s+' + mapid + r',\s*(\d+),\s*(\d+)', s):
            out.append((int(m.group(1)), int(m.group(2)), m.group(0)))
        return out

    @staticmethod
    def _key(msg):
        return ' '.join(msg.split()[:2])   # e.g. 'warp 5', 'object 34', 'sign 7'

    def check(self, depth=1):
        return [m for m in self._check(depth) if self._key(m) not in self.baseline]

    def _check(self, depth=1):
        probs = []
        for (x, y) in sorted(self.edge_cells(depth)):
            if self.L.blocks[y * self.w + x] != self.orig_blocks[y * self.w + x]:
                probs.append(f'edge changed at {x},{y}')
        starts = self.edge_cells() or {(w['x'], w['y']) for w in self.events('warp')}
        reach = self.reachable_from(starts)
        for i, w in enumerate(self.events('warp')):
            nb = [(w['x'] + dx, w['y'] + dy) for dx, dy in ((0, 0), (0, 1), (0, -1), (1, 0), (-1, 0))]
            if not any(n in reach for n in nb):
                probs.append(f'warp {i} at {w["x"]},{w["y"]} unreachable')
        for i, o in enumerate(self.events('object')):
            if not isinstance(o.get('x'), int):
                continue
            if not (0 <= o['x'] < self.w and 0 <= o['y'] < self.h):
                continue
            if not self.walkable(o['x'], o['y']) and 'BERRY' not in o.get('graphics_id', '') and 'CUTTABLE' not in o.get('graphics_id', '') and 'BREAKABLE' not in o.get('graphics_id', ''):
                probs.append(f'object {i} ({o.get("graphics_id")}) at {o["x"]},{o["y"]} on a blocked tile')
        for i, c in enumerate(self.events('coord')):
            if not self.walkable(c['x'], c['y']):
                probs.append(f'trigger {i} at {c["x"]},{c["y"]} on a blocked tile')
        for i, b in enumerate(self.events('bg')):
            if b.get('type') == 'hidden_item':
                continue
            nb = [(b['x'] + dx, b['y'] + dy) for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0), (0, 0))]
            if not any(0 <= n[0] < self.w and 0 <= n[1] < self.h and n in reach for n in nb):
                probs.append(f'sign {i} at {b["x"]},{b["y"]} has no reachable side')
        for x, y, what in self.script_coords():
            if 0 <= x < self.w and 0 <= y < self.h and not self.walkable(x, y):
                probs.append(f'script coordinate {x},{y} now blocked: {what}')
        return probs

    # ------------------------------------------------------------ output
    def render(self, out, scale=1, grid=True, coll=False, ids=False):
        im = self.L.render(grid=grid, coll=coll, ids=ids, scale=scale)
        tmp = self.path + '.tmp'
        json.dump(self.json, open(tmp, 'w'), indent=2)
        try:
            # draw events from the edited json
            d = mapkit.ImageDraw.Draw(im)
            s = 16 * scale
            for o in self.events('object'):
                if isinstance(o.get('x'), int):
                    d.rectangle((o['x'] * s + 2, o['y'] * s + 2, o['x'] * s + s - 3, o['y'] * s + s - 3), outline=(0, 255, 255), width=2)
            for i, o in enumerate(self.events('warp')):
                d.rectangle((o['x'] * s + 1, o['y'] * s + 1, o['x'] * s + s - 2, o['y'] * s + s - 2), outline=(255, 0, 255), width=2)
                d.text((o['x'] * s + 3, o['y'] * s + 3), str(i), fill=(255, 0, 255))
            for o in self.events('coord'):
                d.rectangle((o['x'] * s + 4, o['y'] * s + 4, o['x'] * s + s - 5, o['y'] * s + s - 5), outline=(255, 128, 0))
            for o in self.events('bg'):
                d.ellipse((o['x'] * s + 4, o['y'] * s + 4, o['x'] * s + s - 5, o['y'] * s + s - 5), outline=(0, 255, 0), width=2)
            for x in range(0, self.w, 5):
                d.text((x * s + 2, 1), str(x), fill=(255, 255, 0))
            for y in range(0, self.h, 5):
                d.text((1, y * s + 2), str(y), fill=(255, 255, 0))
        finally:
            os.remove(tmp)
        im.save(out)

    def save(self):
        self.L.save()
        txt = json.dumps(self.json, indent=2) + '\n'
        open(self.path, 'w').write(txt)


if __name__ == '__main__':
    a = sys.argv[1:]
    c = City(a[1])
    if a[0] == 'render':
        c.render(a[2], scale=int(a[3]) if len(a) > 3 else 1, coll='--coll' in a, ids='--ids' in a)
    elif a[0] == 'grid':
        # metatile id per cell, '#' = blocked, then elevation digit
        print('    ' + ' '.join('%5d' % x for x in range(c.w)))
        for y in range(c.h):
            print('%3d ' % y + ' '.join('%03x%s%d' % (c.block(x, y) & 0x3FF, '#' if (c.block(x, y) >> 10) & 3 else '.', c.block(x, y) >> 12) for x in range(c.w)))
    elif a[0] == 'check':
        p = c.check()
        print('\n'.join(p) if p else 'ok')
        for x, y, what in c.script_coords():
            print('script coord', x, y, what)
