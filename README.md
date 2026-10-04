# PokéBall Orange

A Dragon Ball Z–themed hack of Pokémon Emerald, built on the [pret/pokeemerald](https://github.com/pret/pokeemerald) decompilation, with graphics adapted from *Dragon Ball Z: Buu's Fury*.

**No ROMs are included.** Build it yourself from source (below), or apply a release patch to your own legally obtained Emerald ROM.

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
- **Restyled overworld.** Buu's Fury–inspired colors with textured grass and dirt. The map layouts are unchanged.
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

Apply `PokeBall_Orange.bps` (from Releases) to a clean **Pokémon Emerald (USA)** ROM with [Flips](https://github.com/Alcaro/Flips) or any BPS patcher. The ROM's SHA-1 should be `f3ae088181bf583e55daf962a92bb46f4f1d07b7`.

The debug build turns on R+SELECT for test shortcuts (badges, warps, fights, Dragon Balls, time of day). To get it, uncomment `#define DBZ_DEBUG` in `include/constants/dbz.h`.

## Building

Follow pokeemerald's [INSTALL.md](INSTALL.md) to set up agbcc and the toolchain, then run:

```
make -j$(nproc)
```

The output file is `pokeemerald.gba`.

## Tools

`tools_pbo/` holds the scripts used to generate the assets: the Buu's Fury decompressor and rippers, sprite and portrait builders, species import, Dragon Ball spot finder, title converter, overworld restyle, and the headless mGBA test runner. Most of them expect the original working paths and are kept for reproducibility.

## Credits

- pret for the pokeemerald decompilation.
- Webfoot Technologies / Atari for *Dragon Ball Z: Buu's Fury*.
- Bird Studio, Shueisha and Toei Animation for Dragon Ball.
- Nintendo, Game Freak and Creatures for Pokémon.
- The rh-hideout pokeemerald-expansion project, used as a reference for the newer species' data.

This is a non-commercial fan project.
