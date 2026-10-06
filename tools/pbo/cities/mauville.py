"""Mauville City v0.5 layout: the Poke Mart and House 1 move up to the north side next to the Pokemon
Center (with the flower bed beside House 1), and the cycling quarter moves south: the bike racks and
Rydel's Cycles now sit on the south street, with House 2 at its west end. The street link from the main
road to the south street moves from the middle to the east end, so the south street becomes an L.
Fixed on purpose: the Gym and everything west of x14 (Wally / uncle / Scott scenes: gym front rows 6-8,
main road row 8 x0-12, Scott's lane x12-13 rows 8-16), the Game Corner yard, the Pokemon Center
(fly spot 22,6), the yellow house on the east edge, the north/south/east/west exits."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('MauvilleCity')
ORDER = {k: list(v) for k, v in c.json.items() if k.endswith('_events') and isinstance(v, list)}
P = dict(TL=0x100, T=0x101, TR=0x102, L=0x108, C=0x05e, R=0x10a, BL=0x110, B=0x111, BR=0x112)
IDS = set(P.values())
G = c.copy_block(1, 6)           # 0x001 grass
TREE_BOTTOM = c.copy_block(22, 1)  # 0x016 forest edge

# ---- erase all roads (not on the map edge; those rows/columns are drawn by the neighbours)
for y in range(1, c.h - 1):
    for x in range(1, c.w - 1):
        if (c.block(x, y) & 0x3FF) in IDS:
            c.set_block(x, y, G)

# ---- lift buildings
bike_sign = c.cut(33, 6, 1, 1, fill=G)
bike = c.cut(34, 2, 5, 4, fill=G)          # Rydel's Cycles, door 35,5 (warp 2)
racks = c.cut(26, 2, 8, 4, fill=G)         # bike racks + their fence (PC-side fence column 25 stays)
house1 = c.cut(31, 11, 5, 4, fill=G)       # door 32,14 (warp 4)
mart = c.cut(22, 11, 4, 4, fill=G)         # door 23,14 (warp 3), signs 24-25,14
house2 = c.cut(18, 11, 4, 4, fill=G)       # door 19,14 (warp 6)
bed = c.cut(15, 12, 3, 3, fill=G)          # flower bed (school kid stands in it)
c.paint(14, 12, 1, 3, G)                   # its fence post column (House 2 covers it next)

# north side, east of the Pokemon Center
c.paste(mart, 26, 2)                       # door 27,5
c.paste(house1, 31, 2)                     # door 32,5
c.paste(bed, 36, 3)
for x in range(36, 39):
    c.set_block(x, 2, TREE_BOTTOM)         # forest edge above the flower bed

# south street
c.paste(house2, 14, 11)                    # door 15,14
c.paste(racks, 18, 11)
c.paste(bike, 27, 11)                      # door 28,14
c.paste(bike_sign, 26, 14)

# ---- roads
roads = c.rects(
    (15, 0, 3, 8),       # north exit
    (0, 8, 40, 2),       # main road, west and east exits
    (7, 6, 3, 2),        # Gym
    (22, 6, 2, 2),       # Pokemon Center
    (27, 6, 2, 2),       # Mart
    (32, 6, 2, 2),       # House 1
    (12, 10, 2, 7),      # Scott's lane by the Game Corner (unchanged)
    (33, 10, 2, 5),      # link to the south street, now at the east end
    (15, 15, 20, 2),     # south street
    (15, 17, 3, 3),      # south exit
)
# doors count as road so the tile below a door continues into it; the north exit continues off-map
context = {(8, 5), (22, 5), (23, 5), (27, 5), (28, 5), (32, 5), (33, 5), (15, 14), (28, 14),
           (15, -1), (16, -1), (17, -1)}
allc = roads | context
for (x, y) in roads:
    if x in (0, c.w - 1) or y in (0, c.h - 1):
        continue                            # map edge stays as the neighbours expect
    n, s, w, e = (x, y - 1) in allc, (x, y + 1) in allc, (x - 1, y) in allc, (x + 1, y) in allc
    if not n:
        k = 'TL' if not w else ('TR' if not e else 'T')
    elif not s:
        k = 'BL' if not w else ('BR' if not e else 'B')
    else:
        k = 'L' if not w else ('R' if not e else 'C')
    c.set_block(x, y, P[k] | (3 << 12))

# ---- people
c.move_event('object', 2, 17, 10)    # Maniac, now watching the south street from the main road side
c.move_event('object', 0, 21, 16)    # Boy, on the south street in front of the bike racks

# keep every event list in its original order (citykit's re-insert can't tell identical signs apart)
for k, lst in ORDER.items():
    c.json[k] = sorted(c.json[k], key=lambda e: next(i for i, o in enumerate(lst) if o is e))
probs = c.check()
print('\n'.join(probs) or 'ok')
route = [(x, 6) for x in range(5, 12)] + [(x, 7) for x in range(5, 12)] + [(x, 8) for x in range(0, 14)] \
    + [(12, y) for y in range(8, 15)] + [(13, y) for y in range(8, 15)]
for (x, y) in route:
    if c.block(x, y) != c.orig_blocks[y * c.w + x]:
        print('SCRIPTED ROUTE CHANGED at', x, y)
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/mauville.png', scale=2)
