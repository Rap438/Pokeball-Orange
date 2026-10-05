// PokeBall Orange: starters from four regions. Goku picks a region from Roshi's bag on Route 101
// (Hoenn, Kanto, Sinnoh or Alola), then one of its three starters. Vegeta grabs from the same bag, so
// his starter line follows the same region: each Hoenn starter in a rival party is swapped for the
// matching type's starter of that region, at the stage that region's line would have at that level.
#include "global.h"
#include "dbz.h"
#include "event_data.h"
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
