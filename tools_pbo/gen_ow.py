#!/usr/bin/env python3
"""Generate PokeBall Orange overworld graphics + C glue for pokeemerald."""
import sys, os, re
sys.path.insert(0, '/home/claude/tools')
import numpy as np
from PIL import Image
from build_ow import (frame_rgba, sheet_frames, quantize, save_indexed, save_jasc, L5, L3, B512, ROM, RGB)
from buudec import decompress
from ssj3 import ssj3_frames
from krillin import krillin_frames

PE = '/home/claude/pokeemerald'
PICS = f'{PE}/graphics/object_events/pics/dbz'
PALS = f'{PE}/graphics/object_events/palettes/dbz'
os.makedirs(PICS, exist_ok=True); os.makedirs(PALS, exist_ok=True)

# ---------------------------------------------------------------- helpers
def canvas32(fr, dy=0, dx=0):
    c = np.zeros((32, 32, 4), np.uint8)
    y0 = dy; ys = slice(max(0, y0), min(32, 32 + y0)); fs = slice(max(0, -y0), max(0, -y0) + (ys.stop - ys.start))
    c[ys, 8 + dx:24 + dx] = fr[fs]
    return c

def crop_waist(fr, row=21):
    f = fr.copy(); f[row:] = 0; return f

def flip(fr): return fr[:, ::-1].copy()

def reflection_pal(pal):
    # bluish, darker version used for water reflections
    out = [pal[0]]
    for c in pal[1:]:
        r, g, b = c
        out.append((int(r * 0.55) & 0xF8, int(g * 0.65) & 0xF8, min(248, int(b * 0.7 + 70)) & 0xF8))
    return out

def write_sheet(name, frames, pal=None, idx=None):
    if idx is None:
        idx, pal = quantize(frames)
    save_indexed(idx, pal, f'{PICS}/{name}.png')
    return idx, pal

# ---------------------------------------------------------------- Goku (player)
GOKU = 1156
def g(i): return frame_rgba(GOKU + i)
def gs(i): return frame_rgba(GOKU + i, True)  # side frames: flip to face west

goku_walk = sheet_frames(L5(GOKU))
goku_run_spec = dict(F=GOKU, B=GOKU + 2, S=GOKU + 3, D=(GOKU + 17, GOKU + 19), U=(GOKU + 21, GOKU + 23), W=(GOKU + 13, GOKU + 15))
goku_run = sheet_frames(goku_run_spec)

# flying (bikes): run frames lifted off the ground
fly = {'S': [g(17), g(19)], 'N': [g(21), g(23)], 'W': [gs(13), gs(15)]}
stand = {'S': g(0), 'N': g(2), 'W': gs(3)}
def fly_frame(d, k=0, dy=-4): return canvas32(fly[d][k % 2], dy)
mach = [canvas32(stand['S'], -3), canvas32(stand['N'], -3), canvas32(stand['W'], -3),
        fly_frame('S', 0), fly_frame('S', 1), fly_frame('N', 0), fly_frame('N', 1), fly_frame('W', 0), fly_frame('W', 1)]
acro_dirs = 'SNWSSNNWWSSSSNNNNWWWWSSNNWW'
acro = []
cnt = {}
for k, d in enumerate(acro_dirs):
    if k < 9: acro.append(mach[k]); continue
    cnt[d] = cnt.get(d, 0) + 1
    acro.append(fly_frame(d, cnt[d], dy=-4 - (cnt[d] % 2) * 2))
# surfing: sitting on the mon -> crop at the waist, sit low
surf = [canvas32(crop_waist(g(0)), 6), canvas32(crop_waist(g(0)), 5),
        canvas32(crop_waist(g(2)), 6), canvas32(crop_waist(g(2)), 5),
        canvas32(crop_waist(gs(3)), 6), canvas32(crop_waist(gs(3)), 5)]
# field move (using an HM): stand, ready, charge, charge, power-up
field = [canvas32(g(0)), canvas32(g(46)), canvas32(g(61)), canvas32(g(62)), canvas32(g(44))]
# fishing: Goku frames + the rod pixels lifted from Brendan's fishing sheet
bf = np.array(Image.open(f'{PE}/graphics/object_events/pics/people/brendan/fishing.png').convert('RGBA'))
fish = []
fish_base = ['W'] * 4 + ['N'] * 4 + ['S'] * 4
for k in range(12):
    base = canvas32(stand[fish_base[k]])
    bfr = bf[:, k * 32:(k + 1) * 32]
    opaque = bfr[..., 3] > 0
    rgb = bfr[..., :3].astype(int)
    dark = (rgb.sum(-1) < 200) & opaque
    mask = np.zeros_like(dark)
    mask[:, :8] = dark[:, :8]; mask[:, 24:] = dark[:, 24:]; mask[:5] = dark[:5]
    out = base.copy()
    out[mask, :3] = (72, 48, 24); out[mask, 3] = 255
    fish.append(out)
water = [canvas32(stand['S']), canvas32(stand['S']), canvas32(stand['N']), canvas32(stand['N']), canvas32(stand['W']), canvas32(stand['W'])]
decor = [g(0)]

# Flying Nimbus (used by the Fly effect; drawn with the player's palette)
def make_nimbus():
    im = np.zeros((32, 32, 4), np.uint8)
    blobs = [(9, 21, 6), (15, 18, 7), (22, 20, 6), (27, 23, 4), (5, 24, 4), (14, 24, 7), (20, 25, 6)]
    tail = [(30, 22, 2), (31, 20, 1.5)]
    mask = np.zeros((32, 32), bool); depth = np.zeros((32, 32))
    for cx, cy, r in blobs + tail:
        yy, xx = np.mgrid[0:32, 0:32]
        d = np.sqrt((xx + .5 - cx) ** 2 + (yy + .5 - cy) ** 2)
        inside = d <= r
        mask |= inside
        depth = np.maximum(depth, np.where(inside, (cy - (yy + .5)) / r, -9))
    ramp = [(168, 104, 16), (232, 168, 32), (248, 216, 64), (255, 248, 168)]
    for y in range(32):
        for x in range(32):
            if not mask[y, x]: continue
            edge = not (mask[max(0, y - 1), x] and mask[min(31, y + 1), x] and mask[y, max(0, x - 1)] and mask[y, min(31, x + 1)])
            if edge: c = ramp[0]
            else:
                t = depth[y, x]
                c = ramp[3] if t > 0.45 else ramp[2] if t > -0.1 else ramp[1]
            im[y, x, :3] = c; im[y, x, 3] = 255
    return im
nimbus = make_nimbus()
# quantize all base-Goku frames together -> one player palette
allg = goku_walk + goku_run + mach + acro + surf + field + fish + water + decor + [nimbus]
idx, gpal = quantize(allg)
parts = {}
o = 0
for nm, fr in [('walking', goku_walk), ('running', goku_run), ('mach_bike', mach), ('acro_bike', acro), ('surfing', surf),
               ('field_move', field), ('fishing', fish), ('watering', water), ('decorating', decor)]:
    parts[nm] = idx[o:o + len(fr)]; o += len(fr)
BR = f'{PE}/graphics/object_events/pics/people/brendan'
save_indexed([idx[-1]], gpal, f'{PE}/graphics/field_effects/pics/bird.png')
for nm, arrs in parts.items():
    save_indexed(arrs, gpal, f'{BR}/{nm}.png')
save_jasc(gpal, f'{PE}/graphics/object_events/palettes/brendan.pal')
save_jasc(reflection_pal(gpal), f'{PE}/graphics/object_events/palettes/brendan_reflection.pal')

# Super Saiyan forms (walking + running in one 18-frame sheet each)
def form_sheet(name, walk, run):
    idx, pal = quantize(walk + run)
    save_indexed(idx[:9], pal, f'{PICS}/{name}.png')
    save_indexed(idx[9:], pal, f'{PICS}/{name}_running.png')
    save_jasc(pal, f'{PALS}/{name}.pal')
    save_jasc(reflection_pal(pal), f'{PALS}/{name}_reflection.pal')

def run_from(spec, base):
    return sheet_frames(dict(F=spec['F'], B=spec['B'], S=spec['S'], D=(base + 17, base + 19), U=(base + 21, base + 23), W=spec['W']))

ssj1 = L5(1223); form_sheet('goku_ssj', sheet_frames(ssj1), run_from(ssj1, 1223))
ssj2 = L5(1081); ssj2['B'] = 1081 + 9; form_sheet('goku_ssj2', sheet_frames(ssj2), run_from(ssj2, 1081))
s3 = ssj3_frames(ssj1)
form_sheet('goku_ssj3', s3, s3)

# ---------------------------------------------------------------- Vegeta (rival)
VEG = 2463
vspec = L5(VEG)
def v(i, fl=False): return frame_rgba(VEG + i, fl)
vwalk = sheet_frames(vspec)
vstand = {'S': v(0), 'N': v(2), 'W': v(3, True)}
vwalkd = {'S': [v(5), v(7)], 'N': [v(9), v(11)], 'W': [v(13, True), v(15, True)]}
vmach = [canvas32(vstand['S'], -3), canvas32(vstand['N'], -3), canvas32(vstand['W'], -3)] + \
        [canvas32(vwalkd[d][k], -4) for d in 'SNW' for k in (0, 1)]
vacro = []
cnt = {}
for k, d in enumerate(acro_dirs):
    if k < 9: vacro.append(vmach[k]); continue
    cnt[d] = cnt.get(d, 0) + 1
    vacro.append(canvas32(vwalkd[d][cnt[d] % 2], -4 - (cnt[d] % 2) * 2))
vsurf = [canvas32(crop_waist(vstand[d]), dy) for d in 'SSNNWW' for dy in [6]][:6]
vfield = [canvas32(vstand['S'])] * 5
allv = vwalk + vwalk + vmach + vacro + vsurf + vfield
idx, vpal = quantize(allv)
o = 0
for nm, n in [('vegeta', 9), ('vegeta_running', 9), ('vegeta_mach_bike', 9), ('vegeta_acro_bike', 27), ('vegeta_surfing', 6), ('vegeta_field_move', 5)]:
    save_indexed(idx[o:o + n], vpal, f'{PICS}/{nm}.png'); o += n
save_jasc(vpal, f'{PALS}/vegeta.pal')

# ---------------------------------------------------------------- NPC cast (16x32, 9 frames)
NPCS = {
    'chichi': dict(F=313, B=315, S=317, D=(319, 321), U=(323, 325), W=(327, 329)),
    'roshi': L3(1774),
    'yamcha': L5(2793), 'tien': L5(2237), 'bulma': L5(208), 'videl': L5(2657),
    'android18': L5(30), 'supreme_kai': L5(2142), 'piccolo': L5(1998), 'mr_satan': L5(1459),
    'great_saiyaman': L3(836), 'uub': L5(2407), 'trunks': L5(2260), 'baba': L5(92),
    'goten': dict(F=766, B=773, S=767, D=(769, 771), U=(773, 775), W=(777, 779)),
    'dende': dict(F=1842, B=1843, S=1844, D=(1842, 1843), U=(1843, 1843), W=(1844, 1845)),
    'announcer': L5(47), 'fat_buu': L5(242), 'broly': L5(1501), 'yamu': L5(359),
    'majin_soldier': L5(384), 'saiyan_soldier': L5(1654), 'lime': L5(2043),
    'dr_brief': L5(420), 'oolong': L5(1926), 'gohan_kid': L5(1961),
    # Buu's Fury townsfolk, mixed in with Emerald's own pedestrians
    'police': L5(336), 'purple_man': L5(1706), 'pink_woman': L5(1729), 'pigtail_girl': L5(1752),
    'old_man': L5(1872), 'village_woman': L5(1894), 'blue_kid': L5(2214), 'farm_girl': L5(2726),
    'brown_woman': L5(2748), 'cap_kid': L5(142), 'green_kid': L5(169), 'striped_man': L5(2235),
    'blonde_girl': L5(2768),
    'super_buu': dict(F=259, B=262, S=256, D=(260, 261), U=(263, 264), W=(255, 257)),
}
for name, spec in NPCS.items():
    fr = sheet_frames(spec)
    idx, pal = quantize(fr)
    save_indexed(idx, pal, f'{PICS}/{name}.png'); save_jasc(pal, f'{PALS}/{name}.pal')
fr = krillin_frames(); idx, pal = quantize(fr)
save_indexed(idx, pal, f'{PICS}/krillin.png'); save_jasc(pal, f'{PALS}/krillin.pal')
print('ok')
