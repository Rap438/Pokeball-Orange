#!/usr/bin/env python3
"""Pixel width of pokeemerald .string text in FONT_NORMAL; report message-box lines that overflow."""
import re, glob, sys
PE = '/home/claude/pokeemerald'
cm = {}
for line in open(f'{PE}/charmap.txt', encoding='utf-8'):
    m = re.match(r"^'(.+?)'\s*=\s*([0-9A-F]{2})\s*$", line)
    if m:
        ch = m.group(1).replace("\\'", "'")
        cm.setdefault(ch, int(m.group(2), 16))
src = open(f'{PE}/src/fonts.c').read()
tab = src[src.index('gFontNormalLatinGlyphWidths[] = {'):]
tab = tab[:tab.index('};')]
W = [int(x) for x in re.findall(r'\b(\d+)\b', tab.split('{', 1)[1])]

def width(text):
    text = re.sub(r'\{[^}]*\}', 'MMMMMMM', text)  # placeholders like {PLAYER}/{STR_VAR_1}: assume 7 wide chars
    w = 0
    i = 0
    while i < len(text):
        for L in (3, 2, 1):
            c = text[i:i + L]
            if c in cm:
                w += W[cm[c]] if cm[c] < len(W) else 6
                i += L
                break
        else:
            w += 6; i += 1
    return w

def lines_of(s):
    s = s.replace('\\n', '\n').replace('\\l', '\n').replace('\\p', '\n').replace('$', '')
    return s.split('\n')

if __name__ == '__main__':
    limit = int(sys.argv[1]) if len(sys.argv) > 1 else 208
    files = sys.argv[2:] or glob.glob(f'{PE}/data/maps/*/*.inc') + glob.glob(f'{PE}/data/scripts/*.inc') + glob.glob(f'{PE}/data/text/*.inc')
    for f in files:
        for n, line in enumerate(open(f), 1):
            m = re.search(r'\.string "(.*)"', line)
            if not m: continue
            for seg in lines_of(m.group(1)):
                if width(seg) > limit:
                    print(f'{f.replace(PE + "/", "")}:{n}: {width(seg)}px: {seg}')
