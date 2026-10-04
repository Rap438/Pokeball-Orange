#!/usr/bin/env python3
"""Helper for the PokeBall Orange script rewrite.

  text_tool.py check  FILE...   validate rewritten .inc files against the story-baseline git tag
  text_tool.py reflow FILE...   re-wrap message pages that overflow the text box (in place)
  text_tool.py dump   FILE      print every message as one line: <label>|<text>

Rules enforced by `check`:
  * everything that is not a `.string` line must be byte-identical to the baseline (labels, commands)
  * same number of messages, in the same order; every message ends with $
  * {TOKENS} are preserved per message (except {PLAYER}, which may be added/removed)
  * every character must exist in charmap.txt; every line fits the 208px text box
"""
import re, subprocess, sys, os
from collections import Counter
sys.path.insert(0, '/home/claude/tools')
from textwidth import width as _w, cm

def width(text):
    # placeholders: {KUN} is empty in English, names/numbers count as ~7 average letters
    t = text.replace('{KUN}', '')
    n = len(TOKEN.findall(t))
    return _w(TOKEN.sub('', t)) + 42 * n


PE = '/home/claude/pokeemerald'
LIMIT = 208
BASE = 'story-baseline'
TOKEN = re.compile(r'\{[^}]*\}')
STR = re.compile(r'^(\s*)\.string "(.*)"\s*$')
TOKEN = re.compile(r'\{[^}]*\}')

def split(lines):
    """-> skeleton (list of non-string lines, with a marker per message), messages [(label, [strings])]"""
    skel, msgs, label = [], [], None
    i = 0
    while i < len(lines):
        m = STR.match(lines[i])
        if not m:
            l = lines[i]
            mm = re.match(r'^(\w+):{1,2}', l)
            if mm:
                label = mm.group(1)
            skel.append(l)
            i += 1
            continue
        block = []
        while i < len(lines) and STR.match(lines[i]):
            block.append(STR.match(lines[i]).group(2))
            i += 1
            if block[-1].endswith('$'):
                break
        skel.append('\0MSG')
        msgs.append((label, block))
    return skel, msgs

def base_lines(path):
    rel = os.path.relpath(path, PE)
    out = subprocess.run(['git', '-C', PE, 'show', f'{BASE}:{rel}'], capture_output=True, text=True)
    if out.returncode != 0:
        return None
    return out.stdout.split('\n')

def chars_ok(text):
    t = TOKEN.sub('', text)
    t = t.replace('\\n', '').replace('\\l', '').replace('\\p', '')
    bad = []
    i = 0
    while i < len(t):
        for L in (3, 2, 1):
            if t[i:i + L] in cm:
                i += L
                break
        else:
            bad.append(t[i]); i += 1
    return bad

def check(path):
    errs = []
    new = open(path, encoding='utf-8').read().split('\n')
    old = base_lines(path)
    if old is None:
        return [f'{path}: not in baseline']
    sk_n, m_n = split(new)
    sk_o, m_o = split(old)
    if sk_n != sk_o:
        for k, (a, b) in enumerate(zip(sk_n, sk_o)):
            if a != b:
                errs.append(f'{path}: non-string line changed near skeleton #{k}: {b!r} -> {a!r}')
                break
        else:
            errs.append(f'{path}: skeleton length differs ({len(sk_o)} -> {len(sk_n)})')
    if len(m_n) != len(m_o):
        errs.append(f'{path}: message count {len(m_o)} -> {len(m_n)}')
    base_set = set()
    for _, bo in m_o:
        base_set.update(re.split(r'\\n|\\l|\\p', ''.join(bo).rstrip('$')))
    for (lab, bn), (_, bo) in zip(m_n, m_o):
        tn, to = ''.join(bn), ''.join(bo)
        if to.endswith('$') and not tn.endswith('$'):
            errs.append(f'{path}:{lab}: message must end with $')
        cn = Counter(t for t in TOKEN.findall(tn) if t != '{PLAYER}')
        co = Counter(t for t in TOKEN.findall(to) if t != '{PLAYER}')
        if cn != co:
            errs.append(f'{path}:{lab}: tokens changed {dict(co)} -> {dict(cn)}')
        bad = chars_ok(tn)
        if bad:
            errs.append(f'{path}:{lab}: characters not in the font: {"".join(sorted(set(bad)))!r}')
        for line in re.split(r'\\n|\\l|\\p', tn.rstrip('$')):
            if width(line) > LIMIT and line not in base_set:
                errs.append(f'{path}:{lab}: line too wide ({width(line)}px): {line!r}')
    return errs

def reflow_text(text):
    pages = text.split('\\p')
    out = []
    for page in pages:
        lines = re.split(r'\\n|\\l', page)
        if all(width(l) <= LIMIT for l in lines):
            out.append(page)
            continue
        joined = ' '.join(l.strip() for l in lines)
        joined = TOKEN.sub(lambda m: m.group(0).replace(' ', '\x01'), joined)   # keep {CLEAR_TO 10} etc. whole
        words = [w.replace('\x01', ' ') for w in joined.split(' ')]
        cur, wrapped = '', []
        for w in words:
            cand = (cur + ' ' + w) if cur else w
            if width(cand) <= LIMIT or not cur:
                cur = cand
            else:
                wrapped.append(cur); cur = w
        wrapped.append(cur)
        s = wrapped[0]
        for i, l in enumerate(wrapped[1:]):
            s += ('\\n' if i == 0 else '\\l') + l
        out.append(s)
    return '\\p'.join(out)

def emit(text, indent):
    parts = re.split(r'(\\n|\\l|\\p)', text)
    lines, cur = [], ''
    for p in parts:
        if p in ('\\n', '\\l', '\\p'):
            lines.append(cur + p); cur = ''
        else:
            cur += p
    lines.append(cur)
    return [f'{indent}.string "{l}"' for l in lines if l != '']

def reflow(path):
    lines = open(path, encoding='utf-8').read().split('\n')
    out, i, n = [], 0, 0
    while i < len(lines):
        m = STR.match(lines[i])
        if not m:
            out.append(lines[i]); i += 1; continue
        indent = m.group(1)
        block = []
        while i < len(lines) and STR.match(lines[i]):
            block.append(lines[i]); i += 1
            if STR.match(block[-1]).group(2).endswith('$'):
                break
        text = ''.join(STR.match(b).group(2) for b in block)
        end = '$' if text.endswith('$') else ''
        body = text[:-1] if end else text
        nb = reflow_text(body)
        if nb != body:
            n += 1
            out += emit(nb + end, indent)
        else:
            out += block
    open(path, 'w', encoding='utf-8').write('\n'.join(out))
    return n

if __name__ == '__main__':
    cmd, files = sys.argv[1], sys.argv[2:]
    if cmd == 'check':
        allerr = []
        for f in files:
            allerr += check(f)
        print('\n'.join(allerr) if allerr else f'OK ({len(files)} files)')
        sys.exit(1 if allerr else 0)
    elif cmd == 'reflow':
        for f in files:
            print(f, reflow(f), 'messages rewrapped')
    elif cmd == 'dump':
        _, msgs = split(open(files[0], encoding='utf-8').read().split('\n'))
        for lab, b in msgs:
            print(f'{lab}|{"".join(b)}')
