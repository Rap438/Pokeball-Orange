# Continuation checklist

The v0.4 plan, with where each step stands after v0.6 (unreleased). Details are in CHANGELOG_v0.6.md and TEST_RESULTS_v0.6.md.

| # | Step | Status |
|---|---|---|
| 1 | Oldale Town + Routes 103/102 with the `build_cove.py` approach | **Superseded.** Oldale moved to PboCove in v0.4 and was rearranged in v0.5. Routes 102 and 103 already get OG's hand-made seasonal art through the Petalburg tileset, and `build_cove.py`'s procedural repaint would replace it. Only revisit with new hand art. |
| 2 | Light pass for towns | **Partly done.** Pokémon Center and Mart sign glows in every town and Rustboro's street lamps (`tools/pbo/place_lights.py`). **Window glow is left:** each town tileset needs its window colours on palette slots nothing else uses, then a `.pla` listing them (docs/tutorials/dns.md). Check with the debug build at night (libfaketime works for the emulator). |
| 3 | Encounters route by route | **Done** for the listed areas: Granite Cave Mawile, Petalburg Woods Shroomish, Rusturf Tunnel, Routes 116/117. Routes 102, 104 and 115 were already on plan. Snubbull comes as a spring guest instead of a swarm. Later routes are still vanilla Emerald tables, which keep the Gen 3 level bands. |
| 4 | Gyms 2-8, Elite Four, Champion, Vegeta | **Done** (`tools/pbo/rebalance_trainers.py`, checked by `tools/pbo/check_trainers.py`). Gym leader rematch teams (`_2`-`_5`) are still vanilla. |
| 5 | Goku's EXP share vs Gen 9 scaled EXP | **Done.** His Pokémon-battle share now uses Gen 9 level scaling. Logging party levels at each badge in a real play-through is still worth doing. |
| 6 | Untested systems | **Partly done.** Tested: Nimbus and its badge/indoor rules, trainer power-up line, Surf and doors with a follower, Dragon Ball pickup and tracker (v0.5). **Still untested:** 1500-step re-scatter, tournament, gravity room, time chamber, bosses, fusion finale. |
| 7 | Opening-to-first-badge play-through (release build) | **Partly done.** Played from the title screen to Roshi's lab. Oldale → Petalburg → Woods → Rustboro → Yamcha is left. |
| 8 | v0.3 save converter | Not started. Only worth doing if players ask for it. |
| 9 | Uppercase-name option | Not started. Only if wanted; would need width checks against the battle UI. |
| 10 | Hardware check on a flash cart (32 MB ROM, 128 KB flash save) | Not possible from here. |

## How to test
- Build the runner against libmgba: `MGBA=/path/to/mgba tools_pbo/build.sh`.
- Drive it with `tools_pbo/emu.py`; see its docstring.
- `make debug` builds the debug ROM. Its debug menu opens with R held + START; hold R through Goku's power-up message first.
- The Script 1-8 slots in `data/scripts/debug.inc` are handy for one-off test setups (give a party, then `trainerbattle_no_intro`). Don't commit them.
