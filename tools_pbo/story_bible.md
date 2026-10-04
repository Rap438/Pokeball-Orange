# PokéBall Orange — Story Bible (script rewrite)

PokéBall Orange is a Dragon Ball Z–themed hack of Pokémon Emerald. Places, Pokémon, moves and game
mechanics stay Pokémon; the cast, factions and tone are Dragon Ball Z. Rewrite dialogue so it reads
like it was written for this world: in-character, coherent, the story ties together. Keep every
gameplay function intact (directions, hints, what an NPC gives you, rules of a facility, puzzle clues).

## The hero
- **{PLAYER}** is GOKU (the player can rename him; always write `{PLAYER}` where the hero's name goes,
  never a literal "GOKU" unless the baseline line already had it). Male. Cheerful, kind, a little naive,
  always hungry, loves a good fight and getting stronger. Raised on Earth, Saiyan by birth.
- Goku can fight on the overworld himself (KI BLAST, KAMEHAMEHA, punches) and transforms SUPER SAIYAN
  as he earns badges. NPCs may react to that ("Was that a SUPER SAIYAN?").
- He trains POKéMON alongside himself: "training together" is a recurring theme.
- `{KUN}` is an honorific placeholder that prints nothing in English — keep it where it was.

## Family & friends
- **CHI-CHI** (the MOM role): Goku's wife. Loving, strict, worries about him, nags about manners/food,
  proud of him. She moved with him to LITTLEROOT TOWN. She heals the team when he rests at home.
- **KRILLIN** (NORMAN's role): Goku's best friend, PETALBURG CITY GYM LEADER (Normal type). Where the
  original said "your dad", it is now Krillin — Goku's old friend who already became a GYM LEADER.
  He is proud of Goku and wants a real battle once Goku has four badges.
- **MASTER ROSHI** (PROF. BIRCH): the old Turtle Hermit, now a POKéMON researcher with a LAB in
  LITTLEROOT. Pervy-but-wise old master; gives Goku his first POKéMON and the POKéDEX.
- **VEGETA** (the rival MAY/BRENDAN): Saiyan prince, proud, competitive, calls Goku "KAKAROT" (only
  Vegeta uses that name). He just moved in next door with BULMA. He helps Roshi's research only because
  Bulma volunteered him, and starts his own POKéMON journey to prove he is stronger than Kakarot. Rival
  battles end with Vegeta challenging Goku to a fistfight (handled by code — you don't need to add it).
  He grudgingly respects Goku more as the story goes on.
- **BULMA** (the rival's mom): Vegeta's wife, genius of CAPSULE CORP. family, lives next door. Friendly
  with Goku and Chi-Chi, teases Vegeta. Tells Goku Vegeta started his own POKéMON adventure.
- **DR. BRIEF** (MR. STONE): Bulma's father, president of CAPSULE CORP. (DEVON's role) in RUSTBORO.
  Jolly inventor. Asks Goku to deliver a letter to his friend SUPREME KAI and the CC GOODS to the
  shipyard at SLATEPORT. (Steven is no longer his son — SUPREME KAI is "an old friend".)
- **UUB** (WALLY): a shy, frail boy with enormous hidden power that hasn't woken up yet. Goku helps him
  catch his first POKéMON; Uub moves to VERDANTURF for his health; he grows stronger and becomes a
  real rival at VICTORY ROAD. Goku sees huge potential in him.
- **SUPREME KAI** (STEVEN): calm, mysterious guardian of the universe who loves rare stones and quietly
  watches over Goku. Appears in Dewford/Granite Cave, Mt. Chimney aftermath, Mossdeep, Sootopolis, etc.
  Trainer name is SHIN.
- **THE ANNOUNCER** (SCOTT): the World Tournament announcer, scouting strong trainers for the BATTLE
  FRONTIER ("the ultimate tournament").

## Gym leaders, Elite Four, Champion
| Town | Original | Now | Personality |
|---|---|---|---|
| Rustboro | ROXANNE (Rock) | YAMCHA | ex-desert bandit, now a careful teacher at the trainer school; a bit unlucky |
| Dewford | BRAWLY (Fighting) | TIEN | stern three-eyed martial artist, trains in the cave/surf |
| Mauville | WATTSON (Electric) | OX-KING | Chi-Chi's huge jolly father, "Wahahaha!", built Mauville's power |
| Lavaridge | FLANNERY (Fire) | VIDEL | fiery, determined, Mr. Satan's daughter, new leader proving herself |
| Petalburg | NORMAN (Normal) | KRILLIN | Goku's best friend |
| Fortree | WINONA (Flying) | GREAT SAIYAMAN | Gohan's goofy masked hero persona, poses, justice speeches |
| Mossdeep | TATE & LIZA (Psychic) | GOTEN & TRUNKS | two kids who finish each other's sentences, mischievous |
| Sootopolis | JUAN (Water) | PICCOLO | stern Namekian, Goku's old rival turned ally |
| Elite Four | SIDNEY | SUPER BUU | gleeful, menacing, enjoys the fight |
| Elite Four | PHOEBE | BABA | Fortuneteller Baba, ghosts and the Other World |
| Elite Four | GLACIA | ANDROID 18 | cool, sarcastic, Krillin's wife |
| Elite Four | DRAKE | DENDE | young Guardian of Earth, dragon/Shenron lore |
| Champion | WALLACE | MR. SATAN | the "World Champion": loud, boastful, secretly kind; Videl's dad |

## The villains — one army
- **RED RIBBON ARMY**, led by **KID BUU** (MAXIE's role, class "RR LEADER"). Kid Buu took over the
  remains of the old Red Ribbon Army. He is pure chaos: he wants to wake the ancient titans GROUDON and
  KYOGRE and let them tear Hoenn apart, just to watch. He speaks in short, gleeful, menacing lines
  ("Hee hee hee!"), childish and terrifying.
- **DR. GERO** (ARCHIE's role, class "RR SCIENTIST"): the army's chief scientist. He runs the SEA squad
  and believes he can *control* KYOGRE with his machines; he thinks he's using Kid Buu. Arrogant,
  cold, calculating. At the end (Seafloor Cavern / Sootopolis) he realizes he has unleashed something he
  cannot control.
- Both of the original teams are the SAME army now. Where Emerald had MAGMA vs AQUA rivalry, write it
  as the army's two squads competing for Kid Buu's favor: the LAND squad (under GENERAL BLUE and MAI,
  chasing GROUDON) and the SEA squad (under DR. GERO with COLONEL SILVER and COLONEL VIOLET, chasing
  KYOGRE). Kid Buu wants BOTH titans awake. Never write "TEAM MAGMA"/"TEAM AQUA", MAJIN or SAIYAN ARMY.
- Officers: **GENERAL BLUE** (TABITHA, trainer name BLUE), **MAI** (COURTNEY), **COLONEL SILVER**
  (MATT, trainer name SILVER), **COLONEL VIOLET** (SHELLY, trainer name VIOLET).
- Grunts are RED RIBBON soldiers: bumbling, loud, salute a lot, scared of Kid Buu.
- Bases: RED RIBBON BASE (Magma Hideout, near Jagged Pass) and RED RIBBON SEA BASE (Aqua Hideout,
  Lilycove). The METEORITE, the MT. CHIMNEY machine, the SUBMARINE theft, the RED ORB/BLUE ORB at MT.
  PYRE, the SEAFLOOR CAVERN — all the same plot beats, now Red Ribbon schemes.
- RAYQUAZA at SKY PILLAR is the dragon that can calm the titans (you may tie it loosely to dragon /
  Shenron imagery, but don't rename it).

## Companies and other names
- CAPSULE CORP. (DEVON CORP), "CC GOODS" (DEVON GOODS), SCOUTER (DEVON SCOPE).
- Places keep their names (LITTLEROOT TOWN, ROUTE 101, MAUVILLE CITY...). POKéMON CENTER, POKé MART,
  GYM, BADGE, POKéDEX, POKéNAV stay. "POKéMON" is always spelled POKéMON.
- Money is ¥ shown by the game; you can call it ZENI in dialogue.
- Items use their in-game PokéBall Orange names (see /home/claude/tools/gen_item_renames.py for the
  mapping, e.g. POTION = SENZU SPROUT, FULL RESTORE = SENZU BEAN, REVIVE = DENDE'S HEAL, REPEL = KI
  HIDER, ESCAPE ROPE = INST. TRANS., EXP. SHARE = TRAINING GI, DEVON GOODS = CC GOODS, MACH BIKE =
  CAPSULE BIKE, ACRO BIKE = HOVER BIKE, MAGMA EMBLEM = RR EMBLEM). Items not in that list keep their
  names. HMs/TMs/moves/abilities/Pokémon keep their names.

## Tone
- Dragon Ball humor and heart: big appetites, training montages, "your power level is rising!",
  scouters, ki, the World Martial Arts Tournament, Kame House, Korin Tower, Kami's Lookout, King Kai,
  Capsule Corp. capsules. Use flavor sparingly so lines stay readable and useful.
- Ordinary townsfolk are ordinary people in a world where martial artists and POKéMON trainers mix.
  Rewrite them in the same spirit (don't make every NPC a DBZ character), and keep any practical info.
- Keep it all-ages, like the original.

## Hard technical rules (the build and the checker enforce these)
1. Only change the text inside `.string "..."` lines. Never touch labels, commands, `.string` count
   logic outside, or anything else. You may change how many `.string` lines a message uses.
2. Each message (a run of consecutive `.string` lines ending with `$`) must still end with `$`.
   Same number of messages, same order.
3. Keep every `{TOKEN}` that the message had (e.g. `{STR_VAR_1}`, `{RIVAL}`, `{PAUSE 30}`, `{COLOR …}`,
   `{PLAY_SE …}`, `{KUN}`); `{PLAYER}` may be added or removed. Don't add other tokens.
4. Line breaks: `\n` = second line of a box, `\l` = scroll to a new line, `\p` = new page. Every line
   must fit 208 pixels (roughly 33–36 characters). Run the reflow tool; it fixes overflow for you.
5. Only use characters the font has: A–Z a–z 0–9 space . , ! ? ' - … é ♂ ♀ & / : ; ( ) + = % and the
   like. No straight double quotes `"` (they break the string) — use “ and ” if needed. No em dash — use
   `-` or `…`. The checker lists anything invalid.
6. Workflow per file:
   `python3 /home/claude/tools/text_tool.py dump FILE` to read the messages,
   edit the `.string` lines,
   `python3 /home/claude/tools/text_tool.py reflow FILE` then
   `python3 /home/claude/tools/text_tool.py check FILE` until it prints OK.
   The original Emerald text (for meaning) is in /home/claude/restyle/orig_text/<same path>.
7. Do NOT run `make`, do NOT commit, do NOT edit files outside your assignment.
