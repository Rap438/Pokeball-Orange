#!/usr/bin/env python3
"""Every map draws its neighbours (across connections) with its OWN tilesets. Flag connections where the
neighbour uses secondary metatiles the viewing map's secondary tileset doesn't have. Run from repo root."""
import glob, json, os, struct, sys
sys.path.insert(0, os.path.dirname(__file__))
import mapkit
lay = mapkit.layouts()
maps = {}
for f in glob.glob('data/maps/*/map.json'):
    m = json.load(open(f))
    maps[m['id']] = m
counts = {}
def sec_count(sym):
    if sym not in counts:
        d = mapkit.tileset_dir(sym)
        counts[sym] = os.path.getsize(os.path.join(d, 'metatiles.bin')) // 16
    return counts[sym]
bad = 0
for mid, m in maps.items():
    if m.get('layout') not in lay:
        continue
    me = lay[m['layout']]
    for c in m.get('connections') or []:
        o = maps.get(c['map'])
        if not o or o.get('layout') not in lay:
            continue
        ol = lay[o['layout']]
        d = open(ol['blockdata_filepath'], 'rb').read()
        blocks = struct.unpack('<%dH' % (len(d) // 2), d)
        w, h = ol['width'], ol['height']
        dr = c['direction']
        cells = [(x, y) for y in range(h) for x in range(w)
                 if (dr == 'down' and y < 8) or (dr == 'up' and y >= h - 8)
                 or (dr == 'left' and x >= w - 8) or (dr == 'right' and x < 8)
                 or dr in ('dive', 'emerge')]
        if dr in ('dive', 'emerge'):
            continue
        top = max(blocks[y * w + x] & 0x3FF for (x, y) in cells)
        if ol['secondary_tileset'] != me['secondary_tileset'] and top >= 512 + sec_count(me['secondary_tileset']):
            print(f"{mid} ({me['secondary_tileset']}) shows {c['map']} ({ol['secondary_tileset']}) which uses metatile {top:#x} > {512 + sec_count(me['secondary_tileset']) - 1:#x}")
            bad += 1
print('connection tileset problems:', bad)
