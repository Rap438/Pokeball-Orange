#!/usr/bin/env python3
"""Build Emerald overworld sprite sheets (9 frames, 16x32, 4bpp indexed) from Buu's Fury small sprites."""
import sys, pickle, struct, os
sys.path.insert(0, '/home/claude/tools')
from buudec import decompress
import numpy as np
from PIL import Image

ROM = open('/home/claude/buu/buu.gba', 'rb').read()
PAL = open('/home/claude/buu/rips/g3.pal', 'rb').read()
B512 = pickle.load(open('/home/claude/buu/npc512.pk', 'rb'))
RGB = np.array([[(c & 31) << 3, ((c >> 5) & 31) << 3, ((c >> 10) & 31) << 3]
                for c in [struct.unpack_from('<H', PAL, (256 + i) * 2)[0] for i in range(256)]], dtype=np.uint8)

def frame_idx(i):
    d, _ = decompress(ROM, B512[i][0])
    return np.frombuffer(d, np.uint8).reshape(4, 2, 8, 8).transpose(0, 2, 1, 3).reshape(32, 16)

def frame_rgba(i, flip=False):
    a = frame_idx(i)
    im = np.zeros((32, 16, 4), np.uint8)
    im[..., :3] = RGB[a]; im[..., 3] = (a > 0) * 255
    if flip: im = im[:, ::-1]
    return im

def L5(s):  # Goku-style: F,F,B,S,S, D4, U4, S4
    return dict(F=s, B=s + 2, S=s + 3, D=(s + 5, s + 7), U=(s + 9, s + 11), W=(s + 13, s + 15))

def L3(s):  # Roshi-style: F,B,S, D4, U4, S4
    return dict(F=s, B=s + 1, S=s + 2, D=(s + 3, s + 5), U=(s + 7, s + 9), W=(s + 11, s + 13))

def sheet_frames(spec, flip_side=True):
    """Return list of 9 RGBA arrays in Emerald order: down, up, left, downwalk1/2, upwalk1/2, leftwalk1/2."""
    order = [(spec['F'], 0), (spec['B'], 0), (spec['S'], 1),
             (spec['D'][0], 0), (spec['D'][1], 0), (spec['U'][0], 0), (spec['U'][1], 0),
             (spec['W'][0], 1), (spec['W'][1], 1)]
    return [frame_rgba(i, flip_side and f) for i, f in order]

def quantize(frames, ncol=15):
    """frames: list of HxWx4 arrays. Returns indexed arrays and 16-color palette (index 0 transparent)."""
    allpx = np.concatenate([f.reshape(-1, 4) for f in frames])
    opaque = allpx[allpx[:, 3] > 0][:, :3]
    cols, counts = np.unique(opaque.reshape(-1, 3), axis=0, return_counts=True)
    if len(cols) <= ncol:
        pal = [tuple(c) for c in cols]
    else:
        # median-cut via PIL on a strip of unique colors weighted by counts
        strip = np.repeat(cols, np.minimum(counts, 200), axis=0)
        im = Image.fromarray(strip.reshape(1, -1, 3))
        q = im.quantize(colors=ncol, method=Image.Quantize.MEDIANCUT)
        p = q.getpalette()[:ncol * 3]
        pal = [tuple(p[i * 3:i * 3 + 3]) for i in range(ncol)]
    palarr = np.array(pal, dtype=np.int32)
    out = []
    for f in frames:
        h, w = f.shape[:2]
        flat = f.reshape(-1, 4).astype(np.int32)
        d = ((flat[:, None, :3] - palarr[None]) ** 2).sum(-1)
        idx = d.argmin(1) + 1
        idx[flat[:, 3] == 0] = 0
        out.append(idx.reshape(h, w).astype(np.uint8))
    full = [(115, 197, 164)] + [tuple(int(v) & 0xF8 for v in c) for c in pal]
    full += [(0, 0, 0)] * (16 - len(full))
    return out, full

def save_indexed(arrs, pal, path, horizontal=True):
    if horizontal:
        a = np.concatenate(arrs, axis=1)
    else:
        a = np.concatenate(arrs, axis=0)
    im = Image.fromarray(a, 'P')
    flat = []
    for c in pal: flat += list(c)
    flat += [0] * (768 - len(flat))
    im.putpalette(flat)
    im.save(path, transparency=0)

def save_jasc(pal, path):
    with open(path, 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n')
        for c in pal: f.write(f'{c[0]} {c[1]} {c[2]}\n')

def build_char(name, spec, outdir, runspec=None, flip_side=True):
    frames = sheet_frames(spec, flip_side)
    allf = list(frames)
    if runspec: allf += sheet_frames(runspec, flip_side)
    idx, pal = quantize(allf)
    os.makedirs(outdir, exist_ok=True)
    save_indexed(idx[:9], pal, f'{outdir}/{name}.png')
    if runspec: save_indexed(idx[9:], pal, f'{outdir}/{name}_running.png')
    save_jasc(pal, f'{outdir}/{name}.pal')
    return idx, pal
