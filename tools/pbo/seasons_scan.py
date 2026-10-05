#!/usr/bin/env python3
"""Shared helpers: find INCGFX/INCBIN declarations in the source and which ones change per season."""
import re, os, glob, hashlib, json
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SEASONS = ['spring', 'summer', 'autumn', 'winter']
BASE = 'summer'
SEASON_DIR = 'graphics/seasons'

INC = r'(INCGFX(?:_U8|_U16|_U32|_COMP)?|INCBIN(?:_U8|_U16|_U32|_S8|_S16|_S32)?)\(\s*"([^"]+)"((?:\s*,\s*"[^"]*")*)\s*\)'
DECL = re.compile(r'^(static\s+)?const\s+(u8|u16|u32|s8|s16|s32)\s+(?:ALIGNED\(\d+\)\s+)?(\w+)((?:\[[^\]]*\])+)\s*(?:ALIGNED\(\d+\)\s*)?=\s*([^;]*?);[ \t]*(?://[^\n]*)?$', re.M | re.S)

def variant_path(season, path):
    if season == BASE:
        return path
    p = os.path.join(SEASON_DIR, season, path)
    return p if os.path.exists(os.path.join(ROOT, p)) else path

def _png_indices(p):
    im = Image.open(p)
    return im.mode, im.size, im.tobytes()

def _png_pal(p):
    im = Image.open(p)
    pal = im.getpalette() or []
    return bytes(pal)

def output_differs(path, ext, season):
    """Would INCGFX(path, ext) produce different bytes in this season than in the base?"""
    v = variant_path(season, path)
    if v == path:
        return False
    a, b = os.path.join(ROOT, path), os.path.join(ROOT, v)
    if path.endswith('.png'):
        if '.gbapal' in ext or ext.endswith('pal'):
            return _png_pal(a) != _png_pal(b)
        return _png_indices(a) != _png_indices(b)
    if path.endswith('.pal'):
        norm = lambda f: open(f).read().replace('\r', '').strip().split()
        return norm(a) != norm(b)
    return open(a, 'rb').read() != open(b, 'rb').read()

def frlg_lines(txt):
    """Per line: True when the line is compiled only for FRLG (this game is built as Emerald)."""
    out, stack = [], []
    for line in txt.split('\n'):
        t = line.strip()
        if t.startswith('#if'):
            cond = t[3:].strip()
            if 'IS_FRLG' in cond:
                stack.append('frlg' if not cond.startswith('!') and '!IS_FRLG' not in cond else 'emerald')
            else:
                stack.append('other')
        elif t.startswith('#else') and stack:
            stack[-1] = {'frlg': 'emerald', 'emerald': 'frlg'}.get(stack[-1], 'other')
        elif t.startswith('#endif') and stack:
            stack.pop()
        out.append('frlg' in stack)
    return out

def scan_decls(files=None):
    """Yield (file, static, type, name, dims, [(macro, path, ext, args)...], fulltext) for every const array built from INC macros."""
    if files is None:
        files = glob.glob(os.path.join(ROOT, 'src/**/*.c'), recursive=True) + glob.glob(os.path.join(ROOT, 'src/**/*.h'), recursive=True)
    for f in files:
        rel = os.path.relpath(f, ROOT)
        if 'seasons_gen' in rel:
            continue
        txt = open(f, encoding='utf-8', errors='replace').read()
        frlg = frlg_lines(txt)
        for m in DECL.finditer(txt):
            if frlg[txt.count('\n', 0, m.start())]:
                continue
            body = m.group(5)
            incs = []
            for im in re.finditer(INC, body):
                args = re.findall(r'"([^"]*)"', im.group(3))
                ext = args[0] if args else ''
                incs.append((im.group(1), im.group(2), ext, args[1:]))
            if not incs:
                continue
            yield rel, bool(m.group(1)), m.group(2), m.group(3), m.group(4), incs, m.group(0), body
