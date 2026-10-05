# PokéBall Orange v0.4 changelog (mapped to the update request)

**Stage delivered:** stages 1-3 (baseline → modern foundations → one playable demo town and route), plus the parts of stage 5 needed for a clean release: story fixes, debug gating, save handling, packaging. Stage 4 (expanding the treatment across the region and rebalancing the whole campaign) is next. See CONTINUATION.md.

## §1 Baseline
- Rebuilt v0.3.0 from source and matched the reference SHA-256. The baseline is kept as tag `v0.3.0-baseline`, a git bundle and the ROM.

## §2 Modern battles and ecosystem
- **Engine.** The game now runs on pokeemerald-expansion 1.17.1, merged under the whole PokéBall Orange layer:
  - Goku, his fights and transformations, Dragon Balls, Shenron, Nimbus and Instant Transmission
  - portraits, HUD and options
  - the custom battle UI and backgrounds
- **Ruleset.** Gen 9 throughout: the Fairy type, physical/special split, Gen 9 moves, abilities, items, AI and species data. See RULESET.md.
- **Roster.** The v0.3 hand-made legendaries are replaced with correct engine data. Xerneas, Zacian, Koraidon and Miraidon are checked.
- **Gimmicks.** Tera and Dynamax are off; Megas and Z-Moves are unreachable.
- **Route 101 and Littleroot encounters.**
  - Azurill 9% and Ralts 1% on Route 101.
  - New river and pool water and fishing tables.
- **Fairy Feather** hidden in Littleroot's flowers.
- **Yamcha (Gym 1)** rebuilt around Sturdy, Rollout and Thunder Wave/Block.
- **Exp. Share** works as the Gen 6-8 key-item toggle.
- **Shiny odds** use the 1/4096 base, and the OPTION labels show the real totals.
- **DBZ item names** moved to expansion item data.
- **Trainers.**
  - Trainer names and classes carried over.
  - The three World Tournament trainers rebuilt in expansion's format.
  - Cast genders and encounter music fixed.
- **Trainer power-up** on the last Pokémon ported as a native battle command.

## §3 Cities and routes (demo: Littleroot Town + Route 101)
- **Littleroot.** It now has:
  - a north gate road with the town sign
  - a central plaza with two street lamps
  - paths to every door and down to Roshi's lab
  - a riverside camp: tent (walk behind its ridge; talk at the flap to nap and heal), campfire, log seat and a Capsule Corp. supply crate with 2 Senzu Sprouts once
  - a river coming in from Route 101 and opening into a rocky pool
  - a hidden find in the flower bed
- **Route 101.** It has:
  - a river along the east edge
  - a worn dirt path that follows the route people actually walk (south gate, pocket, clearing, tall grass, north gate)
  - ledges, trees, tall grass and story triggers untouched
- **Warps, coord events and NPC scripts** are unchanged. The town sign moved to the gate.
- **Oldale** uses the new tileset (a superset of the old one) so the path draws correctly across the seam. `check_connections.py` audits every map seam.

## §4 Art direction (in-engine demo)
- New secondary tileset **PboCove**, built by `tools/pbo/build_cove.py`: 391 tiles and 249 metatiles, Petalburg's set plus new art.
- **Warm localized light.** At night:
  - The campfire throws a dithered pool of warm light, because palette 11 is light-blended (`.pla`).
  - Light-sprite halos glow over the fire and both lamps.
  - The lamp glass glows.
  - By day the lit grass is identical to normal grass.
- **Shadows.** Engine shadows under every object and the follower, plus baked shadows under the tent, crate, log and lamps.
- **Depth.**
  - The tent ridge layer covers Goku when he walks behind it.
  - River banks show a dirt face where the ground drops to the water.
  - The existing tree canopies stay.
- **Animation.** Flames (4 frames) and the river surface (4 frames).
- **Day/night.** The engine's day/night system replaces v0.3's palette tint. OPTION > DAY/NIGHT still turns it off, and DBZ flashes and battle backgrounds respect it.
- **Evidence.** Captures are in `screens/`.

## §5 Following Pokémon
- **On by default.** The first party Pokémon that isn't an Egg or fainted follows Goku.
- **Toggle.** OPTION > FOLLOWER ON/OFF, saved with the game.
- **Behaviour** comes from the engine's follower system:
  - directional and idle animation, catch-up
  - talking to it
  - hiding on water, warps, battles
  - a Substitute doll if a sprite is missing
- **PBO integration.**
  - The follower steps into its ball for personal fights and comes back after.
  - Ki blasts fly past it.
  - The SHADOWS option drives the engine's shadows.
- **Sprite audit** (`follower_sprite_audit.md`): every base species in the roster has a follower sprite and a shiny palette; 78 of 93 alternate forms do.

## §6 Systems
- **Shenron.**
  - "Sent to the PC" message fixed: the result was being clobbered.
  - Preview Pokémon pictures had Shenron's palette.
  - Full wish cycle re-tested, including cancel and PC overflow.
- **Goku's EXP share** re-hooked to the engine's EXP calculation.
- **Ki controls** moved to their own input bits so they can't collide with engine keys.
- **Instant Transmission** tested with the follower.

## §7 Story and presentation
- Vegeta always knows Kakarot. The first-meeting branch that treated him as a stranger is gone.
- Roshi greets his old student (intro and lab). Chi-Chi talks to her husband: "our room", and Roshi is "your old master".
- Red Ribbon grunts no longer call adult Goku "kid".
- Fixed "RED RIBBON ARMY and RED RIBBON ARMY" and "CAPSULE CORP. CORPORATI…".
- The naming screen shows Goku instead of Vegeta.
- **Release gating.** `make release` disables DBZ debug (R+SELECT, fight skip) and every engine debug menu. The debug build keeps a WARP CAMP shortcut to the demo.

## §8 Saves
- v0.3 saves are recognised and explained instead of shown as "erased". Nothing is overwritten without confirmation. See SAVES.md for why they can't load.

## Tooling (in `tools/pbo/`)
- `mapkit.py`: render and edit layouts, metatile sheets
- `build_cove.py`: the demo tileset and map painter
- `check_connections.py`: map seam audit
- `audit_followers.py`: follower sprite audit
- `item_renames.py`, `port_trainer_names.py`: identity data ports
