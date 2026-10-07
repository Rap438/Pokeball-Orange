# PokéBall Orange v0.6 changelog (unreleased)

## Fixes
- **A fresh clone builds again.** Two things v0.5 needed were never committed:
  - `tools/compresSmol/` was gitignored as a whole directory. Its source is restored from pokeemerald-expansion 1.17.1 and only the build outputs are ignored now.
  - The location battle views (`graphics/dbz/battle_views`, all four seasons) only existed as `.4bpp`/`.gbapal`, which git drops. `tools/pbo/gen_battle_view_art.py` rebuilds them from the current seasonal tilesets (same method as v0.3's generator) as `.png`/`.pal` files git keeps, and regenerates the battle-view tables.
- **Bag: striped item list and black pocket bar.** The art pack renumbered the bag tiles but the bag code still used vanilla's tile numbers. After any pocket switch the item list turned navy/orange striped, and a black bar covered the pocket name box. The list now keeps its plain background, and the pocket switch wipes the list from the navy panel.
- **FLYING NIMBUS description** was 16 pixels too wide for the bag and shop windows; shortened.
- **Debug build** compiles with GCC 13 (Ubuntu's `gcc-arm-none-eabi`). Two locals in the daycare code are initialised.

## Trainers
Species, levels and IVs are unchanged (the Gen 3 level curve). Every team gets real abilities, held items, coverage moves and a plan, the same way Yamcha was rebuilt in v0.4.

| Trainer | Plan |
|---|---|
| Tien (Fighting) | Guts Machop, Pure Power Meditite, Makuhita with Fake Out and Bullet Punch (so a Fairy lead isn't a free win) |
| Ox-King (Electric) | Volt Switch Voltorb, paralysis, Fire Fang Manectric for Grass types |
| Videl (Fire) | Sun team: Simple Numel, Drought Torkoal with Overheat, Solar Beam and White Herb |
| Krillin (Normal) | Teeter Dance, Encore, Yawn; Gluttony Linoone with Belly Drum + Sitrus |
| Great Saiyaman (Flying) | Fairy-coverage Swablu, Harvest Tropius, Spikes Skarmory, Dragon Dance Altaria |
| Goten & Trunks (Psychic, doubles) | Claydol, Lunatone and Solrock all have Levitate, so the two Earthquakes only hit Goku's side; Tailwind Xatu |
| Piccolo (Water) | Rain team with two Swift Swim users and Draining Kiss |
| Super Buu (Dark) | Priority, Spikes, Super Luck + Scope Lens Absol |
| Baba (Ghost) | Burns, Prankster Recover, Destiny Bond, Trick Room Dusclops on Eviolite |
| Android 18 (Ice) | Hail, Ice Body; Sheer Cold is gone (a one-hit KO shouldn't decide an Elite Four battle) |
| Dende (Dragon) | Rock Head Shelgon on Eviolite, Focus Energy + Sniper Kingdra, Dragon Dance Salamence |
| Mr. Satan (Champion) | Rain, Toxic Spikes, Life Orb Ludicolo, Dragon Dance Gyarados, Scald + Recover Milotic |
| Vegeta (Rustboro to Lilycove) | Abilities, items and movesets by level band; Rustboro's AI is Basic Trainer like the rest |

- **AI:** gyms use Basic Trainer, HP Aware and Ace Pokémon. Gyms 6-8, the Elite Four and the Champion add Smart Switching.
- **Tools:** `tools/pbo/rebalance_trainers.py` makes the edit. `tools/pbo/check_trainers.py` checks every move, ability and item against the engine's data.
- **Not rebuilt:** gym leader rematch teams (`_2` to `_5`) are still expansion's vanilla data.

## Encounters
- **Granite Cave:** Mawile (Steel/Fairy) next to Sableye on 1F, B1F and B2F (5/5/9%), so there's a Fairy/Steel answer to Tien before the badge.
- **Petalburg Woods:** Shroomish raised from 15% to 25%.
- **Rusturf Tunnel:** a rock-tunnel habitat, with Geodude and Aron next to Whismur.
- **Routes 116 and 117:** Azurill 5% and Ralts 1% on Route 116; Ralts 1% on Route 117 next to Marill.
- **Already on plan:** Route 102 (Ralts 4%), Route 104 (Marill 20%) and Route 115 (Jigglypuff 10%) met the plan already. Snubbull is reachable as a spring guest Pokémon instead of a swarm.

## Goku
- **Goku's share of Pokémon-battle EXP follows Gen 9 level scaling,** the same as a Pokémon's: more when the foe outlevels him, less when he outlevels it.
  - Before, it was a flat quarter of the base EXP whatever his level. Under Gen 9's EXP formula that gave him about 40% more from wild battles than in v0.3.
  - Overworld fight EXP is unchanged.

## Night lights
- **Light glows in every town:** Pokémon Center and Mart signs glow at night in every town, and Rustboro's 17 street lamps light up. They're placed from the current maps by `tools/pbo/place_lights.py`, so they follow the v0.5 town layouts.
- **Windows don't glow yet.** That needs the art pack's window colours on their own palette slots.

## Tooling
- **`make check` passes** apart from the engine's own known failures. 16 tests hardcoded vanilla item names and now use PokéBall Orange's names.
- **Headless test runner:**
  - `tools_pbo/build.sh` builds it against any libmgba (`MGBA=...`).
  - It now reads commands interactively.
  - `tools_pbo/emu.py` drives it from Python, with BFS pathfinding over the live collision grid.
