#!/usr/bin/env python3
"""Sanity-check hand-built trainer teams in src/data/trainers.party against the engine's data.

For every Pokemon of the named trainers (or every trainer with --all) it checks that
  - the species exists,
  - the ability is one of the species' abilities,
  - each move exists and is learnable by the species or one of its pre-evolutions
    (src/data/pokemon/all_learnables.json, the union of level-up, TM, tutor and egg moves),
  - the held item exists.
usage: check_trainers.py [--all | TRAINER_X ...]
"""
import json, os, re, sys

R = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def norm(s):
    return re.sub(r'[^A-Z0-9]', '_', s.upper().replace('é', 'E')).strip('_')

def load():
    learn = json.load(open(f'{R}/src/data/pokemon/all_learnables.json'))
    moves = set(re.findall(r'^\s*(MOVE_\w+)\s*=', open(f'{R}/include/constants/moves.h').read(), re.M))
    items = set(re.findall(r'#define (ITEM_\w+)\s', open(f'{R}/include/constants/items.h').read()))
    items |= set(re.findall(r'^\s*(ITEM_\w+)\s*[=,]', open(f'{R}/include/constants/items.h').read(), re.M))
    abil, prevo = {}, {}
    for fn in sorted(os.listdir(f'{R}/src/data/pokemon/species_info')):
        txt = open(f'{R}/src/data/pokemon/species_info/{fn}').read()
        for m in re.finditer(r'\[(SPECIES_\w+)\]\s*=\s*\{(.*?)\n    \},', txt, re.S):
            sp, body = m.group(1), m.group(2)
            a = re.search(r'\.abilities = \{([^}]*)\}', body)
            if a:
                abil[sp] = {x.strip() for x in a.group(1).split(',')} - {'ABILITY_NONE'}
            for e in re.finditer(r'\{EVO_\w+,[^}]*?(SPECIES_\w+)\s*(?:,[^}]*)?\}', body):
                prevo.setdefault(e.group(1), sp)
    return learn, moves, items, abil, prevo

def parse(path):
    trainers, cur, mon = {}, None, None
    for ln, line in enumerate(open(path), 1):
        line = line.rstrip('\n')
        m = re.match(r'=== (TRAINER_\w+) ===', line)
        if m:
            cur = trainers.setdefault(m.group(1), {'mons': [], 'line': ln}); mon = None
            continue
        if cur is None:
            continue
        if not line.strip():
            mon = None
            continue
        if mon is None and not re.match(r'^[A-Z][\w ]*:', line) and not line.startswith('- '):
            sp, _, item = line.partition(' @ ')
            sp = re.sub(r'\s*\((M|F)\)\s*$', '', sp).strip()
            nick = re.match(r'.*\((.*)\)$', sp)
            if nick:
                sp = nick.group(1)
            mon = {'species': sp, 'item': item.strip(), 'moves': [], 'ability': None, 'line': ln}
            cur['mons'].append(mon)
        elif mon is not None:
            if line.startswith('- '):
                mon['moves'].append(line[2:].strip())
            elif line.startswith('Ability:'):
                mon['ability'] = line.split(':', 1)[1].strip()
    return trainers

def main(argv):
    learn, moves, items, abil, prevo = load()
    trainers = parse(f'{R}/src/data/trainers.party')
    names = list(trainers) if argv == ['--all'] else argv
    bad = 0
    for t in names:
        for mon in trainers[t]['mons']:
            sp = 'SPECIES_' + norm(mon['species'])
            where = f"{t} {mon['species']} (line {mon['line']})"
            if sp not in abil:
                print(f'{where}: unknown species'); bad += 1; continue
            if mon['ability'] and 'ABILITY_' + norm(mon['ability']) not in abil[sp]:
                print(f"{where}: ability {mon['ability']} not in {sorted(abil[sp])}"); bad += 1
            if mon['item'] and 'ITEM_' + norm(mon['item']) not in items:
                print(f"{where}: unknown item {mon['item']}"); bad += 1
            chain, s = [], sp
            while s and s not in chain:
                chain.append(s); s = prevo.get(s)
            ok = set()
            for s in chain:
                ok |= set(learn.get(s.replace('SPECIES_', ''), []))
            for mv in mon['moves']:
                c = 'MOVE_' + norm(mv)
                if c not in moves:
                    print(f'{where}: unknown move {mv}'); bad += 1
                elif c not in ok:
                    print(f'{where}: {mv} not learnable'); bad += 1
    print(f'{len(names)} trainers checked, {bad} problems')
    return 1 if bad else 0

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
