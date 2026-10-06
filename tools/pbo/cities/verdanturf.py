"""Verdanturf Town v0.5 layout: the Battle Tent drops to the south-west corner, the Friendship Rater's
house takes its old spot under the cliff, and Wanda's house shifts down beside the east house so all
three south buildings share one lane. The roads become a main street (rows 9-10) with a Mart/PC
forecourt, a footpath up to Rusturf Tunnel, and a southern lane joined by two side streets.
The Mart, Pokemon Center and the east house touch the connected map edges, so they stay put."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City, KINDS

c = City('VerdanturfTown')
# citykit's paste() re-sorts by matching fields, which shuffles look-alike events (the two Mart signs);
# remember the real order by identity and restore it before saving
ORDER = {k: [id(e) for e in c.json.get(k) or []] for k in KINDS.values()}
P = {k: 0x001 for k in ('TL', 'T', 'TR', 'L', 'C', 'R', 'BL', 'B', 'BR')}   # plain dirt path, no edges
G = c.ground                       # 0x204 grass
G2 = c.copy_block(4, 10)           # 0x205 grass variant
TREE = c.copy_block(0, 4)          # 0x0c7 single tree
FL_Y = c.copy_block(7, 6)          # 0x211 yellow flowers
FL_R = c.copy_block(0, 10)         # 0x214 flowers
FL_A = c.copy_block(5, 14)         # 0x215
FL_B = c.copy_block(6, 13)         # 0x213
FENCE = c.copy_block(4, 12)        # 0x34d white fence

# sign posts first (the garden wipe below would erase Wanda's)
tent_sign = c.cut(1, 8, 1, 1, fill=G)
wanda_sign = c.cut(7, 14, 1, 1, fill=G)
town_sign = c.cut(14, 6, 1, 1, fill=G)

# erase the old roads and the little fenced garden in the middle
for (x, y) in c.path_cells(P):
    c.set_block(x, y, G)
c.paint(4, 12, 4, 3, G)
for (x, y) in ((0, 10), (10, 10), (1, 17), (6, 17), (12, 17), (3, 18)):
    c.set_block(x, y, G)

# lift the movable buildings (events ride along)
tent = c.cut(1, 3, 5, 5, fill=G)              # Battle Tent, door 3,7 (+ sign post 1,8 handled below)
friend = c.cut(0, 11, 4, 4, fill=G)           # Friendship Rater's house, door 1,14
wanda = c.cut(8, 11, 5, 4, fill=G)            # Wanda's house, door 10,14
c.paint(0, 11, 1, 4, TREE)                    # close the tree line where the old house stood

c.paste(friend, 1, 4)                         # door -> 2,7
c.paste(tent, 1, 11)                          # door -> 3,15
c.paste(tent_sign, 2, 16)
c.paste(wanda, 9, 12)                         # door -> 11,15
c.paste(wanda_sign, 8, 15)
c.paste(town_sign, 17, 8)

road = c.rects(
    (2, 9, 16, 2),      # main street, open to Route 117 on the east
    (2, 8, 1, 1),       # Friendship Rater's door
    (8, 2, 1, 7),       # footpath up to Rusturf Tunnel
    (12, 4, 1, 1),      # Mart door
    (16, 4, 1, 1),      # Pokemon Center door
    (11, 5, 7, 2),      # Mart / PC forecourt
    (14, 7, 2, 2),      # forecourt down to the main street
    (6, 11, 2, 5),      # west side street
    (14, 11, 2, 5),     # east side street
    (3, 16, 15, 1),     # south lane: tent 3,15, Wanda 11,15, house 17,15
)
c.draw_paths(road, P)

# decorations
FL = [FL_Y, FL_R, FL_A, FL_B]
for i, (x, y) in enumerate(((5, 4), (5, 5), (6, 7), (11, 7), (12, 7), (17, 7), (16, 7),
                            (9, 11), (13, 11), (8, 13), (1, 16), (13, 17), (4, 17))):
    if c.block(x, y) in (G, G2):
        c.set_block(x, y, FL[i % 4])
c.paint(9, 8, 5, 1, FENCE)                    # fence along the lawn north of the main street

c.move_event('object', 0, 12, 17)   # man, strolling below the south lane
c.move_event('object', 2, 8, 12)    # boy, by the west side street
c.move_event('object', 3, 5, 9)     # camper, on the main street
for k, ids in ORDER.items():
    if k in c.json:
        c.json[k].sort(key=lambda e: ids.index(id(e)))
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/verdanturf.png', scale=2)
