#!/usr/bin/env python3
"""Parse species data for selected species from pokeemerald-expansion."""
import re, glob, json
EXP = '/home/claude/expansion'

NEW = [  # (our constant, expansion key, gfx folder, display name)
    ('UXIE', 'UXIE'), ('MESPRIT', 'MESPRIT'), ('AZELF', 'AZELF'), ('DIALGA', 'DIALGA'), ('PALKIA', 'PALKIA'),
    ('HEATRAN', 'HEATRAN'), ('REGIGIGAS', 'REGIGIGAS'), ('GIRATINA', 'GIRATINA_ALTERED'), ('CRESSELIA', 'CRESSELIA'),
    ('COBALION', 'COBALION'), ('TERRAKION', 'TERRAKION'), ('VIRIZION', 'VIRIZION'), ('TORNADUS', 'TORNADUS_INCARNATE'),
    ('THUNDURUS', 'THUNDURUS_INCARNATE'), ('RESHIRAM', 'RESHIRAM'), ('ZEKROM', 'ZEKROM'), ('LANDORUS', 'LANDORUS_INCARNATE'),
    ('KYUREM', 'KYUREM'), ('XERNEAS', 'XERNEAS_NEUTRAL'), ('YVELTAL', 'YVELTAL'), ('ZYGARDE', 'ZYGARDE_50'),
    ('TYPE_NULL', 'TYPE_NULL'), ('SILVALLY', 'SILVALLY_NORMAL'), ('TAPU_KOKO', 'TAPU_KOKO'), ('TAPU_LELE', 'TAPU_LELE'),
    ('TAPU_BULU', 'TAPU_BULU'), ('TAPU_FINI', 'TAPU_FINI'), ('COSMOG', 'COSMOG'), ('COSMOEM', 'COSMOEM'),
    ('SOLGALEO', 'SOLGALEO'), ('LUNALA', 'LUNALA'), ('NECROZMA', 'NECROZMA'), ('ZACIAN', 'ZACIAN_HERO'),
    ('ZAMAZENTA', 'ZAMAZENTA_HERO'), ('ETERNATUS', 'ETERNATUS'), ('KUBFU', 'KUBFU'), ('URSHIFU', 'URSHIFU_SINGLE_STRIKE'),
    ('REGIELEKI', 'REGIELEKI'), ('REGIDRAGO', 'REGIDRAGO'), ('GLASTRIER', 'GLASTRIER'), ('SPECTRIER', 'SPECTRIER'),
    ('CALYREX', 'CALYREX'), ('ENAMORUS', 'ENAMORUS_INCARNATE'), ('WO_CHIEN', 'WO_CHIEN'), ('CHIEN_PAO', 'CHIEN_PAO'),
    ('TING_LU', 'TING_LU'), ('CHI_YU', 'CHI_YU'), ('KORAIDON', 'KORAIDON'), ('MIRAIDON', 'MIRAIDON'),
    ('OKIDOGI', 'OKIDOGI'), ('MUNKIDORI', 'MUNKIDORI'), ('FEZANDIPITI', 'FEZANDIPITI'), ('OGERPON', 'OGERPON_TEAL'),
    ('TERAPAGOS', 'TERAPAGOS_NORMAL'),
    ('GIBLE', 'GIBLE'), ('GABITE', 'GABITE'), ('GARCHOMP', 'GARCHOMP'), ('DEINO', 'DEINO'), ('ZWEILOUS', 'ZWEILOUS'),
    ('HYDREIGON', 'HYDREIGON'), ('GOOMY', 'GOOMY'), ('SLIGGOO', 'SLIGGOO'), ('GOODRA', 'GOODRA'),
    ('JANGMO_O', 'JANGMO_O'), ('HAKAMO_O', 'HAKAMO_O'), ('KOMMO_O', 'KOMMO_O'), ('DREEPY', 'DREEPY'),
    ('DRAKLOAK', 'DRAKLOAK'), ('DRAGAPULT', 'DRAGAPULT'), ('FRIGIBAX', 'FRIGIBAX'), ('ARCTIBAX', 'ARCTIBAX'),
    ('BAXCALIBUR', 'BAXCALIBUR'),
]

def strip_conditionals(text):
    """Keep the first branch of #if/#else/#endif blocks (handles nesting)."""
    out = []; stack = []
    for line in text.split('\n'):
        s = line.strip().rstrip('\\').strip()
        if s.startswith('#if'):
            stack.append([True, False]); continue  # [keeping, seen_else]
        if s.startswith('#elif') or s.startswith('#else'):
            if stack: stack[-1] = [False, True]
            continue
        if s.startswith('#endif'):
            if stack: stack.pop()
            continue
        if all(k for k, _ in stack):
            out.append(line)
    return '\n'.join(out)

SRC = ''
for f in sorted(glob.glob(f'{EXP}/src/data/pokemon/species_info/gen_*_families.h')):
    SRC += open(f).read() + '\n'

def find_block(text, start):
    i = text.index('{', start); depth = 0
    for j in range(i, len(text)):
        if text[j] == '{': depth += 1
        elif text[j] == '}':
            depth -= 1
            if depth == 0: return text[i:j + 1]
    raise ValueError

def macro_expand(key):
    m = re.search(r'\[SPECIES_' + key + r'\]\s*=\s*(\w+)\((.*?)\),\s*\n', SRC)
    if not m: return None
    name, args = m.group(1), [a.strip() for a in m.group(2).split(',')]
    d = re.search(r'#define ' + name + r'\(([^)]*)\)(.*?)\n\s*\n', SRC, re.S)
    params = [p.strip() for p in d.group(1).split(',')]
    body = d.group(2).replace('\\\n', '\n')
    for p, a in sorted(zip(params, args), key=lambda x: -len(x[0])):
        body = re.sub(r'##\s*' + re.escape(p) + r'\b', a, body)
        body = re.sub(r'\b' + re.escape(p) + r'\s*##', a, body)
        body = re.sub(r'\b' + re.escape(p) + r'\b', a, body)
    return body

def get_block(key):
    m = re.search(r'\[SPECIES_' + key + r'\]\s*=\s*\n\s*\{', SRC)
    if m:
        return strip_conditionals(find_block(SRC, m.start()))
    b = macro_expand(key)
    if b is None:
        raise KeyError(key)
    return strip_conditionals(b)

def num(expr):
    expr = expr.strip()
    m = re.match(r'\(?[^?]*\)?\s*\?\s*(\d+)\s*:\s*(\d+)', expr)
    if m: return int(m.group(1))
    if expr == 'GIRATINA_EXP_YIELD': return 306
    m = re.match(r'-?\d+', expr)
    return int(m.group(0)) if m else expr

def field(block, name):
    m = re.search(r'\.' + name + r'\s*=\s*(.*?),\s*\n', block)
    return m.group(1).strip() if m else None

def parse(key):
    b = get_block(key)
    d = {}
    for s in ['baseHP', 'baseAttack', 'baseDefense', 'baseSpeed', 'baseSpAttack', 'baseSpDefense', 'catchRate', 'expYield', 'eggCycles']:
        d[s] = num(field(b, s))
    for s in ['evYield_HP', 'evYield_Attack', 'evYield_Defense', 'evYield_Speed', 'evYield_SpAttack', 'evYield_SpDefense']:
        v = field(b, s); d[s] = num(v) if v else 0
    d['types'] = re.findall(r'TYPE_\w+', re.search(r'\.types\s*=\s*MON_TYPES\((.*?)\)', b).group(1))
    d['genderRatio'] = field(b, 'genderRatio')
    d['friendship'] = field(b, 'friendship')
    d['growthRate'] = field(b, 'growthRate')
    d['eggGroups'] = re.findall(r'EGG_GROUP_\w+', re.search(r'\.eggGroups\s*=\s*MON_EGG_GROUPS\((.*?)\)', b).group(1))
    d['abilities'] = re.findall(r'ABILITY_\w+', re.search(r'\.abilities\s*=\s*\{(.*?)\}', b).group(1))
    d['bodyColor'] = field(b, 'bodyColor')
    d['name'] = re.search(r'\.speciesName\s*=\s*_\("(.*?)"\)', b).group(1)
    d['learnset'] = field(b, 'levelUpLearnset')
    ev = re.search(r'\.evolutions\s*=\s*EVOLUTION\((.*?)\),\s*\n', b, re.S)
    d['evolutions'] = re.findall(r'\{(EVO_\w+),\s*(\w+),\s*SPECIES_(\w+)', ev.group(1)) if ev else []
    fs = re.search(r'\.frontPicSize\s*=\s*MON_COORDS_SIZE\((\d+),\s*(\d+)\)', b)
    bs = re.search(r'\.backPicSize\s*=\s*MON_COORDS_SIZE\((\d+),\s*(\d+)\)', b)
    d['frontSize'] = (int(fs.group(1)), int(fs.group(2))) if fs else (64, 64)
    d['backSize'] = (int(bs.group(1)), int(bs.group(2))) if bs else (64, 64)
    d['frontY'] = num(field(b, 'frontPicYOffset') or '0')
    d['backY'] = num(field(b, 'backPicYOffset') or '0')
    d['iconPal'] = num(field(b, 'iconPalIndex') or '0')
    d['elevation'] = num(field(b, 'enemyMonElevation') or '0')
    fp = re.search(r'\.frontPic\s*=\s*gMonFrontPic_(\w+)', b)
    d['gfx'] = fp.group(1) if fp else None
    d['frontAnimId'] = field(b, 'frontAnimId')
    return d

def learnset(sym):
    for f in glob.glob(f'{EXP}/src/data/pokemon/level_up_learnsets/*.h'):
        t = open(f).read()
        m = re.search(r'static const struct LevelUpMove ' + sym + r'\[\] = \{(.*?)\};', t, re.S)
        if m:
            return [(int(a), mv) for a, mv in re.findall(r'LEVEL_UP_MOVE\(\s*(\d+),\s*(MOVE_\w+)\)', m.group(1))]
    return []

if __name__ == '__main__':
    out = {}
    for ours, key in NEW:
        d = parse(key)
        d['moves'] = learnset(d['learnset']) if d['learnset'] else []
        out[ours] = d
        print(ours, d['name'], d['types'], d['baseHP'], d['abilities'], d['evolutions'], d['gfx'], len(d['moves']), d['frontSize'])
    json.dump(out, open('/home/claude/tools/new_species.json', 'w'), indent=1)
