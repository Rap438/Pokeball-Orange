#!/usr/bin/env python3
"""PokeBall Orange: rename items to Dragon Ball Z flavour. Effects stay exactly the same.
Rewrites item names + descriptions and swaps the old names in dialogue text (idempotent)."""
import re, glob, os
PE = '/home/claude/pokeemerald'

# ITEM constant: (new name, description lines or None to keep, swap name in dialogue?)
R = {
    'POTION':       ('SENZU SPROUT', ['A tiny sprout from', 'KORIN\'s garden.', 'Restores 20 HP.'], True),
    'SUPER_POTION': ('SENZU LEAF',   ['A leaf of the', 'SENZU plant.', 'Restores 50 HP.'], True),
    'HYPER_POTION': ('KORIN WATER',  ['Water from atop', 'KORIN TOWER.', 'Restores 200 HP.'], True),
    'MAX_POTION':   ('SACRED WATER', ['Ultra-holy water.', 'Fully restores the', 'HP of a POKéMON.'], True),
    'FULL_RESTORE': ('SENZU BEAN',   ['One bean heals all', 'HP and status', 'problems at once.'], True),
    'REVIVE':       ("DENDE'S HEAL", ["DENDE's healing", 'power. Revives a', 'fainted POKéMON.'], True),
    'MAX_REVIVE':   ("KAMI'S HEAL",  ["KAMI's own power.", 'Revives a fainted', 'POKéMON fully.'], True),
    'FULL_HEAL':    ('MEDI CAPSULE', ['A CAPSULE CORP.', 'remedy that heals', 'any status problem.'], True),
    'ETHER':        ('KI DROP',      ['Restores 10 PP of', 'one move with a', 'burst of ki.'], True),
    'MAX_ETHER':    ('KI FLASK',     ['Fully restores the', 'PP of one move', 'with stored ki.'], True),
    'ELIXIR':       ('KI SPRING',    ['Restores 10 PP of', 'every move with a', 'surge of ki.'], True),
    'MAX_ELIXIR':   ('KI FOUNTAIN',  ['Fully restores the', 'PP of all moves', 'with pure ki.'], True),
    'RARE_CANDY':   ('ZENKAI CANDY', ['A SAIYAN-style', 'power surge. Raises', 'a level by one.'], True),
    'HP_UP':        ('TURTLE SHELL', ["ROSHI's training", 'shell. Raises the', 'base HP.'], False),
    'PROTEIN':      ('WEIGHTED GI',  ['A heavy training', 'gi. Raises the base', 'ATTACK stat.'], False),
    'IRON':         ('SAIYAN ARMOR', ['SAIYAN battle gear.', 'Raises the base', 'DEFENSE stat.'], False),
    'CALCIUM':      ('KI SCROLL',    ['Secret ki arts.', 'Raises the base', 'SP. ATK stat.'], False),
    'ZINC':         ('SPIRIT CHARM', ['A calming charm.', 'Raises the base', 'SP. DEF stat.'], False),
    'CARBOS':       ('KAIO WEIGHTS', ["KING KAI's gravity", 'training. Raises', 'the base SPEED.'], False),
    'PP_UP':        ('KI BOOST',     ['Slightly raises', 'the max PP of a', 'selected move.'], True),
    'PP_MAX':       ('KI MAX',       ['Raises the PP of a', 'move to its', 'maximum.'], True),
    'REPEL':        ('KI HIDER',     ['Hides your ki so', 'weak wild POKéMON', 'stay away (100).'], True),
    'SUPER_REPEL':  ('KI CLOAK',     ['Hides your ki so', 'weak wild POKéMON', 'stay away (200).'], True),
    'MAX_REPEL':    ('KI VOID',      ['Hides your ki so', 'weak wild POKéMON', 'stay away (250).'], True),
    'ESCAPE_ROPE':  ('INST. TRANS.', ['INSTANT TRANSMIS-', 'SION. Escape from', 'caves and dungeons.'], True),
    'POKE_DOLL':    ('SAIBA DOLL',   ['A SAIBAMAN decoy.', 'Use it to flee', 'any wild battle.'], True),
    'X_ATTACK':     ('KAIO-KEN',     ['Raises ATTACK', 'during one battle.', 'Times two!'], True),
    'X_DEFEND':     ('KI BARRIER',   ['Raises DEFENSE', 'during one battle.', 'Wears off after.'], True),
    'X_SPEED':      ('ZANZOKEN',     ['Afterimage speed!', 'Raises SPEED during', 'one battle.'], True),
    'X_SPECIAL':    ('KI CHARGE',    ['Raises SP. ATK', 'during one battle.', 'Wears off after.'], True),
    'X_ACCURACY':   ('SCOUTER LOCK', ['Locks on target.', 'Raises accuracy', 'during one battle.'], True),
    'DIRE_HIT':     ('FOCUS FIST',   ['Raises the', 'critical-hit ratio', 'in one battle.'], True),
    'GUARD_SPEC':   ('AURA GUARD',   ['Prevents stat', 'reduction for five', 'turns in battle.'], True),
    'LEFTOVERS':    ('SENZU POUCH',  ['A hold item that', 'restores HP a bit', 'every turn.'], True),
    'FOCUS_BAND':   ('SAIYAN PRIDE', ['A hold item that', 'may let the holder', 'endure a KO hit.'], True),
    'EXP_SHARE':    ('TRAINING GI',  ['A hold item that', 'shares battle EXP.', 'points.'], True),
    'CHOICE_BAND':  ('MAJIN MARK',   ['Powers up a move', 'but only that move', 'can be used.'], True),
    'NUGGET':       ('ZENI POUCH',   ['A pouch stuffed', 'with ZENI. Sells', 'for a high price.'], True),
    'DEVON_GOODS':  ('CC GOODS',     ['Parts ordered by', 'CAPSULE CORP. for', 'delivery.'], True),
    'DEVON_SCOPE':  ('SCOUTER',      ['A SAIYAN SCOUTER.', 'Spots unseeable', 'POKéMON nearby.'], True),
    'MAGMA_EMBLEM': ('RR EMBLEM',    ["The mark of", "KID BUU's RED", 'RIBBON ARMY.'], True),
    'MACH_BIKE':    ('CAPSULE BIKE', ['A folding bike in', 'a capsule. Fast,', 'smooth riding.'], True),
    'ACRO_BIKE':    ('HOVER BIKE',   ['A CAPSULE CORP.', 'hover bike for', 'tricks and jumps.'], True),
}

def c_string(lines):
    return '\n'.join(f'    "{l}{"\\n" if i < len(lines) - 1 else ""}"' for i, l in enumerate(lines))

items_p = f'{PE}/src/data/items.h'; items = open(items_p).read()
desc_p = f'{PE}/src/data/text/item_descriptions.h'; descs = open(desc_p).read()
old_names = {}
for const, (name, desc, _) in R.items():
    assert len(name) <= 12, name
    m = re.search(r'\[ITEM_' + const + r'\]\s*=\s*\{(.*?)\n    \},', items, re.S)
    assert m, const
    block = m.group(1)
    nm = re.search(r'\.name = _\("(.*?)"\)', block)
    old_names[const] = nm.group(1)
    dsym = re.search(r'\.description = (\w+)', block).group(1)
    nb = block.replace(nm.group(0), f'.name = _("{name}")')
    items = items[:m.start(1)] + nb + items[m.end(1):]
    if desc:
        for l in desc:
            assert len(l) <= 19, (const, l)
        descs = re.sub(r'(static const u8 ' + dsym + r'\[\] = _\(\n).*?(\);)',
                       lambda mm: mm.group(1) + c_string(desc) + mm.group(2), descs, count=1, flags=re.S)
open(items_p, 'w').write(items)
open(desc_p, 'w').write(descs)

# dialogue: swap old names (longest first) inside .string lines and C _("...") strings
swaps = sorted([(old_names[c], R[c][0]) for c in R if R[c][2] and old_names[c] != R[c][0]], key=lambda x: -len(x[0]))
def swap_text(t):
    for old, new in swaps:
        t = re.sub(r'(?<![A-Z.])' + re.escape(old) + r'(?![A-Z])', new, t)
    return t
files = glob.glob(f'{PE}/data/maps/*/*.inc') + glob.glob(f'{PE}/data/scripts/*.inc') + glob.glob(f'{PE}/data/text/*.inc')
n = 0
for f in files:
    s = open(f).read()
    out = []
    for line in s.split('\n'):
        if '.string "' in line:
            nl = swap_text(line)
            if nl != line: n += 1
            line = nl
        out.append(line)
    ns = '\n'.join(out)
    if ns != s: open(f, 'w').write(ns)
print('renamed', len(R), 'items; dialogue lines changed:', n)
print({k: v for k, v in old_names.items() if v == R[k][0]})
