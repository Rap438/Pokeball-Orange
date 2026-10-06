"""Dewford Town v0.5 layout: the north cottage moves down to the south-west corner beside the Gym, the
old lot becomes a palm grove with the town sign at the north entrance, the south-east tree clutter is
tidied into one tree line, and the townsfolk spread out (fisherman on the beach).
Fixed on purpose: Hall + House1 (they touch the connection edges), Pokemon Center (fly spot 2,11),
Gym, and Mr. Briney's dock (boat 12,8 / Briney 12,9 / player route 11-13,8-10)."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('DewfordTown')
ORDER = {k: list(v) for k, v in c.json.items() if k.endswith('_events') and isinstance(v, list)}
G = c.ground                          # 0x124 sand
TREE = c.copy_block(0, 1)             # 0x243 solid tree
ROCK = c.copy_block(18, 15)           # 0x0e2 boulder

# cottage (House2, warp 4) with the two trees behind its roof -> south-west corner, door at 1,17
house = c.cut(7, 4, 4, 5, fill=G)
c.paste(house, 0, 13)
for x in (1, 2, 3):
    c.set_block(x, 18, G)             # doorstep + walkway to the Gym forecourt

# the old lot becomes a small grove hugging the canal, with the town sign facing the north entrance
for (x, y) in ((9, 4), (10, 4), (10, 5), (9, 5), (10, 6)):
    c.set_block(x, y, TREE)
sign = c.cut(10, 10, 1, 1, fill=G)
c.paste(sign, 7, 6)

# south-east: clear the scattered trees and rock, one clean tree line along the south shore,
# boulders moved up beside House1
for y in (17, 18):
    for x in range(12, 18):
        c.set_block(x, y, G)
for x in range(13, 18):
    c.set_block(x, 18, TREE)
c.set_block(15, 16, ROCK)
c.set_block(5, 18, TREE)              # a tree each side of the Gym forecourt
c.set_block(11, 18, TREE)

c.move_event('object', 0, 6, 10)      # woman, in the open square between Center and Gym
c.move_event('object', 4, 6, 4)       # trendy-phrase boy, by the town sign at the north entrance
c.move_event('object', 2, 16, 10)     # Old Rod fisherman, on the beach by the water

# keep every event list in its original order (citykit's re-insert can't tell identical signs apart)
for k, lst in ORDER.items():
    c.json[k] = sorted(c.json[k], key=lambda e: next(i for i, o in enumerate(lst) if o is e))
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/dewford.png', scale=2)
