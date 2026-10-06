"""Slateport City v0.5 layout (west quarter): the Poke Mart moves up to the north-west lot on the main
street, the Name Rater's house moves next to the Pokemon Center, the Pokemon Fan Club moves to the
middle lot by the south avenue, and the old Fan Club corner becomes a little flower park with the city
sign. The old two-avenue grid is replaced by: a lane off the main street, a PC/Mart street, a jogged
lane down past the park, and a market street in front of the market gate.
Fixed on purpose (cutscenes / fly spot): main street rows 13-15 (Scott leaves the Battle Tent along
row 13 x10-18; Gabby & Ty leave along rows 14-15 to x18), the Battle Tent + trigger 10,13, the Harbor
and its forecourt (Stern interview 25-31,13-16), the Pokemon Center (fly spot 19,20), the Museum and
its approach (Scott walks row 27 x21-30), Stern's Shipyard, the piers, the market."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('SlateportCity')
ORDER = {k: list(v) for k, v in c.json.items() if k.endswith('_events') and isinstance(v, list)}
P = dict(TL=0x208, T=0x209, TR=0x20a, L=0x210, C=0x211, R=0x212, BL=0x218, B=0x219, BR=0x21a)
INNER = dict(NW=0x232, NE=0x233, SW=0x23a, SE=0x23b)     # concave corners where roads meet
IDS = set(P.values()) | set(INNER.values())
G = c.copy_block(4, 9)          # 0x001 grass
BUSH = c.copy_block(7, 10)      # 0x243 round bush
FLOWER = c.copy_block(2, 31)    # 0x004 flowers

# ---- erase the old roads of the west quarter (rows 16-31, x2-19); main street rows 13-15 is redrawn identically
for y in range(16, 32):
    for x in range(2, 20):
        if (c.block(x, y) & 0x3FF) in IDS:
            c.set_block(x, y, G)

# ---- lift the three buildings and the sign posts that go with them
rater_sign = c.cut(8, 19, 1, 1, fill=G)
club_sign = c.cut(8, 26, 1, 1, fill=G)
city_sign = c.cut(16, 22, 1, 1, fill=G)
rater = c.cut(4, 16, 4, 4, fill=G)       # Name Rater's house, door 5,19 (warp 6)
mart = c.cut(12, 23, 4, 4, fill=G)       # Poke Mart, door 13,26 (warp 1), signs 14-15,26
club = c.cut(2, 22, 5, 6, fill=G)        # Fan Club + porch, door 4,26 (warp 4)

c.paste(mart, 4, 16)                     # door 5,19
c.paste(rater, 13, 16)                   # door 14,19
c.paste(club, 12, 22)                    # door 14,26, porch 13-15,27
c.paste(rater_sign, 12, 19)
c.paste(club_sign, 16, 27)

# ---- roads
main = c.rects((8, 13, 22, 3), (17, 10, 3, 3))           # main street + north road (unchanged geometry)
new = c.rects(
    (8, 16, 2, 4),       # lane from the main street down past the Mart
    (4, 20, 16, 2),      # Mart / Name Rater / Pokemon Center street (fly spot 19,20)
    (17, 22, 3, 10),     # south avenue (continues to the beach below row 31)
    (10, 22, 2, 6),      # lane between the park and the Fan Club
    (2, 28, 18, 2),      # market street in front of the market gate and the Fan Club porch
)
write = main | new
# cells that count as road for the edge shapes but are not repainted here
context = c.rects((17, 32, 3, 3), (20, 26, 2, 4), (9, 30, 3, 2))


def draw(write, context):
    allc = write | context
    for (x, y) in write:
        n, s, w, e = (x, y - 1) in allc, (x, y + 1) in allc, (x - 1, y) in allc, (x + 1, y) in allc
        if not n:
            k = P['TL'] if not w else (P['TR'] if not e else P['T'])
        elif not s:
            k = P['BL'] if not w else (P['BR'] if not e else P['B'])
        elif not w:
            k = P['L']
        elif not e:
            k = P['R']
        elif (x - 1, y - 1) not in allc:
            k = INNER['NW']
        elif (x + 1, y - 1) not in allc:
            k = INNER['NE']
        elif (x - 1, y + 1) not in allc:
            k = INNER['SW']
        elif (x + 1, y + 1) not in allc:
            k = INNER['SE']
        else:
            k = P['C']
        c.set_block(x, y, k | (3 << 12))


draw(write, context)

# ---- the little park in the old Fan Club corner (x2-9, rows 22-27)
for (x, y) in ((2, 22), (9, 22), (2, 27), (9, 27), (10, 16), (12, 16)):
    c.set_block(x, y, BUSH)
for (x, y) in ((3, 23), (4, 23), (7, 23), (8, 23), (3, 26), (4, 26), (7, 26), (8, 26),
               (10, 18), (11, 18)):
    c.set_block(x, y, FLOWER)
c.paste(city_sign, 6, 24)

# ---- people
c.move_event('object', 15, 4, 25)    # Maniac strolls the park
c.move_event('object', 16, 12, 21)   # Woman by the Name Rater / PC street
c.move_event('object', 2, 6, 14)     # Rich Boy, west end of the main street

# keep every event list in its original order (citykit's re-insert can't tell identical signs apart)
for k, lst in ORDER.items():
    c.json[k] = sorted(c.json[k], key=lambda e: next(i for i, o in enumerate(lst) if o is e))
probs = c.check()
print('\n'.join(probs) or 'ok')
# geometry along the scripted routes must be untouched
for (x, y) in [(x, 13) for x in range(8, 30)] + [(x, 14) for x in range(8, 30)] + [(x, 15) for x in range(18, 30)] \
        + [(x, 27) for x in range(20, 32)] + [(28, 13), (28, 12), (29, 13), (10, 12), (10, 14)]:
    if (c.block(x, y) >> 10) & 3 != (c.orig_blocks[y * c.w + x] >> 10) & 3:
        print('SCRIPTED ROUTE CHANGED at', x, y)
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/slateport.png', scale=2)
