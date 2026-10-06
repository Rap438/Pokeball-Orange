# PokéBall Orange v0.5 changelog

## Fixes from playtesting
- **Goku vs beaten trainers is back.** The engine's switch to Event Snippets had dropped the hook. Beat a trainer's POKéMON, talk to them again, and Goku can offer a hand-to-hand round.
- **Dragon Balls show on the overworld again.** The spawn hooks for the injected Dragon Ball objects were lost in the engine port.

## Dragon Balls
- **Dragon Ball tracker.** Once the DRAGON RADAR is in the bag, a strip of seven balls sits above the HUD:
  - balls Goku holds glow orange with a red star;
  - missing balls are dark sockets;
  - while the balls are stone after a wish, all seven show as grey stone.

## Goku's medicine
- **Healing scales with Goku's max HP** (30 + 8 per level), because flat POKéMON amounts would mean nothing later.

  | Item | Goku heals |
  |---|---|
  | SENZU SPROUT | 25% |
  | SENZU LEAF | 50% |
  | KORIN WATER | 75% |
  | SACRED WATER | 100% |
  | SENZU BEAN | full, and revives him from a KO |
  | DENDE'S HEAL | revives at 50% |
  | KAMI'S HEAL | revives at 100% |
  | Drinks, berries, herbal powder | 10-60% |

- **Bag descriptions show both scales.**

## Starters from four regions
- **Pick the region, then the starter.** On Route 101, Roshi's bag holds POKé BALLS from HOENN, KANTO, SINNOH and ALOLA:
  - Hoenn: Treecko, Torchic, Mudkip
  - Kanto: Bulbasaur, Charmander, Squirtle
  - Sinnoh: Turtwig, Chimchar, Piplup
  - Alola: Rowlet, Litten, Popplio
- **Vegeta grabs from the same bag.** His starter line becomes the same region's counterpart, at the stage that line would have at his level.

## Art and seasons
- **OG's art pack is in.** It is native indexed art: world tilesets, battle backgrounds, location battle views, bag, party, Pokédex, Pokénav map, trainer card with Goku's hands, all 20 menu frames, and the HUD.
  - **Summer** is the base art.
  - **Spring, autumn and winter** keep only the files that differ.
- **Seasons at runtime.** The season follows the game clock's month (spring Mar-May, summer Jun-Aug, autumn Sep-Nov, winter Dec-Feb). OPTION > SEASON can lock one.
  - **When it changes:** only while a map or screen loads, so it never changes mid-screen.
  - **What switches together:** world tiles and palettes, battle environments and views, menus, frames and HUD.
  - **What never changes:** tile IDs, collision and layouts.
  - **Lighting:** day/night tint and light blending apply on top of the seasonal palette.
  - **Interiors** keep stable materials, as the pack intends.
- **Camp seam fixed.** The camp's grass is matched to the town's seasonal grass, so there is no seam.

## Wild Pokémon
- **Visible wild POKéMON** (pokeemerald-expansion overworld encounters). They wander tall grass, caves and water, and walking into one starts the battle.
  - **Temperament follows type:** fighters and dark types come at Goku; normal and fairy types wander up curious; fliers and electric types bolt; Abra and Kadabra teleport off; psychic, ghost and steel types watch; the rest ignore him.
  - **OPTION > WILD MONS: HIDDEN** brings back classic step encounters.
- **Guest POKéMON from every region.** About one wild encounter in eight is a first-stage POKéMON from any generation, picked by where it is:
  - caves: rock, ground, ghost, poison, dark, steel, fighting, dragon;
  - water and fishing: water;
  - Rock Smash: rock, ground, steel;
  - grass, by season:
    - spring: grass, bug, fairy, normal, flying;
    - summer: fire, electric, bug, grass, normal, flying;
    - autumn: ghost, dark, ground, fighting, psychic, poison;
    - winter: ice, steel, psychic, dragon, normal, flying.

  Legendaries never appear this way; Shenron grants them. Over a year every non-legendary line can be caught.
- **Town POKéMON.** Ambient POKéMON live in the towns, placed by habitat (for example Wingull on beaches, Marill and Azurill by ponds, Numel and Torkoal in Lavaridge's ash, Swablu and Tropius in Fortree). Talk to one and it turns and cries.

## Towns
- **13 towns and cities rearranged:** Oldale, Petalburg, Rustboro, Dewford, Slateport, Mauville, Verdanturf, Fallarbor, Lavaridge, Fortree, Lilycove, Mossdeep, Pacifidlog. Littleroot was already redone in v0.4. Sootopolis (crater) and Ever Grande (League) are unchanged.
  - **What changed:** buildings moved together with their doors, signs and NPCs; road networks redrawn; decorations re-placed.
  - **What was kept:** buildings and routes used by cutscenes keep their geometry.
  - **Tooling:** tools/pbo/citykit.py and one script per city in tools/pbo/cities/.
- **Fly-in spots** follow moved POKéMON CENTERS.
- **Dragon Ball spots** that ended up under a building were moved to the nearest open tile.

## Story text
- **Consistency pass over all maps, shared scripts, trainers and Match Call.**
  - **Family and friends:** Ox-King greets his son-in-law; Gohan slips "Dad?!" behind the Great Saiyaman mask; Goten and Trunks; Android 18 knows her husband's best friend; Yamcha, Tien and Dr. Brief know Goku.
  - **Adult hero:** nobody calls adult Goku "kid" anymore.
  - **Names:** leftover original names (Wally, Scott, Steven) are gone, and Krillin's Match Calls no longer act like Goku's dad.
  - **Gameplay text:** tips match visible wild POKéMON and the starter regions. Field-move hints match the Super Saiyan powers.
- **Two fixes in shared scripts:**
  - The League lobby attendant is MR. POPO; Dende stays in the Elite Four.
  - The START menu's Goku entry is now **KI POWER**, so it no longer shares a name with the trainer card.
