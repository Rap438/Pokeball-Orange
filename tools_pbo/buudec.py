#!/usr/bin/env python3
"""Port of the Buu's Fury IWRAM decompressor (0x03000040)."""
import struct

class Stream:
    def __init__(self, rom, off):
        self.rom = rom; self.p = off; self.r6 = 0; self.sl = 0
    def word(self):
        v = struct.unpack_from('<I', self.rom, self.p)[0]; self.p += 4; return v
    def bit(self):
        self.sl -= 1
        if self.sl < 0:
            self.r6 = self.word(); self.sl = 31
        c = self.r6 >> 31; self.r6 = (self.r6 << 1) & 0xFFFFFFFF
        return c
    def bits(self, n):
        if self.sl == 0:
            self.r6 = self.word(); self.sl = 32
        fp = min(n, self.sl)
        r7 = self.r6 >> (32 - fp) if fp else 0
        self.r6 = (self.r6 << fp) & 0xFFFFFFFF if fp < 32 else 0
        self.sl -= fp; n -= fp
        if n == 0: return r7
        self.r6 = self.word(); self.sl = 32 - n
        r7 = (self.r6 >> (32 - n)) + (r7 << n)
        self.r6 = (self.r6 << n) & 0xFFFFFFFF
        return r7
    def gamma(self):
        r9 = 1
        while True:
            r9 = r9 * 2 + self.bit()
            if r9 > 0x40000: raise ValueError('gamma')
            if not self.bit(): return r9

def decompress(rom, off, r3=8, r4=0, maxlen=1 << 20):
    s = Stream(rom, off)
    out = bytearray()
    r5 = 8; ip = 1
    while True:
        if len(out) > maxlen: raise ValueError('too long')
        if s.bit():
            out.append((s.bits(r3) + r4) & 0xFF); continue
        if s.bit():
            r9 = s.gamma()
            if r9 == 2:
                r9 = s.gamma()
            else:
                r9 -= 3
                r7 = s.bits(r5)
                ip = r7 + (r9 << r5)
                r9 = s.gamma()
                if ip >= 0x10000: r9 += 3
                elif ip >= 0x37ff: r9 += 2
                elif ip >= 0x27f: r9 += 1
                elif ip <= 127: r9 += 4
            if ip > len(out): raise ValueError('bad offset')
            for _ in range(r9): out.append(out[-ip])
            continue
        if s.bit():
            r7 = s.bits(4) - 1
            if r7 == 0: out.append(0)
            elif r7 > 0:
                if r7 > len(out): raise ValueError('bad short')
                out.append(out[-r7])
            else:
                if s.bit():
                    while True:
                        if len(out) > maxlen: raise ValueError('too long')
                        for _ in range(256): out.append(s.bits(8))
                        if not s.bit(): break
                else:
                    r4 = 0
                    r3 = 7 + s.bit()
                    if r3 != 8:
                        r4 = s.bits(8)
            continue
        r7 = s.bits(7)
        if r7:
            ip = r7; r9 = s.bits(2) + 2
            if ip > len(out): raise ValueError('bad offset7')
            for _ in range(r9): out.append(out[-ip])
        else:
            r7 = s.bits(2)
            if r7 == 0: return bytes(out), s.p - off
            r5 = s.bits(r7 + 3)

if __name__ == '__main__':
    import sys
    rom = open(sys.argv[1], 'rb').read()
    o = int(sys.argv[2], 16)
    d, n = decompress(rom, o)
    print(len(d), n)
