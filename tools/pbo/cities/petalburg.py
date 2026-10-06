"""Petalburg City v0.5 layout. The north half is cutscene ground (Gym boy tour, Scott, Wally's tutorial,
Wally's dad walking the player home), so the Gym, Wally's house, the west road, the Gym approach and the
east road keep their exact tiles. The rest is reshuffled:
  * the Pokemon Center leaves the middle of town for the lakeside pocket west of the main road
    (where the old house stood), with its own plaza street;
  * the Poke Mart comes down from the north-east corner into the middle, opening onto the east road;
  * the old lakeside house moves up into the north-east corner on the old Mart spur;
  * the city sign now greets travellers at the Route 102 entrance; flower beds fill the old spots."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('PetalburgCity')
ORDER = {k: list(c.json.get(k) or []) for k in ('object_events', 'warp_events', 'bg_events', 'coord_events')}
P = dict(TL=0x118, T=0x119, TR=0x11a, L=0x120, C=0x121, R=0x122, BL=0x128, B=0x129, BR=0x12a)
G = c.ground                              # 0x001 grass
FLOWER = c.copy_block(14, 24)             # 0x004 flowers
SHORE = c.copy_block(19, 9)               # 0x002 grass under the pond bank
TREE_L, TREE_R = c.copy_block(28, 9), c.copy_block(29, 9)   # 0x1dc / 0x1dd tree bottom halves
KEEP = {(7, 13): c.copy_block(7, 13), (8, 13): c.copy_block(8, 13),   # road under tree tops
        (17, 9): c.copy_block(17, 9)}                                  # Gym sign top over the road

# cells the cutscenes walk (see report): must come out identical
ROUTES = (c.rects((15, 8, 1, 10), (15, 17, 15, 1), (7, 5, 1, 8), (7, 12, 9, 1), (4, 10, 13, 4),
                  (0, 12, 6, 2), (16, 10, 1, 2)))
before = {p: c.block(*p) for p in ROUTES}

for (x, y) in c.path_cells(P):
    c.set_block(x, y, G)

mart = c.cut(24, 10, 4, 3, fill=G)        # Mart body (roof top row 9 is merged with pond bank/trees)
c.set_block(24, 9, SHORE); c.set_block(25, 9, SHORE)
c.set_block(26, 9, TREE_L); c.set_block(27, 9, TREE_R)
pc = c.cut(19, 13, 4, 4, fill=G)          # Pokemon Center, door 20,16
house1 = c.cut(9, 16, 4, 4, fill=G)       # lakeside house, door 10,19
c.set_block(10, 20, G)                    # its doormat stays behind otherwise
sign = c.cut(17, 16, 1, 1, fill=G)        # city sign

c.paste(house1, 24, 10)                   # door 25,13
c.paste(mart, 18, 14)                     # door 19,16
for i, m in enumerate((0x028, 0x029, 0x029, 0x02b)):    # Mart roof top on plain grass
    c.set_block(18 + i, 13, m | (3 << 12))
c.paste(pc, 10, 15)                       # door 11,18
c.paste(sign, 27, 16)

road = c.rects(
    (2, 12, 15, 2),     # west road to Route 104 (cutscene ground, unchanged)
    (6, 6, 2, 6),       # Wally's house (unchanged)
    (14, 9, 3, 3),      # Gym forecourt (unchanged)
    (15, 14, 2, 13),    # main road south
    (15, 17, 13, 2),    # east road to Route 102 (unchanged), Mart door 19,16
    (25, 14, 2, 3),     # north-east house spur, door 25,13
    (11, 19, 6, 2),     # Pokemon Center plaza, door 11,18
    (15, 25, 7, 2),     # south lane to the garden house, door 20,24
)
c.draw_paths(road, P)
for p, b in KEEP.items():
    c.set_block(*p, b)

for (x, y) in ((9, 19), (9, 20), (10, 21), (9, 21), (13, 21), (14, 21), (17, 14), (17, 15),
               (22, 15), (23, 15), (22, 16), (23, 16), (24, 15), (9, 15), (14, 17)):
    if c.block(x, y) == G:
        c.set_block(x, y, FLOWER)

c.move_event('object', 7, 14, 15)    # Gym boy's resting spot (he is moved to 5,11 before the tour)

changed = [p for p in ROUTES if c.block(*p) != before[p]]
assert not changed, changed
for k, lst in ORDER.items():
    if lst:
        c.json[k] = lst
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/petalburg.png', scale=2)
