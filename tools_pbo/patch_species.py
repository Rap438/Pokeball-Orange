#!/usr/bin/env python3
"""Hook the generated species fragments into pokeemerald's tables (idempotent)."""
import re
PE = '/home/claude/pokeemerald'

def insert_in_table(path, table_regex, inc):
    s = open(f'{PE}/{path}').read()
    if inc in s:
        return
    m = re.search(table_regex, s)
    assert m, (path, table_regex)
    end = s.index('\n};', m.end())
    s = s[:end] + f'\n#include "{inc}"' + s[end:]
    open(f'{PE}/{path}', 'w').write(s)

def append(path, inc):
    s = open(f'{PE}/{path}').read()
    if inc in s: return
    open(f'{PE}/{path}', 'w').write(s.rstrip('\n') + f'\n#include "{inc}"\n')

# species constants
p = f'{PE}/include/constants/species.h'; s = open(p).read()
if 'dbz_species.h' not in s:
    s = s.replace('#define SPECIES_CHIMECHO 411\n', '#define SPECIES_CHIMECHO 411\n#include "constants/dbz_species.h"\n')
    s = s.replace('#define SPECIES_EGG 412\n', '#define SPECIES_EGG (DBZ_LAST_NEW_SPECIES + 1)\n')
    open(p, 'w').write(s)
p = f'{PE}/include/data.h'; s = open(p).read()
s = s.replace('#define SPECIES_SHINY_TAG 500', '#define SPECIES_SHINY_TAG 1000'); open(p, 'w').write(s)

T = [
    ('src/data/pokemon/species_info.h', r'const struct SpeciesInfo gSpeciesInfo\[\] =\s*\{', 'data/pokemon/dbz_species_info.h'),
    ('src/data/text/species_names.h', r'const u8 gSpeciesNames\[\]\[POKEMON_NAME_LENGTH \+ 1\] = \{', 'data/text/dbz_species_names.h'),
    ('src/data/pokemon_graphics/front_pic_table.h', r'gMonFrontPicTable\[\] =\s*\{', 'data/pokemon_graphics/dbz_front_pic_table.h'),
    ('src/data/pokemon_graphics/still_front_pic_table.h', r'gMonStillFrontPicTable\[\] =\s*\{', 'data/pokemon_graphics/dbz_still_front_pic_table.h'),
    ('src/data/pokemon_graphics/back_pic_table.h', r'gMonBackPicTable\[\] =\s*\{', 'data/pokemon_graphics/dbz_back_pic_table.h'),
    ('src/data/pokemon_graphics/palette_table.h', r'gMonPaletteTable\[\] =\s*\{', 'data/pokemon_graphics/dbz_palette_table.h'),
    ('src/data/pokemon_graphics/shiny_palette_table.h', r'gMonShinyPaletteTable\[\] =\s*\{', 'data/pokemon_graphics/dbz_shiny_palette_table.h'),
    ('src/data/pokemon_graphics/front_pic_coordinates.h', r'gMonFrontPicCoords\[\] =\s*\{', 'data/pokemon_graphics/dbz_front_pic_coordinates.h'),
    ('src/data/pokemon_graphics/back_pic_coordinates.h', r'gMonBackPicCoords\[\] =\s*\{', 'data/pokemon_graphics/dbz_back_pic_coordinates.h'),
    ('src/data/pokemon_graphics/footprint_table.h', r'gMonFootprintTable\[\] =\s*\{', 'data/pokemon_graphics/dbz_footprint_table.h'),
    ('src/data/pokemon_graphics/front_pic_anims.h', r'gMonFrontAnimsPtrTable\[\] =\s*\{', 'data/pokemon_graphics/dbz_front_pic_anims.h'),
    ('src/pokemon_icon.c', r'const u8 \*const gMonIconTable\[\] =\s*\{', 'data/pokemon/dbz_icon_table.h'),
    ('src/pokemon_icon.c', r'const u8 gMonIconPaletteIndices\[\] =\s*\{', 'data/pokemon/dbz_icon_palette_indices.h'),
    ('src/data/pokemon/level_up_learnset_pointers.h', r'gLevelUpLearnsets\[NUM_SPECIES\] =\s*\{', 'data/pokemon/dbz_level_up_learnset_pointers.h'),
    ('src/data/pokemon/tmhm_learnsets.h', r'gTMHMLearnsets\[NUM_SPECIES\] =\s*\{', 'data/pokemon/dbz_tmhm_learnsets.h'),
    ('src/data/pokemon/evolution.h', r'gEvolutionTable\[NUM_SPECIES\]\[EVOS_PER_MON\] =\s*\{', 'data/pokemon/dbz_evolution.h'),
]
for path, rx, inc in T:
    insert_in_table(path, rx, inc)

append('src/data/pokemon/level_up_learnsets.h', 'data/pokemon/dbz_level_up_learnsets.h')
append('src/data/graphics/pokemon.h', 'data/graphics/dbz_pokemon.h')
append('src/anim_mon_front_pics.c', 'data/graphics/dbz_anim_front_pics.h')

# generic front animation for the new species
p = f'{PE}/src/data/pokemon_graphics/front_pic_anims.h'; s = open(p).read()
if 'sAnims_DBZNew' not in s.split('gMonFrontAnimsPtrTable')[0]:
    s = s.replace('SINGLE_ANIMATION(None);', '''static const union AnimCmd sAnim_DBZNew_1[] =
{
    ANIMCMD_FRAME(1, 18),
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(1, 18),
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_END,
};
SINGLE_ANIMATION(DBZNew);
SINGLE_ANIMATION(None);''', 1)
    open(p, 'w').write(s)

# cries
p = f'{PE}/src/pokemon.c'; s = open(p).read()
if 'sDBZCryBorrow' not in s:
    s = s.replace('''u16 SpeciesToCryId(u16 species)
{''', '''#include "data/pokemon/dbz_cry_borrow.h"

u16 SpeciesToCryId(u16 species)
{
    if (species >= DBZ_FIRST_NEW_SPECIES && species <= DBZ_LAST_NEW_SPECIES)
        species = sDBZCryBorrow[species - DBZ_FIRST_NEW_SPECIES];
''')
    open(p, 'w').write(s)

# pokedex flags: new species have no national dex number
p = f'{PE}/src/pokedex.c'; s = open(p).read()
if 'nationalDexNo == 0 ||' not in s:
    s = re.sub(r'(s8 GetSetPokedexFlag\(u16 nationalDexNo, u8 caseID\)\n\{\n(?:    [^\n]*;\n)*)',
               lambda m: m.group(1) + '\n    if (nationalDexNo == 0 || nationalDexNo > NATIONAL_DEX_COUNT)\n        return 0;\n', s, count=1)
    open(p, 'w').write(s)
print('patched')
