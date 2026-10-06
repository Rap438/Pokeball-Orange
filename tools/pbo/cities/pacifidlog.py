"""Pacifidlog Town v0.5 layout (deliberately small).

The town is a packed grid of identical floating houses on the open sea, so only whole house units
(spire tile + 5x5 log platform + dock) move, and only into open water; the log bridges keep their
top/bottom and left/right pairs intact (the sinking-log step callback works on pairs).

Kept in place: the Pokemon Center platform (fly lands on 8,16), the two houses that touch the
Route 131 / Route 132 seam columns (House4 at x0-4, House5 at x15-19), House1 and its bridge.

Rearranged:
  * House3 (centre) rises two rows to sit right under the Pokemon Center; its dock now drops onto the
    south boardwalk through a new two-log bridge;
  * House2 (west) sinks three rows to the south-west corner and now connects from the side of its
    platform straight onto the Pokemon Center's long bridge; its dock becomes a little fishing pier;
  * the old dock bridge that ran down the west side is gone.
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('PacifidlogTown')
ORDER = {k: [id(e) for e in v] for k, v in c.json.items() if k.endswith('_events') and isinstance(v, list)}
o = c.copy_block
WATER = o(9, 28)                     # 0x170, elevation 1
LOG_TOP, LOG_BOT = o(3, 25), o(3, 26)          # vertical log pair 0x258 / 0x260

house2 = c.cut(1, 19, 5, 6, fill=WATER)        # spire (3,19), door (3,22), dock (3,24)
house3 = c.cut(10, 21, 5, 6, fill=WATER)       # spire (12,21), door (12,24), dock (12,26)
for y in (25, 26):                             # House2's old dock bridge
    c.set_block(3, y, WATER)
for x in (4, 5):                               # west half of the old row-26 boardwalk
    c.set_block(x, 26, WATER)

c.paste(house3, 10, 19)                        # door (12,22), dock (12,24)
c.set_block(12, 25, LOG_TOP)                   # dock -> south boardwalk (row 27)
c.set_block(12, 26, LOG_BOT)

c.paste(house2, 1, 22)                         # door (3,25), dock (3,27) = pier
# its east platform edge (5,26) meets the kept log pair (6,26)-(7,26) -> PC bridge at (8,26)

c.move_event('object', 0, 3, 27)               # girl now looks out from the new pier

for k, ids in ORDER.items():
    c.json[k].sort(key=lambda e: ids.index(id(e)))
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/pacifidlog.png', scale=2)
