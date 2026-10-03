#!/usr/bin/env python3
"""Render BG layers from a VRAM/PAL/IO rip (8bpp or 4bpp text BGs)."""
import struct, sys, numpy as np
from PIL import Image
pre = sys.argv[1]
vram = open(pre + '.vram', 'rb').read(); pal = open(pre + '.pal', 'rb').read(); io = open(pre + '.io', 'rb').read()
P = np.array([[((c & 31) << 3), ((c >> 5 & 31) << 3), ((c >> 10 & 31) << 3)] for c in struct.unpack('<256H', pal[:512])], np.uint8)
def layer(b):
    c = struct.unpack_from('<H', io, 8 + 2 * b)[0]
    cb = (c >> 2) & 3; sb = (c >> 8) & 31; c256 = (c >> 7) & 1
    out = np.zeros((256, 256, 3), np.uint8)
    for ty in range(32):
        for tx in range(32):
            e = struct.unpack_from('<H', vram, sb * 0x800 + (ty * 32 + tx) * 2)[0]
            t = e & 0x3FF; hf = e >> 10 & 1; vf = e >> 11 & 1; pn = e >> 12
            if c256:
                d = vram[cb * 0x4000 + t * 64: cb * 0x4000 + t * 64 + 64]
                a = np.frombuffer(d, np.uint8).reshape(8, 8)
                img = P[a]
            else:
                d = vram[cb * 0x4000 + t * 32: cb * 0x4000 + t * 32 + 32]
                a = np.frombuffer(d, np.uint8)
                a = np.stack([a & 15, a >> 4], 1).reshape(8, 8)
                img = P[pn * 16 + a]; img[a == 0] = (255, 0, 255)
            if hf: img = img[:, ::-1]
            if vf: img = img[::-1]
            out[ty * 8:ty * 8 + 8, tx * 8:tx * 8 + 8] = img
    return out
ims = [layer(b) for b in range(4)]
W = np.concatenate(ims, 1)
Image.fromarray(W).resize((W.shape[1] * 2, W.shape[0] * 2), Image.NEAREST).save(pre + '_layers.png')
