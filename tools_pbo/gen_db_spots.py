#!/usr/bin/env python3
"""Find walkable, on-foot-reachable hiding spots for the Dragon Balls across Hoenn."""
import json, re, struct, random, os
PE = '/home/claude/pokeemerald'

# metatile behavior enum
beh = {}
src = open(f'{PE}/include/constants/metatile_behaviors.h').read()
body = src[src.index('enum'):]
idx = 0
for line in body.splitlines():
    m = re.match(r'\s*(MB_\w+)\s*(=\s*(\w+))?\s*,', line)
    if m:
        if m.group(3): idx = int(m.group(3), 0)
        beh[m.group(1)] = idx; idx += 1
bname = {v: k for k, v in beh.items()}
GOOD = {beh[n] for n in ['MB_NORMAL', 'MB_TALL_GRASS', 'MB_SHORT_GRASS', 'MB_SAND', 'MB_ASHGRASS', 'MB_FOOTPRINTS', 'MB_CAVE', 'MB_LONG_GRASS']}
def walkable_beh(b):
    n = bname.get(b, '')
    bad = ['WATER', 'POND', 'OCEAN', 'WATERFALL', 'JUMP', 'LEDGE', 'IMPASSABLE', 'DOOR', 'WARP', 'ARROW', 'HOLE', 'CURRENT', 'ICE', 'MUDDY', 'SLIDE', 'COUNTER', 'PC', 'TELEVISION', 'SHELF', 'TRASH', 'MAP', 'DIVE', 'PUDDLE', 'SECRET', 'CRACKED', 'CRACK']
    return not any(k in n for k in bad)

# tileset symbol -> metatile attributes
attrs = {}
mt = open(f'{PE}/src/data/tilesets/metatiles.h').read()
for sym, path in re.findall(r'const u16 gMetatileAttributes_(\w+)\[\] = INCBIN_U16\("([^"]+)"\)', mt):
    d = open(f'{PE}/{path}', 'rb').read()
    attrs[sym] = [struct.unpack_from('<H', d, i)[0] & 0xFF for i in range(0, len(d), 2)]
hdr = open(f'{PE}/src/data/tilesets/headers.h').read()
tsattr = {}
for name, body in re.findall(r'const struct Tileset (gTileset_\w+) =\s*\{(.*?)\};', hdr, re.S):
    m = re.search(r'\.metatileAttributes = gMetatileAttributes_(\w+)', body)
    if m: tsattr[name] = attrs[m.group(1)]

layouts = {L['id']: L for L in json.load(open(f'{PE}/data/layouts/layouts.json'))['layouts'] if 'id' in L}

DUNGEONS = ['PetalburgWoods', 'RusturfTunnel', 'GraniteCave_1F', 'JaggedPass', 'FieryPath', 'MeteorFalls_1F_1R',
            'MtPyre_Exterior', 'MtPyre_Summit', 'MtChimney', 'VictoryRoad_1F', 'ShoalCave_LowTideEntranceRoom', 'NewMauville_Entrance']
maps = sorted([m for m in os.listdir(f'{PE}/data/maps') if re.fullmatch(r'Route1\d\d', m)] +
              [m for m in os.listdir(f'{PE}/data/maps') if (m.endswith('Town') or m.endswith('City')) and not m.startswith('Underwater')] +
              DUNGEONS)

random.seed(1992)
spots = []
for mname in maps:
    p = f'{PE}/data/maps/{mname}/map.json'
    if not os.path.exists(p): continue
    M = json.load(open(p))
    L = layouts[M['layout']]
    W, H = L['width'], L['height']
    blk = open(f'{PE}/{L["blockdata_filepath"]}', 'rb').read()
    prim = tsattr[L['primary_tileset']]; sec = tsattr[L['secondary_tileset']]
    def tile(x, y):
        v = struct.unpack_from('<H', blk, (y * W + x) * 2)[0]
        mid = v & 0x3FF; col = (v >> 10) & 3; elev = v >> 12
        b = prim[mid] if mid < 512 else (sec[mid - 512] if mid - 512 < len(sec) else 0)
        return col, elev, b
    T = [[tile(x, y) for x in range(W)] for y in range(H)]
    occupied = set()
    for o in M.get('object_events', []): occupied.add((o['x'], o['y']))
    for e in M.get('warp_events', []): occupied.add((e['x'], e['y']))
    for e in M.get('coord_events', []): occupied.add((e['x'], e['y']))
    for e in M.get('bg_events', []): occupied.add((e['x'], e['y']))
    warps = [(e['x'], e['y']) for e in M.get('warp_events', [])]
    # flood fill from warps and connection edges
    starts = list(warps)
    for c in (M.get('connections') or []):
        d = c['direction']
        if d == 'up': starts += [(x, 0) for x in range(W)]
        if d == 'down': starts += [(x, H - 1) for x in range(W)]
        if d == 'left': starts += [(0, y) for y in range(H)]
        if d == 'right': starts += [(W - 1, y) for y in range(H)]
    def passable(x, y):
        col, elev, b = T[y][x]
        return col == 0 and walkable_beh(b)
    seen = set(); stack = [s for s in starts if 0 <= s[0] < W and 0 <= s[1] < H]
    for s in list(stack):
        # warps sit on door tiles: seed their walkable neighbours
        for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0)):
            stack.append((s[0] + dx, s[1] + dy))
    while stack:
        x, y = stack.pop()
        if not (0 <= x < W and 0 <= y < H) or (x, y) in seen or not passable(x, y): continue
        seen.add((x, y))
        e0 = T[y][x][1]
        for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0)):
            nx, ny = x + dx, y + dy
            if 0 <= nx < W and 0 <= ny < H and (nx, ny) not in seen:
                e1 = T[ny][nx][1]
                if e0 == e1 or e0 in (0, 15) or e1 in (0, 15):
                    stack.append((nx, ny))
    cands = []
    for (x, y) in seen:
        col, elev, b = T[y][x]
        if b not in GOOD or elev in (0, 15) or (x, y) in occupied: continue
        if any(abs(x - wx) + abs(y - wy) < 3 for wx, wy in warps): continue
        if x < 1 or y < 1 or x >= W - 1 or y >= H - 1: continue
        # prefer spots with some walkable neighbours (not dead-end ledge slivers)
        nb = sum(1 for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0)) if (x + dx, y + dy) in seen)
        if nb < 2: continue
        cands.append((x, y, elev))
    if not cands: continue
    random.shuffle(cands)
    chosen = []
    for c in cands:
        if all(abs(c[0] - d[0]) + abs(c[1] - d[1]) >= 6 for d in chosen):
            chosen.append(c)
        if len(chosen) >= 6: break
    for x, y, e in chosen:
        spots.append((M['id'], x, y, e))

out = ['// generated by tools/gen_db_spots.py -- Dragon Ball hiding spots (on-foot reachable)',
       'struct DragonBallSpot { u16 mapId; u8 x; u8 y; u8 elevation; };',
       'static const struct DragonBallSpot sDragonBallSpots[] = {']
for mid, x, y, e in spots:
    out.append(f'    {{{mid}, {x}, {y}, {e}}},')
out.append('};')
open(f'{PE}/src/data/dbz_dragonball_spots.h', 'w').write('\n'.join(out) + '\n')
print(len(spots), 'spots over', len(set(s[0] for s in spots)), 'maps')
