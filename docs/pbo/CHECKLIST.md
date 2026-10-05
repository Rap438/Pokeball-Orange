# PokéBall Orange v0.4 — implementation checklist

Statuses: **verified existing** (already correct, checked), **implemented** (code/data done, builds), **tested** (exercised in the mGBA core through the headless harness, with screenshots or memory checks), **in progress**, **needs work**, **blocked**.
Nothing here is **hardware-verified**: no real GBA or flash cart was used.

Reference build v0.3.0 (`PokeBall_Orange(1).gba`, 16,777,216 bytes, SHA-256 `ff88f646…6c6a`) was rebuilt from source byte-for-byte before any change. The baseline is kept as git tag `v0.3.0-baseline`, a bundle, and the ROM.

## 1. Baseline
| Item | Status | Notes |
|---|---|---|
| Source matches the reference build | tested | Rebuilt ROM is identical to the reference SHA-256 |
| Recoverable baseline | implemented | Tag `v0.3.0-baseline`, git bundle, ROM copy; v0.3 stays in repo history |
| Missing source | n/a | Nothing missing. All v0.3 source and generator tools are in the repo |

## 2. Battles and ecosystem
| Item | Status | Notes |
|---|---|---|
| Ruleset chosen and documented | implemented | Gen 9 (Scarlet/Violet + DLC) through pokeemerald-expansion 1.17.1 `GEN_LATEST`. Exceptions are listed in RULESET.md |
| Fairy type: chart, typings, moves, labels, dex, AI, abilities/items | implemented, partly tested | Native to the engine. Azumarill shows Water/Fairy in the summary (`screens/14`). The type chart was checked in data |
| Physical/special split, move data, abilities, items, weather/terrain, AI | implemented | Native Gen 9 engine data and battle AI |
| Xerneas / Zacian / Koraidon / Miraidon | verified | Fairy, Fairy Aura / Fairy, Intrepid Sword (Crowned Fairy/Steel) / Fighting-Dragon, Orichalcum Pulse / Electric-Dragon, Hadron Engine |
| Whole-roster audit (stats, typings, abilities, learnsets, evos, forms, dex) | implemented | The v0.3 hand-added species are replaced by the engine's complete Gen 1–9 data; nothing is renamed or faked |
| Species IDs stable | **not possible** | The engine change renumbers species and breaks v0.3 saves. See SAVES.md |
| Megas / Z / Dynamax / Tera | implemented (off) | Dynamax and Tera flags are 0. No Mega Ring, Z-Ring or stones are distributed |
| Encounters: Route 101 and Littleroot (habitats, Fairy access, water/fishing) | implemented, tested | Azurill 9%, Ralts 1%, river tables. A wild battle was run |
| Gym 1 (Yamcha) strategy rebalance | implemented | Not battled in a test |
| Rest of the region's encounters, trainers, gyms, rivals | **needs work** | Still on expansion's vanilla Emerald data. See CONTINUATION.md |
| Goku combat considered in balance | implemented (notes) | See RULESET.md "Balance" |

## 3. Cities and routes
| Item | Status | Notes |
|---|---|---|
| Demo town: Littleroot Town | tested | Gate road and sign, plaza with lamps, door spurs, lab path, riverside camp, river and pool, flower-bed find |
| Adjoining route: Route 101 | tested | River on the east, worn path along the real route, light patches cleaned up |
| Collision, warps, NPCs, triggers, story gates | tested | Water is unwalkable and the surf prompt appears. The tent ridge is walk-behind and the tent body is solid. The gate trigger fired as before. All warps and coord events are unchanged |
| Traversal can't skip progression / trap | implemented, partly tested | Ki blasts pass the follower. The river only blocks dead-end edges. Ledges and trees on Route 101 are untouched (only plain grass is repainted) |
| Seams with neighbours | tested | Oldale moved to the superset tileset, verified in game. `tools/pbo/check_connections.py` added |
| Rest of the region | **needs work** | |

## 4. Art direction
| Item | Status | Notes |
|---|---|---|
| Warm localized light (fire, lamps) | tested | Light-blended palette (`.pla`) plus light sprites at night (`screens/04-07`) |
| Grounding shadows | tested | Engine shadows under every object plus baked prop shadows |
| Layered vegetation / depth / height | implemented, tested | Walk-behind tent ridge (`screens/05`), existing tree canopies, river bank faces |
| Readable paths | tested | |
| Consistent palettes / pixel scale | implemented | New art uses General's grass ramp, 16x16 metatiles |
| GBA-feasible only | yes | Palettes, tiles, metatile layers, OBJ blending, tile animation |
| Region-wide treatment | **needs work** | Demo area only |

## 5. Following Pokémon
| Item | Status | Notes |
|---|---|---|
| First eligible party mon follows (not an Egg, not fainted) | tested | Engine `GetFirstLiveMon` |
| On/off option | tested | OPTION > FOLLOWER. The setting survived save and continue |
| Directional/idle animation, catch-up | tested | Engine follower system |
| Interaction | tested | Emote and message (`screens/08`) |
| Map transitions, warps | tested | Debug warps, Instant Transmission and the Route 101 connection. House doors use the engine's standard handling and were not tested separately |
| Battles | tested | Hidden during battle as usual |
| DBZ personal fights | tested | Steps into its ball during the fight, comes back after (`screens/09`) |
| Surf / Fly / Nimbus / IT | implemented, IT tested | Engine hides the follower on water. Fly and Nimbus warp and respawn it |
| Eggs / fainted / empty party / missing sprites | implemented | Engine rules, plus a Substitute doll for missing sprites |
| Sprite coverage audit (forms and shiny separately) | implemented | follower_sprite_audit.md: 455/455 base species with shiny palettes, 78/93 forms |

## 6. Systems
| Item | Status | Notes |
|---|---|---|
| Shenron: summon, cancel, wish, PC overflow, consumption, scatter | tested | Cancelling keeps the balls. 5 wishes run: party fills, then the PC. **Fixed:** the "sent to PC" message never showed, and the preview pic used Shenron's palette |
| Dragon Ball persistence after save | tested | DB spot vars survived save and continue |
| Collection / Radar / re-scatter after 1500 steps | verified existing (code), not re-walked | |
| Goku EXP share and leveling | implemented, tested | Re-hooked to expansion's getexp. A fight's EXP triggered party evolution (`screens/10`) |
| SSJ transform, hints, ki controls | tested | Moved to dedicated input bits so they no longer clash with the engine's R/debug keys |
| Obstacles (trees, rocks, boulders) | verified existing (code) | Not re-tested in v0.4 |
| Nimbus vs Fly rules | verified existing (code) | Same as v0.3: needs the Feather Badge. Not re-tested |
| Trainer power-up on the last Pokémon | implemented | Ported as a native battle command. Not seen in a test battle |

## 7. Story and presentation
| Item | Status | Notes |
|---|---|---|
| Adult Goku vs child dialogue | implemented | Chi-Chi's house and town lines; "kid" lines aimed at Goku |
| Inconsistent introductions (Roshi, Chi-Chi on Roshi) | implemented | |
| Vegeta recognising Kakarot | implemented | Both rival-house branches, 1F and 2F |
| "Red Ribbon Army and Red Ribbon Army" | implemented | |
| Capsule Corp naming ("CAPSULE CORP. CORPORATI…") | implemented | |
| Naming screen showed Vegeta | implemented, tested | Now shows Goku |
| Debug features gated for release | tested | `make release` turns off DBZ debug (R+SELECT) and the engine's debug menus. Checked on the release ROM |
| Name casing (data vs dialogue) | decided | Menus use the engine's mixed case (Pikachu, Senzu Bean); dialogue keeps Gen 3 caps |
| Opening to first badge polish | **in progress** | Intro and Littleroot done. First gym team done but not played through |

## 8. Saves
| Item | Status | Notes |
|---|---|---|
| v0.4 save/continue (party, Goku vars, DB vars, story vars, follower setting) | tested | Debug and release builds |
| v0.3 saves | **break (explained)** | Detected and announced, never silently erased. See SAVES.md |
