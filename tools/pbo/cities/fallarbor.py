"""Fallarbor Town v0.5 layout: the Mart and Cozmo's house trade places, the ash crater moves beside
the Mart, and the road becomes a loop around a central ash garden instead of one straight street."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('FallarborTown')
P = dict(TL=0x2e8, T=0x2e9, TR=0x2ea, L=0x2f0, C=0x2f1, R=0x2f2, BL=0x2f8, B=0x2f9, BR=0x2fa)
G = c.ground
for (x, y) in c.path_cells(P):
    c.set_block(x, y, G)

mart = c.cut(14, 12, 4, 4, fill=G)
cozmo = c.cut(5, 13, 4, 5, fill=G)
crater = c.cut(1, 14, 3, 3, fill=G)
tuft = c.copy_block(1, 11)                # 0x29f ash tuft
c.paste(cozmo, 14, 12)
c.paste(mart, 6, 14)
c.paste(crater, 1, 15)
# clear the tufts the old road ran past, scatter a few new ones round the garden
for x in range(1, 5):
    c.set_block(x, 11, G)
for x in range(15, 19):
    c.set_block(x, 11, G)

road = c.rects(
    (1, 9, 18, 2),      # main street, both exits
    (7, 8, 3, 1),       # battle tent forecourt
    (14, 8, 2, 1),      # Pokemon Center
    (1, 7, 2, 2),       # move relearner
    (11, 11, 2, 7),     # south loop, west side
    (11, 17, 6, 1),     # south loop, bottom (Cozmo's door at 15,16)
    (6, 18, 7, 1),      # Mart front (door at 7,17)
)
c.draw_paths(road, P)
for (x, y) in ((4, 12), (9, 12), (14, 18), (17, 18), (3, 18), (16, 10 + 1)):
    if c.block(x, y) == G:
        c.set_block(x, y, tuft)

c.move_event('object', 0, 9, 12)    # girl, now beside the garden
c.move_event('object', 3, 9, 13)    # her Azurill
c.move_event('object', 2, 13, 14)   # gentleman by the south loop
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/fallarbor.png', scale=2)
