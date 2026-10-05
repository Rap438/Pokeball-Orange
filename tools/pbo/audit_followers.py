#!/usr/bin/env python3
"""Follower sprite coverage for the PokeBall Orange roster (v0.3 roster: national dex 1-386 plus the
species Shenron can grant), with forms and shiny palettes reported separately.

Writes docs/pbo/follower_sprite_audit.md. Run from the repo root."""
import glob, os, re

ROOT = os.getcwd()
blocks = {}
for f in sorted(glob.glob('src/data/pokemon/species_info/gen_*_families.h')):
    s = open(f).read()
    # split on top-level species entries
    for m in re.finditer(r'\n    \[(SPECIES_\w+)\] =\n    \{\n(.*?)\n    \},', s, re.S):
        blocks[m.group(1)] = m.group(2)

# national dex numbers and base species
natdex = {}
for sp, b in blocks.items():
    m = re.search(r'\.natDexNum = (NATIONAL_DEX_\w+)', b)
    if m:
        natdex[sp] = m.group(1)
dexnum = {}
for l in open('include/constants/pokedex.h'):
    m = re.match(r'\s+(NATIONAL_DEX_\w+),', l)
    if m:
        dexnum[m.group(1)] = len(dexnum)   # NATIONAL_DEX_NONE = 0

wish = set(re.findall(r'(SPECIES_\w+)', open('src/data/dbz_wish_list.h').read()))
species_h = open('include/constants/species.h').read()
alias = dict(re.findall(r'(SPECIES_\w+) = (SPECIES_\w+),', species_h))


def resolve(sp):
    return alias.get(sp, sp)


roster_dex = set(n for n in range(1, 387)) | set(dexnum.get(natdex.get(resolve(s), ''), -1) for s in wish)
rows = []
for sp, b in blocks.items():
    dn = dexnum.get(natdex.get(sp, ''), -1)
    if dn not in roster_dex or dn <= 0:
        continue
    ow = 'OVERWORLD(' in b
    sub = 'sPicTable_Substitute' in b or 'Substitute' in (re.search(r'OVERWORLD\(\s*(\w+)', b).group(1) if ow else '')
    pal = ow and re.search(r'OVERWORLD\(\s*\w+,[^)]*gOverworldPalette_\w+', b, re.S) is not None
    shiny = ow and re.search(r'gShinyOverworldPalette_\w+', b) is not None
    fem = 'OVERWORLD_FEMALE(' in b
    form = 'formSpeciesIdTable' in b and not re.search(r'\.formSpeciesIdTable', b) is None
    is_base = (resolve(sp) == sp) and not re.search(r'_(MEGA|GMAX|PRIMAL)', sp)
    rows.append((dn, sp, ow and not sub, pal, shiny, fem, is_base))

rows.sort()
base = [r for r in rows if r[6] and not re.search(r'_(MEGA|GMAX|PRIMAL|TOTEM)', r[1])]
# a species "base form" = the first entry for each dex number
first = {}
for r in rows:
    first.setdefault(r[0], r)
bases = list(first.values())
forms = [r for r in rows if first[r[0]] is not r]
battle_only = [r for r in forms if re.search(r'_(MEGA|GMAX|PRIMAL|TOTEM)', r[1])]
forms = [r for r in forms if r not in battle_only]


def stats(rs):
    n = len(rs)
    return n, sum(1 for r in rs if r[2]), sum(1 for r in rs if r[2] and r[4])


out = ['# Follower sprite audit (v0.4)', '',
       'Roster: national dex #1-386 plus every species Shenron can grant (the v0.3 roster).',
       'Data source: `src/data/pokemon/species_info/gen_*_families.h` in pokeemerald-expansion 1.17.1.',
       'A missing overworld sprite falls back to the engine\'s Substitute doll (`OW_SUBSTITUTE_PLACEHOLDER`).', '']
for title, rs in (('Base species', bases), ('Alternate forms (non-battle)', forms), ('Battle-only forms (never follow)', battle_only)):
    n, ow, sh = stats(rs)
    out.append(f'**{title}:** {n} entries, {ow} with their own overworld sprite, {sh} of those with a shiny overworld palette.')
out.append('')
miss = [r for r in bases if not r[2]]
out.append('## Base species without their own overworld sprite')
out.append(', '.join(r[1].replace('SPECIES_', '') for r in miss) if miss else 'None.')
out.append('')
miss_sh = [r for r in bases if r[2] and not r[4]]
out.append('## Base species whose follower has no shiny palette (shows normal colours)')
out.append(', '.join(r[1].replace('SPECIES_', '') for r in miss_sh) if miss_sh else 'None.')
out.append('')
mf = [r for r in forms if not r[2]]
out.append('## Forms that fall back (to the Substitute doll or the base sprite)')
out.append(', '.join(r[1].replace('SPECIES_', '') for r in mf) if mf else 'None.')
out.append('')
fem = [r for r in rows if r[5]]
out.append(f'## Female-difference follower sprites: {len(fem)}')
out.append('')
os.makedirs('docs/pbo', exist_ok=True)
open('docs/pbo/follower_sprite_audit.md', 'w').write('\n'.join(out) + '\n')
print('\n'.join(out[:12]))
