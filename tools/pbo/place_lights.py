#!/usr/bin/env python3
"""Night lights for the towns (CONTINUATION.md step 2, the light-sprite half).

Adds OBJ_EVENT_GFX_LIGHT_SPRITE objects, the way pokeemerald-expansion's DNS lighting does
(docs/tutorials/dns.md; its Hoenn example is commit a5b079d8's parent):
  - a Pokemon Center sign glow two tiles right of every Pokemon Center door,
  - a Poke Mart sign glow two tiles right of every Mart door,
  - a lamp glow on every lamp-post metatile (Rustboro's street lamps, PboCove's plaza lamps).
Positions come from the current maps, so they follow the v0.5 town rearrangement. Light objects
that are already there are kept; re-running adds nothing new.
usage: place_lights.py
"""
import json, os, struct

R = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
MAPS = ['LittlerootTown', 'OldaleTown', 'PetalburgCity', 'RustboroCity', 'DewfordTown', 'SlateportCity',
        'MauvilleCity', 'VerdanturfTown', 'FallarborTown', 'LavaridgeTown', 'FortreeCity', 'LilycoveCity',
        'MossdeepCity', 'SootopolisCity', 'PacifidlogTown', 'EverGrandeCity']
# secondary tileset -> lamp-post metatiles that get a round glow
LAMPS = {
    'gTileset_Rustboro': {530, 531},
    'gTileset_PboCove': {668, 679},
}

def light(x, y, kind):
    return {
        'graphics_id': 'OBJ_EVENT_GFX_LIGHT_SPRITE', 'x': x, 'y': y, 'elevation': 3,
        'movement_type': 'MOVEMENT_TYPE_NONE', 'movement_range_x': 0, 'movement_range_y': 0,
        'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': kind,
        'script': '0x0', 'flag': '0',
    }

def main():
    layouts = {l['id']: l for l in json.load(open(f'{R}/data/layouts/layouts.json'))['layouts'] if 'id' in l}
    total = 0
    for m in MAPS:
        path = f'{R}/data/maps/{m}/map.json'
        j = json.load(open(path))
        lay = layouts[j['layout']]
        w, h = lay['width'], lay['height']
        blk = open(f"{R}/{lay['blockdata_filepath']}", 'rb').read()
        at = lambda x, y: struct.unpack_from('<H', blk, (y * w + x) * 2)[0] & 0x3FF
        have = {(o['x'], o['y']) for o in j['object_events'] if o['graphics_id'] == 'OBJ_EVENT_GFX_LIGHT_SPRITE'}
        want = []
        for wp in j['warp_events']:
            d = wp['dest_map']
            if d.endswith('_POKEMON_CENTER_1F'):
                want.append((wp['x'] + 2, wp['y'], 'LIGHT_TYPE_PKMN_CENTER_SIGN'))
            elif d.endswith('_MART'):
                want.append((wp['x'] + 2, wp['y'], 'LIGHT_TYPE_POKE_MART_SIGN'))
        lamps = LAMPS.get(lay['secondary_tileset'], set())
        for y in range(h):
            for x in range(w):
                if at(x, y) in lamps:
                    want.append((x, y, 'LIGHT_TYPE_BALL'))
        added = 0
        for x, y, kind in want:
            if (x, y) in have or not (0 <= x < w and 0 <= y < h):
                continue
            j['object_events'].append(light(x, y, kind))
            have.add((x, y)); added += 1
        if added:
            with open(path, 'w') as f:
                f.write(json.dumps(j, indent=2, ensure_ascii=False) + '\n')
        print(f'{m}: +{added}')
        total += added
    print('lights added:', total)

if __name__ == '__main__':
    main()
