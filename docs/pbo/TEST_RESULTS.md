# Test results (v0.4)

**Method:** the libmgba core driven headlessly by scripted button input (`tools_pbo/play.py`, `runner.c`), with screenshots and memory reads (`getvar`, `peek8`). Everything below ran on the emulator. **Nothing was tested on real hardware.**

| # | Test | Build | Result |
|---|---|---|---|
| 1 | v0.3.0 source rebuild equals the reference ROM SHA-256 | v0.3 | pass |
| 2 | Boot, title, new game, Roshi intro, naming (shows Goku), truck, arrival with Chi-Chi | debug | pass |
| 3 | Wild battle (Zigzagoon): move, EXP, return to field | debug | pass |
| 4 | Personal fight: spawn, punches, ki charge and blast, win, EXP leading to evolution | debug | pass |
| 5 | Super Saiyan transform plus control hints; walking in SSJ | debug | pass |
| 6 | Shenron: summon, cancel (balls kept, wishes stay 0, party unchanged) | debug | pass |
| 7 | Shenron: wish x5, party 2→6, then the 7th mon goes to the PC, stone timer 1500, wish count 5 | debug | pass after fix (the "sent to PC" message was skipped; re-tested after the fix, `screens/17`) |
| 8 | Wish list preview pictures have correct colours | debug | pass after fix |
| 9 | Save, then continue: party, Goku vars, Dragon Ball vars | debug | pass |
| 10 | Save, then continue: story vars | release | pass |
| 11 | FOLLOWER OFF in OPTION, save, continue, follower still off | debug | pass |
| 12 | Follower: spawns, follows, talk (emote and message), hidden during a personal fight, shown after Instant Transmission | debug | pass |
| 13 | v0.3 `.sav` on v0.4 shows the legacy notice, not "erased" | debug | pass |
| 14 | Release build: R+SELECT DBZ debug and R+START engine debug menu don't open | release | pass |
| 15 | Demo town at day, morning, evening and night: light pool, lamps, river animation, walk-behind tent | debug | pass (`screens/01-07`) |
| 16 | Collision: river not walkable (surf prompt), tent/crate/log/fire solid, tent ridge walkable | debug | pass |
| 17 | Route 101 path and river across the Littleroot seam, and Oldale's view of Route 101 | debug | pass after fix (Oldale moved to the superset tileset) |
| 18 | Instant Transmission to Oldale | debug | pass |
| 19 | Fairy: Azumarill Water/Fairy with Huge Power in the summary | debug | pass (`screens/14-15`) |
| 20 | BPS patches apply to clean Emerald and match the ROMs byte-for-byte | both | pass |

## Not tested this session
- Nimbus flight
- Gym 1 battle
- Trainer power-up message in a real trainer battle
- Tournament, gravity room, time chamber
- Boss fights
- Fusion finale
- Dragon Ball pickup and Radar on the overworld
- Re-scatter after 1500 steps
- Surf with a follower

The code paths for these are unchanged or ported 1:1, but they weren't run in v0.4.
