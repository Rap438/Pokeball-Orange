"""Lavaridge Town v0.5 layout: the Poke Mart comes down off the cliff to the south row (taking the
east house's lot, roof re-cut for a grass backdrop), and that house moves up under the cliff where
the Mart stood. A new sand lane runs along the south row (Gym, Herb Shop, Mart) and joins the main
street through a side street, so the whole town is now one loop.
Fixed on purpose: hot springs (multi-elevation water), Gym, Herb Shop and Pokemon Center - the
rival / Go-Goggles cutscene opens the Herb Shop door at 12,15, walks row 16 from x=12 to x=6,
rides the bike up column 9 from y=16 to y=9, and in the Pokemon Center variant walks 11,9 -> 9,9 ->
9,8 then rides row 9 east to x=17. All of those tiles stay walkable and NPC-free."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City, KINDS

c = City('LavaridgeTown')
ORDER = {k: [id(e) for e in c.json.get(k) or []] for k in KINDS.values()}

P = dict(TL=0x118, T=0x119, TR=0x11a, L=0x120, C=0x121, R=0x122, BL=0x128, B=0x129, BR=0x12a)  # sand road
G = c.ground                        # 0x001 grass, elevation 3
CLIFF_TOP = c.copy_block(13, 2)     # 0x271 rock shelf
CLIFF_FOOT = c.copy_block(12, 3)    # 0x279 cliff foot
FLOWER = c.copy_block(12, 7)        # 0x004 red flowers


def draw(cells, open_=()):
    """9-slice like City.draw_paths, but cells in open_ (doors, the connection edge) count as road
    for picking edges without being painted, so stubs run straight into doors / off the map."""
    cells, around = set(cells), set(cells) | set(open_)
    for (x, y) in cells:
        n, s_, w, e = (x, y - 1) in around, (x, y + 1) in around, (x - 1, y) in around, (x + 1, y) in around
        if not n:
            k = 'TL' if not w else ('TR' if not e else 'T')
        elif not s_:
            k = 'BL' if not w else ('BR' if not e else 'B')
        else:
            k = 'L' if not w else ('R' if not e else 'C')
        c.set_block(x, y, P[k] | (3 << 12))


# erase the old sand roads (column 19 is the Route 112 seam: leave it alone)
for (x, y) in c.path_cells(P):
    if x < 19:
        c.set_block(x, y, G)

# signs first
town_sign = c.cut(13, 8, 1, 1, fill=G)
herb_sign = c.cut(14, 16, 1, 1, fill=G)

# swap the Mart (on the cliff shelf) with the east house (south row)
mart = c.cut(14, 2, 4, 4, fill=[[CLIFF_TOP], [CLIFF_FOOT], [G], [G]])    # door 15,5
house = c.cut(15, 12, 4, 4, fill=G)                                      # door 16,15
mart.blocks[0] = [0x3028, 0x3029, 0x3029, 0x302b]   # Mart roof over grass (same tiles as Mauville / Slateport)
c.paste(house, 14, 4)                               # door -> 15,7
c.paste(mart, 15, 12)                               # door -> 16,15
c.paste(herb_sign, 10, 15)                          # beside the Herb Shop, off the cutscene path
c.paste(town_sign, 17, 8)                           # by the east entrance

road = c.rects(
    (8, 9, 11, 2),      # main street, runs on into Route 112 (col 19 kept as is)
    (9, 7, 2, 2),       # Pokemon Center door 9,6
    (15, 8, 2, 1),      # house door 15,7
    (8, 11, 2, 5),      # side street down to the south row (rival's bike route, column 9)
    (3, 16, 15, 2),     # south lane: Gym 5,15, Herb Shop 12,15, Mart 16,15
)
draw(road, open_={(9, 6), (15, 7), (19, 9), (19, 10), (5, 15), (12, 15), (16, 15)})

# flowers along the new lots
for (x, y) in ((12, 8), (13, 8), (10, 11), (10, 12), (14, 11), (2, 18), (11, 18), (14, 18), (17, 11)):
    if c.block(x, y) == G:
        c.set_block(x, y, FLOWER)

c.move_event('object', 0, 12, 7)    # expert by the hot springs -> under the tree east of the PC
c.move_event('object', 3, 7, 8)     # twin wanders the hot-spring edge (column 7, off every scripted path)
c.move_event('object', 2, 3, 8)     # old man, other side of the springs
c.move_event('object', 8, 6, 7)     # egg woman
for k, ids in ORDER.items():
    if k in c.json:
        c.json[k].sort(key=lambda e: ids.index(id(e)))

# cutscene tiles must stay walkable and free of NPCs
route = {(x, 16) for x in range(5, 13)} | {(9, y) for y in range(7, 17)} | {(x, 9) for x in range(9, 18)}
for (x, y) in route:
    assert c.walkable(x, y), (x, y)
for o in c.events('object'):
    if 'RIVAL' not in o.get('local_id', ''):
        assert (o['x'], o['y']) not in route, o

probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/lavaridge.png', scale=2)
