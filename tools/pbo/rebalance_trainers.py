#!/usr/bin/env python3
"""Gym 2-8, Elite Four, Champion and Vegeta team rebuild for the Gen 9 engine (CONTINUATION.md step 4).

Same method as Yamcha in v0.4: species, levels and IVs stay exactly as they were (the Gen 3 level curve),
and each team gets real abilities, held items, coverage moves and a plan. The script edits the existing
blocks in src/data/trainers.party in place and refuses to run if a team's species/levels don't match what
it expects, so it can't silently overwrite a team that was changed by hand. Re-running it is a no-op.
Afterwards: tools/pbo/check_trainers.py validates every move, ability and item against the engine data.
usage: rebalance_trainers.py
"""
import os, re, sys

R = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PARTY = os.path.join(R, 'src/data/trainers.party')

GYM_AI = 'Basic Trainer / HP Aware / Ace Pokemon'
LATE_AI = 'Basic Trainer / Smart Switching / HP Aware / Ace Pokemon'

# trainer -> (AI line or None to keep, [(species, item, ability, [moves])...]) in party order
TEAMS = {
    # Gym 2, Tien (Fighting). Machop and Makuhita hit through bulk, Meditite's Pure Power hits hard but
    # is frail; Bullet Punch means a Fairy lead isn't a free win.
    'TRAINER_BRAWLY_1': (GYM_AI, [
        ('Machop', None, 'Guts', ['Low Kick', 'Karate Chop', 'Rock Tomb', 'Bulk Up']),
        ('Meditite', None, 'Pure Power', ['Force Palm', 'Confusion', 'Detect', 'Bulk Up']),
        ('Makuhita', 'Sitrus Berry', 'Thick Fat', ['Fake Out', 'Arm Thrust', 'Bullet Punch', 'Bulk Up']),
    ]),
    # Gym 3, Ox-King (Electric). Volt Switch pivoting, paralysis, and Fire Fang for the Grass types that
    # resist Electric.
    'TRAINER_WATTSON_1': (GYM_AI, [
        ('Voltorb', None, 'Aftermath', ['Volt Switch', 'Shock Wave', 'Light Screen', 'Self Destruct']),
        ('Electrike', None, 'Static', ['Spark', 'Bite', 'Quick Attack', 'Howl']),
        ('Magneton', None, 'Sturdy', ['Thunder Wave', 'Shock Wave', 'Mirror Shot', 'Metal Sound']),
        ('Manectric', 'Sitrus Berry', 'Static', ['Shock Wave', 'Fire Fang', 'Thunder Wave', 'Howl']),
    ]),
    # Gym 4, Videl (Fire). A sun team: Torkoal's Drought powers Overheat and a one-turn Solar Beam for
    # Water types; White Herb undoes the Overheat drop once.
    'TRAINER_FLANNERY_1': (GYM_AI, [
        ('Numel', None, 'Simple', ['Flame Burst', 'Bulldoze', 'Growth', 'Take Down']),
        ('Slugma', None, 'Flame Body', ['Flame Burst', 'Rock Tomb', 'Light Screen', 'Will O Wisp']),
        ('Camerupt', None, 'Solid Rock', ['Lava Plume', 'Bulldoze', 'Rock Slide', 'Yawn']),
        ('Torkoal', 'White Herb', 'Drought', ['Overheat', 'Solar Beam', 'Body Slam', 'Protect']),
    ]),
    # Gym 5, Krillin (Normal). Status and tempo: Teeter Dance, Encore, Yawn, then Linoone's Belly Drum
    # with Gluttony + Sitrus to heal straight back up (no priority move, so Goku's side gets a turn to answer).
    # Shadow Claw covers Ghost types.
    'TRAINER_NORMAN_1': (GYM_AI, [
        ('Spinda', None, 'Own Tempo', ['Teeter Dance', 'Psybeam', 'Sucker Punch', 'Facade']),
        ('Vigoroth', None, 'Vital Spirit', ['Slash', 'Shadow Claw', 'Encore', 'Bulk Up']),
        ('Linoone', 'Sitrus Berry', 'Gluttony', ['Belly Drum', 'Slash', 'Seed Bomb', 'Facade']),
        ('Slaking', 'Sitrus Berry', 'Truant', ['Facade', 'Shadow Claw', 'Yawn', 'Slack Off']),
    ]),
    # Gym 6, Great Saiyaman (Flying). Swablu brings Fairy coverage, Tropius recycles Sitrus with Harvest,
    # Skarmory lays Spikes, Altaria sets up.
    'TRAINER_WINONA_1': (LATE_AI, [
        ('Swablu', None, 'Natural Cure', ['Disarming Voice', 'Aerial Ace', 'Sing', 'Safeguard']),
        ('Tropius', 'Sitrus Berry', 'Harvest', ['Magical Leaf', 'Air Cutter', 'Leech Seed', 'Synthesis']),
        ('Pelipper', None, 'Keen Eye', ['Water Pulse', 'Air Cutter', 'Protect', 'Supersonic']),
        ('Skarmory', None, 'Sturdy', ['Steel Wing', 'Aerial Ace', 'Spikes', 'Sand Attack']),
        ('Altaria', 'Lum Berry', 'Natural Cure', ['Dragon Dance', 'Dragon Breath', 'Aerial Ace', 'Earthquake']),
    ]),
    # Gym 7, Goten & Trunks (Psychic, double battle). Claydol, Lunatone and Solrock all have Levitate and
    # Xatu flies, so both Earthquakes hit only Goku's side. Xatu sets Tailwind.
    'TRAINER_TATE_AND_LIZA_1': ('Basic Trainer / Smart Switching / HP Aware', [
        ('Claydol', None, 'Levitate', ['Earthquake', 'Psychic', 'Ancient Power', 'Reflect']),
        ('Xatu', None, 'Magic Bounce', ['Psychic', 'Air Slash', 'Tailwind', 'Confuse Ray']),
        ('Lunatone', 'Sitrus Berry', 'Levitate', ['Psychic', 'Ice Beam', 'Calm Mind', 'Hypnosis']),
        ('Solrock', 'Sitrus Berry', 'Levitate', ['Rock Slide', 'Zen Headbutt', 'Earthquake', 'Will O Wisp']),
    ]),
    # Gym 8, Piccolo (Water). Luvdisc sets rain for the Swift Swim pair; Draining Kiss gives Fairy
    # coverage, Whiscash sets up Dragon Dance.
    'TRAINER_JUAN_1': (LATE_AI, [
        ('Luvdisc', None, 'Swift Swim', ['Rain Dance', 'Water Pulse', 'Draining Kiss', 'Attract']),
        ('Whiscash', None, 'Oblivious', ['Dragon Dance', 'Waterfall', 'Earthquake', 'Zen Headbutt']),
        ('Sealeo', None, 'Thick Fat', ['Surf', 'Aurora Beam', 'Body Slam', 'Encore']),
        ('Crawdaunt', None, 'Adaptability', ['Crabhammer', 'Crunch', 'Aqua Jet', 'Taunt']),
        ('Kingdra', 'Chesto Berry', 'Swift Swim', ['Surf', 'Dragon Pulse', 'Ice Beam', 'Rest']),
    ]),
    # Elite Four 1, Super Buu (Dark). Priority (Sucker Punch, Fake Out), Spikes, and a high-crit Absol.
    'TRAINER_SIDNEY': ('Basic Trainer / Smart Switching / HP Aware / Ace Pokemon / Force Setup First Turn', [
        ('Mightyena', None, 'Intimidate', ['Crunch', 'Fire Fang', 'Play Rough', 'Taunt']),
        ('Shiftry', None, 'Chlorophyll', ['Leaf Blade', 'Sucker Punch', 'Fake Out', 'Swords Dance']),
        ('Cacturne', None, 'Water Absorb', ['Seed Bomb', 'Sucker Punch', 'Spikes', 'Leech Seed']),
        ('Crawdaunt', None, 'Adaptability', ['Crabhammer', 'Crunch', 'Aqua Jet', 'Swords Dance']),
        ('Absol', 'Scope Lens', 'Super Luck', ['Night Slash', 'Psycho Cut', 'Play Rough', 'Swords Dance']),
    ]),
    # Elite Four 2, Baba (Ghost). Burns, Prankster Recover, Destiny Bond, and a Trick Room Dusclops ace
    # on Eviolite.
    'TRAINER_PHOEBE': (LATE_AI, [
        ('Dusclops', None, 'Pressure', ['Will O Wisp', 'Night Shade', 'Pain Split', 'Protect']),
        ('Banette', None, 'Cursed Body', ['Shadow Claw', 'Sucker Punch', 'Knock Off', 'Thunder Wave']),
        ('Sableye', None, 'Prankster', ['Shadow Ball', 'Foul Play', 'Recover', 'Confuse Ray']),
        ('Banette', None, 'Insomnia', ['Shadow Ball', 'Thunderbolt', 'Psychic', 'Destiny Bond']),
        ('Dusclops', 'Eviolite', 'Frisk', ['Trick Room', 'Shadow Punch', 'Earthquake', 'Ice Punch']),
    ]),
    # Elite Four 3, Android 18 (Ice). Hail chips non-Ice types, heals Ice Body and makes Blizzard always hit
    # (Sealeo can't learn Snowscape).
    # Sheer Cold is gone: a one-hit KO shouldn't decide an Elite Four battle.
    'TRAINER_GLACIA': (LATE_AI, [
        ('Sealeo', None, 'Ice Body', ['Hail', 'Ice Beam', 'Body Slam', 'Encore']),
        ('Glalie', None, 'Ice Body', ['Ice Beam', 'Crunch', 'Light Screen', 'Protect']),
        ('Sealeo', None, 'Thick Fat', ['Blizzard', 'Surf', 'Body Slam', 'Rest']),
        ('Glalie', None, 'Inner Focus', ['Ice Fang', 'Earthquake', 'Crunch', 'Explosion']),
        ('Walrein', 'Sitrus Berry', 'Thick Fat', ['Surf', 'Ice Beam', 'Earthquake', 'Yawn']),
    ]),
    # Elite Four 4, Dende (Dragon). Rock Head Shelgon on Eviolite, Focus Energy + Sniper Kingdra,
    # Dragon Dance Salamence ace.
    'TRAINER_DRAKE': (LATE_AI, [
        ('Shelgon', 'Eviolite', 'Rock Head', ['Double Edge', 'Dragon Claw', 'Zen Headbutt', 'Protect']),
        ('Altaria', None, 'Natural Cure', ['Dragon Dance', 'Dragon Claw', 'Earthquake', 'Roost']),
        ('Kingdra', None, 'Sniper', ['Surf', 'Dragon Pulse', 'Ice Beam', 'Focus Energy']),
        ('Flygon', None, 'Levitate', ['Earthquake', 'Dragon Claw', 'Fire Punch', 'U Turn']),
        ('Salamence', 'Sitrus Berry', 'Intimidate', ['Dragon Dance', 'Dragon Claw', 'Earthquake', 'Fire Fang']),
    ]),
    # Champion, Mr. Satan (Water). Rain from Wailord, Toxic Spikes, Life Orb Ludicolo, Dragon Dance
    # Gyarados on Lum, and a Milotic ace with Scald and Recover.
    'TRAINER_WALLACE': (LATE_AI, [
        ('Wailord', None, 'Pressure', ['Rain Dance', 'Water Spout', 'Ice Beam', 'Heavy Slam']),
        ('Tentacruel', None, 'Clear Body', ['Hydro Pump', 'Sludge Bomb', 'Ice Beam', 'Toxic Spikes']),
        ('Ludicolo', 'Life Orb', 'Swift Swim', ['Giga Drain', 'Surf', 'Ice Beam', 'Fake Out']),
        ('Whiscash', 'Chesto Berry', 'Oblivious', ['Earthquake', 'Waterfall', 'Zen Headbutt', 'Rest']),
        ('Gyarados', 'Lum Berry', 'Intimidate', ['Dragon Dance', 'Waterfall', 'Earthquake', 'Ice Fang']),
        ('Milotic', 'Leftovers', 'Marvel Scale', ['Scald', 'Ice Beam', 'Recover', 'Toxic']),
    ]),
}

# Vegeta: (species, min level) -> (item, ability, moves). The highest band at or below the mon's level
# applies. Mons below every band keep the engine's default level-up moves (as do the L5 starters).
# Non-Hoenn starter regions swap the starter species at load (src/dbz_starters.c), which resets its
# moves to that species' defaults but keeps the held item.
RIVAL_SETS = {
    ('Wingull', 13): (None, 'Keen Eye', ['Water Gun', 'Wing Attack', 'Supersonic', 'Quick Attack']),
    ('Lotad', 13): (None, 'Swift Swim', ['Absorb', 'Water Gun', 'Growl', 'Mist']),
    ('Slugma', 13): (None, 'Flame Body', ['Ember', 'Rock Throw', 'Harden', 'Smog']),
    ('Torkoal', 13): (None, 'White Smoke', ['Ember', 'Smog', 'Withdraw', 'Rapid Spin']),
    ('Treecko', 15): ('Oran Berry', 'Overgrow', None),
    ('Torchic', 15): ('Oran Berry', 'Blaze', None),
    ('Mudkip', 15): ('Oran Berry', 'Torrent', None),
    ('Slugma', 18): (None, 'Flame Body', ['Incinerate', 'Rock Throw', 'Yawn', 'Smog']),
    ('Wingull', 18): (None, 'Keen Eye', ['Wing Attack', 'Water Pulse', 'Supersonic', 'Quick Attack']),
    ('Lombre', 18): (None, 'Swift Swim', ['Fake Out', 'Mega Drain', 'Bubble Beam', 'Fury Swipes']),
    ('Grovyle', 20): ('Oran Berry', 'Overgrow', ['Bullet Seed', 'Quick Attack', 'Fury Cutter', 'Detect']),
    ('Combusken', 20): ('Oran Berry', 'Blaze', ['Double Kick', 'Flame Charge', 'Peck', 'Bulk Up']),
    ('Marshtomp', 20): ('Oran Berry', 'Torrent', ['Mud Shot', 'Water Pulse', 'Rock Tomb', 'Protect']),
    ('Slugma', 29): (None, 'Flame Body', ['Flame Burst', 'Ancient Power', 'Will O Wisp', 'Light Screen']),
    ('Pelipper', 29): (None, 'Keen Eye', ['Water Pulse', 'Air Cutter', 'Protect', 'Supersonic']),
    ('Lombre', 29): (None, 'Swift Swim', ['Fake Out', 'Giga Drain', 'Bubble Beam', 'Icy Wind']),
    ('Grovyle', 31): ('Sitrus Berry', 'Overgrow', ['Leaf Blade', 'X Scissor', 'Quick Attack', 'Detect']),
    ('Combusken', 31): ('Sitrus Berry', 'Blaze', ['Blaze Kick', 'Double Kick', 'Bulk Up', 'Rock Slide']),
    ('Marshtomp', 31): ('Sitrus Berry', 'Torrent', ['Muddy Water', 'Mud Shot', 'Rock Slide', 'Protect']),
    ('Tropius', 31): ('Sitrus Berry', 'Harvest', ['Magical Leaf', 'Air Slash', 'Leech Seed', 'Synthesis']),
    ('Slugma', 32): (None, 'Flame Body', ['Flamethrower', 'Ancient Power', 'Will O Wisp', 'Light Screen']),
    ('Pelipper', 32): (None, 'Keen Eye', ['Water Pulse', 'Air Slash', 'Protect', 'Roost']),
    ('Ludicolo', 32): (None, 'Swift Swim', ['Giga Drain', 'Surf', 'Ice Beam', 'Fake Out']),
    ('Grovyle', 34): ('Sitrus Berry', 'Overgrow', ['Leaf Blade', 'X Scissor', 'Quick Attack', 'Swords Dance']),
    ('Combusken', 34): ('Sitrus Berry', 'Blaze', ['Blaze Kick', 'Brick Break', 'Bulk Up', 'Rock Slide']),
    ('Marshtomp', 34): ('Sitrus Berry', 'Torrent', ['Muddy Water', 'Mud Shot', 'Rock Slide', 'Protect']),
}
RIVAL_RE = re.compile(r'TRAINER_(MAY|BRENDAN)_(ROUTE_103|RUSTBORO|ROUTE_110|ROUTE_119|LILYCOVE)_(TREECKO|TORCHIC|MUDKIP)$')

def split_blocks(text):
    parts = re.split(r'(?m)^(?==== TRAINER_\w+ ===$)', text)
    return parts[0], parts[1:]

def parse_mons(block):
    """header lines, [mon dicts with 'species','level','ivs','extra'] for a trainer block."""
    lines = block.rstrip('\n').split('\n')
    # header ends at the first blank line
    i = lines.index('') if '' in lines else len(lines)
    header, rest = lines[:i], lines[i:]
    mons, cur = [], None
    for line in rest:
        if not line.strip():
            cur = None
            continue
        if cur is None:
            sp = line.split(' @ ')[0].strip()
            cur = {'species': sp, 'level': None, 'ivs': None, 'raw': [line]}
            mons.append(cur)
            continue
        cur['raw'].append(line)
        if line.startswith('Level:'):
            cur['level'] = int(line.split(':')[1])
        elif line.startswith('IVs:'):
            cur['ivs'] = line
    return header, mons

def render(header, ai, mons, sets):
    out = []
    for h in header:
        if ai and h.startswith('AI:'):
            h = f'AI: {ai}'
        out.append(h)
    for mon, (species, item, ability, moves) in zip(mons, sets):
        if species != mon['species']:
            raise SystemExit(f"expected {species}, found {mon['species']}")
        out.append('')
        out.append(species + (f' @ {item}' if item else ''))
        out.append(f"Level: {mon['level']}")
        if ability:
            out.append(f'Ability: {ability}')
        if mon['ivs']:
            out.append(mon['ivs'])
        if moves:
            out += [f'- {m}' for m in moves]
        else:
            out += [l for l in mon['raw'] if l.startswith('- ')]
    return '\n'.join(out) + '\n\n'

def rival_set(species, level):
    best = None
    for (sp, lv), s in RIVAL_SETS.items():
        if sp == species and lv <= level and (best is None or lv > best[0]):
            best = (lv, s)
    return best[1] if best else None

def main():
    pre, blocks = split_blocks(open(PARTY).read())
    done = set()
    for n, block in enumerate(blocks):
        name = re.match(r'=== (TRAINER_\w+) ===', block).group(1)
        header, mons = parse_mons(block)
        if name in TEAMS:
            ai, sets = TEAMS[name]
            if len(sets) != len(mons):
                raise SystemExit(f'{name}: expected {len(sets)} mons, found {len(mons)}')
            blocks[n] = render(header, ai, mons, sets)
            done.add(name)
        elif RIVAL_RE.match(name):
            sets = []
            for mon in mons:
                s = rival_set(mon['species'], mon['level'])
                if s:
                    sets.append((mon['species'],) + s)
                else:
                    sets.append((mon['species'], None, None, None))
            blocks[n] = render(header, 'Basic Trainer', mons, sets)
            done.add(name)
    missing = set(TEAMS) - done
    if missing:
        raise SystemExit(f'not found: {sorted(missing)}')
    open(PARTY, 'w').write(pre + ''.join(blocks).rstrip('\n') + '\n')
    print(f'rebuilt {len(done)} trainers')

if __name__ == '__main__':
    main()
