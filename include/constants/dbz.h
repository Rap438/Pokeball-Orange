#ifndef GUARD_CONSTANTS_DBZ_H
#define GUARD_CONSTANTS_DBZ_H

// PokeBall Orange constants shared by C and event scripts

// Debug helpers: R+SELECT on the overworld opens the debug script, START ends a personal fight.
// Off in release builds (`make release` defines RELEASE, which also turns off the engine's own debug menus).
#ifndef RELEASE
#define DBZ_DEBUG
#endif

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
#define DBZ_ENEMY_GENERAL_BLUE   4
#define DBZ_ENEMY_DR_GERO        5
#define DBZ_ENEMY_KID_BUU        6
#define DBZ_ENEMY_SPARRING       7
#define DBZ_ENEMY_FIGHTER        8

// story flags (permanent, unused in Emerald)
#define FLAG_DBZ_BOSS_BLUE      FLAG_UNUSED_0x020
#define FLAG_DBZ_BOSS_BUU       FLAG_UNUSED_0x021
#define FLAG_DBZ_BOSS_GERO      FLAG_UNUSED_0x022
#define FLAG_DBZ_FINALE         FLAG_UNUSED_0x023
#define FLAG_DBZ_TOURNAMENT     FLAG_UNUSED_0x024
#define FLAG_DBZ_GRAVITY_INTRO  FLAG_UNUSED_0x025
#define FLAG_DBZ_CHAMBER_CLEAR  FLAG_UNUSED_0x026
#define FLAG_DBZ_TOURNEY_ROUND1 FLAG_UNUSED_0x027
#define FLAG_DBZ_TOURNEY_ROUND2 FLAG_UNUSED_0x028
#define FLAG_DBZ_TOURNEY_ROUND3 FLAG_UNUSED_0x029
#define FLAG_DBZ_FOLLOWER_OFF   FLAG_UNUSED_0x02A
#define FLAG_DBZ_CAMP_CRATE     FLAG_UNUSED_0x02B   // Littleroot camp supply crate looted
#define FLAG_DBZ_FLOWER_FEATHER FLAG_UNUSED_0x02C   // Littleroot flower-bed Fairy Feather found
#define FLAG_DBZ_SEASON_LOCK    FLAG_UNUSED_0x02E   // OPTION > SEASON locked (else the clock's month decides)
#define FLAG_DBZ_SEASON_BIT0    FLAG_UNUSED_0x02F   // locked season = BIT0 + 2 * BIT1 (spring, summer, autumn, winter)
#define FLAG_DBZ_SEASON_BIT1    FLAG_UNUSED_0x030
#define FLAG_DBZ_WILD_HIDDEN    FLAG_UNUSED_0x031   // OPTION > WILD MONS: HIDDEN (classic random encounters instead of visible ones)
#define FLAG_DBZ_EXP_SHARE_ON   FLAG_UNUSED_0x02D   // Training Gi (Exp. Share) switched on; engine I_EXP_SHARE_FLAG   // OPTION > FOLLOWER: OFF (engine follower flag, see config/overworld.h)

// VAR_0x8008 for special DBZ_SetNextFightFlags
#define DBZ_FIGHTF_FUSION    (1 << 0)
#define DBZ_FIGHTF_SCRIPTED  (1 << 1)
#define DBZ_FIGHTF_CHAMBER   (1 << 2)
#define DBZ_FIGHTF_FIGHTER(n) ((n) << 4)

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
#define DBZ_OPT_SHINY_SHIFT    11         // 3 bits: 0 base 1/4096, 1 ~1/2048, 2 ~1/820, 3 ~1/240, 4 always
#define DBZ_OPT_AUTORUN_OFF    (1 << 14)
#define DBZ_OPT_NIMBUS_MAP     (1 << 15)   // Flying Nimbus without the 3D flight

// VAR_DBZ_MISC bits
#define DBZ_MISC_MOVE_MASK     0x0003     // selected special: 0 ki blast, 1 Kamehameha, 2 Spirit Bomb
#define DBZ_MISC_HIDE_KI       (1 << 2)
#define DBZ_MISC_GRAVITY_SHIFT 3          // bits 3-12: gravity-training steps left / 2
#define DBZ_MISC_GRAVITY_MASK  (0x3FF << DBZ_MISC_GRAVITY_SHIFT)
#define DBZ_MISC_REGION_SHIFT  13         // bits 13-14: starter region (0 Hoenn, 1 Kanto, 2 Sinnoh, 3 Alola)
#define DBZ_MISC_REGION_MASK   (3 << DBZ_MISC_REGION_SHIFT)

#define DBZ_REGION_HOENN  0
#define DBZ_REGION_KANTO  1
#define DBZ_REGION_SINNOH 2
#define DBZ_REGION_ALOLA  3

// techniques (unlocked by badges, announced once via VAR_DBZ_SEEN_FORMS bits 8-10)
#define DBZ_TECH_KAIOKEN        1
#define DBZ_TECH_INSTANT_TRANS  2
#define DBZ_TECH_SPIRIT_BOMB    3

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
