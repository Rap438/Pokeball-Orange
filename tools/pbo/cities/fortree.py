"""Fortree City v0.5 layout (ground level, west side). The treetop walkways, ladders and treehouses
stay exactly where they are (multi-elevation), and so does the Gym with its Kecleon-blocked approach
(columns 18-27, rows 5-13). On the forest floor the Poke Mart climbs to the clearing under the
north-west canopy, the Pokemon Center moves south (one tile east of the old Mart lot) and faces a
new dirt clearing, and the old dirt patches become one dirt road: entrance meadow -> Mart -> House 1
ladder, with a lane down the Center's west side to a clearing that reaches the House 5 ladder.
East side: only the two wandering NPCs move."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City, KINDS

c = City('FortreeCity')
ORDER = {k: [id(e) for e in c.json.get(k) or []] for k in KINDS.values()}

GRASS = c.copy_block(1, 7)          # 0x001
TREE_A = c.copy_block(1, 4)         # 0x0c6
TREE_B = c.copy_block(2, 4)         # 0x0c7
SHADOW_GRASS = c.copy_block(1, 6)   # 0x017 tree foot over grass
SHADOW_DIRT = c.copy_block(8, 7)    # 0x253 tree foot over dirt
DIRT = dict(TL=0x258, T=0x259, TR=0x25a, L=0x260, C=0x261, R=0x262, BL=0x268, B=0x269, BR=0x26a)

# signs and buildings first (events ride along)
city_sign = c.cut(6, 9, 1, 1)
pc = c.cut(4, 3, 4, 4)          # door 5,6
mart = c.cut(3, 11, 4, 4)       # door 4,14

# new forest floor for x 0..12, y 3..19.  k keep  g grass  D dirt  T tree  M/P building lots
PLAN = [
    #0123456789012
    'kkkkMMMMkkkkk',  # 3
    'kkkkMMMMkkkkk',  # 4
    'kkkkMMMMkkkkk',  # 5
    'kkkkMMMMTTkTk',  # 6
    'kggggDDgkkDkk',  # 7
    'kgDDDDDDDDDkk',  # 8
    'kgDDDDDDDDDkk',  # 9
    'kggDDggggTTkk',  # 10
    'kTTDDPPPPTTkk',  # 11
    'kTTDDPPPPTkkk',  # 12
    'kTTDDPPPPTkkk',  # 13
    'kTTDDPPPPTkkk',  # 14
    'kTTDDDDDDTkkk',  # 15
    'kTTDDDDDDDkkk',  # 16
    'kTTDDDDDDDDDD',  # 17
    'kkkkkkkkkkkkk',  # 18
    'kkkkkkkkkkkkk',  # 19
]
cell = {}
for j, row in enumerate(PLAN):
    for i, ch in enumerate(row):
        cell[(i, 3 + j)] = ch
dirt = {p for p, ch in cell.items() if ch == 'D'}
grass = {p for p, ch in cell.items() if ch == 'g'}
for (x, y), ch in cell.items():
    if ch == 'g':
        c.set_block(x, y, GRASS)
    elif ch == 'T':
        c.set_block(x, y, TREE_A if (x * 7 + y * 3) % 3 else TREE_B)


GRASS_IDS = {0x001, 0x00e, 0x00f}


def is_grass(x, y):
    return 0 <= x < c.w and 0 <= y < c.h and (c.block(x, y) & 0x3FF) in GRASS_IDS


def lay_dirt(cells):
    """Fortree's dirt only shows an edge where it meets grass; against trees and buildings it runs flush."""
    for (x, y) in cells:
        c.set_block(x, y, DIRT['C'] | (3 << 12))
    for (x, y) in cells:
        n, s_, w, e = (not is_grass(x, y - 1)), (not is_grass(x, y + 1)), (not is_grass(x - 1, y)), (not is_grass(x + 1, y))
        if not n:
            k = 'TL' if not w else ('TR' if not e else 'T')
        elif not s_:
            k = 'BL' if not w else ('BR' if not e else 'B')
        else:
            k = 'L' if not w else ('R' if not e else 'C')
        c.set_block(x, y, DIRT[k] | (3 << 12))


lay_dirt(dirt)

# east side: the dirt patch under House 4's ladder now runs on as a road to the Route 120 entrance
c.set_block(36, 7, SHADOW_DIRT)
lay_dirt({(x, y) for x in range(32, 38) for y in (8, 9)})

# a tree standing right above open ground gets the matching shadowed foot tile
for (x, y), ch in cell.items():
    if ch == 'T':
        if (x, y + 1) in grass:
            c.set_block(x, y, SHADOW_GRASS)
        elif (x, y + 1) in dirt:
            c.set_block(x, y, SHADOW_DIRT)

c.paste(mart, 4, 3)              # door -> 5,6 (opens onto the dirt spur at 5..6,7)
c.paste(pc, 5, 11)               # door -> 6,14 (opens onto the south clearing)
c.paste(city_sign, 2, 7)         # greets you at the Route 119 entrance

c.move_event('object', 4, 7, 7)      # old man, on the meadow beside the Mart
c.move_event('object', 5, 3, 16)     # gameboy kid, south clearing
c.move_event('object', 2, 34, 9)     # woman, east dirt patch (wanders 8..10)
c.move_event('object', 1, 34, 17)    # girl, beside the decoration shop ladder (not on 37,17)
for k, ids in ORDER.items():
    if k in c.json:
        c.json[k].sort(key=lambda e: ids.index(id(e)))

# the Gym block and the Kecleon bridge approach must be untouched
for y in range(5, 14):
    for x in range(18, 28):
        assert c.block(x, y) == c.orig_blocks[y * c.w + x], (x, y)

probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/fortree.png', scale=2)
