"""Oldale Town v0.5 layout: the Pokemon Center moves up to the north-west corner, the two houses
trade sides (east house now sits under the Mart, west house where the Center was), and the old
blob plaza becomes a street grid: a top boulevard (Center + Mart), a north-south spine between the
Route 103 and Route 101 exits, a west street to Route 102 and an east lane. The town sign now greets
travellers at the south plaza, with a flower bed in the south-east corner.

The Mart stays put: the Mart employee's potion tour walks column x=13 from y=15 up to y=7 (and
detours through 12,12..14), so that column is kept walkable (road or grass)."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('OldaleTown')
P = dict(TL=0x1d0, T=0x1d1, TR=0x1d2, L=0x1d8, C=0x1d9, R=0x1da, BL=0x1e0, B=0x1e1, BR=0x1e2)
G = c.ground                              # 0x001 grass
FLOWER = c.copy_block(3, 5)               # 0x004 red flowers (walkable)
edges = c.edge_cells()
# citykit's paste() re-sorts events by matching fields, which swaps look-alike signs (two PC signs,
# two Mart signs); remember the original objects and put the lists back in their original order.
ORDER = {k: list(c.json.get(k) or []) for k in ('object_events', 'warp_events', 'bg_events', 'coord_events')}

for (x, y) in c.path_cells(P):
    c.set_block(x, y, G)

house1 = c.cut(4, 4, 4, 4, fill=G)        # north-west house, door 5,7
pc = c.cut(5, 13, 4, 4, fill=G)           # Pokemon Center, door 6,16 (+ 2 signs)
house2 = c.cut(14, 13, 4, 4, fill=G)      # south-east house, door 15,16
sign = c.cut(11, 9, 1, 1, fill=G)         # town sign
for (x, y) in ((11, 8), (9, 9), (10, 10)):   # old garden flowers
    c.set_block(x, y, G)

c.paste(pc, 4, 3)                         # door 5,6
c.paste(house1, 14, 9)                    # door 15,12
c.paste(house2, 4, 12)                    # door 5,15
c.paste(sign, 11, 15)

road = c.rects(
    (9, 0, 2, 20),      # spine: Route 103 (north) to Route 101 (south)
    (4, 7, 12, 2),      # top boulevard: Pokemon Center 5,6 and Mart 14,6
    (2, 10, 7, 2),      # west street to Route 102
    (11, 13, 6, 2),     # east lane: house 15,12 (and the Mart employee's start 13,14)
    (5, 16, 7, 2),      # south plaza: house 5,15
    (7, 18, 5, 2),      # Route 101 exit, full width
)
c.draw_paths(road, P)
for (x, y) in edges:                      # seams stay exactly as they were
    c.set_block(x, y, c.orig_blocks[y * c.w + x])

bed = [(x, y) for x in range(14, 17) for y in range(16, 19)]   # south-east flower bed
for (x, y) in bed + [(12, 15), (2, 9), (3, 13), (3, 14), (8, 12), (17, 13), (17, 14), (2, 4), (8, 4), (8, 5)]:
    if c.block(x, y) == G:
        c.set_block(x, y, FLOWER)

c.move_event('object', 0, 7, 9)     # girl (faces left) between the Center and the west street
c.move_event('object', 2, 13, 17)   # footprints man sketching in the flower bed (faces right)

for k, lst in ORDER.items():
    if lst:
        c.json[k] = lst
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/oldale.png', scale=2)
