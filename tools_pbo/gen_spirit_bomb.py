#!/usr/bin/env python3
"""Spirit Bomb sprite: two 32x32 frames of a glowing blue-white energy ball (4bpp, own palette)."""
import numpy as np, math
from PIL import Image
PAL = [(255, 0, 255), (16, 40, 120), (32, 80, 200), (64, 128, 248), (112, 176, 255), (168, 216, 255),
       (216, 240, 255), (255, 255, 255), (80, 200, 255), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0), (0, 0, 0)]
frames = []
for f in range(2):
    a = np.zeros((32, 32), np.uint8)
    for y in range(32):
        for x in range(32):
            dx, dy = x - 15.5, y - 15.5
            r = math.hypot(dx, dy)
            if r > 15.5:
                continue
            ang = math.atan2(dy, dx)
            swirl = math.sin(ang * 5 + r * 0.6 + f * 1.3) * 1.2
            v = 7 - (r + swirl) / 15.5 * 6.2
            c = int(max(1, min(7, round(v))))
            if r > 13.5:
                c = 1 if (int(ang * 6 + f) % 2) else 2
            a[y, x] = c
    # sparkles
    rng = np.random.RandomState(7 + f)
    for _ in range(10):
        x, y = rng.randint(3, 29, 2)
        if math.hypot(x - 15.5, y - 15.5) < 14:
            a[y, x] = 8
    frames.append(a)
img = np.concatenate(frames, axis=0)
im = Image.fromarray(img, 'P')
flat = [v for c in PAL for v in c] + [0] * (768 - 48)
im.putpalette(flat)
im.save('/home/claude/pokeemerald/graphics/dbz/spirit_bomb.png', transparency=0)
print('ok')
