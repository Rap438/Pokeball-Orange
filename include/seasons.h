#ifndef GUARD_SEASONS_H
#define GUARD_SEASONS_H

// PokeBall Orange: four seasons. The season is taken from the game clock's month (spring Mar-May,
// summer Jun-Aug, autumn Sep-Nov, winter Dec-Feb) unless OPTION > SEASON locks one. It is re-read
// whenever a map loads, so a change shows up at the next door or route edge, never mid-screen.
// All seasonal art (world tiles and palettes, battle backgrounds, menus, frames, HUD) is selected
// together through Season_Resolve(); tile IDs, collision and layouts never change.

enum Season
{
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_WINTER,
    SEASON_COUNT,
};

#define SEASON_SETTING_AUTO 0   // OPTION value; 1..4 = locked to SEASON_SPRING..SEASON_WINTER

struct SeasonResource
{
    const void *base;
    u32 size;   // 0 for compressed data (exact pointer match only)
    const void *variants[SEASON_COUNT];
};

enum Season Season_Get(void);
void Season_Update(void);
u8 Season_GetSetting(void);
void Season_SetSetting(u8 setting);
const void *Season_Resolve(const void *ptr);

#endif // GUARD_SEASONS_H
