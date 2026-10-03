#!/usr/bin/env python3
"""Build trainer front pics (64x64, 16 colors) from Buu's Fury dialogue portraits, and Goku's battle back pic."""
import sys, pickle, struct
sys.path.insert(0, '/home/claude/tools')
import numpy as np
from PIL import Image
from buudec import decompress
from build_ow import quantize, save_indexed, save_jasc

ROM = open('/home/claude/buu/buu.gba', 'rb').read()
PPAL = open('/home/claude/buu/rips/p5.pal', 'rb').read()
SPAL = open('/home/claude/buu/rips/g3.pal', 'rb').read()
PORTS = pickle.load(open('/home/claude/buu/portraits.pk', 'rb'))
PE = '/home/claude/pokeemerald'
FP = f'{PE}/graphics/trainers/front_pics'

def rgb_table(pal):
    return np.array([[(c & 31) << 3, ((c >> 5) & 31) << 3, ((c >> 10) & 31) << 3]
                     for c in [struct.unpack_from('<H', pal, (256 + i) * 2)[0] for i in range(256)]], np.uint8)
PRGB = rgb_table(PPAL); SRGB = rgb_table(SPAL)

def portrait(i):
    d, _ = decompress(ROM, PORTS[i][0])
    a = np.frombuffer(d, np.uint8).reshape(64, 64)
    return a

def cutout(i, border=5):
    """Return 64x64 RGBA bust: frame and flat background removed, figure bottom-aligned."""
    a = portrait(i)
    inner = a[border:64 - border, border:64 - border]
    h, w = inner.shape
    # background = colors on the inner border ring that are not used heavily by the figure
    corners = np.concatenate([inner[:5, :5].ravel(), inner[:5, -5:].ravel()])
    bg = set(int(v) for v in np.unique(corners) if int(PRGB[v].astype(int).sum()) > 90)
    # flood fill from ring through bg-colored pixels
    mask = np.zeros_like(inner, bool)
    stack = [(y, x) for y in range(h) for x in (0, w - 1)] + [(y, x) for x in range(w) for y in (0, h - 1)]
    while stack:
        y, x = stack.pop()
        if y < 0 or x < 0 or y >= h or x >= w or mask[y, x]: continue
        if int(inner[y, x]) not in bg: continue
        mask[y, x] = True
        stack += [(y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)]
    # erode halo: pixels next to the background whose color is close to a background color
    bgcols = np.array([PRGB[v] for v in bg], int) if bg else np.zeros((0, 3), int)
    for _ in range(2):
        newm = mask.copy()
        for y in range(h):
            for x in range(w):
                if mask[y, x]: continue
                nb = [(y + dy, x + dx) for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)) if 0 <= y + dy < h and 0 <= x + dx < w]
                if not any(mask[p] for p in nb): continue
                c = PRGB[inner[y, x]].astype(int)
                if len(bgcols) and ((bgcols - c) ** 2).sum(1).min() < 3600 and c.sum() > 90:
                    newm[y, x] = True
        mask = newm
    rgba = np.zeros((h, w, 4), np.uint8)
    rgba[..., :3] = PRGB[inner]; rgba[..., 3] = np.where(mask, 0, 255)
    out = np.zeros((64, 64, 4), np.uint8)
    oy = 64 - h; ox = (64 - w) // 2
    out[oy:, ox:ox + w] = rgba
    return out

def save_front(name, i, palfile=None):
    fr = cutout(i)
    idx, pal = quantize([fr])
    save_indexed(idx, pal, f'{FP}/{name}.png')
    if palfile: save_jasc(pal, palfile)
    return fr

# portrait index for each Emerald trainer front pic
FRONT = {
    'leader_roxanne': 151, 'leader_brawly': 127, 'leader_wattson': 9, 'leader_flannery': 144,
    'leader_norman': 82, 'leader_winona': 39, 'leader_tate_and_liza': None, 'leader_juan': 104,
    'elite_four_sidney': 122, 'elite_four_phoebe': 3, 'elite_four_glacia': 1, 'elite_four_drake': 17,
    'champion_wallace': 68, 'steven': 76, 'wally': 134,
    'magma_leader_maxie': 83, 'aqua_leader_archie': 8, 'magma_admin': 118, 'aqua_admin_m': 106, 'aqua_admin_f': 126,
    'magma_grunt_m': 14, 'magma_grunt_f': 98, 'aqua_grunt_m': 23, 'aqua_grunt_f': 99,
}

def tate_and_liza():
    a = cutout(54); b = cutout(128)  # Goten & Trunks side by side
    out = np.zeros((64, 64, 4), np.uint8)
    sa = np.array(Image.fromarray(a).resize((44, 44), Image.NEAREST))
    sb = np.array(Image.fromarray(b).resize((44, 44), Image.NEAREST))
    out[20:, 0:44] = sa
    m = sb[..., 3] > 0
    reg = out[20:, 20:64]
    reg[m] = sb[m]
    return out

if __name__ == '__main__':
    import os
    for name, i in FRONT.items():
        path = f'{FP}/{name}.png'
        if not os.path.exists(path):
            print('missing', path); continue
        if i is None:
            fr = tate_and_liza(); idx, pal = quantize([fr]); save_indexed(idx, pal, path)
        else:
            save_front(name, i)
    # rival (May slot) = Vegeta; may.png front uses palettes/may.pal
    fr = cutout(136); idx, pal = quantize([fr])
    save_indexed(idx, pal, f'{FP}/may.png'); save_jasc(pal, f'{PE}/graphics/trainers/palettes/may.pal')
    # player: Goku front portrait + back pic share palettes/brendan.pal
    front = cutout(47)
    backs = []
    for off in (0x50E0C8, 0x50E1B0, 0x50EE98, 0x50EFA4):
        d, _ = decompress(ROM, off)
        a = np.frombuffer(d, np.uint8).reshape(4, 4, 8, 8).transpose(0, 2, 1, 3).reshape(32, 32)
        r = np.zeros((32, 32, 4), np.uint8); r[..., :3] = SRGB[a]; r[..., 3] = (a > 0) * 255
        r = np.array(Image.fromarray(r).resize((64, 64), Image.NEAREST))
        backs.append(r)
    idx, pal = quantize([front] + backs)
    save_indexed([idx[0]], pal, f'{FP}/brendan.png')
    save_indexed(idx[1:], pal, f'{PE}/graphics/trainers/back_pics/brendan.png', horizontal=False)
    save_jasc(pal, f'{PE}/graphics/trainers/palettes/brendan.pal')
    print('trainer pics done')
