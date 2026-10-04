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

// Goku's own level (Buu's Fury style) and overworld fights
#define VAR_DBZ_GOKU_LEVEL      VAR_UNUSED_0x4083
#define VAR_DBZ_GOKU_EXP_LO     VAR_UNUSED_0x408B
#define VAR_DBZ_GOKU_EXP_HI     VAR_UNUSED_0x4091
#define VAR_DBZ_GOKU_ANNOUNCED  VAR_UNUSED_0x409B   // last level we told the player about
#define VAR_DBZ_PENDING_FIGHT   VAR_UNUSED_0x409D   // gym leader local id + 1 waiting to fight
#define VAR_DBZ_TOD_OVERRIDE    VAR_UNUSED_0x40A1   // debug: 0 = clock, 1 day, 2 morning, 3 evening, 4 night
#define VAR_DBZ_GOKU_DAMAGE     VAR_UNUSED_0x40A8   // HP Goku is missing (0 = full health)
#define VAR_DBZ_OPTIONS         VAR_UNUSED_0x40B8   // PokeBall Orange settings, see DBZ_OPT_*
#define VAR_DBZ_FIGHTS_WON      VAR_UNUSED_0x40BB
#define VAR_DBZ_FIGHTS_LOST     VAR_UNUSED_0x40DB
#define VAR_DBZ_WISHES          VAR_UNUSED_0x40DC   // Shenron wishes made
#define VAR_DBZ_MISC            VAR_UNUSED_0x40E5   // bit 0: Kamehameha selected; bits 8-15: best SSJ form shown on card
#define DBZ_GOKU_MAX_LEVEL      100
#define LOCALID_DBZ_ENEMY       240

#define DBZ_ENEMY_TRAINER        0
#define DBZ_ENEMY_SAIYAN_SOLDIER 1
#define DBZ_ENEMY_MAJIN_SOLDIER  2
#define DBZ_ENEMY_VEGETA         3

// VAR_DBZ_OPTIONS layout (all zero = defaults)
#define DBZ_OPT_DAYNIGHT_OFF   (1 << 0)
#define DBZ_OPT_SHADOWS_OFF    (1 << 1)
#define DBZ_OPT_AMBUSH_SHIFT   2          // 2 bits: 0 normal, 1 rare, 2 often, 3 off
#define DBZ_OPT_DIFF_SHIFT     4          // 2 bits: 0 normal, 1 easy, 2 hard
#define DBZ_OPT_CLASSIC_BG     (1 << 6)
#define DBZ_OPT_SPARKS_OFF     (1 << 7)
#define DBZ_OPT_POWERUP_OFF    (1 << 8)
#define DBZ_OPT_HINTS_OFF      (1 << 9)
#define DBZ_OPT_HUD_OFF        (1 << 10)
#define DBZ_OPT_SHINY_SHIFT    11         // 3 bits: 0 1/8192, 1 1/4096, 2 1/1024, 3 1/256, 4 always

#define DBZ_DIFF_EASY   0
#define DBZ_DIFF_NORMAL 1
#define DBZ_DIFF_HARD   2

#define DBZ_FIGHT_WON   1
#define DBZ_FIGHT_LOST  2

#define DBZ_REPORT_NONE      0
#define DBZ_REPORT_LEVEL     1
#define DBZ_REPORT_LEARNED   2
#define DBZ_REPORT_NO_ROOM   3

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
