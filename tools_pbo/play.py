#!/usr/bin/env python3
"""Drive the headless emulator.
usage: play.py ROM [--load STATE] [--save STATE] [--out sheet.png] [--scale N] -- cmd; cmd; ...
Commands are runner commands separated by ';'. 'shot' with no arg auto-names a frame for the sheet.
"""
import sys, subprocess, os, tempfile
from PIL import Image

args = sys.argv[1:]
rom = args.pop(0)
load = save = None
out = 'sheet.png'
scale = 2
cols = 4
while args and args[0] != '--':
    a = args.pop(0)
    if a == '--load': load = args.pop(0)
    elif a == '--save': save = args.pop(0)
    elif a == '--out': out = args.pop(0)
    elif a == '--scale': scale = int(args.pop(0))
    elif a == '--cols': cols = int(args.pop(0))
args.pop(0)
cmds = ' '.join(args).split(';')
tmp = tempfile.mkdtemp()
lines = []
shots = []
if load: lines.append(f'load {os.path.abspath(load)}')
for c in cmds:
    c = c.strip()
    if not c: continue
    if c.startswith('rip '):
        pre = os.path.abspath(c.split()[1])
        lines += [f'dump 7000000 400 {pre}.oam', f'dump 6000000 18000 {pre}.vram', f'dump 5000000 400 {pre}.pal', f'dump 4000000 60 {pre}.io']
        continue
    if c == 'shot':
        f = os.path.join(tmp, f'{len(shots):03d}.ppm'); shots.append(f); c = f'shot {f}'
    lines.append(c)
if save: lines.append(f'save {os.path.abspath(save)}')
scr = os.path.join(tmp, 's.txt'); open(scr, 'w').write('\n'.join(lines) + '\n')
env = dict(os.environ)
if 'SB1PTR' not in env:
    elf = os.path.splitext(rom)[0] + '.elf'
    if os.path.exists(elf):
        nm = subprocess.run(['/opt/xpack/xpack-arm-none-eabi-gcc-14.2.1-1.1/bin/arm-none-eabi-nm', elf], capture_output=True, text=True).stdout
        for l in nm.splitlines():
            if l.endswith(' gSaveBlock1Ptr'): env['SB1PTR'] = l.split()[0]
r = subprocess.run(['/home/claude/tools/runner', rom, scr], capture_output=True, text=True, env=env)
if r.stdout: print(r.stdout, end='')
if r.stderr: print(r.stderr, end='', file=sys.stderr)
if shots:
    ims = [Image.open(f) for f in shots]
    n = len(ims); c = min(cols, n); rows = (n + c - 1) // c
    W = Image.new('RGB', (240 * c + 4 * (c - 1), 160 * rows + 4 * (rows - 1)), (255, 0, 255))
    for i, im in enumerate(ims):
        W.paste(im, ((i % c) * 244, (i // c) * 164))
    W = W.resize((W.width * scale, W.height * scale), Image.NEAREST)
    W.save(out)
    print('sheet', out, n)
