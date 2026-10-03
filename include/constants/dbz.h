#ifndef GUARD_CONSTANTS_DBZ_H
#define GUARD_CONSTANTS_DBZ_H

// PokeBall Orange constants shared by C and event scripts

// Debug helpers: R+SELECT on the overworld opens the debug script. Remove for release builds.
#define DBZ_DEBUG

#define VAR_DBZ_FORM            VAR_UNUSED_0x40F7   // current Saiyan form of Goku
#define VAR_DBZ_SEEN_FORMS      VAR_UNUSED_0x404E   // bitmask of forms already introduced

// Dragon Balls
#define VAR_DBZ_DB_SPOT_1       VAR_UNUSED_0x40F8   // ..._7 = 0x40FE: spot index + 1, 0 = not placed, 0xFFFF = collected
#define VAR_DBZ_DB_STONE_STEPS  VAR_UNUSED_0x40FF   // steps left until stone balls re-scatter
#define DBZ_DB_COUNT            7
#define DBZ_DB_COLLECTED        0xFFFF
#define DBZ_DB_STONE_STEPS      1500
#define LOCALID_DBZ_DRAGON_BALL 230                 // 230..236

#define DBZ_FORM_BASE   0
#define DBZ_FORM_SSJ    1
#define DBZ_FORM_SSJ2   2
#define DBZ_FORM_SSJ3   3
#define DBZ_FORM_COUNT  4

#define DBZ_HIT_NOTHING 0
#define DBZ_HIT_TREE    1
#define DBZ_HIT_ROCK    2
#define DBZ_HIT_BOULDER 3
#define DBZ_HIT_OBJECT  4
#define DBZ_HIT_WALL    5

#endif // GUARD_CONSTANTS_DBZ_H
