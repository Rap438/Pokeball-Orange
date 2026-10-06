# Test results (v0.5)

**Method:** the libmgba core driven headlessly by scripted input (`tools_pbo/play.py`, `runner.c`), with screenshots and memory reads. Everything ran in an emulator. **Nothing was tested on real hardware.** Captures are in `screens/`.

| # | Test | Build | Result |
|---|---|---|---|
| 1 | Beat a trainer's POKéMON (Lass Janice, Route 116), talk again, YES, fistfight, EXP | debug | pass |
| 2 | Dragon Ball visible on the overworld (Route 109); pick it up (var set to collected) | debug | pass |
| 3 | Tracker above the HUD: 6 of 7 lit; all grey while stone | debug | pass |
| 4 | Senzu Sprout on Goku Lv 40: +87 HP (25% of 350) | debug | pass |
| 5 | Senzu Bean revives a KO'd Goku to full; Dende's Heal revives to half; Dende's Heal goes to the party when Goku is healthy | debug | pass |
| 6 | Route 101 region menu → Kanto shows Bulbasaur/Charmander/Squirtle; Alola shows Rowlet/Litten/Popplio; Hoenn shows Torchic | debug | pass |
| 7 | Charmander chosen → first battle → lab → Route 103 Vegeta leads with Squirtle | debug | pass |
| 8 | Seasons: camp, Route 101, wild battle view, bag, party, option menu in all four seasons | debug | pass |
| 9 | Season lock (winter) saved, rebooted from the battery save, still winter | debug | pass |
| 10 | Visible wild POKéMON spawn on Route 101; walking into a Wurmple starts the battle | debug | pass |
| 11 | Guest POKéMON: a Poltchageist (Gen 9) spawned on Route 101 in autumn | debug | pass |
| 12 | Town POKéMON: Dewford's Wingull turns and cries when talked to | debug | pass |
| 13 | Every door in the 13 rearranged towns entered from the tile below it (map changed to the right interior) | debug | pass. 9 doors can't be reached on a flat path because of stairs or water, as in the original maps; they were entered directly and all work, except Lavaridge warp 5 at (9,2), which isn't a door you enter from below (unchanged from the original). |
| 14 | Fallarbor Mart: walk from the road, enter, exit back to the same spot | debug | pass |
| 15 | Map seam audit (`check_connections.py`) unchanged from before the city work | — | pass |
| 16 | Release build: boots, intro to the house, R+SELECT debug and engine debug menu don't open | release | pass |

## Not tested this session
- Every scripted cutscene in the rearranged towns. Their routes were kept identical tile for tile, but they weren't played.
- Flying (Instant Transmission) to each moved POKéMON CENTER.
- The release build's opening past the house (the same scripts were tested in the debug build).
- The v0.4 → v0.5 save carry-over in a town whose layout changed.
- Plus the items still listed in v0.4's TEST_RESULTS.md (Nimbus, Gym 1, tournament, bosses, finale, Surf with a follower).
