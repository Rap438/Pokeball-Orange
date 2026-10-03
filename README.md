# PokéBall Orange

A Dragon Ball Z–themed hack of Pokémon Emerald, built on the [pret/pokeemerald](https://github.com/pret/pokeemerald) decompilation, with graphics adapted from *Dragon Ball Z: Buu's Fury*.

**No ROMs are included.** Build it yourself from source (below), or apply a release patch to your own legally obtained Emerald ROM.

## What's in it

- **Goku as the hero.** Chi-Chi replaces Mom, Master Roshi is the professor, Vegeta is the rival, Krillin is the Normal-type gym leader, and the rest of the Z cast fills the other roles.
- **Super Saiyan whenever you want.** R powers up to SSJ, SSJ2 and SSJ3, which unlock with badges 1, 3 and 4.
- **Ki blast and Kamehameha.** Tap L to fire a ki blast. Hold L and release to fire a Kamehameha.
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
- **Overworld DBZ fights** happen in real time on the map. Controls: A punches, tapping L fires a ki blast, holding L fires a Kamehameha, R powers up and B dashes.
  - Saiyan and Majin soldiers ambush Goku on routes and in cities. A KI HIDER keeps them away.
  - Trainers you've already beaten can be challenged hand to hand.
  - Every gym leader fights Goku after handing over the badge.
- **Goku levels up** the way he does in Buu's Fury.
  - Overworld fights give Goku most of the EXP and give the team a share.
  - Pokémon battles give the team the full EXP and give Goku a share.
  - Super Saiyan forms multiply Goku's power.
- Turn-based Pokémon battles are unchanged.

Work in progress: an overworld pass for shading, shadows and day/night, mixed DBZ pedestrians, the story text pass, location-based battle backgrounds, and the trainer power-up on their last Pokémon.

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
