# Save compatibility (v0.4)

## v0.3 saves don't carry over
v0.4 moves the game from the vanilla pokeemerald engine to pokeemerald-expansion 1.17.1, which is what brings in the Gen 9 battle system, the Fairy type and the full roster. That change breaks old saves for three concrete reasons:

1. **Save block layout.** The expansion engine uses different save structures:
   - SaveBlock1 is 0x3CD0 bytes instead of 0x3D88.
   - Pokémon data has new fields such as the separate shiny flag.
   - There is an extra SaveBlock3 stored in the sector tails.

   Every sector of an old save fails the new checksums.
2. **Species numbering.** v0.3 appended its hand-made species (Xerneas, Zacian and others) after Deoxys. In the expansion engine those numbers belong to Gen 4 species. A v0.3 Xerneas would come back as a different Pokémon.
3. **Item numbering.** The Dragon Balls, Radar and Nimbus moved to new item IDs (874-882).

A converter would have to rebuild every Pokémon and item from one format to the other. It's possible, but it isn't in this release. See CONTINUATION.md.

## What the game does with an old save
On boot, v0.4 checks whether the cartridge holds a valid v0.3 save (all sectors check out against the v0.3 sizes). If so, it shows:

> "This save is from PokeBall Orange v0.3 and can't be loaded by this version. It is still on the cartridge. Saving a new game here will replace it, so back up your .sav file first, or keep playing it with the v0.3 ROM."

**Nothing is erased at boot.** The old data stays on the cartridge until you save a new v0.4 game over it, and the game asks for confirmation before overwriting. Without this check, the engine would have said "The save file has been erased due to corruption", which is misleading.

## What to do
- **To keep a v0.3 run:** copy your `.sav` file somewhere safe, and keep playing it with the v0.3.0 ROM/patch, which is still in the repo history (tag `v0.3.0-baseline`).
- **To play v0.4:** start a new game. Emulators normally use a `.sav` named after the ROM file, so naming the v0.4 ROM differently keeps the two saves separate.

## Inside v0.4
These were tested by saving, rebooting and continuing:
- party
- Goku's level, EXP, damage and options variables
- Dragon Ball spots and stone timer
- story variables
- the FOLLOWER on/off setting

The debug and release builds use the same save format, so a save carries over between them.
