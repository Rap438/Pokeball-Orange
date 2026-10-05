#!/usr/bin/env python3
"""PokeBall Orange: Dragon Ball flavour names for existing items (expansion format).
Effects, prices and pockets are untouched; only the bag name and description change.
Idempotent: run from the repo root."""
import re

# ITEM constant: (name, description lines)
R = {
    'POTION':       ('Senzu Sprout', ["A tiny sprout from", "Korin's garden.", "Restores 20 HP."]),
    'SUPER_POTION': ('Senzu Leaf',   ["A leaf of the", "Senzu plant.", "Restores 60 HP."]),
    'HYPER_POTION': ('Korin Water',  ["Water from atop", "Korin Tower.", "Restores 120 HP."]),
    'MAX_POTION':   ('Sacred Water', ["Ultra-holy water.", "Fully restores the", "HP of a Pokémon."]),
    'FULL_RESTORE': ('Senzu Bean',   ["One bean heals all", "HP and status", "problems at once."]),
    'REVIVE':       ("Dende's Heal", ["Dende's healing", "power. Revives a", "fainted Pokémon."]),
    'MAX_REVIVE':   ("Kami's Heal",  ["Kami's own power.", "Fully revives a", "fainted Pokémon."]),
    'FULL_HEAL':    ('Medi Capsule', ["A Capsule Corp.", "remedy that heals", "any status problem."]),
    'ETHER':        ('Ki Drop',      ["Restores 10 PP of", "one move with a", "burst of ki."]),
    'MAX_ETHER':    ('Ki Flask',     ["Fully restores the", "PP of one move", "with stored ki."]),
    'ELIXIR':       ('Ki Spring',    ["Restores 10 PP of", "every move with a", "surge of ki."]),
    'MAX_ELIXIR':   ('Ki Fountain',  ["Fully restores the", "PP of all moves", "with pure ki."]),
    'RARE_CANDY':   ('Zenkai Candy', ["A Saiyan-style", "power surge. Raises", "a level by one."]),
    'HP_UP':        ('Turtle Shell', ["Roshi's training", "shell. Raises the", "base HP."]),
    'PROTEIN':      ('Weighted Gi',  ["A heavy training", "gi. Raises the base", "Attack stat."]),
    'IRON':         ('Saiyan Armor', ["Saiyan battle gear.", "Raises the base", "Defense stat."]),
    'CALCIUM':      ('Ki Scroll',    ["Secret ki arts.", "Raises the base", "Sp. Atk stat."]),
    'ZINC':         ('Spirit Charm', ["A calming charm.", "Raises the base", "Sp. Def stat."]),
    'CARBOS':       ('Kaio Weights', ["King Kai's gravity", "training. Raises", "the base Speed."]),
    'PP_UP':        ('Ki Boost',     ["Slightly raises", "the max PP of a", "selected move."]),
    'PP_MAX':       ('Ki Max',       ["Raises the PP of a", "move to its", "maximum."]),
    'REPEL':        ('Ki Hider',     ["Hides your ki so", "weak wild Pokémon", "stay away (100)."]),
    'SUPER_REPEL':  ('Ki Cloak',     ["Hides your ki so", "weak wild Pokémon", "stay away (200)."]),
    'MAX_REPEL':    ('Ki Void',      ["Hides your ki so", "weak wild Pokémon", "stay away (250)."]),
    'ESCAPE_ROPE':  ('Instant Trans.', ["Instant Transmis-", "sion. Escape from", "caves and dungeons."]),
    'POKE_DOLL':    ('Saiba Doll',   ["A Saibaman decoy.", "Use it to flee", "any wild battle."]),
    'X_ATTACK':     ('Kaio-ken',     ["Sharply raises", "Attack during one", "battle. Times two!"]),
    'X_DEFENSE':    ('Ki Barrier',   ["Sharply raises", "Defense during one", "battle."]),
    'X_SPEED':      ('Zanzoken',     ["Afterimage speed!", "Sharply raises", "Speed in battle."]),
    'X_SP_ATK':     ('Ki Charge',    ["Sharply raises", "Sp. Atk during one", "battle."]),
    'X_ACCURACY':   ('Scouter Lock', ["Locks on target.", "Sharply raises", "accuracy in battle."]),
    'DIRE_HIT':     ('Focus Fist',   ["Sharply raises the", "critical-hit ratio", "in one battle."]),
    'GUARD_SPEC':   ('Aura Guard',   ["Prevents stat", "reduction for five", "turns in battle."]),
    'LEFTOVERS':    ('Senzu Pouch',  ["A hold item that", "restores HP a bit", "every turn."]),
    'FOCUS_BAND':   ('Saiyan Pride', ["A hold item that", "may let the holder", "endure a KO hit."]),
    'EXP_SHARE':    ('Training Gi',  ["Shares battle EXP.", "with the whole", "team."]),
    'CHOICE_BAND':  ('Majin Mark',   ["Powers up a move", "but only that move", "can be used."]),
    'NUGGET':       ('Zeni Pouch',   ["A pouch stuffed", "with zeni. Sells", "for a high price."]),
    'DEVON_PARTS':  ('CC Goods',     ["Parts ordered by", "Capsule Corp. for", "delivery."]),
    'DEVON_SCOPE':  ('Scouter',      ["A Saiyan scouter.", "Spots unseeable", "Pokémon nearby."]),
    'MAGMA_EMBLEM': ('RR Emblem',    ["The mark of", "Kid Buu's Red", "Ribbon Army."]),
    'MACH_BIKE':    ('Capsule Bike', ["A folding bike in", "a capsule. Fast,", "smooth riding."]),
    'ACRO_BIKE':    ('Hover Bike',   ["A Capsule Corp.", "hover bike for", "tricks and jumps."]),
}

p = 'src/data/items.h'
s = open(p).read()
missing = []
for const, (name, desc) in R.items():
    assert len(name) <= 20, name
    m = re.search(r'\n    \[ITEM_' + const + r'\] =\n    \{\n(.*?)\n    \},', s, re.S)
    if not m:
        missing.append(const); continue
    block = m.group(1)
    nb = re.sub(r'\.name = ITEM_NAME\(".*?"\)', lambda _m: f'.name = ITEM_NAME("{name}")', block, count=1)
    nb = re.sub(r'\n\s*\.pluralName = ITEM_PLURAL_NAME\(".*?"\),', '', nb)
    d = '\n'.join(f'            "{l}{chr(92)+"n" if i < len(desc) - 1 else ""}"' for i, l in enumerate(desc))
    nb = re.sub(r'\.description = COMPOUND_STRING\(\n.*?\),\n(?=        \.)', lambda _m: '.description = COMPOUND_STRING(\n' + d + '),\n', nb, count=1, flags=re.S)
    s = s[:m.start(1)] + nb + s[m.end(1):]
open(p, 'w').write(s)
print('renamed', len(R) - len(missing), 'missing', missing)
