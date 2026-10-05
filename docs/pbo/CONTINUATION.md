# Continuation checklist (after v0.4)

Ordered so that every step leaves a working, testable build. Branch: `expansion-port`. Test states: `mkex.sh`, `capture_all.sh`.

## Stage 4: expand the art and maps, rebalance the progression
1. **Oldale Town + Route 103/102.** Apply `build_cove.py`'s approach:
   - one `MapPaint` function per map
   - add the map's layout to `main()`
   - switch it to `gTileset_PboCove` (a superset of Petalburg)
   - run `check_connections.py`

   Budget: 121 tiles and 263 metatiles left in PboCove. Larger towns need their own superset tileset built the same way.
2. **Light pass for towns with windows.**
   - Add `.pla` light colours to the window palettes of each secondary tileset. Expansion's DNS docs point at commit a5b079d8 for ready-made Hoenn palettes.
   - Add `OBJ_EVENT_GFX_LIGHT_SPRITE` objects on lamps and Pokémon Center and Mart signs.
3. **Encounters, route by route** (`src/data/wild_encounters.json`): give each area a habitat theme and roughly a 5-10% Fairy option where it fits.
   - Route 102: Ralts up to 4%.
   - Petalburg Woods: Shroomish and Breloom line.
   - Route 104: Marill.
   - Rusturf Tunnel.
   - Dewford/Granite Cave: Sableye and Mawile, a Fairy/Steel answer to Tien's Fighting gym.
   - Route 116/117: Azurill, Marill, Ralts, Snubbull via swarm.

   Keep Gen 3 level bands.
4. **Gyms 2-8, Elite Four, rivals** (`src/data/trainers.party`). Same method as Yamcha:
   - same levels
   - real abilities, held items and coverage moves
   - AI flags `Basic Trainer` and above
   - Vegeta's teams scaled to the player's starter and rebuilt for Gen 9 typings

   Play-test each gym with a fresh state.
5. **Personal fight scaling.** Re-check Goku's EXP share against Gen 9 scaled EXP. Log party levels at each badge using the debug fight/level tools.

## Stage 5: cleanup and release
6. **Untested systems** to re-run on v0.4:
   - Nimbus flight and its Feather Badge rule
   - Dragon Ball pickup and Radar on the overworld
   - 1500-step re-scatter
   - tournament, gravity room, time chamber
   - bosses and fusion finale
   - trainer power-up line
   - Surf with a follower
   - house doors with a follower
7. **Opening to first badge play-through** in the release build: intro → Route 101 rescue → Roshi → Oldale → Route 102 → Petalburg (Krillin) → Petalburg Woods → Rustboro → Yamcha. Then the first personal fight after the badge and the first SSJ.
8. **Optional v0.3 save converter** (offline tool): read the v0.3 sectors, map species, items and flags to v0.4 IDs, write a v0.4 save. Only worth doing if players ask for it.
9. **Uppercase-name option** (if wanted): a build-time switch that uppercases species, move and item names, with width checks against the battle UI.
10. **Hardware check** on a flash cart. The 32 MB ROM needs a cart with 32 MB and 128 KB flash save support.
