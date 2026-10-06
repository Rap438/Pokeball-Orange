#!/usr/bin/env python3
"""Place ambient (non-battle) Pokemon in towns where their type fits: water types by the water's edge or
on the beach, flyers in open spaces, ground/fire types on sand and ash, the rest on grass or quiet ground.
They wander a tile or two, face Goku when talked to and cry (EventScript_DBZ_AmbientMon).
Idempotent: removes previously placed ambient objects first. Run from the repo root."""
import json, os, random, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from citykit import City

SCRIPT = 'EventScript_DBZ_AmbientMon'
TOWNS = {
    'LittlerootTown':  [('AZURILL', 'water')],
    'OldaleTown':      [('ZIGZAGOON', 'grass'), ('TAILLOW', 'open')],
    'PetalburgCity':   [('MARILL', 'water'), ('ZIGZAGOON', 'grass')],
    'RustboroCity':    [('SKITTY', 'ground'), ('WHISMUR', 'ground')],
    'DewfordTown':     [('WINGULL', 'beach'), ('MAKUHITA', 'beach')],
    'SlateportCity':   [('WINGULL', 'beach'), ('CORPHISH', 'water'), ('PELIPPER', 'beach')],
    'MauvilleCity':    [('ELECTRIKE', 'ground'), ('MAGNEMITE', 'open')],
    'VerdanturfTown':  [('WHISMUR', 'grass'), ('SKITTY', 'grass')],
    'FallarborTown':   [('SPINDA', 'ash'), ('SWABLU', 'open')],
    'LavaridgeTown':   [('NUMEL', 'ash'), ('TORKOAL', 'ash')],
    'FortreeCity':     [('SWABLU', 'grass'), ('TROPIUS', 'grass')],
    'LilycoveCity':    [('WINGULL', 'beach'), ('PELIPPER', 'beach'), ('LOTAD', 'water')],
    'MossdeepCity':    [('WINGULL', 'beach'), ('SPOINK', 'grass')],
    'PacifidlogTown':  [('CORSOLA', 'any'), ('WINGULL', 'any')],
    'EverGrandeCity':  [('ALTARIA', 'grass')],
}

beh = {}
src = open('include/constants/metatile_behaviors.h').read()
body = src[src.index('enum'):]
idx = 0
for line in body.splitlines():
    m = re.match(r'\s*(MB_\w+)\s*(=\s*(\w+))?\s*,', line)
    if m:
        if m.group(3):
            idx = int(m.group(3), 0)
        beh[idx] = m.group(1)
        idx += 1


def behavior(c, x, y):
    return beh.get(c.L.pair.attr(c.block(x, y) & 0x3FF) & 0xFF, '')


def place(name, mons, rng):
    c = City(name)
    objs = c.json['object_events']
    c.json['object_events'] = [o for o in objs if o.get('script') != SCRIPT]
    starts = c.edge_cells() or {(w['x'], w['y']) for w in c.events('warp')}
    reach = c.reachable_from(starts)
    busy = set()
    for k in ('object', 'warp', 'bg', 'coord'):
        for e in c.events(k):
            if isinstance(e.get('x'), int):
                busy.add((e['x'], e['y']))
    warps = [(w['x'], w['y']) for w in c.events('warp')]
    hl = json.load(open('src/data/heal_locations.json'))['heal_locations']
    busy |= {(h['x'], h['y']) for h in hl if h['map'] == c.json['id']}

    def free(x, y, r):
        return all((x + i, y + j) not in busy for i in range(-r, r + 1) for j in range(-r, r + 1))

    def near(x, y, test, r=1):
        return any(0 <= x + i < c.w and 0 <= y + j < c.h and test(behavior(c, x + i, y + j))
                   for i in range(-r, r + 1) for j in range(-r, r + 1))

    def open_area(x, y):
        return all(0 <= x + i < c.w and 0 <= y + j < c.h and c.walkable(x + i, y + j) for i in (-1, 0, 1) for j in (-1, 0, 1))

    water = lambda b: 'WATER' in b or 'POND' in b or 'OCEAN' in b

    def roam(x, y):
        # only wander where every neighbour is plain open ground: no roofs/canopies it could slip behind
        for i in (-1, 0, 1):
            for j in (-1, 0, 1):
                tx, ty = x + i, y + j
                if not (0 <= tx < c.w and 0 <= ty < c.h) or not c.walkable(tx, ty) or (tx, ty) in busy:
                    return False
                if (c.L.pair.attr(c.block(tx, ty) & 0x3FF) >> 12) != 0 or 'WATER' in behavior(c, tx, ty):
                    return False
        return True
    sandy = lambda b: 'SAND' in b or 'ASH' in b
    cands = [t for t in reach
             if (c.block(*t) >> 12) == 3 and free(*t, 1)
             and min(abs(t[0] - wx) + abs(t[1] - wy) for wx, wy in warps) >= 3
             and 'TALL_GRASS' not in behavior(c, *t) and 'LONG_GRASS' not in behavior(c, *t)
             and not any(k in behavior(c, *t) for k in ('OCEAN', 'POND', 'DEEP', 'WATERFALL', 'CURRENT', 'PUDDLE'))
             and 2 <= t[0] < c.w - 2 and 2 <= t[1] < c.h - 2]
    placed = []
    for species, habitat in mons:
        tests = {
            'water': lambda t: near(*t, water),
            'beach': lambda t: sandy(behavior(c, *t)) or near(*t, water, 2),
            'ash':   lambda t: sandy(behavior(c, *t)) or 'ASH' in behavior(c, *t),
            'open':  lambda t: open_area(*t),
            'grass': lambda t: open_area(*t) and not sandy(behavior(c, *t)),
            'ground': lambda t: open_area(*t),
            'any':   lambda t: True,
        }
        ok_tile = lambda t: habitat in ('water', 'beach') or 'WATER' not in behavior(c, *t)
        good = sorted(t for t in cands if ok_tile(t) and tests[habitat](t) and all(abs(t[0] - p[0]) + abs(t[1] - p[1]) >= 4 for p in placed))
        if not good:
            good = sorted(t for t in cands if ok_tile(t) and all(abs(t[0] - p[0]) + abs(t[1] - p[1]) >= 4 for p in placed))
        if not good:
            print(f'{name}: no spot for {species}')
            continue
        x, y = rng.choice(good)
        placed.append((x, y))
        moves = roam(x, y)
        busy |= {(x + i, y + j) for i in (-1, 0, 1) for j in (-1, 0, 1)}
        c.json['object_events'].append({
            'graphics_id': f'OBJ_EVENT_GFX_SPECIES({species})',
            'x': x, 'y': y, 'elevation': 3,
            'movement_type': 'MOVEMENT_TYPE_WANDER_AROUND' if moves else 'MOVEMENT_TYPE_LOOK_AROUND',
            'movement_range_x': 1, 'movement_range_y': 1,
            'trainer_type': 'TRAINER_TYPE_NONE',
            'trainer_sight_or_berry_tree_id': '0',
            'script': SCRIPT,
            'flag': '0',
        })
        print(f'{name}: {species} ({habitat}) at {x},{y} on {behavior(c, x, y)}{" (wanders)" if moves else ""}')
    open(c.path, 'w').write(json.dumps(c.json, indent=2) + '\n')


rng = random.Random(5)
for town, mons in TOWNS.items():
    place(town, mons, rng)
