// PokeBall Orange: starters from four regions. Goku picks a region from Roshi's bag on Route 101
// (Hoenn, Kanto, Sinnoh or Alola), then one of its three starters. Vegeta grabs from the same bag, so
// his starter line follows the same region: each Hoenn starter in a rival party is swapped for the
// matching type's starter of that region, at the stage that region's line would have at that level.
#include "global.h"
#include "dbz.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "sound.h"
#include "constants/event_objects.h"
#include "pokemon.h"
#include "string_util.h"
#include "constants/dbz.h"
#include "constants/species.h"
#include "constants/trainers.h"

u8 DBZ_GetStarterRegion(void)
{
    return (VarGet(VAR_DBZ_MISC) & DBZ_MISC_REGION_MASK) >> DBZ_MISC_REGION_SHIFT;
}

// special: VAR_RESULT = region picked from MULTI_DBZ_STARTER_REGION (B/cancel keeps Hoenn)
void DBZ_SetStarterRegion(void)
{
    u16 region = gSpecialVar_Result;
    if (region > DBZ_REGION_ALOLA)
        region = DBZ_REGION_HOENN;
    VarSet(VAR_DBZ_MISC, (VarGet(VAR_DBZ_MISC) & ~DBZ_MISC_REGION_MASK) | (region << DBZ_MISC_REGION_SHIFT));
}

struct StarterLine
{
    u16 species[3];
    u8 evoLevel[2];
};

// [region][grass, fire, water]
static const struct StarterLine sLines[4][3] =
{
    [DBZ_REGION_HOENN] = {
        {{SPECIES_TREECKO, SPECIES_GROVYLE, SPECIES_SCEPTILE}, {16, 36}},
        {{SPECIES_TORCHIC, SPECIES_COMBUSKEN, SPECIES_BLAZIKEN}, {16, 36}},
        {{SPECIES_MUDKIP, SPECIES_MARSHTOMP, SPECIES_SWAMPERT}, {16, 36}},
    },
    [DBZ_REGION_KANTO] = {
        {{SPECIES_BULBASAUR, SPECIES_IVYSAUR, SPECIES_VENUSAUR}, {16, 32}},
        {{SPECIES_CHARMANDER, SPECIES_CHARMELEON, SPECIES_CHARIZARD}, {16, 36}},
        {{SPECIES_SQUIRTLE, SPECIES_WARTORTLE, SPECIES_BLASTOISE}, {16, 36}},
    },
    [DBZ_REGION_SINNOH] = {
        {{SPECIES_TURTWIG, SPECIES_GROTLE, SPECIES_TORTERRA}, {18, 32}},
        {{SPECIES_CHIMCHAR, SPECIES_MONFERNO, SPECIES_INFERNAPE}, {14, 36}},
        {{SPECIES_PIPLUP, SPECIES_PRINPLUP, SPECIES_EMPOLEON}, {16, 36}},
    },
    [DBZ_REGION_ALOLA] = {
        {{SPECIES_ROWLET, SPECIES_DARTRIX, SPECIES_DECIDUEYE}, {17, 34}},
        {{SPECIES_LITTEN, SPECIES_TORRACAT, SPECIES_INCINEROAR}, {17, 34}},
        {{SPECIES_POPPLIO, SPECIES_BRIONNE, SPECIES_PRIMARINA}, {17, 34}},
    },
};

// Called for every mon of a trainer party after it is generated.
void DBZ_RemapRivalStarter(struct Pokemon *mon, u8 trainerClass)
{
    u8 region = DBZ_GetStarterRegion();
    u16 species, target;
    u8 type, stage, level;

    if (region == DBZ_REGION_HOENN || trainerClass != TRAINER_CLASS_RIVAL)
        return;
    species = GetMonData(mon, MON_DATA_SPECIES);
    for (type = 0; type < 3; type++)
    {
        for (stage = 0; stage < 3; stage++)
            if (sLines[DBZ_REGION_HOENN][type].species[stage] == species)
                break;
        if (stage < 3)
            break;
    }
    if (type == 3)
        return;

    level = GetMonData(mon, MON_DATA_LEVEL);
    stage = 0;
    while (stage < 2 && level >= sLines[region][type].evoLevel[stage])
        stage++;
    target = sLines[region][type].species[stage];

    SetMonData(mon, MON_DATA_SPECIES, &target);
    SetMonData(mon, MON_DATA_NICKNAME, GetSpeciesName(target));
    GiveMonInitialMoveset(mon);
    CalculateMonStats(mon);
}

// ------------------------------------------------------------------ overworld wild Pokemon temperament
// Species with a behaviour set in their species data keep it. The rest act on type: fighters and dark
// types come at Goku, curious normal and fairy types wander up to him, skittish fliers and electric types
// bolt, Abra and Kadabra teleport away when spotted, psychic, ghost and steel types keep an eye on him, the rest
// (bugs, plants, water, rock, fire...) mind their own business.
enum OverworldWildEncounterBehaviors DBZ_GetOWEBehavior(enum Species species)
{
    enum Type t1, t2;
    species = SanitizeSpeciesId(species);
    if (gSpeciesInfo[species].overworldEncounterBehavior != OWE_IGNORE_PLAYER)
        return gSpeciesInfo[species].overworldEncounterBehavior;
    if (species == SPECIES_ABRA || species == SPECIES_KADABRA)
        return OWE_DESPAWN_ON_NOTICE;
    t1 = gSpeciesInfo[species].types[0];
    t2 = gSpeciesInfo[species].types[1];
#define HAS(t) (t1 == (t) || t2 == (t))
    if (HAS(TYPE_FIGHTING) || HAS(TYPE_DARK) || HAS(TYPE_DRAGON))
        return OWE_CHASE_PLAYER_SLOW;
    if (HAS(TYPE_FLYING) || HAS(TYPE_ELECTRIC))
        return OWE_FLEE_PLAYER_NORMAL;
    if (HAS(TYPE_PSYCHIC) || HAS(TYPE_STEEL) || HAS(TYPE_GHOST))
        return OWE_WATCH_PLAYER_NORMAL;
    if (HAS(TYPE_NORMAL) || HAS(TYPE_FAIRY))
        return OWE_APPROACH_PLAYER_SLOW;
#undef HAS
    return OWE_IGNORE_PLAYER;
}

// ------------------------------------------------------------------ town Pokemon
// special: the talked-to ambient Pokemon cries; STR_VAR_1 = its name, STR_VAR_2 = its sound
void DBZ_AmbientMonCry(void)
{
    u8 id = GetObjectEventIdByLocalIdAndMap(gSpecialVar_LastTalked, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    u16 species = SPECIES_NONE;
    if (id < OBJECT_EVENTS_COUNT && (gObjectEvents[id].graphicsId & OBJ_EVENT_MON))
        species = SanitizeSpeciesId(gObjectEvents[id].graphicsId & OBJ_EVENT_MON_SPECIES_MASK);
    if (species == SPECIES_NONE)
        species = SPECIES_PIKACHU;
    StringCopy(gStringVar1, GetSpeciesName(species));
    {
        enum Type t = gSpeciesInfo[species].types[0], t2 = gSpeciesInfo[species].types[1];
        const u8 *sound = COMPOUND_STRING("Kyuu!");
        if (t == TYPE_FLYING || t2 == TYPE_FLYING)        sound = COMPOUND_STRING("Pii, pii!");
        else if (t == TYPE_WATER || t2 == TYPE_WATER)     sound = COMPOUND_STRING("Puru-ru!");
        else if (t == TYPE_ELECTRIC || t2 == TYPE_ELECTRIC) sound = COMPOUND_STRING("Bzzt! Bzzt!");
        else if (t == TYPE_FIRE || t2 == TYPE_FIRE)       sound = COMPOUND_STRING("Gwaa…");
        else if (t == TYPE_FIGHTING || t2 == TYPE_FIGHTING) sound = COMPOUND_STRING("Hah! Hah!");
        StringCopy(gStringVar2, sound);
    }
    PlayCry_Normal(species, 0);
}

// ------------------------------------------------------------------ guest Pokemon from every region
// About one wild encounter in eight is a "guest": a first-stage Pokemon from any generation whose type fits
// the place and the season (caves: rock, ground, ghost, poison, dark, steel, fighting, dragon; water and
// fishing: water, plus ice in winter; Rock Smash: rock, ground, steel; grass by season, see sSeasonTypes).
// Legendaries, Mythicals, Ultra Beasts and Paradox Pokemon never appear (Shenron grants those); strong
// single-stage Pokemon only show up from level 25. This makes the whole National Dex catchable over a year.
#include "seasons.h"
#include "wild_encounter.h"
#include "random.h"
#include "constants/map_types.h"

static EWRAM_DATA u8 sIsEvolvedBits[(SPECIES_PECHARUNT + 8) / 8] = {0};
static EWRAM_DATA bool8 sIsEvolvedReady = FALSE;

static void BuildEvolvedBits(void)
{
    u32 i, j;
    for (i = SPECIES_BULBASAUR; i <= SPECIES_PECHARUNT; i++)
    {
        const struct Evolution *evo;
        if (!IsSpeciesEnabled(i) || (evo = GetSpeciesEvolutions(i)) == NULL)
            continue;
        for (j = 0; evo[j].method != EVOLUTIONS_END; j++)
        {
            u32 t = SanitizeSpeciesId(evo[j].targetSpecies);
            if (t <= SPECIES_PECHARUNT)
                sIsEvolvedBits[t / 8] |= 1 << (t % 8);
        }
    }
    sIsEvolvedReady = TRUE;
}

static const u8 sSeasonTypes[SEASON_COUNT][6] = {
    [SEASON_SPRING] = {TYPE_GRASS, TYPE_BUG, TYPE_FAIRY, TYPE_NORMAL, TYPE_FLYING, TYPE_GRASS},
    [SEASON_SUMMER] = {TYPE_FIRE, TYPE_ELECTRIC, TYPE_BUG, TYPE_GRASS, TYPE_NORMAL, TYPE_FLYING},
    [SEASON_AUTUMN] = {TYPE_GHOST, TYPE_DARK, TYPE_GROUND, TYPE_FIGHTING, TYPE_PSYCHIC, TYPE_POISON},
    [SEASON_WINTER] = {TYPE_ICE, TYPE_STEEL, TYPE_PSYCHIC, TYPE_DRAGON, TYPE_NORMAL, TYPE_FLYING},
};
static const u8 sCaveTypes[] = {TYPE_ROCK, TYPE_GROUND, TYPE_GHOST, TYPE_POISON, TYPE_DARK, TYPE_STEEL, TYPE_FIGHTING, TYPE_DRAGON};
static const u8 sRockTypes[] = {TYPE_ROCK, TYPE_GROUND, TYPE_STEEL};

static bool32 TypeIn(enum Species s, const u8 *types, u32 n)
{
    u32 i;
    for (i = 0; i < n; i++)
        if (gSpeciesInfo[s].types[0] == types[i] || gSpeciesInfo[s].types[1] == types[i])
            return TRUE;
    return FALSE;
}

static bool32 GuestFits(enum Species s, u32 area, u32 level)
{
    const struct SpeciesInfo *info = &gSpeciesInfo[s];
    if (!IsSpeciesEnabled(s) || (sIsEvolvedBits[s / 8] & (1 << (s % 8))))
        return FALSE;
    if (info->isRestrictedLegendary || info->isSubLegendary || info->isMythical || info->isUltraBeast || info->isParadox || info->isTotem)
        return FALSE;
    if (GetSpeciesBaseStatTotal(s) >= 450 && level < 25)
        return FALSE;
    switch (area)
    {
    case WILD_AREA_WATER:
    case WILD_AREA_FISHING:
        if (Season_Get() == SEASON_WINTER && TypeIn(s, (const u8[]){TYPE_ICE}, 1) && TypeIn(s, (const u8[]){TYPE_WATER}, 1))
            return TRUE;
        return TypeIn(s, (const u8[]){TYPE_WATER}, 1);
    case WILD_AREA_ROCKS:
        return TypeIn(s, sRockTypes, ARRAY_COUNT(sRockTypes));
    default:
        if (TypeIn(s, (const u8[]){TYPE_WATER}, 1) && !TypeIn(s, (const u8[]){TYPE_BUG, TYPE_GRASS, TYPE_FLYING, TYPE_GROUND}, 4))
            return FALSE;   // pure swimmers stay in the water
        if (gMapHeader.mapType == MAP_TYPE_UNDERGROUND)
            return TypeIn(s, sCaveTypes, ARRAY_COUNT(sCaveTypes));
        return TypeIn(s, sSeasonTypes[Season_Get()], 6);
    }
}

enum Species DBZ_MaybeGuestSpecies(enum Species species, u32 area, u32 level)
{
    u32 s, seen = 0;
    enum Species pick = species;
    if (Random() % 8 != 0)
        return species;
    if (!sIsEvolvedReady)
        BuildEvolvedBits();
    // reservoir sampling over the whole National Dex: every fitting Pokemon is equally likely
    for (s = SPECIES_BULBASAUR; s <= SPECIES_PECHARUNT; s++)
    {
        if (GuestFits(s, area, level) && Random() % ++seen == 0)
            pick = s;
    }
    return pick;
}
