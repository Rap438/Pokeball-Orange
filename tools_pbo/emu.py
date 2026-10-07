#!/usr/bin/env python3
"""Interactive driver for the headless runner (tools_pbo/runner, built by tools_pbo/build.sh).

    from emu import Emu
    e = Emu('pokeemerald.gba')
    e.run(600); e.press('A'); e.shot('out.png')
    e.sym('gSaveBlock1Ptr'); e.peek32(addr)

Symbols come from the .elf next to the ROM (arm-none-eabi-nm).
"""
import os, re, subprocess, tempfile, itertools, struct
from collections import deque
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
MAP_OFFSET = 7

def _behaviors():
    """metatile behavior name -> value, from the enum in include/constants/metatile_behaviors.h"""
    txt = open(os.path.join(ROOT, 'include/constants/metatile_behaviors.h')).read()
    body = txt[txt.index('{') + 1:txt.index('}')]
    vals, n = {}, 0
    for item in body.split(','):
        item = re.sub(r'//.*', '', item).strip()
        if not item:
            continue
        name, _, v = item.partition('=')
        if v.strip():
            n = int(v.strip(), 0)
        vals[name.strip()] = n
        n += 1
    return vals

_B = _behaviors()
# not walkable without a field move: water (except shallow), waterfalls, ledges (one way)
BLOCKED_BEHAVIORS = {v for k, v in _B.items()
                     if ('WATER' in k and 'SHALLOW' not in k and 'PUDDLE' not in k and 'DOOR' not in k and 'WARP' not in k)
                     or k.startswith('MB_JUMP_') or k == 'MB_WATERFALL'}

class Emu:
    def __init__(self, rom, sav=None, runner=None):
        self.rom = os.path.abspath(rom)
        env = dict(os.environ)
        if sav:
            env['SAV'] = os.path.abspath(sav)
        self.syms = {}
        elf = os.path.splitext(self.rom)[0] + '.elf'
        if os.path.exists(elf):
            nm = subprocess.run(['arm-none-eabi-nm', elf], capture_output=True, text=True).stdout
            for l in nm.splitlines():
                p = l.split()
                if len(p) == 3:
                    self.syms[p[2]] = int(p[0], 16)
            if 'gSaveBlock1Ptr' in self.syms:
                env['SB1PTR'] = f"{self.syms['gSaveBlock1Ptr']:x}"
        self.p = subprocess.Popen([runner or os.path.join(HERE, 'runner'), self.rom, '-'], stdin=subprocess.PIPE,
                                  stdout=subprocess.PIPE, text=True, bufsize=1, env=env)
        self.tmp = tempfile.mkdtemp()
        self._n = itertools.count()

    def cmd(self, line):
        """send a runner command, wait for it, return its output lines"""
        tag = f'@@{next(self._n)}'
        self.p.stdin.write(line + '\n' + f'echo {tag}\n')
        self.p.stdin.flush()
        out = []
        while True:
            l = self.p.stdout.readline()
            if not l:
                raise RuntimeError('runner exited')
            if l.strip() == tag:
                return out
            out.append(l.rstrip('\n'))

    def run(self, n):
        self.cmd(f'run {n}')

    def press(self, keys, n=2, after=0):
        self.cmd(f'press {keys} {n}')
        if after:
            self.run(after)

    def hold(self, keys, n):
        self.cmd(f'hold {keys} {n}')

    def walk(self, d, steps, frames=16):
        for _ in range(steps):
            self.hold(d, frames)
        self.run(8)

    def shot(self, path=None, scale=2):
        ppm = os.path.join(self.tmp, f'{next(self._n)}.ppm')
        self.cmd(f'shot {ppm}')
        im = Image.open(ppm)
        if path:
            im.resize((im.width * scale, im.height * scale), Image.NEAREST).save(path)
        return im

    def save(self, path):
        self.cmd(f'save {os.path.abspath(path)}')

    def load(self, path):
        self.cmd(f'load {os.path.abspath(path)}')

    def sym(self, name):
        return self.syms[name]

    def _peek(self, kind, addr):
        out = self.cmd(f'{kind} {addr:x}')
        return int(out[-1].split(': ')[1], 16)

    def peek8(self, addr): return self._peek('peek8', addr)
    def peek16(self, addr): return self._peek('peek16', addr)
    def peek32(self, addr): return self._peek('peek32', addr)
    def poke8(self, addr, v): self.cmd(f'poke8 {addr:x} {v:x}')
    def poke16(self, addr, v): self.cmd(f'poke16 {addr:x} {v:x}')
    def poke32(self, addr, v): self.cmd(f'poke32 {addr:x} {v:x}')

    def read(self, addr, n):
        return bytes(self.peek8(addr + i) for i in range(n))

    def pos(self):
        """player map coordinates (gSaveBlock1Ptr->pos) and (map group, map num)"""
        sb = self.peek32(self.sym('gSaveBlock1Ptr'))
        x, y = self.peek16(sb), self.peek16(sb + 2)
        loc = self.peek16(sb + 4)
        return (x - (x >> 15 << 16), y - (y >> 15 << 16)), (loc & 0xFF, loc >> 8)

    def walk_to(self, x, y, tries=40):
        """step towards (x, y), x first then y; no pathfinding. Returns True when there."""
        for _ in range(tries):
            (px, py), _m = self.pos()
            if (px, py) == (x, y):
                return True
            if px != x:
                self.hold('RIGHT' if x > px else 'LEFT', 16)
            else:
                self.hold('DOWN' if y > py else 'UP', 16)
            self.run(4)
        return False

    def dump(self, addr, n):
        f = os.path.join(self.tmp, f'{next(self._n)}.bin')
        self.cmd(f'dump {addr:x} {n:x} {f}')
        return open(f, 'rb').read()

    def grid(self):
        """walkable[y][x] for the current map, from the live map grid, metatile behaviors and NPCs"""
        bml = self.dump(self.sym('gBackupMapLayout'), 12)
        w, h, mp = struct.unpack('<iiI', bml)
        cells = struct.unpack(f'<{w * h}H', self.dump(mp, w * h * 2))
        layout = self.peek32(self.sym('gMapHeader'))
        attrs = []
        for off in (0x10, 0x14):
            ts = self.peek32(layout + off)
            ap = self.peek32(ts + 0x10)
            attrs.append(struct.unpack('<512H', self.dump(ap, 1024)))
        npcs = set()
        objs = self.dump(self.sym('gObjectEvents'), 16 * 0x24)
        for i in range(16):
            o = objs[i * 0x24:(i + 1) * 0x24]
            if o[0] & 1 and not o[2] & 1:
                x, y = struct.unpack_from('<hh', o, 0x10)
                npcs.add((x - MAP_OFFSET, y - MAP_OFFSET))
        W, H = w - 2 * MAP_OFFSET, h - 2 * MAP_OFFSET
        ok = [[False] * W for _ in range(H)]
        for y in range(H):
            for x in range(W):
                c = cells[(y + MAP_OFFSET) * w + x + MAP_OFFSET]
                mid = c & 0x3FF
                beh = (attrs[0][mid] if mid < 512 else attrs[1][mid - 512]) & 0xFF
                ok[y][x] = not (c & 0x0C00) and beh not in BLOCKED_BEHAVIORS and (x, y) not in npcs
        return ok

    def goto(self, x, y, tries=6):
        """walk to (x, y) on the current map by BFS over the live collision grid. True when there."""
        for _ in range(tries):
            (px, py), m0 = self.pos()
            if (px, py) == (x, y):
                return True
            ok = self.grid()
            ok[y][x] = True
            prev = {(px, py): None}
            q = deque([(px, py)])
            while q:
                c = q.popleft()
                if c == (x, y):
                    break
                for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    n = (c[0] + dx, c[1] + dy)
                    if 0 <= n[1] < len(ok) and 0 <= n[0] < len(ok[0]) and ok[n[1]][n[0]] and n not in prev:
                        prev[n] = c; q.append(n)
            if (x, y) not in prev:
                return False
            path, c = [], (x, y)
            while c != (px, py):
                path.append(c); c = prev[c]
            for nx, ny in reversed(path):
                (cx, cy), m = self.pos()
                if m != m0:
                    return False
                d = 'RIGHT' if nx > cx else 'LEFT' if nx < cx else 'DOWN' if ny > cy else 'UP'
                self.hold(d, 16); self.run(2)
                if self.pos()[0] != (nx, ny):
                    self.run(30)
                    break   # blocked (NPC moved in, or script): re-plan
        return self.pos()[0] == (x, y)

    def face(self, d):
        self.hold(d, 3); self.run(10)

    def close(self):
        try:
            self.p.stdin.close(); self.p.wait(5)
        except Exception:
            self.p.kill()

def sheet(images, path, cols=4, scale=2):
    """paste screenshots into one contact sheet"""
    n = len(images); c = min(cols, n); rows = (n + c - 1) // c
    W = Image.new('RGB', (240 * c + 4 * (c - 1), 160 * rows + 4 * (rows - 1)), (255, 0, 255))
    for i, im in enumerate(images):
        W.paste(im, ((i % c) * 244, (i // c) * 164))
    W.resize((W.width * scale, W.height * scale), Image.NEAREST).save(path)
