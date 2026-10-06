"""Rustboro City v0.5 layout. The north half is cutscene ground and stays as it is: Devon Corp and its
forecourt (scientist / Match Call scene), the Rusturf tunnel road (Devon employee triggers), the Gym
and the stolen-goods chase (row 21 and the main road column x=20), and the rival scene at the south
gate (rows 49-53). The south half of town is rebuilt around a new crossroads on rows 40-41:
  * the Pokemon Center and the Poke Mart cross the main road to the east side, stacked on the new
    east street (Center) and the south avenue (Mart);
  * the fountain leaves the east side for a small plaza where the Center stood, beside Cutter's house;
  * the house that sat in the east block moves west onto the south avenue, where the Mart was;
  * the west street (rows 40-41) now runs straight through to the east block."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from citykit import City

c = City('RustboroCity')
ORDER = {k: list(c.json.get(k) or []) for k in ('object_events', 'warp_events', 'bg_events', 'coord_events')}
P = dict(TL=0x2f8, T=0x2f9, TR=0x2fa, L=0x300, C=0x05e, R=0x302, BL=0x308, B=0x309, BR=0x30a)
G = c.ground                              # 0x2bb paving sand
KEEP = {(15, 56): c.copy_block(15, 56), (16, 56): c.copy_block(16, 56)}   # road meets the grass path

# cells the cutscenes walk / stand on (see report): must come out identical
ROUTES = c.rects((10, 14, 4, 4),          # Devon doors 11,15 / 12,15 and the steps below them
                 (13, 21, 8, 1), (20, 9, 1, 13),   # Aqua grunt + Devon employee chase
                 (28, 8, 4, 5),           # tunnel-road triggers 30,9 29,10 31,10 30,11 30,12
                 (23, 20, 1, 5),          # stolen-goods triggers
                 (12, 49, 8, 5))          # rival scene
before = {p: c.block(*p) for p in ROUTES}

for (x, y) in c.path_cells(P):
    c.set_block(x, y, G)

pc = c.cut(15, 35, 4, 4, fill=G)          # Pokemon Center, door 16,38 (+ 2 signs)
mart = c.cut(15, 42, 4, 4, fill=G)        # Poke Mart, door 16,45 (+ 2 signs)
house3 = c.cut(24, 42, 6, 5, fill=G)      # east-block house, door 26,46
fountain = c.cut(27, 38, 3, 3, fill=G)

c.paste(pc, 24, 36)                       # door 25,39 onto the east street
c.paste(mart, 24, 43)                     # door 25,46 onto the south avenue
c.paste(house3, 13, 42)                   # door 15,46 onto the south avenue
c.paste(fountain, 15, 36)                 # plaza by Cutter's house

road = c.rects(
    (20, 9, 17, 2),     # tunnel road (unchanged)
    (20, 9, 2, 40),     # main road (unchanged)
    (18, 21, 17, 2),    # Gym street (unchanged)
    (8, 40, 22, 2),     # west street, now through to the east block (Pokemon Center 25,39)
    (15, 47, 14, 2),    # south avenue: Mart 25,46, house 15,46
    (15, 47, 2, 10),    # south gate road (unchanged)
)
c.draw_paths(road, P)
for p, b in KEEP.items():
    c.set_block(*p, b)

c.move_event('object', 2, 28, 36)    # ninja boy (faces up), was where the Center now stands

changed = [p for p in ROUTES if c.block(*p) != before[p]]
assert not changed, changed
for k, lst in ORDER.items():
    if lst:
        c.json[k] = lst
probs = c.check()
print('\n'.join(probs) or 'ok')
if '--save' in sys.argv:
    c.save()
c.render(sys.argv[1] if len(sys.argv) > 1 and not sys.argv[1].startswith('--') else '/tmp/rustboro.png', scale=2)
