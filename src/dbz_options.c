// PokeBall Orange settings (stored in VAR_DBZ_OPTIONS, edited in the OPTION menu) and shiny odds.
#include "global.h"
#include "dbz.h"
#include "event_data.h"
#include "random.h"
#include "pokemon.h"

static u16 Opts(void)
{
    return VarGet(VAR_DBZ_OPTIONS);
}

bool8 DBZ_OptDayNight(void)   { return !(Opts() & DBZ_OPT_DAYNIGHT_OFF); }
bool8 DBZ_OptShadows(void)    { return !(Opts() & DBZ_OPT_SHADOWS_OFF); }
bool8 DBZ_OptMapBattleBg(void){ return !(Opts() & DBZ_OPT_CLASSIC_BG); }
bool8 DBZ_OptSparks(void)     { return !(Opts() & DBZ_OPT_SPARKS_OFF); }
bool8 DBZ_OptPowerUp(void)    { return !(Opts() & DBZ_OPT_POWERUP_OFF); }
bool8 DBZ_OptHints(void)      { return !(Opts() & DBZ_OPT_HINTS_OFF); }
bool8 DBZ_OptHud(void)        { return !(Opts() & DBZ_OPT_HUD_OFF); }
bool8 DBZ_OptAutoRun(void)    { return !(Opts() & DBZ_OPT_AUTORUN_OFF); }

// 0 off, 1 rare, 2 normal, 3 often
u8 DBZ_OptAmbush(void)
{
    static const u8 sMap[4] = {2, 1, 3, 0};
    return sMap[(Opts() >> DBZ_OPT_AMBUSH_SHIFT) & 3];
}

u8 DBZ_OptDifficulty(void)
{
    switch ((Opts() >> DBZ_OPT_DIFF_SHIFT) & 3)
    {
    case 1:  return DBZ_DIFF_EASY;
    case 2:  return DBZ_DIFF_HARD;
    default: return DBZ_DIFF_NORMAL;
    }
}

u8 DBZ_OptShiny(void)
{
    u8 v = (Opts() >> DBZ_OPT_SHINY_SHIFT) & 7;
    return v > 4 ? 0 : v;
}

// Raw get/set used by the OPTION menu. Each setting is a small number in the order it is listed.
u8 DBZ_GetOptionValue(u8 id)
{
    u16 o = Opts();
    switch (id)
    {
    case 0: return (o & DBZ_OPT_DAYNIGHT_OFF) ? 1 : 0;
    case 1: return (o & DBZ_OPT_SHADOWS_OFF) ? 1 : 0;
    case 2: return (o & DBZ_OPT_HUD_OFF) ? 1 : 0;
    case 3: return DBZ_OptDifficulty();                       // EASY NORMAL HARD
    case 4: { static const u8 s[4] = {2, 1, 3, 0}; return s[(o >> DBZ_OPT_AMBUSH_SHIFT) & 3]; } // OFF RARE NORMAL OFTEN
    case 5: return (o & DBZ_OPT_CLASSIC_BG) ? 1 : 0;
    case 6: return (o & DBZ_OPT_SPARKS_OFF) ? 1 : 0;
    case 7: return (o & DBZ_OPT_POWERUP_OFF) ? 1 : 0;
    case 8: return (o & DBZ_OPT_HINTS_OFF) ? 1 : 0;
    case 9: return DBZ_OptShiny();
    case 10: return (o & DBZ_OPT_AUTORUN_OFF) ? 1 : 0;
    }
    return 0;
}

static u16 SetBit(u16 o, u16 bit, u8 on)
{
    return on ? (o | bit) : (o & ~bit);
}

void DBZ_SetOptionValue(u8 id, u8 v)
{
    u16 o = Opts();
    switch (id)
    {
    case 0: o = SetBit(o, DBZ_OPT_DAYNIGHT_OFF, v); break;
    case 1: o = SetBit(o, DBZ_OPT_SHADOWS_OFF, v); break;
    case 2: o = SetBit(o, DBZ_OPT_HUD_OFF, v); break;
    case 3:
    {
        static const u8 s[3] = {1, 0, 2};
        o = (o & ~(3 << DBZ_OPT_DIFF_SHIFT)) | (s[v % 3] << DBZ_OPT_DIFF_SHIFT);
        break;
    }
    case 4:
    {
        static const u8 s[4] = {3, 1, 0, 2};
        o = (o & ~(3 << DBZ_OPT_AMBUSH_SHIFT)) | (s[v & 3] << DBZ_OPT_AMBUSH_SHIFT);
        break;
    }
    case 5: o = SetBit(o, DBZ_OPT_CLASSIC_BG, v); break;
    case 6: o = SetBit(o, DBZ_OPT_SPARKS_OFF, v); break;
    case 7: o = SetBit(o, DBZ_OPT_POWERUP_OFF, v); break;
    case 8: o = SetBit(o, DBZ_OPT_HINTS_OFF, v); break;
    case 9: o = (o & ~(7 << DBZ_OPT_SHINY_SHIFT)) | ((v % 5) << DBZ_OPT_SHINY_SHIFT); break;
    case 10: o = SetBit(o, DBZ_OPT_AUTORUN_OFF, v); break;
    }
    VarSet(VAR_DBZ_OPTIONS, o);
}

// ------------------------------------------------------------------ shiny odds
// Called for every Pokemon the player will own (wild, gift, egg, Shenron). Keeps gender, ability and
// nature: only the upper personality bytes are changed to make it shiny.
static const u16 sShinyThreshold[5] = {8, 16, 64, 256, 0xFFFF};

void DBZ_ApplyShinyOdds(u32 *personality, u32 otId)
{
    u8 setting = DBZ_OptShiny();
    u32 p = *personality;
    u16 t = sShinyThreshold[setting];
    u16 otXor = (otId >> 16) ^ (otId & 0xFFFF);
    u8 nature = p % 25;
    u16 j;

    if (setting == 0)
        return;
    if ((otXor ^ (p >> 16) ^ (p & 0xFFFF)) < SHINY_ODDS)
        return;   // already shiny
    if (t != 0xFFFF && (Random() % 65536) >= (u32)(t - SHINY_ODDS))
        return;
    for (j = 0; j < 256; j++)
    {
        u16 lo = (p & 0xFF) | (((j + (Random() & 0xFF)) & 0xFF) << 8);
        u16 hi = otXor ^ lo ^ (Random() & 7);
        u32 np = ((u32)hi << 16) | lo;
        if (np % 25 == nature)
        {
            *personality = np;
            return;
        }
    }
}
