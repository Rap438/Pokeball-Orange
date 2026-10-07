# Test results (v0.6, unreleased)

**Method:**
- **Engine suite:** `make check`, the engine's own test suite (5861 tests).
- **Emulator:** mGBA (libmgba built from source) driven headlessly by `tools_pbo/emu.py`, with screenshots and memory reads.
- **Nights:** faked with libfaketime.
- **Battles:** gym and Elite Four battles were started from temporary debug-menu scripts. These were never committed.
- **Hardware:** nothing was tested on real hardware.

| # | Test | Build | Result |
|---|---|---|---|
| 1 | Fresh clone builds (release and debug) with Ubuntu's `gcc-arm-none-eabi` 13.2 | both | pass |
| 2 | `make check` | test | pass. 5395 passed and the rest are known failing, expected failing or TO_DO. The 18 v0.5 failures (16 renamed-item test strings, the Nimbus text width, bag sort order) are fixed and re-run |
| 3 | New game in the release build: title → intro → truck → Chi-Chi → clock → TV → Bulma → Vegeta's room → Route 101 rescue → region menu → Torchic → first battle → Roshi's lab | release | pass |
| 4 | Regenerated location battle view on Route 101 (real spot, platforms, night tint) | release | pass |
| 5 | Regenerated battle views: all 19 views × 4 seasons rendered offline from the generated data; every metatile used by their maps resolves to a tile | — | pass |
| 6 | Tien, Ox-King, Videl, Krillin, Great Saiyaman, Goten & Trunks (double), Piccolo, Mr. Satan: battle starts, portrait, new moves and abilities in use (Simple Growth, Rain Dance, Water Spout…) | debug | pass |
| 7 | Super Buu (Intimidate), Baba (Pressure), Android 18, Dende; Vegeta Rustboro, Route 110, Route 119 (Harvest Tropius at Lilycove) | debug | pass |
| 8 | Trainer last-Pokémon power-up line ("Their last POKéMON is fired up!") | debug | pass |
| 9 | Flying Nimbus: refused indoors (Krillin's advice), fly map, flight, landing at Oldale's moved Pokémon Center | debug | pass |
| 10 | Bag: pocket switching keeps the plain list background, no black bar over the pocket name | debug | pass |
| 11 | Night lights: Oldale P.C sign and Rustboro street lamps glow at night | debug | pass |
| 12 | Surf with a follower: the follower is put away while surfing and comes back after landing | debug | pass |
| 13 | House door with a follower: it enters with Goku and comes back out after his first step outside | debug | pass |

## Not tested
- **Long or scripted systems:**
  - the 1500-step Dragon Ball re-scatter
  - the tournament
  - the gravity room
  - the time chamber
  - the bosses and the fusion finale
  - a full play-through to the first badge in the release build (this session stopped at Roshi's lab)
- **Balance:** full gym battles to the end. The battles above checked that each team loads and fights with its new sets; they don't prove the difficulty is right.
- **Mart sign glows:** only Pokémon Center signs were looked at; Mart signs use the same offset.
- **Real hardware.**
