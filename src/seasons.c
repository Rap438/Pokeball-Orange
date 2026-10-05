#include "global.h"
#include "seasons.h"
#include "event_data.h"
#include "rtc.h"
#include "constants/dbz.h"
#include "constants/rtc.h"

#include "data/seasons_gen.h"

static EWRAM_DATA u8 sSeason = 0;
static EWRAM_DATA bool8 sSeasonValid = FALSE;

static enum Season SeasonFromMonth(void)
{
    switch (GetMonth())
    {
    case MONTH_MAR: case MONTH_APR: case MONTH_MAY: return SEASON_SPRING;
    case MONTH_JUN: case MONTH_JUL: case MONTH_AUG: return SEASON_SUMMER;
    case MONTH_SEP: case MONTH_OCT: case MONTH_NOV: return SEASON_AUTUMN;
    case MONTH_DEC: case MONTH_JAN: case MONTH_FEB: return SEASON_WINTER;
    }
    return SEASON_SUMMER;
}

u8 Season_GetSetting(void)
{
    if (!FlagGet(FLAG_DBZ_SEASON_LOCK))
        return SEASON_SETTING_AUTO;
    return 1 + (FlagGet(FLAG_DBZ_SEASON_BIT0) ? 1 : 0) + (FlagGet(FLAG_DBZ_SEASON_BIT1) ? 2 : 0);
}

void Season_SetSetting(u8 setting)
{
    if (setting == SEASON_SETTING_AUTO || setting > SEASON_COUNT)
    {
        FlagClear(FLAG_DBZ_SEASON_LOCK);
        return;
    }
    setting--;
    FlagSet(FLAG_DBZ_SEASON_LOCK);
    if (setting & 1) FlagSet(FLAG_DBZ_SEASON_BIT0); else FlagClear(FLAG_DBZ_SEASON_BIT0);
    if (setting & 2) FlagSet(FLAG_DBZ_SEASON_BIT1); else FlagClear(FLAG_DBZ_SEASON_BIT1);
}

void Season_Update(void)
{
    u8 setting = Season_GetSetting();
    sSeason = setting == SEASON_SETTING_AUTO ? SeasonFromMonth() : setting - 1;
    sSeasonValid = TRUE;
}

enum Season Season_Get(void)
{
    if (!sSeasonValid)
    {
        // before a save is loaded (title, intro): the clock alone decides
        sSeason = SeasonFromMonth();
        sSeasonValid = TRUE;
    }
    return sSeason;
}

const void *Season_Resolve(const void *ptr)
{
    u32 i, season, p = (u32)ptr;

    if (p < 0x08000000 || p >= 0x0A000000)
        return ptr;
    season = Season_Get();
    if (season == SEASON_SUMMER)   // summer is the base art
        return ptr;
    for (i = 0; i < gSeasonResourceCount; i++)
    {
        u32 base = (u32)gSeasonResources[i].base;
        // size 0: compressed data, only its exact start may be swapped
        if (p == base || (p > base && p < base + gSeasonResources[i].size))
            return (const u8 *)gSeasonResources[i].variants[season] + (p - base);
    }
    return ptr;
}
