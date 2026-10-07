# Battle ruleset and balance notes (v0.4)

## Ruleset
**Generation 9 main-series rules (Pokémon Scarlet/Violet, including The Teal Mask and The Indigo Disk), as implemented by pokeemerald-expansion 1.17.1 with `GEN_LATEST = GEN_9`.**

Why Gen 9: as of October 2026 it is still the newest released main-series turn-based ruleset. Generation 10 (*Pokémon Winds and Waves*) has been announced for 2027 but isn't out ([GameSpot](https://www.gamespot.com/articles/pokemon-winds-and-wave-the-10th-generation-of-pokemon-officially-revealed/1100-6538467/), [VICE](https://www.vice.com/en/article/pokemon-winds-and-waves-announced-gen-10-starters-revealed-release-date-delayed-to-2027/)). *Legends: Z-A* uses real-time battles and *Champions* is a battle-only spin-off, so neither is used. Expansion's `GEN_CHAMPIONS` settings stay off.

Gen 9 covers:
- the physical/special split, move power/accuracy/PP/priority/effects/targeting
- abilities with real behaviour, held items, status, weather, terrain
- the Fairy type with the full 18-type chart
- scaled EXP, EXP on catch, 1/4096 shiny odds
- trainer AI that knows these mechanics

Species data (stats, typings, abilities, learnsets, evolutions, forms, dex entries) is expansion's Gen 9 data. The old hand-made additions are gone. That fixes, for example:
- Xerneas: was Normal/Pressure, now Fairy/Fairy Aura
- Zacian: was Normal/Guts, now Fairy/Intrepid Sword (Crowned: Fairy/Steel)
- Koraidon: was Drought, now Orichalcum Pulse
- Miraidon: was Static, now Hadron Engine

## Deliberate exceptions
| Area | Choice | Why |
|---|---|---|
| Battle gimmicks | Tera and Dynamax off (flags 0). Megas and Z-Moves exist in the engine but no Mega Ring, Z-Ring or stone is given anywhere | Out of scope for this update and not part of PokéBall Orange's identity; Saiyan transformations are the gimmick |
| Exp. Share | Gen 6-8 key-item toggle (the renamed "Training Gi") instead of Gen 9's permanent always-on share | With always-on share the player outlevels the Gen 3 trainer curve. The toggle lets players keep a challenging curve |
| Roster | The v0.3 roster: national dex #1-386 plus the legendaries and pseudo-legendaries Shenron grants. Other Gen 4-9 species exist in data but aren't placed in the wild | The request doesn't ask for every Pokémon; wild tables stay Hoenn-flavoured |
| Overworld presentation | Gen 3 fishing minigame, berry growth and map pop-ups | Not battle rules; kept for the Emerald feel |
| Name casing | Menus and battle use the engine's modern mixed case (Azumarill, Senzu Bean); dialogue keeps Gen 3 caps for names | Uppercasing 1000+ species and move names would overflow the battle UI |
| Item names | DBZ flavour names (Senzu Sprout = Potion, etc.); effects are the Gen 9 effects (Super Potion heals 60, Hyper Potion 120) | Identity |
| Shiny odds option | Base 1/4096 (Gen 6+). OPTION offers about 1/2048, 1/820, 1/240 or always | Labels now match the real totals |

## Balance
**Principles:** keep the Gen 3 level curve, and add challenge through team building (abilities, items, status and move combos) rather than by raising levels. Fairy types and variety should show up early. Goku's own fights are balanced separately: his damage and HP use his own level and power level, never Pokémon stats, so Gen 9 changes don't affect them. Two things do feed back into Pokémon balance:
- Goku's fights give the party EXP.
- Personal fights can trigger evolutions.

The Exp. Share toggle exists partly to offset this.

**v0.4 changes**
- **Route 101** (Lv 2-3) is a meadow habitat:
  - Zigzagoon, Wurmple and Poochyena as before.
  - Azurill (Normal/Fairy, Hoenn-native) at 9%, and Ralts (Psychic/Fairy) at 1% as a rare find.
  - Taillow and Seedot for early team variety.
  - The new river has water encounters (Azurill, Marill, Lotad) and fishing (Magikarp, Goldeen, Barboach, with Marill and Azumarill deep on the Super Rod).
- **Littleroot's pool** uses the same water tables, so Marill (Water/Fairy) is the Surf-era Fairy pickup at home.
- **Fairy Feather** is hidden in Littleroot's flower bed. It's a Gen 9 Fairy-boosting item, so early Fairy users get a payoff.
- **Yamcha (Gym 1)** levels are unchanged (12/12/15), but the team now has a plan:
  - Geodude with Defense Curl into Rollout
  - Aron with Metal Claw for Fairy and Ice answers
  - Nosepass with Thunder Wave, Block and Rock Tomb
  - All three have Sturdy, so one-hit KOs don't work.
- **Tournament trainers** (J. Chun, M. Mask, Vegeta) moved into expansion format, with IVs scaled by round.
- **Cast data:** names and classes carried over, genders and encounter music fixed (Yamcha and Vegeta were set female).

**v0.6 changes** (details in CHANGELOG_v0.6.md)
- **Gyms 2-8, the Elite Four, the Champion and Vegeta** follow Yamcha's method: same species, levels and IVs, with real abilities, held items, coverage and a plan per team.
  - Gyms use the AI flags Basic Trainer, HP Aware and Ace Pokémon. Gym 6 onward adds Smart Switching.
  - Sheer Cold is gone from Android 18's team.
- **Fairy and Steel access before Tien:** Mawile in Granite Cave. Plus habitat tweaks in Petalburg Woods, Rusturf Tunnel and Routes 116/117.
- **Goku's share of Pokémon-battle EXP** uses Gen 9 level scaling instead of a flat quarter, so grinding weak wild Pokémon no longer levels him faster than his team.
- **Obedience** is the engine's Gen 8+ rule (unchanged, noted here because it surprises people): a Pokémon may ignore orders when the level it was *met* at is above the badge cap, even if Goku caught it. Training a low-level catch past the cap is fine.

**Still to do:** gym leader rematch teams and the later routes' encounter tables are vanilla Emerald data. See CONTINUATION.md.
