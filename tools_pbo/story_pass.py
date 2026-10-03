#!/usr/bin/env python3
"""PokeBall Orange story/text pass: swap Emerald's cast, teams and companies for the DBZ versions.
Rewraps any message page that no longer fits the text box. Idempotent (works from pristine copies)."""
import re, glob, os, shutil, sys
sys.path.insert(0, '/home/claude/tools')
from textwidth import width

PE = '/home/claude/pokeemerald'
ORIG = '/home/claude/restyle/orig_text'
LIMIT = 208

# order matters: longer / more specific phrases first
SWAPS = [
    # family -> Goku's circle
    (r"You're your father's child", "You're KRILLIN's best buddy"),
    (r"NORMAN's child", "KRILLIN's best buddy"),
    (r"NORMAN's kid", "KRILLIN's best buddy"),
    (r"your father's", "KRILLIN's"), (r"Your father's", "KRILLIN's"),
    (r"your father", "KRILLIN"), (r"Your father", "KRILLIN"),
    (r"your DAD", "KRILLIN"), (r"Your DAD", "KRILLIN"),
    (r"your MOM", "CHI-CHI"), (r"Your MOM", "CHI-CHI"),
    (r"\bDAD\b", "KRILLIN"), (r"\bMOM\b", "CHI-CHI"),
    (r"\bNORMAN\b", "KRILLIN"),
    # professor / rival / friend
    (r"PROF\. BIRCH", "MASTER ROSHI"), (r"\bPROF\.\s?BIRCH\b", "MASTER ROSHI"), (r"\bBIRCH\b", "ROSHI"),
    (r"\bMAY\b", "VEGETA"), (r"\bBRENDAN\b", "VEGETA"),
    (r"\bWALLY\b", "UUB"),
    # gym leaders, elite four, champion
    (r"\bROXANNE\b", "YAMCHA"), (r"\bBRAWLY\b", "TIEN"), (r"\bWATTSON\b", "BULMA"), (r"\bFLANNERY\b", "VIDEL"),
    (r"\bWINONA\b", "SAIYAMAN"), (r"\bTATE\b", "GOTEN"), (r"\bLIZA\b", "TRUNKS"), (r"\bJUAN\b", "PICCOLO"),
    (r"\bSIDNEY\b", "SUPER BUU"), (r"\bPHOEBE\b", "BABA"), (r"\bGLACIA\b", "ANDROID 18"), (r"\bDRAKE\b", "DENDE"),
    (r"\bWALLACE\b", "MR. SATAN"),
    (r"STEVEN STONE", "SUPREME KAI"), (r"\bSTEVEN\b", "SUPREME KAI"),
    (r"MR\. STONE", "DR. BRIEF"),
    (r"\bSCOTT\b", "ANNOUNCER"),
    # villains
    (r"TEAM MAGMA", "MAJIN ARMY"), (r"TEAM AQUA", "SAIYAN ARMY"),
    (r"\bMAGMA\b", "MAJIN"), (r"\bAQUA\b", "SAIYAN"),
    (r"\bMAXIE\b", "MAJIN BUU"), (r"\bARCHIE\b", "BROLY"),
    (r"\bTABITHA\b", "DABURA"), (r"\bCOURTNEY\b", "PUI PUI"), (r"\bMATT\b", "NAPPA"), (r"\bSHELLY\b", "RADITZ"),
    # companies
    (r"DEVON CORPORATION", "CAPSULE CORP."), (r"DEVON CORP\.", "CAPSULE CORP."), (r"\bDEVON\b", "CAPSULE CORP."),
    (r"CAPSULE CORP\.\.", "CAPSULE CORP."),
]
COMPILED = [(re.compile(a), b) for a, b in SWAPS]

def swap(t):
    for rx, b in COMPILED:
        t = rx.sub(b, t)
    return t

def orig(path):
    o = os.path.join(ORIG, os.path.relpath(path, PE))
    if not os.path.exists(o):
        os.makedirs(os.path.dirname(o), exist_ok=True)
        shutil.copy2(path, o)
    return o

TOKEN = re.compile(r'(\\n|\\l|\\p)')

def reflow(text):
    """text without trailing '$'. Re-wrap pages that overflow."""
    pages = text.split('\\p')
    out = []
    for page in pages:
        lines = re.split(r'\\n|\\l', page)
        if all(width(l) <= LIMIT for l in lines):
            out.append(page)
            continue
        words = ' '.join(l.strip() for l in lines).split(' ')
        cur, wrapped = '', []
        for w in words:
            cand = (cur + ' ' + w) if cur else w
            if width(cand) <= LIMIT or not cur:
                cur = cand
            else:
                wrapped.append(cur); cur = w
        wrapped.append(cur)
        page_s = wrapped[0]
        for i, l in enumerate(wrapped[1:]):
            page_s += ('\\n' if i == 0 else '\\l') + l
        out.append(page_s)
    return '\\p'.join(out)

def emit(text, indent='\t'):
    # one .string per line segment, break codes kept at the end of each line
    parts = TOKEN.split(text)
    lines, cur = [], ''
    for p in parts:
        if p in ('\\n', '\\l', '\\p'):
            lines.append(cur + p); cur = ''
        else:
            cur += p
    lines.append(cur)
    lines[-1] += '$'
    return [f'{indent}.string "{l}"' for l in lines if l != '']

def process_inc(path):
    src = open(orig(path), encoding='utf-8').read().split('\n')
    out, i, changed, reflowed = [], 0, 0, 0
    while i < len(src):
        line = src[i]
        if '.string "' not in line:
            out.append(line); i += 1; continue
        # gather a full message block (until a string containing $)
        block = []
        while i < len(src) and '.string "' in src[i]:
            block.append(src[i]); i += 1
            if re.search(r'\$"\s*$', block[-1]) or block[-1].rstrip().endswith('$"'):
                break
        texts = [re.search(r'\.string "(.*)"', b).group(1) for b in block]
        full = ''.join(texts)
        new = swap(full)
        if new == full or not full.endswith('$'):
            if new != full:
                # unterminated block (rare): plain per-line swap
                out += [b.replace(t, swap(t)) for b, t in zip(block, texts)]
                changed += 1
            else:
                out += block
            continue
        changed += 1
        body = new[:-1]
        rb = reflow(body)
        if rb != body:
            reflowed += 1
        indent = re.match(r'(\s*)', block[0]).group(1)
        out += emit(rb, indent)
    open(path, 'w', encoding='utf-8').write('\n'.join(out))
    return changed, reflowed

def process_c(path):
    s = open(orig(path), encoding='utf-8').read()
    n = 0
    def rep(m):
        nonlocal n
        t = swap(m.group(2))
        if t != m.group(2): n += 1
        return m.group(1) + t + m.group(3)
    s2 = re.sub(r'(_\(")((?:[^"\\]|\\.)*)("\))', rep, s)
    open(path, 'w', encoding='utf-8').write(s2)
    return n

if __name__ == '__main__':
    tot_c = tot_r = 0
    files = sorted(glob.glob(f'{PE}/data/maps/*/scripts.inc') + glob.glob(f'{PE}/data/maps/*/text.inc')
                   + glob.glob(f'{PE}/data/text/*.inc') + glob.glob(f'{PE}/data/scripts/*.inc'))
    for f in files:
        if os.path.basename(f).startswith('dbz_'):
            continue
        c, r = process_inc(f)
        tot_c += c; tot_r += r
    cn = 0
    for f in [f'{PE}/src/strings.c', f'{PE}/src/battle_message.c', f'{PE}/src/data/trainers.h',
              f'{PE}/src/pokenav_match_call_data.c', f'{PE}/src/data/text/match_call_messages.h',
              f'{PE}/src/match_call.c', f'{PE}/src/data/text/trainer_class_names.h']:
        if os.path.exists(f):
            cn += process_c(f)
    # names that must fit fixed-size fields
    fixes = {f'{PE}/src/data/trainers.h': [('_("SUPREME KAI")', '_("SHIN")')],
             f'{PE}/src/data/text/trainer_class_names.h': [('_("SAIYAN LEADER")', '_("SAIYAN BOSS")'),
                                                           ('_("MAJIN LEADER")', '_("MAJIN BOSS")')]}
    for f, reps in fixes.items():
        t = open(f).read()
        for a, b in reps: t = t.replace(a, b)
        t = t.replace('_("GOTEN&TRUNKS")', '_("GOTEN&TRKS")')
        open(f, 'w').write(t)
    print(f'message blocks changed {tot_c}, rewrapped {tot_r}, C strings {cn}')
