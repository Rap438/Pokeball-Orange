# PokéBall Orange

A Dragon Ball Z–themed hack of Pokémon Emerald, built on [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion) 1.17.1 (from v0.4; earlier versions used [pret/pokeemerald](https://github.com/pret/pokeemerald)), with graphics adapted from *Dragon Ball Z: Buu's Fury*.

**No ROMs are included.** Build it yourself from source (below), or apply a release patch to your own legally obtained Emerald ROM.

## Coming in v0.6 (unreleased)
See [docs/pbo/CHANGELOG_v0.6.md](docs/pbo/CHANGELOG_v0.6.md). In short:
- A fresh clone builds again (missing tool source and battle-view art restored)
- **Gyms 2-8, the Elite Four, Mr. Satan and Vegeta rebuilt** for Gen 9: abilities, items, coverage and a plan per team
- Mawile in Granite Cave, plus other encounter tweaks
- Goku's battle EXP share follows Gen 9 level scaling
- Pokémon Center/Mart signs and Rustboro's lamps glow at night
- Bag pocket-switch and Nimbus text display fixes

## New in v0.5
See [docs/pbo/CHANGELOG_v0.5.md](docs/pbo/CHANGELOG_v0.5.md). In short:
- OG's art pack with **four seasons** that follow the game clock (or lock one in OPTION)
- **Starters from Hoenn, Kanto, Sinnoh or Alola**, with Vegeta's line following the region
- **Visible wild Pokémon**, seasonal **guest Pokémon from every region**, and Pokémon living in the towns
- **13 towns rearranged** into new layouts
- Dragon Balls visible on the ground with a **tracker above the HUD** once you have the Radar
- Goku's medicine heals a **share of his HP**, and the Senzu Bean revives him
- Fistfights with beaten trainers are back; story text consistency pass

## What's in it

- **Goku as the hero.** Chi-Chi replaces Mom, Master Roshi is the professor, Vegeta is the rival, Krillin is the Normal-type gym leader, and the rest of the Z cast fills the other roles.
- **Super Saiyan whenever you want.** R powers up to SSJ, SSJ2 and SSJ3, which unlock with badges 1, 3 and 4.
- **Ki blast and Kamehameha.** L fires the selected special: tap it for a KI BLAST, or hold it to charge a KAMEHAMEHA and let go. Hold L and tap R to swap specials.
- **Goku's moves replace HMs.**
  - SSJ plus Kamehameha cuts trees.
  - SSJ2 smashes rocks.
  - SSJ3 pushes Strength boulders.
- **Dragon Balls.**
  - Seven balls are scattered across Hoenn when you start a new game.
  - Roshi gives you the Dragon Radar along with the Pokédex.
  - Collect all seven and Shenron grants any Pokémon you name: the 386 originals plus every legendary and pseudo-legendary from Gen 4 to Gen 9.
  - After a wish, the balls turn to stone and re-scatter.
- **Flying Nimbus.** A key item that works like Fly. You get it on Route 119 together with HM Fly, and it needs the Feather Badge.
- **DBZ item names.** Senzu Bean, Korin Water, Kaio-Ken, Scouter, Capsule Bike and others. Every item keeps its original effect.
- **Restyled overworld.** OG's seasonal art pack (v0.5); towns rearranged into new layouts.
- **Custom title screen.**
- **Overworld DBZ fights** happen in real time on the map. A throws a punch/kick combo (Buu's Fury fighter frames with a hit spark at the fist), L fires the selected special, R powers up and B dashes.
  - Red Ribbon Army soldiers ambush Goku on routes and in cities. A KI HIDER keeps them away.
  - Trainers you've already beaten can be challenged hand to hand.
  - Every gym leader fights Goku after handing over the badge.
  - **Vegeta** challenges Goku to a fistfight after every rival battle. He's faster, fires ki blasts, has more HP, pays more EXP, and goes Super Saiyan from the fourth badge on.
  - Damage comes from the power-level ratio between fighters, with a cap per hit, so no single blow decides a fight. Opponents power up when Goku does.
- **Goku's health carries over** between fights. The overworld HUD shows it all the time. Pokémon Centers and resting at home heal him, and healing items from the bag work on him too (DENDE'S HEAL and KAMI'S HEAL revive him). If he's knocked out he wakes up at the last place he rested with half his HP.
- **Overworld HUD:** Goku's head (hair shows his Saiyan form), the selected special with a charge meter, HP bar and power level. It hides during scripts and menus.
- **Goku levels up** the way he does in Buu's Fury.
  - Overworld fights give Goku most of the EXP and give the team a share.
  - Pokémon battles give the team the full EXP and give Goku a share.
  - Super Saiyan forms multiply Goku's power level.
- **Trainer Card:** the back shows Goku's power level, level, EXP and EXP to the next level, HP, power, defense, current and max form, Dragon Balls held, wishes made, and fights won/lost.
- **Options:** the OPTION menu scrolls and adds Day/Night, Shadows, Goku HUD, Fight Level (easy/normal/hard), Ambushes (off/rare/normal/often), Battle BG (map view/classic), SSJ2 Sparks, Power-Ups, Form Hints and Shiny Odds (1/8192, 1/4096, 1/1024, 1/256, Always — applies to wild, gift, egg and Shenron Pokémon).
- **Battle UI** restyled: navy panels with orange and gold frames for the text box, FIGHT/BAG/POKéMON/RUN menu, move list and healthboxes.
- Turn-based Pokémon battles are unchanged.

### New in v0.4

- **Modern battles.** Gen 9 rules on the expansion engine: Fairy type, physical/special split, real abilities, Gen 9 moves, items, AI and species data. Details and exceptions are in [docs/pbo/RULESET.md](docs/pbo/RULESET.md).
- **Following Pokémon.** Your first healthy party Pokémon walks behind Goku, steps into its ball for his fistfights, and can be talked to. OPTION > FOLLOWER turns it off.
- **New Littleroot Town and Route 101.** A riverside camp, plaza lamps, worn paths and a river.
  - At night the campfire and lamps light up the grass around them.
  - You can walk behind the tent, nap in it, and raid the supply crate.
- **Day and night.** Uses the engine's system, with window and lamp lights. OPTION > DAY/NIGHT still turns it off.
- **Early Fairy types.** Azurill and a rare Ralts on Route 101; Marill in the new river.
- **Story fixes.** Vegeta always knows Kakarot, Roshi knows his old student, Chi-Chi talks to her husband, and assorted name typos are fixed.
- **Saves.** v0.4 can't load v0.3 saves. The game tells you so and leaves the old save alone until you save over it. See [docs/pbo/SAVES.md](docs/pbo/SAVES.md).

Full notes:
- [changelog](docs/pbo/CHANGELOG_v0.4.md)
- [checklist](docs/pbo/CHECKLIST.md)
- [tests](docs/pbo/TEST_RESULTS.md)
- [what's next](docs/pbo/CONTINUATION.md)
- [screenshots](docs/pbo/screens)

### New in v0.3.0

- **Talking portraits.** Buu's Fury portraits appear above the text box when the cast speaks: Goku, Vegeta, Vegito, Chi-Chi, Bulma, Roshi, Krillin, the gym leaders, the Elite Four, Mr. Satan, Kid Buu, Dr. Gero and more.
- **Deeper fights.**
  - Hold B while standing still to guard. A perfectly timed guard takes no damage, stuns the attacker and makes your next hit a counter.
  - A A A is a three-hit combo; the finisher knocks the opponent back two tiles.
  - Big hits shake the screen and flash white, charging throws aura sparks, and every fight plays its own Emerald battle theme.
  - **Beam struggles:** fire a Kamehameha at an opponent who uses beams and the two can lock. Mash A to push your beam through.
- **Boss fights with phases.** General Blue (paralysis stare) after his battle on Mt. Chimney, Kid Buu (teleports and regenerates) in the Red Ribbon base, Dr. Gero (drinks ki attacks) in the Seafloor Cavern. Each gets stronger at two-thirds and one-third HP.
- **Fusion finale.** In Sootopolis, Kid Buu can't be beaten alone. Vegeta arrives with the Potara and the two fuse into **Vegito** for the last fight.
- **Goku's techniques** (START → GOKU):
  - **Kaio-ken** (badge 5): press SELECT in a fight for 1.5x power that slowly drains HP.
  - **Instant Transmission** (badge 6): lock on to the ki of anyone in a town you've visited. No Pokémon needed.
  - **Spirit Bomb** (badge 8): hold L and tap R until it's selected, hold L to gather energy, let go to throw it.
  - **Hide Ki:** a reusable KI HIDER.
- **Capsule Corp. Gravity Room** (Rustboro, after you return the goods): 600 steps of 10x gravity, double fight EXP.
- **World Martial Arts Tournament** (Slateport Battle Tent, after badge 4), hosted by Mr. Satan: Jackie Chun (Pokémon), Spopovich (hand to hand), Mighty Mask (Pokémon), then Vegeta with Pokémon and then fists.
- **Hyperbolic Time Chamber** (Pokémon League lobby, after the credits): Dende sends you into the white void for five waves of sparring with triple EXP.
- **Bigger Shenron wishes:** a Pokémon, Goku's power (+5 levels), your team's power (+3 levels each), or ¥50,000.
- **Living battle backgrounds:** water sparkles and grass sways in the map-view battle backgrounds.
- **Auto-run** (on by default; B walks; toggle in OPTION). Goku no longer needs the Running Shoes.
- **Fixes:** a full Kamehameha now charges at full speed (the HUD redraw was halving the frame rate), a stray sprite clean-up that could scroll the camera away after a fight, and a missed L press while Goku was mid-step.

- **Overworld style:**
  - Soft drop shadows under every character.
  - Deeper cel-style shading.
  - Morning, evening and night lighting that follows the game clock.
  - SSJ2 lightning crackles around Goku.
- **Battle backgrounds** show the actual spot on the map where the battle started, with time-of-day lighting.
- **Trainers power up** with an aura and a screen shake when they send out their last Pokémon. It's visual only.
- **Pedestrians** are a mix of Buu's Fury townsfolk and Emerald's own NPCs.
- **Rewritten script.** The whole game's dialogue was rewritten in-world, not just name-swapped:
  - Goku and Chi-Chi move to Littleroot. Vegeta and Bulma live next door, and Bulma tells Goku Vegeta has gone off on his own Pokémon journey to prove he's stronger.
  - Master Roshi is the professor, Krillin is Goku's best friend and Petalburg's leader, Uub is the shy kid with hidden power, Supreme Kai watches over Goku, Dr. Brief runs Capsule Corp.
  - Gym leaders: Yamcha, Tien, Ox-King, Videl, Krillin, Great Saiyaman, Goten & Trunks, Piccolo. Elite Four: Super Buu, Baba, Android 18, Dende. Champion: Mr. Satan.
  - One villain team: the **Red Ribbon Army**, run by **Kid Buu**, who wants to wake Groudon and Kyogre just to watch Hoenn burn. Dr. Gero runs the sea squad and thinks he can control Kyogre. Officers: General Blue, Mai, Colonel Silver, Colonel Violet.
  - The story bible used for the rewrite is in `tools_pbo/story_bible.md`.

## Playing

Apply `PokeBall_Orange_v0.4.bps` (from Releases; the patched ROM is 32 MB) to a clean **Pokémon Emerald (USA)** ROM with [Flips](https://github.com/Alcaro/Flips) or any BPS patcher. The ROM's SHA-1 should be `f3ae088181bf583e55daf962a92bb46f4f1d07b7`.

The debug build turns on R+SELECT for a three-page test menu (soldier, Vegeta and boss fights, the finale, badges and team, time of day, warps, Dragon Balls and Shenron, the tournament, the Time Chamber, the Gravity Room) and lets START end a fight in Goku's favor (B+START: Goku loses). Since v0.4 a plain `make` gives the debug build and `make release` gives the release build. The debug build also has the engine's own debug menu on R+START and a WARP CAMP shortcut to the new Littleroot.

Fight music reuses Emerald's own battle themes; there's no original soundtrack.

## Building

Follow [INSTALL.md](INSTALL.md) to set up the arm-none-eabi toolchain (the expansion engine builds with a modern GCC, not agbcc), then run:

```
make release -j$(nproc)   # release build -> pokeemerald-release.gba
make -j$(nproc)           # debug build   -> pokeemerald.gba
```

The demo tileset and the Littleroot/Route 101 layouts are generated. If you edit `tools/pbo/build_cove.py`, run `python3 tools/pbo/build_cove.py` from the repo root before building.

## Tools

`tools_pbo/` holds the scripts used to generate the assets: the Buu's Fury decompressor and rippers, sprite and portrait builders, species import, Dragon Ball spot finder, title converter, overworld restyle, and the headless mGBA test runner. Most of them expect the original working paths and are kept for reproducibility.

## Credits

- pret for the pokeemerald decompilation.
- rh-hideout and the pokeemerald-expansion contributors for the engine PokéBall Orange runs on since v0.4 (battle engine, species data, followers, day/night, overworld sprites).
- Webfoot Technologies / Atari for *Dragon Ball Z: Buu's Fury*.
- Bird Studio, Shueisha and Toei Animation for Dragon Ball.
- Nintendo, Game Freak and Creatures for Pokémon.

This is a non-commercial fan project.
