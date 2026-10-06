#!/usr/bin/env python3
"""BFS walk planner for test scripts.
usage: path.py MAP SX SY GX GY [FACE]  -> prints play.py commands to walk from (SX,SY) to (GX,GY)
Uses layout collision bits and avoids object event tiles. Doesn't know ledges, water or sight lines."""
import sys, json, os
os.chdir('/home/claude/pokeemerald'); sys.path.insert(0, '/home/claude/pokeemerald/tools/pbo')
import mapkit
from collections import deque
m, sx, sy, gx, gy = sys.argv[1], *map(int, sys.argv[2:6])
face = sys.argv[6] if len(sys.argv) > 6 else None
mj = json.load(open(f'/home/claude/pokeemerald/data/maps/{m}/map.json'))
L = mapkit.Layout(mj['layout'])
objs = {(o['x'], o['y']) for o in mj.get('object_events', []) if isinstance(o.get('x'), int)}
D = {'UP': (0, -1), 'DOWN': (0, 1), 'LEFT': (-1, 0), 'RIGHT': (1, 0)}
prev = {(sx, sy): None}
q = deque([(sx, sy)])
while q:
    p = q.popleft()
    if p == (gx, gy): break
    for k, (dx, dy) in D.items():
        n = (p[0] + dx, p[1] + dy)
        if n in prev or not (0 <= n[0] < L.w and 0 <= n[1] < L.h): continue
        if L.get(*n)[1] or n in objs: continue
        prev[n] = (p, k); q.append(n)
if (gx, gy) not in prev: sys.exit('no path')
steps = []
p = (gx, gy)
while prev[p]: p, k = prev[p][0], prev[p][1]; steps.append(k)
steps.reverse()
out = ''
prev = os.environ.get('FACING', 'DOWN')   # after a warp the player faces down
for k in steps:
    if k != prev:
        out += f'press {k} 3; run 8; '   # tap to face the new direction (a tap only turns)
        prev = k
    out += f'press {k} 8; run 10; '
if face and face != prev: out += f'press {face} 3; run 10; '
print(out)
