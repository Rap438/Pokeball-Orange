"""Lilycove City v0.5 layout.

Kept in place (cutscenes / fly spot / escape warp / cliff tiers): Department Store + rival spot, Museum,
Pokemon Center (fly lands on 24,15), Harbor (escape warp 12,33), Team Aqua hideout, the stairs, the
beach, the eastern cliff plateaus and House2.

Rearranged:
  * the Contest Hall leaves the middle of town and becomes the landmark at the west gate (Route 121
    side), framed by its trees and a fence line;
  * the Cove Lily Motel moves to the centre, under the Pokemon Center;
  * House3 moves east next to the old motel flower beds, with trees on its west side;
  * on the cliff terrace the Trainer Fan Club and House4 trade places;
  * on the upper terrace House1 trades places with the tree pair east of it;
  * the low-town road network is erased and redrawn: the main street moves up a row so it runs right
    under the terrace doors, a new north-south lane runs between the hall and the motel, the stair
    lane runs down to the harbour road, and the city sign moves to the west gate.
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('LilycoveCity')
ORDER = {k: [id(e) for e in v] for k, v in c.json.items() if k.endswith('_events') and isinstance(v, list)}
P = dict(TL=0x22a, T=0x22b, TR=0x22c, L=0x232, C=0x233, R=0x234, BL=0x23a, B=0x23b, BR=0x23c)
GRASS = 0x001 | (3 << 12)
FLOWER = 0x004 | (3 << 12)
o = c.copy_block                       # original blocks, read before anything moves
SIGNPOST = o(6, 15)                    # 0x003
FENCE_L, FENCE_M, FENCE_R = o(8, 14), o(9, 14), o(10, 14)
CLIFF_TOP, CLIFF_BOT = o(8, 11), o(8, 12)
TREE = [[o(27 + i, 20 + j) for i in range(2)] for j in range(5)]      # free-standing 2x5 tree
ROOF_TOP = [o(11 + i, 11) for i in range(4)]                          # house roof ridge, grass behind
CORNER = {k: o(*k) for k in ((11, 12), (14, 12), (37, 12), (41, 12))}


def autotile(cells, tiles, elev, virtual=()):
    """9-slice paint; `virtual` cells count as path neighbours but are not painted (stairs, etc.)."""
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


# 1. erase the low-town roads (elevation 3); the upper-terrace road (elevation 5) stays
for (x, y) in c.path_cells(P):
    if c.block(x, y) >> 12 == 3:
        c.set_block(x, y, GRASS)

# 2. lift the movable buildings
hall = c.cut(18, 18, 11, 7, fill=GRASS)       # trees + Contest Hall + trees (door 23,24)
motel = c.cut(35, 20, 7, 5, fill=GRASS)       # signpost + motel (door 37,24)
house3 = c.cut(10, 19, 4, 4, fill=GRASS)      # door 11,22
fanclub = c.cut(36, 11, 6, 4)                 # flowers/signpost column + fan club (door 39,14)
house4 = c.cut(11, 11, 4, 4)                  # door 12,14
# old contest hall flowers / sign and the leftover fence line across the middle
for y in range(19, 25):
    for x in (17, 29):
        c.set_block(x, y, GRASS)
for x in range(8, 31):
    c.set_block(x, 19, GRASS)

# 3. Contest Hall at the west gate: trees 8-9, hall 10-16, trees 17-18, door (13,24)
c.paste(hall, 8, 18)
c.set_block(8, 19, FENCE_L)
c.set_block(9, 19, FENCE_M)
c.set_block(17, 19, FENCE_M)
c.set_block(18, 19, FENCE_R)
c.set_block(19, 24, SIGNPOST)
c.move_event('bg', 5, 19, 24)                 # Contest Hall sign beside the hall
for y in (22, 23):
    c.set_block(19, y, FLOWER)
    c.set_block(22, y, FLOWER)

# 4. motel in the centre: signpost 23, motel 24-29, door (25,24)
c.paste(motel, 23, 20)
for x in range(24, 30):
    c.set_block(x, 18, FLOWER)

# 5. House3 east: trees 35-36, house 38-41 (door 39,24); the old motel flower bed 42-44 stays
for j in range(5):
    for i in range(2):
        c.set_block(35 + i, 20 + j, TREE[j][i])
c.paste(house3, 38, 21)
for i in range(4):
    c.set_block(38 + i, 21, ROOF_TOP[i])      # its old ridge had the fence line behind it

# 6. cliff terrace: Fan Club to the west slot, House4 to the east slot
c.paste(fanclub, 9, 11)                       # signpost (9,14), fan club 10-14, door (12,14)
c.set_block(9, 11, CLIFF_TOP)
c.set_block(9, 12, CLIFF_BOT)
c.set_block(8, 14, FLOWER)
c.set_block(10, 12, CORNER[(11, 12)])         # roof corners drawn against the cliff face
c.set_block(14, 12, CORNER[(14, 12)])
c.paste(house4, 37, 11)                       # house 37-40, door (38,14)
c.set_block(37, 12, CORNER[(37, 12)])         # roof corners drawn against grass
c.set_block(40, 12, CORNER[(41, 12)])
c.set_block(36, 11, CLIFF_BOT)
c.set_block(36, 12, FLOWER)
c.set_block(36, 13, FLOWER)
c.set_block(36, 14, FENCE_R)
c.set_block(41, 11, CLIFF_BOT)
c.set_block(41, 12, GRASS)
c.set_block(41, 13, FLOWER)
c.set_block(41, 14, GRASS)

# 7. upper terrace: House1 and the tree pair east of it trade places (door 42,6 -> 46,6)
c.swap((41, 2), (45, 2), 4, 5)
autotile(c.rects((31, 8, 17, 2)), P, 5, virtual={(30, 8), (30, 9), (31, 10), (32, 10)})

# 8. city sign to the west gate, off the road
c.set_block(6, 15, GRASS)
c.set_block(4, 17, SIGNPOST)
c.move_event('bg', 4, 4, 17)

# 9. new low-town roads
road = c.rects(
    (2, 15, 40, 2),      # main street under the terrace doors, from the Route 121 gate
    (31, 13, 2, 12),     # stair lane down to the harbour road
    (20, 17, 2, 8),      # new lane between the Contest Hall and the motel
    (7, 25, 39, 2),      # harbour road past the hall, motel and House3 doors
    (7, 27, 2, 6),       # down to the harbour
    (7, 33, 8, 2),       # harbour forecourt (door 12,32)
)
autotile(road, P, 3, virtual={(31, 12), (32, 12)})

# 10. people
c.move_event('object', 1, 14, 16)     # girl strolls the main street by the hall
c.move_event('object', 3, 33, 19)     # rich boy by the stair lane
c.move_event('object', 17, 19, 20)    # school kid by the new lane (faces right)
c.move_event('object', 19, 40, 27)    # the chatting couple step off the road below House3
c.move_event('object', 18, 41, 27)
c.move_event('object', 5, 26, 27)     # woman wanders the lawn south of the motel

# citykit's paste() re-sorts by matching fields, which swaps the two identical Pokemon Center signs:
# put every event list back in its exact original order
for k, ids in ORDER.items():
    c.json[k].sort(key=lambda e: ids.index(id(e)))

probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/lilycove.png', scale=2)
