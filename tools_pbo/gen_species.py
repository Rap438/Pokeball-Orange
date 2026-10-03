#!/usr/bin/env python3
"""Add Gen 4-9 legendaries + pseudo-legendary lines to pokeemerald (Gen 3 moves/abilities/types only)."""
import json, re, os, shutil
import numpy as np
from PIL import Image

PE = '/home/claude/pokeemerald'
EXP = '/home/claude/expansion'
D = json.load(open('/home/claude/tools/new_species.json'))
ORDER = list(D.keys())

FOLDER = {k: k.lower() for k in ORDER}

NAMES = {'FEZANDIPITI': 'FEZANDIPTI'}

CRY = dict(UXIE='JIRACHI', MESPRIT='CELEBI', AZELF='MEW', DIALGA='GROUDON', PALKIA='KYOGRE', HEATRAN='ENTEI',
           REGIGIGAS='REGIROCK', GIRATINA='RAYQUAZA', CRESSELIA='LATIAS', COBALION='RAIKOU', TERRAKION='ENTEI',
           VIRIZION='SUICUNE', TORNADUS='ARTICUNO', THUNDURUS='ZAPDOS', RESHIRAM='HO_OH', ZEKROM='LUGIA',
           LANDORUS='MOLTRES', KYUREM='REGICE', XERNEAS='SUICUNE', YVELTAL='HO_OH', ZYGARDE='SEVIPER',
           TYPE_NULL='ABSOL', SILVALLY='ABSOL', TAPU_KOKO='XATU', TAPU_LELE='TOGETIC', TAPU_BULU='TROPIUS',
           TAPU_FINI='LAPRAS', COSMOG='CLEFFA', COSMOEM='PUPITAR', SOLGALEO='ENTEI', LUNALA='LATIOS',
           NECROZMA='DEOXYS', ZACIAN='ARCANINE', ZAMAZENTA='MANECTRIC', ETERNATUS='RAYQUAZA', KUBFU='TEDDIURSA',
           URSHIFU='URSARING', REGIELEKI='REGISTEEL', REGIDRAGO='REGIROCK', GLASTRIER='RAPIDASH', SPECTRIER='PONYTA',
           CALYREX='CELEBI', ENAMORUS='ARTICUNO', WO_CHIEN='TORKOAL', CHIEN_PAO='SNEASEL', TING_LU='DONPHAN',
           CHI_YU='MAGMAR', KORAIDON='SALAMENCE', MIRAIDON='FLYGON', OKIDOGI='PRIMEAPE', MUNKIDORI='AIPOM',
           FEZANDIPITI='PIDGEOT', OGERPON='LOMBRE', TERAPAGOS='LAPRAS', GIBLE='BAGON', GABITE='SHELGON',
           GARCHOMP='SALAMENCE', DEINO='HOUNDOUR', ZWEILOUS='HOUNDOOM', HYDREIGON='GYARADOS', GOOMY='DRATINI',
           SLIGGOO='DRAGONAIR', GOODRA='DRAGONITE', JANGMO_O='TRAPINCH', HAKAMO_O='VIBRAVA', KOMMO_O='FLYGON',
           DREEPY='DUSKULL', DRAKLOAK='DUSCLOPS', DRAGAPULT='ALTARIA', FRIGIBAX='SNORUNT', ARCTIBAX='GLALIE',
           BAXCALIBUR='WALREIN')

ABIL_SUB = dict(TELEPATHY=None, JUSTIFIED='INNER_FOCUS', PRANKSTER='PRESSURE', DEFIANT='GUTS', TURBOBLAZE='PRESSURE',
                TERAVOLT='PRESSURE', SAND_FORCE='SAND_VEIL', SHEER_FORCE='INTIMIDATE', FAIRY_AURA='PRESSURE',
                DARK_AURA='PRESSURE', AURA_BREAK='PRESSURE', RKS_SYSTEM='BATTLE_ARMOR', ELECTRIC_SURGE='STATIC',
                PSYCHIC_SURGE='SYNCHRONIZE', GRASSY_SURGE='OVERGROW', MISTY_SURGE='WATER_VEIL', UNAWARE='OWN_TEMPO',
                FULL_METAL_BODY='CLEAR_BODY', SHADOW_SHIELD='PRESSURE', PRISM_ARMOR='BATTLE_ARMOR',
                INTREPID_SWORD='GUTS', DAUNTLESS_SHIELD='STURDY', SLOW_START='PRESSURE', UNSEEN_FIST='INNER_FOCUS',
                TRANSISTOR='STATIC', DRAGONS_MAW='PRESSURE', CHILLING_NEIGH='INTIMIDATE', GRIM_NEIGH='PRESSURE',
                UNNERVE='PRESSURE', CONTRARY=None, TABLETS_OF_RUIN='PRESSURE', SWORD_OF_RUIN='PRESSURE',
                VESSEL_OF_RUIN='PRESSURE', BEADS_OF_RUIN='PRESSURE', ORICHALCUM_PULSE='DROUGHT',
                HADRON_ENGINE='STATIC', TOXIC_CHAIN='POISON_POINT', GUARD_DOG='INTIMIDATE', FRISK='KEEN_EYE',
                TECHNICIAN=None, TERA_SHIFT='PRESSURE', THERMAL_EXCHANGE='THICK_FAT', ICE_BODY='THICK_FAT',
                SAP_SIPPER='SHED_SKIN', HYDRATION='RAIN_DISH', GOOEY=None, BULLETPROOF='SOUNDPROOF',
                OVERCOAT='SHELL_ARMOR', INFILTRATOR=None, CURSED_BODY=None)

EVO_FIX = {
    'TYPE_NULL': [('EVO_FRIENDSHIP', '0', 'SILVALLY')],
    'COSMOEM': [('EVO_ITEM', 'ITEM_SUN_STONE', 'SOLGALEO'), ('EVO_ITEM', 'ITEM_MOON_STONE', 'LUNALA')],
    'KUBFU': [('EVO_LEVEL', '50', 'URSHIFU')],
    'GOOMY': [('EVO_LEVEL', '40', 'SLIGGOO')],
    'SLIGGOO': [('EVO_LEVEL', '50', 'GOODRA')],
    'HAKAMO_O': [('EVO_LEVEL', '45', 'KOMMO_O')],
}

TYPE_FILLER = {
    'TYPE_DRAGON': ['MOVE_DRAGON_RAGE', 'MOVE_DRAGON_BREATH', 'MOVE_DRAGON_CLAW', 'MOVE_OUTRAGE'],
    'TYPE_PSYCHIC': ['MOVE_CONFUSION', 'MOVE_PSYBEAM', 'MOVE_PSYCHIC', 'MOVE_FUTURE_SIGHT'],
    'TYPE_FAIRY': ['MOVE_SWEET_KISS', 'MOVE_CHARM', 'MOVE_MOONLIGHT', 'MOVE_DOUBLE_EDGE'],
    'TYPE_NORMAL': ['MOVE_TACKLE', 'MOVE_HEADBUTT', 'MOVE_SLASH', 'MOVE_DOUBLE_EDGE'],
}

# ---------------------------------------------------------- vanilla references
moves_h = open(f'{PE}/include/constants/moves.h').read()
VMOVES = set(re.findall(r'#define (MOVE_\w+)\s', moves_h))
abil_h = open(f'{PE}/include/constants/abilities.h').read()
VABIL = set(re.findall(r'#define (ABILITY_\w+)\s', abil_h))
tms = re.findall(r'F\((\w+)\)', open(f'{PE}/include/constants/tms_hms.h').read())
LEARNABLES = json.load(open(f'{EXP}/src/data/pokemon/all_learnables.json'))
LEARN_KEY = dict(GIRATINA='GIRATINA', TORNADUS='TORNADUS', THUNDURUS='THUNDURUS', LANDORUS='LANDORUS',
                 XERNEAS='XERNEAS', ZYGARDE='ZYGARDE', SILVALLY='SILVALLY', ZACIAN='ZACIAN', ZAMAZENTA='ZAMAZENTA',
                 URSHIFU='URSHIFU_SINGLE_STRIKE', ENAMORUS='ENAMORUS', OGERPON='OGERPON', TERAPAGOS='TERAPAGOS')

def cname(k):  # Gible -> Gible ; TYPE_NULL -> TypeNull
    return ''.join(p.capitalize() for p in k.split('_'))

def map_type(t):
    return 'TYPE_NORMAL' if t == 'TYPE_FAIRY' else t

def types_of(d):
    ts = d['types']
    if len(ts) == 1: ts = [ts[0], ts[0]]
    a, b = ts
    if a == 'TYPE_FAIRY' and b == 'TYPE_FAIRY': return ['TYPE_NORMAL', 'TYPE_NORMAL']
    if a == 'TYPE_FAIRY': a = 'TYPE_NORMAL'
    if b == 'TYPE_FAIRY': b = a
    return [a, b]

def abilities_of(d):
    out = []
    for a in d['abilities']:
        if a == 'ABILITY_NONE': continue
        n = a[len('ABILITY_'):]
        if a in VABIL: out.append(a)
        elif n in ABIL_SUB:
            if ABIL_SUB[n]: out.append('ABILITY_' + ABIL_SUB[n])
        else:
            out.append('ABILITY_PRESSURE')
    seen = []
    for a in out:
        if a not in seen: seen.append(a)
    if not seen: seen = ['ABILITY_PRESSURE']
    return (seen + ['ABILITY_NONE'])[:2]

def learnset_of(k, d):
    mv = [(lv, m) for lv, m in d['moves'] if m in VMOVES and m != 'MOVE_NONE']
    # dedupe keeping order
    seen = set(); out = []
    for lv, m in mv:
        if m in seen: continue
        seen.add(m); out.append((max(1, lv), m))
    t = types_of(d)
    filler = TYPE_FILLER.get(t[0], TYPE_FILLER['TYPE_NORMAL'])
    lvl = 1
    while len(out) < 4:
        for m in filler:
            if m not in seen and m in VMOVES:
                out.append((lvl, m)); seen.add(m); lvl += 10
                break
        else:
            break
    out.sort(key=lambda x: x[0])
    return out

def tm_of(k):
    key = LEARN_KEY.get(k, k)
    mv = set(LEARNABLES.get(key, []))
    return [t for t in tms if ('MOVE_' + t) in mv]

def gender(g):
    if g is None: return 'MON_GENDERLESS'
    return g

def friendship(f):
    if f is None: return '0'
    return {'STANDARD_FRIENDSHIP': '70'}.get(f, f)

# ---------------------------------------------------------- graphics
os.makedirs(f'{PE}/graphics/pokemon', exist_ok=True)
for k in ORDER:
    src = f'{EXP}/graphics/pokemon/{FOLDER[k]}'
    dst = f'{PE}/graphics/pokemon/{FOLDER[k]}'
    os.makedirs(dst, exist_ok=True)
    for f in ['back.png', 'icon.png', 'normal.pal', 'shiny.pal', 'footprint.png']:
        shutil.copy(f'{src}/{f}', f'{dst}/{f}')
    if os.path.exists(f'{src}/anim_front.png'):
        a = Image.open(f'{src}/anim_front.png')
    else:
        fr = Image.open(f'{src}/front.png')
        a = Image.new('P', (64, 128)); a.putpalette(fr.getpalette())
        a.paste(fr, (0, 0)); a.paste(fr, (0, 64))
        if 'transparency' in fr.info: a.info['transparency'] = fr.info['transparency']
    a.save(f'{dst}/anim_front.png')
    a.crop((0, 0, 64, 64)).save(f'{dst}/front.png')

# ---------------------------------------------------------- C fragments
first = 412
consts = []
for i, k in enumerate(ORDER):
    consts.append(f'#define SPECIES_{k} {first + i}')
egg = first + len(ORDER)

gfx, animfront, info, names, front_t, still_t, back_t, pal_t, shiny_t, fcoord, bcoord, foot, anims, icon_t, iconpal = ([] for _ in range(15))
lus, lup, tmh, evo, cry = [], [], [], [], []
for k in ORDER:
    d = D[k]; C = cname(k); F = FOLDER[k]
    gfx += [f'const u32 gMonStillFrontPic_{C}[] = INCGFX_U32("graphics/pokemon/{F}/front.png", ".4bpp.lz");',
            f'const u32 gMonPalette_{C}[] = INCGFX_U32("graphics/pokemon/{F}/normal.pal", ".gbapal.lz");',
            f'const u32 gMonBackPic_{C}[] = INCGFX_U32("graphics/pokemon/{F}/back.png", ".4bpp.lz");',
            f'const u32 gMonShinyPalette_{C}[] = INCGFX_U32("graphics/pokemon/{F}/shiny.pal", ".gbapal.lz");',
            f'const u8 gMonIcon_{C}[] = INCGFX_U8("graphics/pokemon/{F}/icon.png", ".4bpp");',
            f'const u8 gMonFootprint_{C}[] = INCGFX_U8("graphics/pokemon/{F}/footprint.png", ".1bpp");']
    animfront.append(f'const u32 gMonFrontPic_{C}[] = INCGFX_U32("graphics/pokemon/{F}/anim_front.png", ".4bpp.lz");')
    t = types_of(d); ab = abilities_of(d)
    eg = d['eggGroups'] + d['eggGroups'][:1] if len(d['eggGroups']) == 1 else d['eggGroups']
    info.append(f'''    [SPECIES_{k}] =
    {{
        .baseHP        = {d['baseHP']},
        .baseAttack    = {d['baseAttack']},
        .baseDefense   = {d['baseDefense']},
        .baseSpeed     = {d['baseSpeed']},
        .baseSpAttack  = {d['baseSpAttack']},
        .baseSpDefense = {d['baseSpDefense']},
        .types = {{ {t[0]}, {t[1]} }},
        .catchRate = {d['catchRate']},
        .expYield = {min(255, d['expYield'])},
        .evYield_HP = {d['evYield_HP']},
        .evYield_Attack = {d['evYield_Attack']},
        .evYield_Defense = {d['evYield_Defense']},
        .evYield_Speed = {d['evYield_Speed']},
        .evYield_SpAttack = {d['evYield_SpAttack']},
        .evYield_SpDefense = {d['evYield_SpDefense']},
        .itemCommon = ITEM_NONE,
        .itemRare = ITEM_NONE,
        .genderRatio = {gender(d['genderRatio'])},
        .eggCycles = {d['eggCycles']},
        .friendship = {friendship(d['friendship'])},
        .growthRate = {d['growthRate']},
        .eggGroups = {{ {eg[0]}, {eg[1]} }},
        .abilities = {{ {ab[0]}, {ab[1]} }},
        .safariZoneFleeRate = 0,
        .bodyColor = {d['bodyColor']},
        .noFlip = FALSE,
    }},''')
    nm = NAMES.get(k, d['name'].upper())[:10]
    names.append(f'    [SPECIES_{k}] = _("{nm}"),')
    front_t.append(f'    SPECIES_SPRITE({k}, gMonFrontPic_{C}),')
    still_t.append(f'    SPECIES_SPRITE({k}, gMonStillFrontPic_{C}),')
    back_t.append(f'    SPECIES_SPRITE({k}, gMonBackPic_{C}),')
    pal_t.append(f'    SPECIES_PAL({k}, gMonPalette_{C}),')
    shiny_t.append(f'    SPECIES_SHINY_PAL({k}, gMonShinyPalette_{C}),')
    fw, fh = d['frontSize']; bw, bh = d['backSize']
    fw = min(64, (fw + 7) // 8 * 8); fh = min(64, (fh + 7) // 8 * 8)
    bw = min(64, (bw + 7) // 8 * 8); bh = min(64, (bh + 7) // 8 * 8)
    fcoord.append(f'    [SPECIES_{k}] = {{ .size = MON_COORDS_SIZE({fw}, {fh}), .y_offset = {d["frontY"]} }},')
    bcoord.append(f'    [SPECIES_{k}] = {{ .size = MON_COORDS_SIZE({bw}, {bh}), .y_offset = {d["backY"]} }},')
    foot.append(f'    [SPECIES_{k}] = gMonFootprint_{C},')
    anims.append(f'    [SPECIES_{k}] = sAnims_DBZNew,')
    icon_t.append(f'    [SPECIES_{k}] = gMonIcon_{C},')
    iconpal.append(f'    [SPECIES_{k}] = {d["iconPal"] if isinstance(d["iconPal"], int) and d["iconPal"] < 6 else 0},')
    ls = learnset_of(k, d)
    lus.append(f'static const u16 s{C}LevelUpLearnset[] = {{\n' + ''.join(f'    LEVEL_UP_MOVE({lv}, {m}),\n' for lv, m in ls) + '    LEVEL_UP_END\n};\n')
    lup.append(f'    [SPECIES_{k}] = s{C}LevelUpLearnset,')
    tl = tm_of(k)
    tmh.append(f'    [SPECIES_{k}] = {{ .learnset = {{\n' + ''.join(f'        .{t} = TRUE,\n' for t in tl) + '    } },')
    evs = EVO_FIX.get(k, [(a, b, c) for a, b, c in d['evolutions'] if c in D and a in ('EVO_LEVEL',)])
    if evs:
        evo.append(f'    [SPECIES_{k}] = {{' + ', '.join(f'{{{a}, {b}, SPECIES_{c}}}' for a, b, c in evs) + '},')
    cry.append(f'    SPECIES_{CRY[k]},')

w = lambda p, lines: open(f'{PE}/{p}', 'w').write('// generated by tools/gen_species.py\n' + '\n'.join(lines) + '\n')
w('include/constants/dbz_species.h', consts + [f'#define DBZ_FIRST_NEW_SPECIES {first}', f'#define DBZ_LAST_NEW_SPECIES {egg - 1}'])
w('src/data/graphics/dbz_pokemon.h', gfx)
w('src/data/graphics/dbz_anim_front_pics.h', animfront)
w('src/data/pokemon/dbz_species_info.h', info)
w('src/data/text/dbz_species_names.h', names)
w('src/data/pokemon_graphics/dbz_front_pic_table.h', front_t)
w('src/data/pokemon_graphics/dbz_still_front_pic_table.h', still_t)
w('src/data/pokemon_graphics/dbz_back_pic_table.h', back_t)
w('src/data/pokemon_graphics/dbz_palette_table.h', pal_t)
w('src/data/pokemon_graphics/dbz_shiny_palette_table.h', shiny_t)
w('src/data/pokemon_graphics/dbz_front_pic_coordinates.h', fcoord)
w('src/data/pokemon_graphics/dbz_back_pic_coordinates.h', bcoord)
w('src/data/pokemon_graphics/dbz_footprint_table.h', foot)
w('src/data/pokemon_graphics/dbz_front_pic_anims.h', anims)
w('src/data/pokemon/dbz_icon_table.h', icon_t)
w('src/data/pokemon/dbz_icon_palette_indices.h', iconpal)
w('src/data/pokemon/dbz_level_up_learnsets.h', lus)
w('src/data/pokemon/dbz_level_up_learnset_pointers.h', lup)
w('src/data/pokemon/dbz_tmhm_learnsets.h', tmh)
w('src/data/pokemon/dbz_evolution.h', evo)
w('src/data/pokemon/dbz_cry_borrow.h', ['static const u16 sDBZCryBorrow[] = {'] + cry + ['};'])
print('species', len(ORDER), 'egg =', egg)
