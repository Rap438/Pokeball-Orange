"""Mossdeep City v0.5 layout.

Kept in place: the Gym, the Pokemon Center (fly lands on 28,17), Steven's house, the Game Corner, the
Space Center and the whole eastern plateau (Team Magma's walk from 44-45,23-26 to 54,29 and the six
Magma triggers at 40-42,21-26), both stairways with their VisitedMossdeep triggers (25-26,25 /
32-33,27), the shoreline and the lower beach.

Rearranged on the town plateau (elevation 5):
  * the Mart moves north onto the gym avenue (old House1 slot), House1 takes the west slot (old House4
    slot) and House4 takes the Mart's old slot on the east side;
  * the road network is erased and redrawn: the three-wide trunk becomes a two-wide one, the gym
    avenue runs west all the way to Steven's door, the Pokemon Center street runs west to the new
    house, and a new south street in front of the Game Corner closes a loop with the Magma lane
    (whose cells are redrawn on exactly the same tiles);
  * trees and flowers re-placed around the new streets.
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('MossdeepCity')
ORDER = {k: [id(e) for e in v] for k, v in c.json.items() if k.endswith('_events') and isinstance(v, list)}
P = dict(TL=0x118, T=0x119, TR=0x11a, L=0x120, C=0x121, R=0x122, BL=0x128, B=0x129, BR=0x12a)
GRASS = 0x001 | (5 << 12)
o = c.copy_block
FLOWER = o(30, 10)                                   # 0x004 at elevation 5
TREE = [[o(19, 18), o(20, 18)], [o(19, 19), o(20, 19)]]    # 2x2 tree: 31a 31b / 322 323


def autotile(cells, tiles, elev, virtual=()):
    cells = set(cells)
    near = cells | set(virtual)
    for (x, y) in cells:
        n, s, w, e = (x, y - 1) in near, (x, y + 1) in near, (x - 1, y) in near, (x + 1, y) in near
        if not n:
            k = 'TL' if not w else ('TR' if not e else 'T')
        elif not s:
            k = 'BL' if not w else ('BR' if not e else 'B')
        else:
            k = 'L' if not w else ('R' if not e else 'C')
        c.set_block(x, y, tiles[k] | (elev << 12))


def tree(x, y):
    for j in range(2):
        for i in range(2):
            c.set_block(x + i, y + j, TREE[j][i])


# 1. erase the town-plateau roads (x <= 41); the eastern plateau paths (Magma route) are untouched
for (x, y) in c.path_cells(P):
    if x <= 41 and c.block(x, y) >> 12 == 5:
        c.set_block(x, y, GRASS)

# 2. Mart -> north slot, House1 -> west slot, House4 -> the Mart's old slot
mart = c.cut(36, 15, 4, 4)       # door (37,18), signs (38,18) (39,18)
house1 = c.cut(27, 6, 4, 4)      # door (28,9)
house4 = c.cut(17, 13, 4, 4)     # door (18,16)
c.paste(mart, 27, 6)             # door (28,9), signs (29,9) (30,9)
c.paste(house1, 17, 13)          # door (18,16)
c.paste(house4, 36, 15)          # door (37,18)

# 3. decorations: the tree in the way of the new PC street goes; new trees and flower beds
for y in (18, 19):
    for x in (19, 20):
        c.set_block(x, y, GRASS)
tree(35, 12)                     # beside the gym avenue
for (x, y) in ((34, 13), (34, 14), (34, 15), (34, 16), (34, 17),      # old third trunk lane -> flower strip
               (23, 19), (24, 19), (34, 21), (34, 22), (34, 23)):
    c.set_block(x, y, FLOWER)

# 4. new roads
road = c.rects(
    (22, 10, 19, 2),     # gym avenue: Mart door (28,9), Gym door (38,9)
    (18, 11, 4, 2),      # ... continued to Steven's door (19,10)
    (32, 12, 2, 15),     # trunk down to the south stairs (32-33,27)
    (18, 17, 14, 2),     # PC street: House1 door (18,16), PC door (28,16)
    (25, 19, 2, 5),      # lane to the west stairs (25-26,24)
    (34, 19, 6, 2),      # east street: House4 door (37,18)
    (40, 19, 2, 8),      # Magma lane (same cells as before; triggers 40-42,21-26)
    (34, 25, 6, 2),      # new south street: Game Corner door (36,24)
)
autotile(road, P, 5, virtual={(25, 24), (26, 24), (32, 27), (33, 27), (42, 25), (42, 26)})

# 5. people
c.move_event('object', 2, 30, 12)    # Pokefan F on the gym avenue lawn
c.move_event('object', 8, 22, 16)    # King's Rock boy between the new house and the trees
c.move_event('object', 3, 26, 21)    # ninja boy keeps pacing the west lane

for k, ids in ORDER.items():
    c.json[k].sort(key=lambda e: ids.index(id(e)))
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/mossdeep.png', scale=2)
