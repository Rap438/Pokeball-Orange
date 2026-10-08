# 3D Flying Nimbus (prototype)

## What it does
After Goku calls the Nimbus and takes off, the game shows a 3D flight over Hoenn. He flies from where he was to the town picked on the fly map, then the normal arrival plays. B skips the flight.

OPTION > NIMBUS VIEW switches it:
- **3D** is the default.
- **MAP** gives the old behaviour (straight to the arrival).

Instant Transmission is not affected.

## How it works
- **Terrain:** `tools/pbo/gen_voxel_hoenn.py` builds a 512×512 heightmap and colour map from the region map art (`graphics/dbz/voxel/terrain.bin`, 512 KB) and the palette (`src/data/dbz_voxel_data.h`).
  - The coastline and route paths come from the map.
  - Land height comes from distance to the coast, the map's relief and noise.
  - Mt. Chimney, Meteor Falls, Mt. Pyre and the Sootopolis crater are raised by hand.
  - Towns are levelled, with coloured roofs.
- **Renderer:** `src/dbz_voxel.c` uses the classic "voxel space" heightmap ray caster.
  - Mode 4 bitmap, 120 two-pixel columns, 72 distance steps.
  - Drawn front to back with a y-buffer, double buffered.
  - The inner loop is ARM code in IWRAM.
  - Four fog levels and a sky gradient come from a 256-colour palette.
- **Rider:** Goku's back frame on the Nimbus cloud is drawn as two hand-written OAM entries. In bitmap modes, sprite tiles must sit at tile 512 and up.
- **Hooks:**
  - `CB_ExitFlyMap` (`region_map.c`) records the start and destination map sections.
  - `Task_UseFly` (`field_effect.c`) runs the flight after the take-off animation, then warps as usual.

## Measured
- **Frame rate:** 20 frames per second in mGBA at normal GBA speed (`gDBZVoxelFrameRate` reads 200 = 20.0 fps). Camera motion is tied to elapsed vblanks, so the flight takes the same time at any frame rate.
- **ROM cost:** about 0.5 MB.

## Known limits
- The world is low resolution, two texels per region-map pixel, so up close it looks blocky. Distant terrain shows some vertical streaking.
- Terrain is derived from the region map, not the real map layouts. Towns are stand-ins, not their actual buildings.
- Only one look: no seasons or night yet.
- Tested in the emulator only.
