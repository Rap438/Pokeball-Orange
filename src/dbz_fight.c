// PokeBall Orange: real-time DBZ fights on the overworld + Goku's own level/EXP/health.
//
// Controls during a fight: D-pad move (B to dash), A punch/kick combo, L fires the selected special
// (tap: ki blast, hold: Kamehameha; hold L + tap R swaps), R powers up.
// Fights happen against trainers (talk to a beaten trainer), gym leaders (right after the badge),
// Red Ribbon soldiers who ambush Goku on routes, and VEGETA after every rival battle.
// Overworld fights lean EXP toward Goku; the party gets a share. Pokemon battles lean toward the
// party; Goku gets a share (see DBZ_GokuGainExpFromBattle, called from Cmd_getexp).
// Damage is based on the power-level ratio between the fighters, capped per hit, so no fight is
// decided by a single blow and Super Saiyan forms help without trivialising everything.
// Goku's HP persists between fights (VAR_DBZ_GOKU_DAMAGE): heal at a POKEMON CENTER, at home, or
// with healing items from the bag.
#include "global.h"
#include "dbz.h"
#include "battle.h"
#include "battle_setup.h"
#include "bg.h"
#include "data.h"
#include "event_data.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "evolution_scene.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "fieldmap.h"
#include "item.h"
#include "money.h"
#include "main.h"
#include "menu.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "palette.h"
#include "util.h"
#include "pokemon.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/items.h"
#include "constants/map_types.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "constants/vars.h"

extern const u8 EventScript_DBZ_FightEnd[];
extern const u8 EventScript_DBZ_GymFight[];
extern const u8 EventScript_DBZ_LevelUpNotice[];
extern const u8 EventScript_DBZ_Ambush[];
extern const u8 EventScript_DBZ_WokeUp[];
extern const u8 EventScript_DBZ_EvolveCheck[];

// ================================================================== Goku's level
static u32 GokuExp(void)
{
    return VarGet(VAR_DBZ_GOKU_EXP_LO) | ((u32)VarGet(VAR_DBZ_GOKU_EXP_HI) << 16);
}

static void SetGokuExp(u32 exp)
{
    VarSet(VAR_DBZ_GOKU_EXP_LO, exp & 0xFFFF);
    VarSet(VAR_DBZ_GOKU_EXP_HI, exp >> 16);
}

// total EXP needed to reach a level: L^3 / 2
static u32 GokuExpForLevel(u8 level)
{
    u32 l = level;
    if (l <= 1)
        return 0;
    return l * l * l / 2;
}

static void InitGokuLevel(void)
{
    // first time: Goku starts a little under his strongest partner (saves from before this feature)
    u8 i, best = 1;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
        {
            u8 l = GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL);
            if (l > best)
                best = l;
        }
    }
    best = best > 3 ? best - 2 : 1;
    VarSet(VAR_DBZ_GOKU_LEVEL, best);
    VarSet(VAR_DBZ_GOKU_ANNOUNCED, best);
    VarSet(VAR_DBZ_GOKU_EXP_LO, (best * best * best / 2) & 0xFFFF);
    VarSet(VAR_DBZ_GOKU_EXP_HI, (best * best * best / 2) >> 16);
}

u8 DBZ_GokuLevel(void)
{
    if (VarGet(VAR_DBZ_GOKU_LEVEL) == 0)
        InitGokuLevel();
    return VarGet(VAR_DBZ_GOKU_LEVEL);
}

void DBZ_Debug_SetGokuLevel(void)
{
    u32 exp = (u32)gSpecialVar_0x8004 * gSpecialVar_0x8004 * gSpecialVar_0x8004 / 2;
    VarSet(VAR_DBZ_GOKU_LEVEL, gSpecialVar_0x8004);
    VarSet(VAR_DBZ_GOKU_ANNOUNCED, gSpecialVar_0x8004);
    VarSet(VAR_DBZ_GOKU_EXP_LO, exp & 0xFFFF);
    VarSet(VAR_DBZ_GOKU_EXP_HI, exp >> 16);
}

// returns number of levels gained
static u8 GokuGainExp(u32 amount)
{
    u8 level = DBZ_GokuLevel();
    u8 start = level;
    u32 exp = GokuExp() + amount;
    if (exp > GokuExpForLevel(DBZ_GOKU_MAX_LEVEL))
        exp = GokuExpForLevel(DBZ_GOKU_MAX_LEVEL);
    SetGokuExp(exp);
    while (level < DBZ_GOKU_MAX_LEVEL && exp >= GokuExpForLevel(level + 1))
        level++;
    VarSet(VAR_DBZ_GOKU_LEVEL, level);
    if (VarGet(VAR_DBZ_GOKU_ANNOUNCED) == 0)
        VarSet(VAR_DBZ_GOKU_ANNOUNCED, start);
    return level - start;
}

// Pokemon battles: Goku trains alongside his team and picks up a smaller share.
void DBZ_GokuGainExpFromBattle(u32 exp)
{
    u32 share = exp / 4;
    if (share == 0)
        share = 1;
    GokuGainExp(share);
}

u32 DBZ_GokuExpTotal(void)
{
    DBZ_GokuLevel();
    return GokuExp();
}

u32 DBZ_GokuExpToNext(void)
{
    u8 level = DBZ_GokuLevel();
    if (level >= DBZ_GOKU_MAX_LEVEL)
        return 0;
    return GokuExpForLevel(level + 1) - GokuExp();
}

// ================================================================== stats, power level and health
static u16 FormPowerPercent(void)
{
    switch (DBZ_GetForm())
    {
    case DBZ_FORM_SSJ:  return 140;
    case DBZ_FORM_SSJ2: return 180;
    case DBZ_FORM_SSJ3: return 230;
    default:            return 100;
    }
}

// power level multiplier of each form, and how much opponents rise to meet it
static u16 FormPLPercent(void)
{
    switch (DBZ_GetForm())
    {
    case DBZ_FORM_SSJ:  return 150;
    case DBZ_FORM_SSJ2: return 200;
    case DBZ_FORM_SSJ3: return 300;
    default:            return 100;
    }
}

static u32 BasePowerLevel(u8 level)
{
    u32 l = level;
    return 30 * l + 3 * l * l;
}

u16 DBZ_GokuMaxHp(void)       { return 30 + 8 * DBZ_GokuLevel(); }
u16 DBZ_GokuPowerStat(void)   { return (4 + 2 * DBZ_GokuLevel()) * FormPowerPercent() / 100; }
u16 DBZ_GokuDefenseStat(void) { return (2 + DBZ_GokuLevel()) * (100 + (FormPowerPercent() - 100) / 2) / 100; }

u32 DBZ_GokuPowerLevel(void)
{
    return BasePowerLevel(DBZ_GokuLevel()) * FormPLPercent() / 100;
}

static u16 StoredGokuHp(void)
{
    u16 max = DBZ_GokuMaxHp();
    u16 dmg = VarGet(VAR_DBZ_GOKU_DAMAGE);
    return dmg >= max ? 0 : max - dmg;
}

void DBZ_SetGokuHp(u16 hp)
{
    u16 max = DBZ_GokuMaxHp();
    if (hp > max)
        hp = max;
    VarSet(VAR_DBZ_GOKU_DAMAGE, max - hp);
}

void DBZ_HealGoku(void)
{
    VarSet(VAR_DBZ_GOKU_DAMAGE, 0);
}

bool8 DBZ_GokuIsKO(void)
{
    return DBZ_GokuHp() == 0;
}

u16 DBZ_GokuCanFight(void)
{
    return !DBZ_GokuIsKO() && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
}

// percent of Goku's max HP a healing item restores (0: does nothing for Goku).
// Goku's max HP grows with his level (30 + 8 per level), so flat Pokemon amounts would be useless
// later on; his scale is a share of his max HP instead, stepping up with how rare each medicine is.
// The Senzu Bean is the one remedy that brings him back from a KO at full strength, as in the show.
u16 DBZ_GokuItemHealPercent(u16 itemId)
{
    bool8 ko = (StoredGokuHp() == 0);
    switch (itemId)
    {
    case ITEM_FULL_RESTORE:  return 100;            // Senzu Bean: full heal, revives too
    case ITEM_REVIVE:        return ko ? 50 : 0;    // Dende's Heal
    case ITEM_MAX_REVIVE:                           // Kami's Heal
    case ITEM_REVIVAL_HERB:  return ko ? 100 : 0;
    }
    if (ko)
        return 0;
    switch (itemId)
    {
    case ITEM_POTION:        return 25;   // Senzu Sprout
    case ITEM_SUPER_POTION:  return 50;   // Senzu Leaf
    case ITEM_HYPER_POTION:  return 75;   // Korin Water
    case ITEM_MAX_POTION:    return 100;  // Sacred Water
    case ITEM_ORAN_BERRY:    return 10;
    case ITEM_BERRY_JUICE:   return 20;
    case ITEM_SITRUS_BERRY:  return 25;
    case ITEM_FRESH_WATER:   return 30;
    case ITEM_ENERGY_POWDER: return 30;
    case ITEM_SODA_POP:      return 40;
    case ITEM_LEMONADE:      return 50;
    case ITEM_MOOMOO_MILK:   return 60;   // a Saiyan appetite
    case ITEM_ENERGY_ROOT:   return 60;
    }
    return 0;
}

// heals Goku with an item (caller removes it from the bag); STR_VAR_1 = HP restored
bool8 DBZ_UseItemOnGoku(u16 itemId)
{
    u16 pct = DBZ_GokuItemHealPercent(itemId);
    u16 max = DBZ_GokuMaxHp(), hp = StoredGokuHp(), heal;
    if (pct == 0 || hp >= max)
        return FALSE;
    heal = (u32)max * pct / 100;
    if (heal == 0)
        heal = 1;
    if (hp + heal > max)
        heal = max - hp;
    DBZ_SetGokuHp(hp + heal);
    ConvertIntToDecimalStringN(gStringVar1, heal, STR_CONV_MODE_LEFT_ALIGN, 4);
    return TRUE;
}

// ================================================================== fight state
enum {
    FIGHT_NONE,
    FIGHT_INTRO,
    FIGHT_ACTIVE,
    FIGHT_OUTRO,
};

enum {
    AI_APPROACH,
    AI_WINDUP,
    AI_RECOVER,
    AI_BEAM_WINDUP,
    AI_STARE,        // General Blue's paralysis stare
};

#define KF_RANGED   (1 << 0)   // throws ki blasts
#define KF_BEAM     (1 << 1)   // fires beams (and answers a Kamehameha with a beam struggle)
#define KF_BOSS     (1 << 2)   // three phases
#define KF_PARALYZE (1 << 3)   // General Blue
#define KF_ABSORB   (1 << 4)   // Dr. Gero drains ki attacks in phase 2+
#define KF_TELEPORT (1 << 5)   // Kid Buu blinks next to Goku and regenerates

struct DbzEnemyKind {
    const u8 *name;
    u8 graphicsId;
    u16 hpPercent;
    u16 powerPercent;
    u8 stepFrames;
    u8 flags;
    u8 expPerLevel;
    u16 music;
};

static const u8 sText_RedRibbon[] = _("RED RIBBON");
static const u8 sText_RRSergeant[] = _("RR SERGEANT");
static const u8 sText_Vegeta[] = _("VEGETA");
static const u8 sText_Goku[] = _("GOKU");
static const u8 sText_Vegito[] = _("VEGITO");
static const u8 sText_GeneralBlue[] = _("GENERAL BLUE");
static const u8 sText_DrGero[] = _("DR. GERO");
static const u8 sText_KidBuu[] = _("KID BUU");
static const u8 sText_Sparring[] = _("TIME SPIRIT");

// World Martial Arts Tournament hand-to-hand opponents (VAR_0x8007 high byte)
static const u8 sText_Spopovich[] = _("SPOPOVICH");
static const u8 sText_Pintar[] = _("PINTAR");
static const u8 sText_Pamput[] = _("PAMPUT");
static const u8 *const sFighterNames[] = { sText_Spopovich, sText_Pintar, sText_Pamput, sText_Vegeta };

static const struct DbzEnemyKind sEnemyKinds[] = {
    [DBZ_ENEMY_TRAINER]        = { NULL,              0,                              100, 100, 18, 0,                              15, MUS_VS_TRAINER },
    [DBZ_ENEMY_SAIYAN_SOLDIER] = { sText_RedRibbon,   OBJ_EVENT_GFX_AQUA_MEMBER_M,    100, 100, 16, KF_RANGED,                      12, MUS_VS_AQUA_MAGMA },
    [DBZ_ENEMY_MAJIN_SOLDIER]  = { sText_RRSergeant,  OBJ_EVENT_GFX_AQUA_MEMBER_F,     90, 108, 14, 0,                              12, MUS_VS_AQUA_MAGMA },
    [DBZ_ENEMY_VEGETA]         = { sText_Vegeta,      OBJ_EVENT_GFX_RIVAL_MAY_NORMAL, 150, 112, 11, KF_RANGED | KF_BEAM,            30, MUS_VS_RIVAL },
    [DBZ_ENEMY_GENERAL_BLUE]   = { sText_GeneralBlue, OBJ_EVENT_GFX_MAGMA_MEMBER_M,   240, 115, 11, KF_BOSS | KF_PARALYZE | KF_RANGED, 40, MUS_VS_AQUA_MAGMA_LEADER },
    [DBZ_ENEMY_DR_GERO]        = { sText_DrGero,      OBJ_EVENT_GFX_ARCHIE,           260, 118, 13, KF_BOSS | KF_ABSORB | KF_RANGED | KF_BEAM, 45, MUS_VS_AQUA_MAGMA_LEADER },
    [DBZ_ENEMY_KID_BUU]        = { sText_KidBuu,      OBJ_EVENT_GFX_MAXIE,            300, 140, 9,  KF_BOSS | KF_TELEPORT | KF_RANGED | KF_BEAM, 60, MUS_VS_RAYQUAZA },
    [DBZ_ENEMY_SPARRING]       = { sText_Sparring,    OBJ_EVENT_GFX_AQUA_MEMBER_M,    110, 108, 12, KF_RANGED,                      36, MUS_VS_ELITE_FOUR },
    [DBZ_ENEMY_FIGHTER]        = { NULL,              OBJ_EVENT_GFX_MAN_3,            130, 105, 13, 0,                              20, MUS_VS_GYM_LEADER },
};

#define ARENA_HALF_W 7
#define ARENA_HALF_H 5

// attack strengths in tenths of a percent of the target's max HP (before the power-level ratio)
#define HIT_PUNCH     100
#define HIT_KICK      115
#define HIT_HEAVY     170
#define HIT_KI        130
#define HIT_KAME      320
#define HIT_SPIRIT    600
#define HIT_E_PUNCH   120
#define HIT_E_KI      130
#define HIT_E_BEAM    200
#define CAP_NORMAL    30
#define CAP_KAME      40
#define CAP_SPIRIT    70

// Goku's attack overlay (Buu's Fury fighter frames)
enum { ATK_DOWN_WIND, ATK_DOWN_HIT, ATK_UP_WIND, ATK_UP_HIT, ATK_SIDE_WIND, ATK_SIDE_HIT };
#define ATK_KICK 6    // kick frames follow the punch frames
#define ATK_FRAMES 14
#define ATK_HIT_AT 4
#define ATK_RECOVER_AT 10

#define GUARD_PERFECT_FRAMES 8
#define KAIOKEN_FRAMES   480
#define KAIOKEN_COOLDOWN 600
#define STRUGGLE_SEGS    5

#define FIGHT_FLAG_FUSION    (1 << 0)
#define FIGHT_FLAG_SCRIPTED  (1 << 1)   // can't be won (story): ends when Goku is worn down
#define FIGHT_FLAG_CHAMBER   (1 << 2)   // Hyperbolic Time Chamber: white world, triple EXP

static EWRAM_DATA struct {
    u8 phase;
    u8 kind;
    u8 level;
    u8 enemyObjId;
    u8 enemyLocalId;
    bool8 spawned;
    bool8 leader;
    bool8 waitScript;     // started from a script that is waiting (rival / boss fights)
    bool8 enemySSJ;
    u8 flags;
    u8 hudWin;
    u8 aiState;
    u8 bossPhase;
    u8 projSprite;
    u8 projDir;
    u8 projDist;
    bool8 projBeam;
    u8 savedRangeX;
    u8 savedRangeY;
    u8 result;
    u8 atkTimer;          // > 0 while Goku's punch/kick plays
    u8 atkDir;
    u8 atkStep;           // 0 punch, 1 kick, 2 heavy finisher
    u8 atkSprite;
    u8 comboWindow;
    u8 hitStop;
    u8 shake;
    u8 flash;
    u8 paralyzed;
    u8 counterReady;
    u16 guardFrames;
    u16 kaioken, kaiokenCooldown;
    u8 name[16];
    u16 enemyHp, enemyMaxHp;
    u16 gokuHp, gokuMaxHp;
    u32 enemyPL;
    u16 timer, aiTimer, rangedCooldown, beamCooldown, teleportTimer;
    u16 gokuInvuln, enemyFlash, attackCooldown;
    s16 arenaX0, arenaY0, arenaX1, arenaY1;
    s16 projTileX, projTileY;
    u32 expPool;
    bool8 hudDirty;
    // beam struggle
    bool8 struggle;
    u8 struggleDir;
    s16 struggleBalance;  // -1600..1600, positive = Goku winning
    u16 struggleTimer;
    u8 gokuSeg[STRUGGLE_SEGS];
    u8 enemySeg[STRUGGLE_SEGS];
    u8 clashSprite;
    s16 startPX, startPY, startEX, startEY;
} sFight = {0};

// EXP results for the end-of-fight messages
#define MAX_REPORTS 24
static EWRAM_DATA struct {
    u8 type;
    u8 partyId;
    u8 level;
    u16 move;
} sReports[MAX_REPORTS] = {0};
static EWRAM_DATA u8 sReportCount = 0;
static EWRAM_DATA u8 sReportPos = 0;
static EWRAM_DATA u8 sEvolveCheck = 0;
static EWRAM_DATA u16 sAmbushSteps = 0;
// EWRAM isn't cleared at boot: these use magic values
#define PENDING_MAGIC 0xA5
static EWRAM_DATA u8 sKoNotice = 0;
static EWRAM_DATA u8 sEvolvePending = 0;
static EWRAM_DATA u8 sFused = 0;
static EWRAM_DATA u16 sChamberHidden = 0;   // objects hidden for the white void of the Time Chamber

bool8 DBZ_IsFighting(void)
{
    return sFight.phase == FIGHT_ACTIVE || sFight.phase == FIGHT_INTRO || sFight.phase == FIGHT_OUTRO;
}

bool8 DBZ_IsFused(void)
{
    return sFused == PENDING_MAGIC;
}

u16 DBZ_GokuHp(void)
{
    if (DBZ_IsFighting())
        return sFight.gokuHp;
    return StoredGokuHp();
}

u16 DBZ_GokuHpMaxNow(void)
{
    if (DBZ_IsFighting())
        return sFight.gokuMaxHp;
    return DBZ_GokuMaxHp();
}

static struct ObjectEvent *Player(void)
{
    return &gObjectEvents[gPlayerAvatar.objectEventId];
}

static struct ObjectEvent *Enemy(void)
{
    return &gObjectEvents[sFight.enemyObjId];
}

bool8 DBZ_FightBlocksTile(struct ObjectEvent *objectEvent, s16 x, s16 y)
{
    if (sFight.phase != FIGHT_ACTIVE && sFight.phase != FIGHT_INTRO)
        return FALSE;
    if (objectEvent != Player() && objectEvent != Enemy())
        return FALSE;
    if (x < sFight.arenaX0 || x > sFight.arenaX1 || y < sFight.arenaY0 || y > sFight.arenaY1)
        return TRUE;
    // no wandering through doors / warps mid-fight
    if (MetatileBehavior_IsWarpDoor(MapGridGetMetatileBehaviorAt(x, y)))
        return TRUE;
    return FALSE;
}

// ------------------------------------------------------------------ difficulty / power levels
static u16 DiffEnemyDamage(void) { static const u8 s[3] = {70, 100, 135}; return s[DBZ_OptDifficulty()]; }
static u16 DiffEnemyHp(void)     { static const u8 s[3] = {85, 100, 120}; return s[DBZ_OptDifficulty()]; }
static s8 DiffWindup(void)       { static const s8 s[3] = {6, 0, -3}; return s[DBZ_OptDifficulty()]; }

// Goku's power level right now (fusion and Kaio-ken included)
u32 DBZ_CurrentPowerLevel(void)
{
    u32 pl;
    if (DBZ_IsFused())
        pl = BasePowerLevel(DBZ_GokuLevel()) * 2 * 300 / 100;   // Goku + Vegeta, Super Saiyan
    else
        pl = DBZ_GokuPowerLevel();
    if (DBZ_IsFighting() && sFight.kaioken)
        pl = pl * 150 / 100;
    if (DBZ_GravityStepsLeft() != 0)
        pl = pl * 110 / 100;
    return pl;
}

// opponents power up when Goku does (a little less than he does)
static u16 EnemyFormScale(void)
{
    u16 goku = DBZ_IsFused() ? 150 : FormPLPercent();
    u16 scale = 100 + (goku - 100) * 6 / 10;
    if (sFight.enemySSJ && scale < 150)
        scale = 150;
    return scale;
}

static u32 CurrentEnemyPL(void)
{
    u32 pl = sFight.enemyPL * EnemyFormScale() / 100;
    if (sEnemyKinds[sFight.kind].flags & KF_BOSS)
        pl = pl * (100 + sFight.bossPhase * 15) / 100;
    return pl;
}

// ------------------------------------------------------------------ HUD (top of screen during a fight)
static void DrawBar(u8 win, u16 x, u16 y, u16 w, u16 cur, u16 max, u8 color)
{
    u16 fill = max ? (u32)w * cur / max : 0;
    if (cur > 0 && fill == 0)
        fill = 1;
    FillWindowPixelRect(win, PIXEL_FILL(2), x, y, w + 2, 6);
    FillWindowPixelRect(win, PIXEL_FILL(10), x + 1, y + 1, w, 4);
    if (fill)
        FillWindowPixelRect(win, PIXEL_FILL(color), x + 1, y + 1, fill, 4);
}

static const u8 sHudColors[] = {10, 1, 2};
static const u8 sText_HudPL[] = _("{STR_VAR_1}  PL {STR_VAR_2}");
static const u8 sText_HudKaioken[] = _("{STR_VAR_1} KAIO-KEN {STR_VAR_2}");

static void DrawHud(void)
{
    u8 win = sFight.hudWin;
    u16 w;
    FillWindowPixelBuffer(win, PIXEL_FILL(10));
    StringCopy(gStringVar1, DBZ_IsFused() ? sText_Vegito : sText_Goku);
    ConvertIntToDecimalStringN(gStringVar2, DBZ_CurrentPowerLevel(), STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, sFight.kaioken ? sText_HudKaioken : sText_HudPL);
    AddTextPrinterParameterized3(win, FONT_SMALL, 4, 0, sHudColors, TEXT_SKIP_DRAW, gStringVar4);
    DrawBar(win, 4, 13, 90, sFight.gokuHp, sFight.gokuMaxHp, sFight.gokuHp * 4 < sFight.gokuMaxHp ? 4 : 6);

    StringCopy(gStringVar1, sFight.name);
    ConvertIntToDecimalStringN(gStringVar2, CurrentEnemyPL(), STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, sText_HudPL);
    w = GetStringWidth(FONT_SMALL, gStringVar4, 0);
    AddTextPrinterParameterized3(win, FONT_SMALL, 236 - w, 0, sHudColors, TEXT_SKIP_DRAW, gStringVar4);
    DrawBar(win, 236 - 92, 13, 90, sFight.enemyHp, sFight.enemyMaxHp, 4);
    CopyWindowToVram(win, COPYWIN_FULL);
    sFight.hudDirty = FALSE;
}

static void CreateHud(void)
{
    struct WindowTemplate t = { .bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 3, .paletteNum = 13, .baseBlock = 0x220 };
    LoadPalette(gStandardMenuPalette, BG_PLTT_ID(13), PLTT_SIZE_4BPP);
    sFight.hudWin = AddWindow(&t);
    PutWindowTilemap(sFight.hudWin);
    DrawHud();
}

static void DestroyHud(void)
{
    if (sFight.hudWin == WINDOW_NONE)
        return;
    FillWindowPixelBuffer(sFight.hudWin, PIXEL_FILL(0));
    ClearWindowTilemap(sFight.hudWin);
    CopyWindowToVram(sFight.hudWin, COPYWIN_FULL);
    RemoveWindow(sFight.hudWin);
    sFight.hudWin = WINDOW_NONE;
    ScheduleBgCopyTilemapToVram(0);
}

// ------------------------------------------------------------------ helpers
// percent of the defender's max HP, scaled by the power-level ratio, capped and floored
static u16 Damage(u32 atkPL, u32 defPL, u16 defMaxHp, u16 strength, u8 capPercent)
{
    u32 ratio, d, cap, floor;
    if (defPL == 0)
        defPL = 1;
    ratio = atkPL * 100 / defPL;
    // soften big gaps: above even it grows at 2/3 speed
    if (ratio > 100)
        ratio = 100 + (ratio - 100) * 2 / 3;
    if (ratio < 35)
        ratio = 35;
    if (ratio > 250)
        ratio = 250;
    d = (u32)defMaxHp * strength * ratio / 100000;
    d = d * (90 + (Random() % 21)) / 100;
    cap = (u32)defMaxHp * capPercent / 100;
    floor = defMaxHp / 50;
    if (d > cap)
        d = cap;
    if (d < floor)
        d = floor;
    if (d < 1)
        d = 1;
    return d;
}

static u8 DirTo(s16 fx, s16 fy, s16 tx, s16 ty)
{
    s16 dx = tx - fx, dy = ty - fy;
    if (abs(dx) >= abs(dy))
        return dx < 0 ? DIR_WEST : DIR_EAST;
    return dy < 0 ? DIR_NORTH : DIR_SOUTH;
}

static void Vec(u8 dir, s16 *dx, s16 *dy)
{
    *dx = 0; *dy = 0;
    switch (dir)
    {
    case DIR_SOUTH: *dy = 1; break;
    case DIR_NORTH: *dy = -1; break;
    case DIR_WEST:  *dx = -1; break;
    case DIR_EAST:  *dx = 1; break;
    }
}

static bool8 EnemyBusy(void)
{
    struct ObjectEvent *e = Enemy();
    if (ObjectEventIsMovementOverridden(e) && !ObjectEventClearHeldMovementIfFinished(e))
        return TRUE;
    return FALSE;
}

static bool8 EnemyCanStep(u8 dir)
{
    struct ObjectEvent *e = Enemy();
    s16 dx, dy;
    Vec(dir, &dx, &dy);
    return GetCollisionAtCoords(e, e->currentCoords.x + dx, e->currentCoords.y + dy, dir) == COLLISION_NONE;
}

static void FlashObjectPalette(struct ObjectEvent *obj, u8 coeff, u16 color)
{
    u8 pal = gSprites[obj->spriteId].oam.paletteNum;
    BlendPalette(OBJ_PLTT_ID(pal), 16, coeff, color);
}

// Vegeta keeps a golden glow once he goes Super Saiyan; bosses glow by phase
static void RestoreEnemyPalette(void)
{
    if (sFight.enemySSJ)
        FlashObjectPalette(Enemy(), 5, RGB(31, 27, 6));
    else if (sFight.bossPhase == 2)
        FlashObjectPalette(Enemy(), 4, RGB(31, 6, 10));
    else
        FlashObjectPalette(Enemy(), 0, RGB_WHITE);
}

static void SpriteCenter(struct ObjectEvent *obj, s16 *x, s16 *y)
{
    struct Sprite *s = &gSprites[obj->spriteId];
    *x = s->x + s->x2;
    *y = s->y + s->y2 + 4;
}

static void Shake(u8 frames)
{
    if (frames > sFight.shake)
        sFight.shake = frames;
    SetCameraPanningCallback(NULL);
}

static void Flash(u8 frames)
{
    sFight.flash = frames;
}

static void UpdateShakeAndFlash(void)
{
    if (sFight.shake)
    {
        sFight.shake--;
        if (sFight.shake == 0)
        {
            SetCameraPanning(0, 0);
            InstallCameraPanAheadCallback();
        }
        else
        {
            SetCameraPanning((sFight.shake & 1) ? 2 : -2, (sFight.shake & 2) ? 1 : -1);
        }
    }
    if (sFight.flash)
    {
        sFight.flash--;
        DBZ_BlendPalettes(PALETTES_ALL, sFight.flash * 3, RGB_WHITE);
        if (sFight.flash == 0)
            RestoreEnemyPalette();
    }
}

// ------------------------------------------------------------------ Goku's attack overlay sprite
#define TAG_DBZ_ATK_PAL 0x2F40
#define ATK_SET_VEGITO 4

static const u32 sAtkGfx_Base[] = INCGFX_U32("graphics/dbz/punch/goku.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u32 sAtkGfx_Ssj[]  = INCGFX_U32("graphics/dbz/punch/goku_ssj.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u32 sAtkGfx_Ssj2[] = INCGFX_U32("graphics/dbz/punch/goku_ssj2.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u32 sAtkGfx_Ssj3[] = INCGFX_U32("graphics/dbz/punch/goku_ssj3.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u32 sAtkGfx_Vegito[] = INCGFX_U32("graphics/dbz/punch/vegito.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u16 sAtkPal_Base[] = INCGFX_U16("graphics/dbz/punch/goku.png", ".gbapal");
static const u16 sAtkPal_Ssj[]  = INCGFX_U16("graphics/dbz/punch/goku_ssj.png", ".gbapal");
static const u16 sAtkPal_Ssj2[] = INCGFX_U16("graphics/dbz/punch/goku_ssj2.png", ".gbapal");
static const u16 sAtkPal_Ssj3[] = INCGFX_U16("graphics/dbz/punch/goku_ssj3.png", ".gbapal");
static const u16 sAtkPal_Vegito[] = INCGFX_U16("graphics/dbz/punch/vegito.png", ".gbapal");

#define ATK_FRAME(gfx, n) {.data = (const u8 *)(gfx) + (n) * 512, .size = 512}
#define ATK_FRAMESET(gfx) { ATK_FRAME(gfx, 0), ATK_FRAME(gfx, 1), ATK_FRAME(gfx, 2), ATK_FRAME(gfx, 3), ATK_FRAME(gfx, 4), ATK_FRAME(gfx, 5), \
                            ATK_FRAME(gfx, 6), ATK_FRAME(gfx, 7), ATK_FRAME(gfx, 8), ATK_FRAME(gfx, 9), ATK_FRAME(gfx, 10), ATK_FRAME(gfx, 11) }
static const struct SpriteFrameImage sAtkImages_Base[] = ATK_FRAMESET(sAtkGfx_Base);
static const struct SpriteFrameImage sAtkImages_Ssj[]  = ATK_FRAMESET(sAtkGfx_Ssj);
static const struct SpriteFrameImage sAtkImages_Ssj2[] = ATK_FRAMESET(sAtkGfx_Ssj2);
static const struct SpriteFrameImage sAtkImages_Ssj3[] = ATK_FRAMESET(sAtkGfx_Ssj3);
static const struct SpriteFrameImage sAtkImages_Vegito[] = ATK_FRAMESET(sAtkGfx_Vegito);

#define ATK_ANIM(n) static const union AnimCmd sAtkAnim_##n[] = { ANIMCMD_FRAME(n, 1), ANIMCMD_END }; \
                    static const union AnimCmd sAtkAnimF_##n[] = { ANIMCMD_FRAME(n, 1, .hFlip = TRUE), ANIMCMD_END };
ATK_ANIM(0) ATK_ANIM(1) ATK_ANIM(2) ATK_ANIM(3) ATK_ANIM(4) ATK_ANIM(5)
ATK_ANIM(6) ATK_ANIM(7) ATK_ANIM(8) ATK_ANIM(9) ATK_ANIM(10) ATK_ANIM(11)

static const union AnimCmd *const sAtkAnims[] = {
    sAtkAnim_0, sAtkAnim_1, sAtkAnim_2, sAtkAnim_3, sAtkAnim_4, sAtkAnim_5,
    sAtkAnim_6, sAtkAnim_7, sAtkAnim_8, sAtkAnim_9, sAtkAnim_10, sAtkAnim_11,
    sAtkAnimF_0, sAtkAnimF_1, sAtkAnimF_2, sAtkAnimF_3, sAtkAnimF_4, sAtkAnimF_5,
    sAtkAnimF_6, sAtkAnimF_7, sAtkAnimF_8, sAtkAnimF_9, sAtkAnimF_10, sAtkAnimF_11,
};

static const struct OamData sOam_Atk32 = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 2,
};

#define ATK_TEMPLATE(name, imgs) static const struct SpriteTemplate name = { \
    .tileTag = TAG_NONE, .paletteTag = TAG_DBZ_ATK_PAL, .oam = &sOam_Atk32, .anims = sAtkAnims, \
    .images = imgs, .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCallbackDummy };
ATK_TEMPLATE(sAtkTemplate_Base, sAtkImages_Base)
ATK_TEMPLATE(sAtkTemplate_Ssj, sAtkImages_Ssj)
ATK_TEMPLATE(sAtkTemplate_Ssj2, sAtkImages_Ssj2)
ATK_TEMPLATE(sAtkTemplate_Ssj3, sAtkImages_Ssj3)
ATK_TEMPLATE(sAtkTemplate_Vegito, sAtkImages_Vegito)

static const struct SpriteTemplate *const sAtkTemplates[] = {
    &sAtkTemplate_Base, &sAtkTemplate_Ssj, &sAtkTemplate_Ssj2, &sAtkTemplate_Ssj3, &sAtkTemplate_Vegito,
};
static const u16 *const sAtkPals[] = { sAtkPal_Base, sAtkPal_Ssj, sAtkPal_Ssj2, sAtkPal_Ssj3, sAtkPal_Vegito };

static u8 AtkSet(void)
{
    return DBZ_IsFused() ? ATK_SET_VEGITO : DBZ_GetForm();
}

static bool8 AtkSpriteValid(void)
{
    u8 id = sFight.atkSprite;
    return id < MAX_SPRITES && gSprites[id].inUse && gSprites[id].template == sAtkTemplates[AtkSet()];
}

bool8 DBZ_PlayerOverlayActive(void)
{
    return DBZ_IsFighting() && (sFight.atkTimer != 0 || sFight.guardFrames != 0) && AtkSpriteValid();
}

static void LoadAtkPalette(void)
{
    struct SpritePalette pal;
    u8 slot;
    FreeSpritePaletteByTag(TAG_DBZ_ATK_PAL);
    pal.data = sAtkPals[AtkSet()];
    pal.tag = TAG_DBZ_ATK_PAL;
    slot = LoadSpritePalette(&pal);
    if (slot != 0xFF)
        UpdateSpritePaletteWithTime(slot);   // match the day/night tint on Goku
}

static void DestroyAtkSprite(void)
{
    if (AtkSpriteValid())
        DestroySprite(&gSprites[sFight.atkSprite]);
    sFight.atkSprite = MAX_SPRITES;
    Player()->invisible = FALSE;
}

static u8 AtkFrame(u8 dir, bool8 hit, bool8 kick)
{
    u8 f;
    switch (dir)
    {
    case DIR_NORTH: f = ATK_UP_WIND; break;
    case DIR_WEST:
    case DIR_EAST:  f = ATK_SIDE_WIND; break;
    default:        f = ATK_DOWN_WIND; break;
    }
    if (hit)
        f++;
    if (kick)
        f += ATK_KICK;
    if (dir == DIR_EAST)
        f += 12;   // h-flipped version
    return f;
}

static void StartAtkSprite(u8 dir, bool8 kick)
{
    struct Sprite *ps = &gSprites[gPlayerAvatar.spriteId];
    u8 id;
    DestroyAtkSprite();
    if (IndexOfSpritePaletteTag(TAG_DBZ_ATK_PAL) == 0xFF)
        LoadAtkPalette();
    id = CreateSprite(sAtkTemplates[AtkSet()], ps->x, ps->y, ps->subpriority);
    sFight.atkSprite = id;
    if (id == MAX_SPRITES)
        return;
    gSprites[id].coordOffsetEnabled = TRUE;
    StartSpriteAnim(&gSprites[id], AtkFrame(dir, FALSE, kick));
    Player()->invisible = TRUE;
}

static void PlaceAtkSprite(s16 lunge, u8 dir)
{
    struct Sprite *ps = &gSprites[gPlayerAvatar.spriteId];
    struct Sprite *s = &gSprites[sFight.atkSprite];
    s16 dx, dy;
    Vec(dir, &dx, &dy);
    s->x = ps->x + dx * lunge;
    s->y = ps->y + dy * lunge;
    s->x2 = 0;
    s->y2 = 0;
    s->oam.priority = ps->oam.priority;
    s->subpriority = ps->subpriority ? ps->subpriority - 1 : 0;   // in front of whatever Goku is in front of
    s->invisible = sFight.gokuInvuln && (sFight.gokuInvuln % 4) >= 2;
}

static void UpdateAtkSprite(void)
{
    s16 lunge = 0;
    bool8 kick = sFight.atkStep != 0;
    if (!AtkSpriteValid())
        return;
    if (sFight.atkTimer == ATK_FRAMES - ATK_HIT_AT)
        StartSpriteAnim(&gSprites[sFight.atkSprite], AtkFrame(sFight.atkDir, TRUE, kick));
    else if (sFight.atkTimer == ATK_FRAMES - ATK_RECOVER_AT)
        StartSpriteAnim(&gSprites[sFight.atkSprite], AtkFrame(sFight.atkDir, FALSE, kick));
    if (sFight.atkTimer <= ATK_FRAMES - ATK_HIT_AT && sFight.atkTimer > ATK_FRAMES - ATK_RECOVER_AT)
        lunge = sFight.atkStep == 2 ? 6 : 3;
    PlaceAtkSprite(lunge, sFight.atkDir);
}

// ------------------------------------------------------------------ guard (hold B standing still)
static void StartGuard(void)
{
    sFight.guardFrames = 1;
    StartAtkSprite(GetPlayerFacingDirection(), FALSE);
    PlaySE(SE_M_DETECT);
}

static void StopGuard(void)
{
    sFight.guardFrames = 0;
    if (sFight.atkTimer == 0)
        DestroyAtkSprite();
}

static void UpdateGuard(void)
{
    u8 pal;
    if (sFight.guardFrames == 0 || !AtkSpriteValid())
        return;
    if (sFight.guardFrames < 0xFFFF)
        sFight.guardFrames++;
    PlaceAtkSprite(0, GetPlayerFacingDirection());
    // blue ki shell, brighter inside the perfect-guard window
    pal = gSprites[sFight.atkSprite].oam.paletteNum;
    BlendPalette(OBJ_PLTT_ID(pal), 16, sFight.guardFrames <= GUARD_PERFECT_FRAMES ? 9 : ((sFight.guardFrames / 4) % 2 ? 5 : 3), RGB(10, 20, 31));
}

// ------------------------------------------------------------------ damage
static void HitEnemy(u16 strength, u8 cap, u8 fromDir, u8 knockback, s16 fxX, s16 fxY)
{
    u16 dmg;
    if (sFight.phase != FIGHT_ACTIVE)
        return;
    if (sFight.counterReady)
    {
        strength = strength * 3 / 2;
        sFight.counterReady = 0;
    }
    dmg = Damage(DBZ_CurrentPowerLevel(), CurrentEnemyPL(), sFight.enemyMaxHp, strength, cap);
    sFight.enemyHp = dmg >= sFight.enemyHp ? 0 : sFight.enemyHp - dmg;
    // story fights Goku can't win alone
    if ((sFight.flags & FIGHT_FLAG_SCRIPTED) && sFight.enemyHp < sFight.enemyMaxHp * 2 / 5)
    {
        sFight.enemyHp = sFight.enemyMaxHp * 3 / 5;
        PlaySE(SE_M_HEAL_BELL);
    }
    sFight.enemyFlash = 10;
    sFight.hudDirty = TRUE;
    sFight.hitStop = strength >= HIT_HEAVY ? 6 : 3;
    DBZ_SpawnImpactAt(fxX, fxY);
    if (strength >= HIT_HEAVY)
    {
        Shake(strength >= HIT_KAME ? 14 : 8);
        if (strength >= HIT_KAME)
            Flash(5);
    }
    PlaySE(strength >= HIT_KAME ? SE_M_EXPLOSION : (strength >= HIT_KICK ? SE_M_MEGA_KICK : SE_M_COMET_PUNCH));
    // getting hit interrupts the enemy's attack
    sFight.aiState = AI_RECOVER;
    sFight.aiTimer = 16;
    if (knockback && sFight.enemyHp > 0)
    {
        struct ObjectEvent *e = Enemy();
        if (!ObjectEventIsMovementOverridden(e) && EnemyCanStep(fromDir))
            ObjectEventSetHeldMovement(e, knockback >= 2 ? GetWalkFasterMovementAction(fromDir) : GetWalkFastMovementAction(fromDir));
        if (knockback >= 2)
            sFight.aiTimer = 30;
    }
}

// returns TRUE if the hit was perfectly guarded
static bool8 HitGoku(u16 strength, bool8 beam)
{
    u16 dmg;
    s16 x, y;
    if (sFight.gokuInvuln || sFight.phase != FIGHT_ACTIVE)
        return FALSE;
    SpriteCenter(Player(), &x, &y);
    if (sFight.guardFrames != 0 && sFight.guardFrames <= GUARD_PERFECT_FRAMES && !beam)
    {
        // perfect guard: no damage, the attacker is wide open
        PlaySE(SE_M_REFLECT);
        DBZ_SpawnImpactAt(x, y - 4);
        sFight.counterReady = 1;
        sFight.aiState = AI_RECOVER;
        sFight.aiTimer = 40;
        sFight.enemyFlash = 12;
        sFight.gokuInvuln = 10;
        return TRUE;
    }
    dmg = Damage(CurrentEnemyPL(), DBZ_CurrentPowerLevel(), sFight.gokuMaxHp, strength, CAP_NORMAL);
    dmg = (u32)dmg * DiffEnemyDamage() / 100;
    if (sFight.guardFrames != 0)
        dmg = dmg / 4;
    if (dmg == 0)
        dmg = 1;
    sFight.gokuHp = dmg >= sFight.gokuHp ? 0 : sFight.gokuHp - dmg;
    sFight.gokuInvuln = sFight.guardFrames ? 16 : 36;
    sFight.hudDirty = TRUE;
    PlaySE(sFight.guardFrames ? SE_M_DOUBLE_TEAM : SE_M_VITAL_THROW);
    if (!sFight.guardFrames)
        Shake(beam ? 12 : 6);
    DBZ_SpawnImpactAt(x, y);
    // scripted story fights end once Goku is worn down
    if ((sFight.flags & FIGHT_FLAG_SCRIPTED) && sFight.gokuHp * 4 < sFight.gokuMaxHp)
        sFight.gokuHp = 0;
    return FALSE;
}

// called by the blast task when a ki blast / Kamehameha (1) / Spirit Bomb (2) lands during a fight
void DBZ_FightOnBlast(u8 hitType, u8 hitLocal, u8 kame)
{
    s16 x, y;
    if (sFight.phase != FIGHT_ACTIVE)
        return;
    if (hitType == DBZ_HIT_OBJECT && hitLocal == sFight.enemyLocalId)
    {
        SpriteCenter(Enemy(), &x, &y);
        // Dr. Gero drinks ki attacks once he's serious
        if ((sEnemyKinds[sFight.kind].flags & KF_ABSORB) && sFight.bossPhase >= 1 && kame != 2)
        {
            u16 heal = sFight.enemyMaxHp / (kame ? 6 : 12);
            sFight.enemyHp = sFight.enemyHp + heal > sFight.enemyMaxHp ? sFight.enemyMaxHp : sFight.enemyHp + heal;
            sFight.hudDirty = TRUE;
            FlashObjectPalette(Enemy(), 10, RGB(8, 31, 8));
            sFight.enemyFlash = 0;
            PlaySE(SE_M_ABSORB);
            return;
        }
        if (kame == 2)
            HitEnemy(HIT_SPIRIT, CAP_SPIRIT, GetPlayerFacingDirection(), 2, x, y);
        else
            HitEnemy(kame ? HIT_KAME : HIT_KI, kame ? CAP_KAME : CAP_NORMAL, GetPlayerFacingDirection(), kame ? 2 : 1, x, y);
    }
}

// ------------------------------------------------------------------ enemy projectiles (ki and beams)
static void FireProjectile(u8 dir, bool8 beam)
{
    s16 x, y;
    SpriteCenter(Enemy(), &x, &y);
    sFight.projSprite = DBZ_CreateFxSpriteFor(x, y, TRUE);
    if (sFight.projSprite == MAX_SPRITES)
        return;
    sFight.projDir = dir;
    sFight.projDist = 0;
    sFight.projBeam = beam;
    sFight.projTileX = Enemy()->currentCoords.x;
    sFight.projTileY = Enemy()->currentCoords.y;
    PlaySE(beam ? SE_M_HYPER_BEAM : SE_M_SWIFT);
}

static void DestroyProjectile(void)
{
    if (sFight.projSprite != MAX_SPRITES)
        DBZ_DestroyFxSprite(sFight.projSprite);
    sFight.projSprite = MAX_SPRITES;
}

static void UpdateProjectile(void)
{
    s16 dx, dy, tx, ty, speed;
    struct Sprite *s;
    if (sFight.projSprite == MAX_SPRITES)
        return;
    s = &gSprites[sFight.projSprite];
    Vec(sFight.projDir, &dx, &dy);
    speed = sFight.projBeam ? 6 : 4;
    s->x += dx * speed;
    s->y += dy * speed;
    sFight.projDist += speed;
    if (sFight.projBeam)
        DBZ_SetFxSpriteFrame(sFight.projSprite, (sFight.projDist / 6) % 2 ? DBZ_FX_HEAD_1 : DBZ_FX_HEAD_0);
    else
        DBZ_SetFxSpriteFrame(sFight.projSprite, (sFight.projDist / 8) % 2 ? DBZ_FX_KI_1 : DBZ_FX_KI_0);
    tx = sFight.projTileX + dx * ((sFight.projDist + 8) / 16);
    ty = sFight.projTileY + dy * ((sFight.projDist + 8) / 16);
    if (tx == Player()->currentCoords.x && ty == Player()->currentCoords.y)
    {
        if (!HitGoku(sFight.projBeam ? HIT_E_BEAM : HIT_E_KI, sFight.projBeam) || sFight.projBeam)
        {
            DestroyProjectile();
            return;
        }
        // perfect guard bats a ki blast away
        DestroyProjectile();
        return;
    }
    if (sFight.projDist > 16 * 7 || MapGridGetCollisionAt(tx, ty))
    {
        DBZ_SpawnImpactAt(s->x, s->y);
        DestroyProjectile();
    }
}

// ------------------------------------------------------------------ beam struggle
static void DestroyStruggleSprites(void)
{
    u8 i;
    for (i = 0; i < STRUGGLE_SEGS; i++)
    {
        if (sFight.gokuSeg[i] != MAX_SPRITES)
            DBZ_DestroyFxSprite(sFight.gokuSeg[i]);
        if (sFight.enemySeg[i] != MAX_SPRITES)
            DBZ_DestroyFxSprite(sFight.enemySeg[i]);
        sFight.gokuSeg[i] = MAX_SPRITES;
        sFight.enemySeg[i] = MAX_SPRITES;
    }
    if (sFight.clashSprite != MAX_SPRITES)
        DBZ_DestroyFxSprite(sFight.clashSprite);
    sFight.clashSprite = MAX_SPRITES;
}

static u8 EnemyInBeamLine(u8 dir, u8 maxTiles)
{
    struct ObjectEvent *p = Player(), *e = Enemy();
    s16 dx, dy, x = p->currentCoords.x, y = p->currentCoords.y;
    u8 i;
    Vec(dir, &dx, &dy);
    for (i = 1; i <= maxTiles; i++)
    {
        x += dx;
        y += dy;
        if (x == e->currentCoords.x && y == e->currentCoords.y)
            return i;
        if (MapGridGetCollisionAt(x, y))
            return FALSE;
    }
    return FALSE;
}

// a Kamehameha is about to fire: beam-capable opponents answer with their own beam
bool8 DBZ_FightTryBeamStruggle(void)
{
    u8 dir = GetPlayerFacingDirection();
    u8 i, dist;
    u16 chance;
    if (sFight.phase != FIGHT_ACTIVE || sFight.struggle || !(sEnemyKinds[sFight.kind].flags & KF_BEAM))
        return FALSE;
    // only on screen (the fight HUD covers the top rows)
    dist = EnemyInBeamLine(dir, dir == DIR_NORTH ? 3 : (dir == DIR_SOUTH ? 4 : 6));
    if (dist == 0)
        return FALSE;
    chance = (sFight.projSprite != MAX_SPRITES && sFight.projBeam) || sFight.aiState == AI_BEAM_WINDUP ? 100
           : (sEnemyKinds[sFight.kind].flags & KF_BOSS) ? 60 : 40;
    if ((Random() % 100) >= chance)
        return FALSE;
    if (dist == 1)
    {
        // point blank: the blast shoves them back a tile so both beams have room
        struct ObjectEvent *e = Enemy();
        s16 dx, dy, nx, ny;
        Vec(dir, &dx, &dy);
        nx = e->currentCoords.x + dx;
        ny = e->currentCoords.y + dy;
        if (!MapGridGetCollisionAt(nx, ny) && GetObjectEventIdByXY(nx, ny) == OBJECT_EVENTS_COUNT
         && nx >= sFight.arenaX0 && nx <= sFight.arenaX1 && ny >= sFight.arenaY0 && ny <= sFight.arenaY1)
        {
            ObjectEventClearHeldMovementIfActive(e);
            MoveObjectEventToMapCoords(e, nx - MAP_OFFSET, ny - MAP_OFFSET);
        }
    }
    DestroyProjectile();
    sFight.struggle = TRUE;
    sFight.struggleDir = dir;
    sFight.struggleBalance = 0;
    sFight.struggleTimer = 0;
    for (i = 0; i < STRUGGLE_SEGS; i++)
    {
        sFight.gokuSeg[i] = DBZ_CreateFxSpriteFor(0, -32, FALSE);
        sFight.enemySeg[i] = DBZ_CreateFxSpriteFor(0, -32, TRUE);
    }
    sFight.clashSprite = DBZ_CreateFxSpriteFor(0, -32, FALSE);
    ObjectEventTurn(Enemy(), DirTo(Enemy()->currentCoords.x, Enemy()->currentCoords.y, Player()->currentCoords.x, Player()->currentCoords.y));
    FlashObjectPalette(Enemy(), 0, RGB_WHITE);
    PlaySE(SE_M_HYPER_BEAM);
    Flash(4);
    return TRUE;
}

bool8 DBZ_IsBeamStruggling(void)
{
    return sFight.struggle;
}

static void PlaceSeg(u8 spriteId, s16 x, s16 y, bool8 vertical, bool8 visible)
{
    if (spriteId == MAX_SPRITES)
        return;
    gSprites[spriteId].x = x;
    gSprites[spriteId].y = y;
    gSprites[spriteId].invisible = !visible;
    DBZ_SetFxSpriteFrame(spriteId, (vertical ? DBZ_FX_BEAM_V0 : DBZ_FX_BEAM_H0) + ((sFight.struggleTimer / 2) % 2));
}

static void EndStruggle(bool8 gokuWon)
{
    s16 x, y;
    DestroyStruggleSprites();
    sFight.struggle = FALSE;
    if (gokuWon)
    {
        SpriteCenter(Enemy(), &x, &y);
        HitEnemy(HIT_KAME * 3 / 2, 50, sFight.struggleDir, 2, x, y);
        Flash(6);
    }
    else
    {
        sFight.guardFrames = 0;
        HitGoku(HIT_E_BEAM * 3 / 2, TRUE);
        Flash(6);
    }
    sFight.aiState = AI_RECOVER;
    sFight.aiTimer = 30;
}

static void UpdateStruggle(void)
{
    s16 gx, gy, ex, ey, mx, my, len, i, dx, dy;
    s32 push;
    bool8 vertical = (sFight.struggleDir == DIR_NORTH || sFight.struggleDir == DIR_SOUTH);
    u32 gpl = DBZ_CurrentPowerLevel(), epl = CurrentEnemyPL();

    sFight.struggleTimer++;
    // mash A to push; the opponent pushes back by power level
    if (JOY_NEW(A_BUTTON))
    {
        sFight.struggleBalance += 64;
        PlaySE(SE_M_SWIFT);
    }
    // about 8 presses a second holds an even opponent; a stronger one needs faster hands
    push = 8 * epl / (gpl ? gpl : 1);
    if (push < 4)
        push = 4;
    if (push > 14)
        push = 14;
    if (DBZ_OptDifficulty() == DBZ_DIFF_EASY)
        push = push * 2 / 3;
    else if (DBZ_OptDifficulty() == DBZ_DIFF_HARD)
        push = push * 5 / 4;
    sFight.struggleBalance -= push;
    if ((sFight.struggleTimer % 8) == 0)
        Shake(4);
    if (sFight.struggleTimer >= 600 && sFight.struggleBalance > -1600 && sFight.struggleBalance < 1600)
        sFight.struggleBalance = sFight.struggleBalance >= 0 ? 1600 : -1600;   // 10 seconds: whoever's ahead wins
    if (sFight.struggleBalance >= 1600 || sFight.struggleBalance <= -1600)
    {
        EndStruggle(sFight.struggleBalance > 0);
        return;
    }

    DBZ_GokuHandsPos(sFight.struggleDir, &gx, &gy);
    SpriteCenter(Enemy(), &ex, &ey);
    Vec(sFight.struggleDir, &dx, &dy);
    ex -= dx * 9;
    ey -= dy * 8;
    len = vertical ? abs(ey - gy) : abs(ex - gx);
    // meeting point: halfway, shifted by the balance
    mx = gx + dx * (len * (1600 + sFight.struggleBalance) / 3200);
    my = gy + dy * (len * (1600 + sFight.struggleBalance) / 3200);
    for (i = 0; i < STRUGGLE_SEGS; i++)
    {
        s16 dg = 8 + i * 16;
        s16 de = 8 + i * 16;
        s16 reachG = vertical ? abs(my - gy) : abs(mx - gx);
        s16 reachE = vertical ? abs(ey - my) : abs(ex - mx);
        PlaceSeg(sFight.gokuSeg[i], gx + dx * dg, gy + dy * dg, vertical, dg < reachG);
        PlaceSeg(sFight.enemySeg[i], ex - dx * de, ey - dy * de, vertical, de < reachE);
    }
    if (sFight.clashSprite != MAX_SPRITES)
    {
        gSprites[sFight.clashSprite].x = mx;
        gSprites[sFight.clashSprite].y = my;
        DBZ_SetFxSpriteFrame(sFight.clashSprite, DBZ_FX_IMPACT_0 + (sFight.struggleTimer / 3) % 2);
    }
}

// ------------------------------------------------------------------ enemy AI
static u8 WindupFrames(void)
{
    s16 f = (sFight.leader || sFight.kind == DBZ_ENEMY_VEGETA || (sEnemyKinds[sFight.kind].flags & KF_BOSS)) ? 10 : 14;
    if (sFight.enemySSJ)
        f -= 2;
    f -= sFight.bossPhase * 2;
    f += DiffWindup();
    return f < 6 ? 6 : f;
}

static void BossPhaseCheck(void)
{
    u8 phase;
    if (!(sEnemyKinds[sFight.kind].flags & KF_BOSS))
        return;
    phase = sFight.enemyHp * 3 > sFight.enemyMaxHp * 2 ? 0 : (sFight.enemyHp * 3 > sFight.enemyMaxHp ? 1 : 2);
    if (phase <= sFight.bossPhase)
        return;
    sFight.bossPhase = phase;
    PlaySE(SE_M_SWAGGER);
    Shake(20);
    Flash(5);
    DBZ_SpawnImpactAt(gSprites[Enemy()->spriteId].x, gSprites[Enemy()->spriteId].y);
    sFight.aiState = AI_RECOVER;
    sFight.aiTimer = 36;
    sFight.hudDirty = TRUE;
}

static void TryTeleport(void)
{
    struct ObjectEvent *e = Enemy(), *p = Player();
    static const s8 sOff[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    u8 i, start = Random() % 4;
    for (i = 0; i < 4; i++)
    {
        u8 k = (start + i) % 4;
        s16 x = p->currentCoords.x + sOff[k][0];
        s16 y = p->currentCoords.y + sOff[k][1];
        if (!MapGridGetCollisionAt(x, y) && GetObjectEventIdByXY(x, y) == OBJECT_EVENTS_COUNT
         && MapGridGetElevationAt(x, y) == p->currentElevation && !DBZ_FightBlocksTile(e, x, y))
        {
            ObjectEventClearHeldMovementIfActive(e);
            MoveObjectEventToMapCoords(e, x - MAP_OFFSET, y - MAP_OFFSET);
            ObjectEventTurn(e, DirTo(x, y, p->currentCoords.x, p->currentCoords.y));
            PlaySE(SE_M_TELEPORT);
            sFight.aiState = AI_WINDUP;
            sFight.aiTimer = WindupFrames() + 4;
            return;
        }
    }
}

static void EnemyAI(void)
{
    struct ObjectEvent *e = Enemy();
    struct ObjectEvent *p = Player();
    s16 ex = e->currentCoords.x, ey = e->currentCoords.y;
    s16 px = p->currentCoords.x, py = p->currentCoords.y;
    s16 dist = abs(px - ex) + abs(py - ey);
    u8 dir, kflags = sEnemyKinds[sFight.kind].flags;
    bool8 vegeta = (sFight.kind == DBZ_ENEMY_VEGETA);
    bool8 aligned = (ex == px || ey == py);

    if (sFight.aiTimer)
        sFight.aiTimer--;
    if (sFight.rangedCooldown)
        sFight.rangedCooldown--;
    if (sFight.beamCooldown)
        sFight.beamCooldown--;
    if (sFight.teleportTimer)
        sFight.teleportTimer--;

    // Vegeta goes Super Saiyan once hurt (from the 4th gym onward)
    if (vegeta && !sFight.enemySSJ && FlagGet(FLAG_BADGE04_GET) && sFight.enemyHp * 2 < sFight.enemyMaxHp)
    {
        sFight.enemySSJ = TRUE;
        sFight.enemyHp += sFight.enemyMaxHp / 5;
        PlaySE(SE_M_SWAGGER);
        Shake(16);
        DBZ_SpawnImpactAt(gSprites[e->spriteId].x, gSprites[e->spriteId].y);
        sFight.aiState = AI_RECOVER;
        sFight.aiTimer = 30;
        sFight.hudDirty = TRUE;
        RestoreEnemyPalette();
        return;
    }
    BossPhaseCheck();

    // Kid Buu regenerates and blinks around
    if ((kflags & KF_TELEPORT) && !(sFight.timer % 60) && sFight.enemyHp < sFight.enemyMaxHp && sFight.bossPhase < 2)
    {
        sFight.enemyHp += sFight.enemyMaxHp / 100 + 1;
        if (sFight.enemyHp > sFight.enemyMaxHp)
            sFight.enemyHp = sFight.enemyMaxHp;
        sFight.hudDirty = TRUE;
    }

    switch (sFight.aiState)
    {
    case AI_APPROACH:
        if (sFight.aiTimer || EnemyBusy())
            break;
        dir = DirTo(ex, ey, px, py);
        if ((kflags & KF_TELEPORT) && sFight.teleportTimer == 0 && dist >= 3)
        {
            sFight.teleportTimer = 150 - sFight.bossPhase * 40;
            TryTeleport();
            break;
        }
        if (dist == 1)
        {
            ObjectEventTurn(e, dir);
            sFight.aiState = AI_WINDUP;
            sFight.aiTimer = WindupFrames();
            PlaySE(SE_M_SWAGGER2);
            break;
        }
        // General Blue's paralysis stare
        if ((kflags & KF_PARALYZE) && sFight.bossPhase >= 1 && aligned && dist <= 4 && sFight.beamCooldown == 0)
        {
            ObjectEventTurn(e, dir);
            sFight.aiState = AI_STARE;
            sFight.aiTimer = 24;
            sFight.beamCooldown = 200;
            PlaySE(SE_M_PSYBEAM);
            break;
        }
        // beams: charge, then fire
        if ((kflags & KF_BEAM) && sFight.beamCooldown == 0 && aligned && dist >= 2 && dist <= 6
         && (vegeta ? FlagGet(FLAG_BADGE02_GET) : TRUE) && (Random() % 3) == 0)
        {
            ObjectEventTurn(e, dir);
            sFight.aiState = AI_BEAM_WINDUP;
            sFight.aiTimer = 30;
            sFight.beamCooldown = 220 - sFight.bossPhase * 50;
            PlaySE(SE_M_CHARGE);
            break;
        }
        if ((kflags & KF_RANGED) && sFight.rangedCooldown == 0 && aligned && dist <= 5 && (Random() % (vegeta ? 2 : 3)) == 0)
        {
            ObjectEventTurn(e, dir);
            FireProjectile(dir, FALSE);
            sFight.rangedCooldown = vegeta ? (sFight.enemySSJ ? 40 : 55) : (80 - sFight.bossPhase * 20);
            sFight.aiState = AI_RECOVER;
            sFight.aiTimer = 18;
            break;
        }
        if (!EnemyCanStep(dir))
        {
            // try the other axis, then anything
            u8 alt = (dir == DIR_EAST || dir == DIR_WEST) ? (py < ey ? DIR_NORTH : DIR_SOUTH) : (px < ex ? DIR_WEST : DIR_EAST);
            if (EnemyCanStep(alt))
                dir = alt;
            else
            {
                dir = 1 + Random() % 4;
                if (!EnemyCanStep(dir))
                {
                    sFight.aiTimer = 6;
                    break;
                }
            }
        }
        // close the last gap quickly
        if (dist <= 3 || vegeta || (kflags & KF_BOSS))
            ObjectEventSetHeldMovement(e, GetWalkFastMovementAction(dir));
        else
            ObjectEventSetHeldMovement(e, GetWalkNormalMovementAction(dir));
        sFight.aiTimer = sEnemyKinds[sFight.kind].stepFrames - (sFight.leader ? 3 : 0) - sFight.bossPhase * 2;
        break;
    case AI_WINDUP:
        FlashObjectPalette(e, (sFight.aiTimer % 4) < 2 ? 8 : 0, RGB_WHITE);
        if (sFight.aiTimer == 0)
        {
            RestoreEnemyPalette();
            ObjectEventSetHeldMovement(e, GetWalkInPlaceFastMovementAction(e->facingDirection));
            // the swing tracks Goku if he is still next to the enemy
            if (dist == 1)
            {
                ObjectEventTurn(e, DirTo(ex, ey, px, py));
                HitGoku(HIT_E_PUNCH + sFight.bossPhase * 15, FALSE);
            }
            else
            {
                PlaySE(SE_M_TAIL_WHIP);
            }
            sFight.aiState = AI_RECOVER;
            sFight.aiTimer = DBZ_OptDifficulty() == DBZ_DIFF_EASY ? 26 : (DBZ_OptDifficulty() == DBZ_DIFF_HARD ? 14 : 18);
        }
        break;
    case AI_BEAM_WINDUP:
        FlashObjectPalette(e, (sFight.aiTimer % 6) < 3 ? 10 : 2, RGB(24, 8, 31));
        if (sFight.aiTimer == 0)
        {
            RestoreEnemyPalette();
            FireProjectile(e->facingDirection, TRUE);
            sFight.aiState = AI_RECOVER;
            sFight.aiTimer = 24;
        }
        break;
    case AI_STARE:
        FlashObjectPalette(e, (sFight.aiTimer % 4) < 2 ? 12 : 0, RGB(8, 16, 31));
        if (sFight.aiTimer == 0)
        {
            RestoreEnemyPalette();
            // only catches Goku if he's still in the line of sight
            if ((ex == px || ey == py) && dist <= 4 && DirTo(ex, ey, px, py) == e->facingDirection && !sFight.gokuInvuln)
            {
                sFight.paralyzed = 50;
                PlaySE(SE_M_BIND);
            }
            sFight.aiState = AI_RECOVER;
            sFight.aiTimer = 10;
        }
        break;
    case AI_RECOVER:
        if (sFight.aiTimer == 0)
            sFight.aiState = AI_APPROACH;
        break;
    }
}

// ------------------------------------------------------------------ player input during a fight
static void TryKaioken(void)
{
    if (!DBZ_HasTechnique(DBZ_TECH_KAIOKEN) || sFight.kaioken || sFight.kaiokenCooldown || DBZ_IsFused())
    {
        PlaySE(SE_FAILURE);
        return;
    }
    sFight.kaioken = KAIOKEN_FRAMES;
    PlaySE(SE_M_SWAGGER);
    Shake(12);
    sFight.hudDirty = TRUE;
}

// returns 0: walk normally, 1: a script started (lock controls), 2: input used, stand still
u8 DBZ_HandleFightInput(struct FieldInput *input)
{
    if (sFight.phase != FIGHT_ACTIVE)
        return 2;            // intro/outro: Goku stands his ground
#ifdef DBZ_DEBUG
    // debug builds: START ends the fight in Goku's favour (hold B too: Goku loses)
    if (input->pressedStartButton)
    {
        if (gMain.heldKeys & B_BUTTON)
            sFight.gokuHp = 0;
        else
            sFight.enemyHp = 0;
        sFight.hudDirty = TRUE;
        return 2;
    }
#endif
    if (sFight.struggle || sFight.paralyzed || DBZ_IsFightBlastActive() || sFight.atkTimer)
        return 2;
    if (input->pressedSelectButton)
    {
        TryKaioken();
        return 2;
    }
    // hold B while standing still to guard
    if ((gMain.heldKeys & B_BUTTON) && !(gMain.heldKeys & DPAD_ANY) && gPlayerAvatar.tileTransitionState == T_NOT_MOVING
     && !DBZ_IsChargingBlast())
    {
        if (sFight.guardFrames == 0)
            StartGuard();
        return 2;
    }
    if (sFight.guardFrames)
        StopGuard();
    // L / R go through the normal power handler (charging, specials, transforming)
    if (DBZ_HandleFieldInput(input))
        return ScriptContext_IsEnabled() ? 1 : 2;
    if (DBZ_IsChargingBlast())
        return 0;
    if (input->pressedAButton && sFight.attackCooldown == 0 && gPlayerAvatar.tileTransitionState == T_NOT_MOVING)
    {
        sFight.atkDir = GetPlayerFacingDirection();
        sFight.atkStep = (sFight.comboWindow != 0) ? (sFight.atkStep + 1) % 3 : 0;
        sFight.atkTimer = ATK_FRAMES + (sFight.atkStep == 2 ? 4 : 0);
        sFight.attackCooldown = sFight.atkTimer + 2;
        sFight.comboWindow = 0;
        StartAtkSprite(sFight.atkDir, sFight.atkStep != 0);
        PlaySE(sFight.atkStep == 2 ? SE_M_SKY_UPPERCUT : SE_M_TAIL_WHIP);
        return 2;
    }
    return 0;    // normal walking / dashing
}

static void ResolveAttackHit(void)
{
    struct ObjectEvent *p = Player();
    s16 dx, dy, fx, fy;
    u8 objId;
    Vec(sFight.atkDir, &dx, &dy);
    objId = GetObjectEventIdByXY(p->currentCoords.x + dx, p->currentCoords.y + dy);
    if (objId == sFight.enemyObjId)
    {
        static const u16 sStrength[3] = { HIT_PUNCH, HIT_KICK, HIT_HEAVY };
        // spark right at the fist
        SpriteCenter(p, &fx, &fy);
        fx += dx * 14;
        fy += dy * 12 - (sFight.atkDir == DIR_NORTH ? 6 : 0);
        HitEnemy(sStrength[sFight.atkStep], CAP_NORMAL, sFight.atkDir, sFight.atkStep == 2 ? 2 : (sFight.atkStep == 1), fx, fy);
    }
}

static void UpdateAttack(void)
{
    u8 hitAt;
    if (sFight.attackCooldown)
        sFight.attackCooldown--;
    if (sFight.comboWindow)
        sFight.comboWindow--;
    if (sFight.atkTimer == 0)
        return;
    hitAt = ATK_FRAMES - ATK_HIT_AT;
    if (sFight.atkTimer == hitAt)
        ResolveAttackHit();
    UpdateAtkSprite();
    if (--sFight.atkTimer == 0)
    {
        DestroyAtkSprite();
        sFight.comboWindow = 24;
        if (sFight.atkStep == 2)
            sFight.comboWindow = 0;   // finisher ends the chain
    }
}

static void UpdateKaioken(void)
{
    if (sFight.kaiokenCooldown)
        sFight.kaiokenCooldown--;
    if (!sFight.kaioken)
        return;
    sFight.kaioken--;
    // red aura, and it strains the body
    if ((sFight.kaioken % 6) == 0)
        DBZ_SpawnAuraSpark(TRUE);
    if ((sFight.kaioken % 60) == 0 && sFight.gokuHp > sFight.gokuMaxHp / 10)
    {
        sFight.gokuHp -= sFight.gokuMaxHp / 100 + 1;
        sFight.hudDirty = TRUE;
    }
    if (sFight.kaioken == 0)
    {
        sFight.kaiokenCooldown = KAIOKEN_COOLDOWN;
        sFight.hudDirty = TRUE;
    }
}

// ------------------------------------------------------------------ main task
static void RestoreFollowerAfterFight(void);

static void EndFight(u8 taskId)
{
    RestoreFollowerAfterFight();
    struct ObjectEvent *e = Enemy();
    DestroyProjectile();
    DestroyStruggleSprites();
    sFight.struggle = FALSE;
    sFight.atkTimer = 0;
    sFight.guardFrames = 0;
    sFight.kaioken = 0;
    sFight.paralyzed = 0;
    DestroyAtkSprite();
    FreeSpritePaletteByTag(TAG_DBZ_ATK_PAL);
    sFight.enemySSJ = FALSE;
    sFight.bossPhase = 0;
    if (sFight.shake)
    {
        sFight.shake = 0;
        SetCameraPanning(0, 0);
        InstallCameraPanAheadCallback();
    }
    DBZ_BlendPalettes(PALETTES_ALL, 0, RGB_WHITE);
    FlashObjectPalette(e, 0, RGB_WHITE);
    if (sChamberHidden)
    {
        u8 i;
        for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
            if (sChamberHidden & (1 << i))
                gObjectEvents[i].invisible = FALSE;
        sChamberHidden = 0;
    }
    Player()->invisible = FALSE;
    gSprites[gPlayerAvatar.spriteId].x2 = 0;
    gSprites[gPlayerAvatar.spriteId].y2 = 0;
    DestroyHud();
    if (sFight.spawned)
    {
        RemoveObjectEventByLocalIdAndMap(sFight.enemyLocalId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    }
    else
    {
        e->range.rangeX = sFight.savedRangeX;
        e->range.rangeY = sFight.savedRangeY;
        e->invisible = FALSE;
        ObjectEventTurn(e, DirTo(e->currentCoords.x, e->currentCoords.y, Player()->currentCoords.x, Player()->currentCoords.y));
    }
    // Goku keeps his wounds (the fusion's HP is its own)
    if (!(sFight.flags & FIGHT_FLAG_FUSION))
        DBZ_SetGokuHp(sFight.gokuHp);
    if (sFight.result == DBZ_FIGHT_WON)
    {
        if (VarGet(VAR_DBZ_FIGHTS_WON) < 9999)
            VarSet(VAR_DBZ_FIGHTS_WON, VarGet(VAR_DBZ_FIGHTS_WON) + 1);
    }
    else if (!(sFight.flags & FIGHT_FLAG_SCRIPTED) && VarGet(VAR_DBZ_FIGHTS_LOST) < 9999)
    {
        VarSet(VAR_DBZ_FIGHTS_LOST, VarGet(VAR_DBZ_FIGHTS_LOST) + 1);
    }
    sFight.phase = FIGHT_NONE;
    gSpecialVar_Result = sFight.result;
    gSpecialVar_0x8009 = sFight.waitScript;
    StringCopy(gStringVar1, sFight.name);
    DestroyTask(taskId);
    Overworld_ChangeMusicToDefault();
    if (sFight.waitScript)
    {
        FreezeObjectEvents();
        ScriptContext_Enable();
    }
    else
    {
        UnfreezeObjectEvents();
        ScriptContext_SetupScript(EventScript_DBZ_FightEnd);
    }
}

static void Task_Fight(u8 taskId)
{
    bool8 scriptRunning = ScriptContext_IsEnabled() || ArePlayerFieldControlsLocked();

    switch (sFight.phase)
    {
    case FIGHT_INTRO:
        if (sFight.timer == 0)
        {
            PlaySE(SE_M_MEGA_KICK);
            CreateHud();
            PlayBGM(sEnemyKinds[sFight.kind].music);
        }
        DBZ_BlendPalettes(PALETTES_BG, (sFight.timer < 8) ? (8 - sFight.timer) : 0, RGB_WHITE);
        if (++sFight.timer >= 30)
        {
            sFight.phase = FIGHT_ACTIVE;
            sFight.timer = 0;
            sFight.aiTimer = 16;
        }
        break;
    case FIGHT_ACTIVE:
        sFight.timer++;
        UpdateAttack();
        UpdateGuard();
        UpdateKaioken();
        UpdateShakeAndFlash();
        if ((sFight.flags & FIGHT_FLAG_CHAMBER) && !sFight.flash && (sFight.timer % 8) == 1)
            DBZ_BlendPalettes(PALETTES_BG, 11, RGB_WHITE);   // the endless white void
        if (sFight.paralyzed)
        {
            sFight.paralyzed--;
            FlashObjectPalette(Player(), (sFight.paralyzed % 4) < 2 ? 8 : 0, RGB(8, 16, 31));
            if (sFight.paralyzed == 0)
                FlashObjectPalette(Player(), 0, RGB_WHITE);
        }
        if (sFight.gokuInvuln)
        {
            sFight.gokuInvuln--;
            if (!sFight.atkTimer && !sFight.guardFrames)
                Player()->invisible = (sFight.gokuInvuln % 4) >= 2;
            if (sFight.gokuInvuln == 0 && !sFight.atkTimer && !sFight.guardFrames)
                Player()->invisible = FALSE;
        }
        if (sFight.enemyFlash)
        {
            sFight.enemyFlash--;
            if (sFight.enemyFlash == 0)
                RestoreEnemyPalette();
            else
                FlashObjectPalette(Enemy(), (sFight.enemyFlash % 4) < 2 ? 12 : 0, RGB(31, 8, 8));
        }
        if (!scriptRunning)
        {
            FreezeObjectEventsExceptTwo(gPlayerAvatar.objectEventId, sFight.enemyObjId);
            if (sFight.struggle)
                UpdateStruggle();
            else if (sFight.hitStop)
                sFight.hitStop--;   // a short freeze on impact sells the hit
            else
                EnemyAI();
            UpdateProjectile();
        }
        if (sFight.hudDirty)
            DrawHud();
        if ((sFight.enemyHp == 0 || sFight.gokuHp == 0) && !DBZ_IsFightBlastActive() && !scriptRunning && !sFight.struggle
         && gPlayerAvatar.tileTransitionState == T_NOT_MOVING && sFight.atkTimer == 0)
        {
            sFight.result = sFight.enemyHp == 0 ? DBZ_FIGHT_WON : DBZ_FIGHT_LOST;
            sFight.phase = FIGHT_OUTRO;
            sFight.timer = 0;
            if (sFight.guardFrames)
                StopGuard();
            PlaySE(sFight.result == DBZ_FIGHT_WON ? SE_M_EXPLOSION : SE_FAINT);
            if (sFight.result == DBZ_FIGHT_WON)
            {
                Shake(16);
                Flash(6);
            }
            Player()->invisible = FALSE;
        }
        break;
    case FIGHT_OUTRO:
        UpdateShakeAndFlash();
        if (sFight.result == DBZ_FIGHT_WON)
            Enemy()->invisible = (sFight.timer % 4) >= 2;
        else
            Player()->invisible = (sFight.timer % 4) >= 2;
        if (++sFight.timer >= 48)
        {
            Enemy()->invisible = !sFight.spawned ? FALSE : TRUE;
            EndFight(taskId);
        }
        break;
    }
}


static u8 TrainerTopLevel(u16 trainerId)
{
    const struct Trainer *t = GetTrainerStructFromId(trainerId);
    u8 i, best = 5;
    for (i = 0; i < t->partySize; i++)
    {
        if (t->party[i].lvl > best)
            best = t->party[i].lvl;
    }
    return best;
}

static bool8 FindSpawnTile(s16 *outX, s16 *outY)
{
    struct ObjectEvent *p = Player();
    static const s8 sOffsets[][2] = {
        {0, -3}, {0, 3}, {-3, 0}, {3, 0}, {2, -2}, {-2, -2}, {2, 2}, {-2, 2}, {0, -2}, {0, 2}, {-2, 0}, {2, 0},
    };
    u8 i, start = Random() % ARRAY_COUNT(sOffsets);
    for (i = 0; i < ARRAY_COUNT(sOffsets); i++)
    {
        u8 k = (start + i) % ARRAY_COUNT(sOffsets);
        s16 x = p->currentCoords.x + sOffsets[k][0];
        s16 y = p->currentCoords.y + sOffsets[k][1];
        if (!MapGridGetCollisionAt(x, y) && GetObjectEventIdByXY(x, y) == OBJECT_EVENTS_COUNT
         && MapGridGetElevationAt(x, y) == p->currentElevation
         && !MetatileBehavior_IsWarpDoor(MapGridGetMetatileBehaviorAt(x, y))
         && !MetatileBehavior_IsSurfableWaterOrUnderwater(MapGridGetMetatileBehaviorAt(x, y)))
        {
            *outX = x;
            *outY = y;
            return TRUE;
        }
    }
    return FALSE;
}

// special: flags for the next fight. VAR_0x8008 = FIGHT_FLAG_* | fighter name index << 4
static EWRAM_DATA u16 sNextFightFlags = 0;
void DBZ_SetNextFightFlags(void)
{
    sNextFightFlags = (PENDING_MAGIC << 8) | (gSpecialVar_0x8008 & 0xFF);
}

// special: the Potara earrings come off (the fusion ends after the finale)
void DBZ_EndFusion(void)
{
    sFused = 0;
    ObjectEventSetGraphicsId(Player(), DBZ_GetPlayerNormalGfx());
}

// VAR_0x8004 = enemy kind, VAR_0x8005 = level, VAR_0x8006 = local id of the opponent object (0: spawn one)
// The follower steps back into its Poke Ball for a personal fight (so it can't block knockback, spawns or
// the arena) and comes back out afterwards. Only undo what we did: a cutscene may have hidden it already.
static EWRAM_DATA bool8 sFollowerHiddenForFight = FALSE;

static void HideFollowerForFight(void)
{
    sFollowerHiddenForFight = FALSE;
    if (OW_FOLLOWERS_ENABLED && !FlagGet(FLAG_TEMP_HIDE_FOLLOWER))
    {
        FlagSet(FLAG_TEMP_HIDE_FOLLOWER);
        RemoveFollowingPokemon();
        sFollowerHiddenForFight = TRUE;
    }
}

static void RestoreFollowerAfterFight(void)
{
    if (sFollowerHiddenForFight)
    {
        FlagClear(FLAG_TEMP_HIDE_FOLLOWER);
        UpdateFollowingPokemon();
        sFollowerHiddenForFight = FALSE;
    }
}

static bool8 StartFightInternal(bool8 waitScript)
{
    const struct DbzEnemyKind *k;
    struct ObjectEvent *p = Player();
    u16 hpBase;
    u8 fighter;

    sFight.kind = gSpecialVar_0x8004 < ARRAY_COUNT(sEnemyKinds) ? gSpecialVar_0x8004 : DBZ_ENEMY_SAIYAN_SOLDIER;
    k = &sEnemyKinds[sFight.kind];
    sFight.level = gSpecialVar_0x8005 ? gSpecialVar_0x8005 : 5;
    sFight.projSprite = MAX_SPRITES;
    sFight.atkSprite = MAX_SPRITES;
    sFight.hudWin = WINDOW_NONE;
    sFight.spawned = FALSE;
    sFight.waitScript = waitScript;
    sFight.enemySSJ = FALSE;
    sFight.atkTimer = 0;
    sFight.comboWindow = 0;
    sFight.attackCooldown = 0;
    sFight.hitStop = 0;
    sFight.gokuInvuln = 0;
    sFight.enemyFlash = 0;
    sFight.rangedCooldown = 50;
    sFight.aiState = AI_APPROACH;
    sFight.result = 0;
    sFight.clashSprite = MAX_SPRITES;
    {
        u8 i;
        for (i = 0; i < STRUGGLE_SEGS; i++)
        {
            sFight.gokuSeg[i] = MAX_SPRITES;
            sFight.enemySeg[i] = MAX_SPRITES;
        }
    }
    sFight.struggle = FALSE;
    sFight.bossPhase = 0;
    sFight.guardFrames = 0;
    sFight.kaioken = 0;
    sFight.kaiokenCooldown = 0;
    sFight.paralyzed = 0;
    sFight.counterReady = 0;
    sFight.shake = 0;
    sFight.flash = 0;
    sFight.atkStep = 0;
    sFight.projBeam = FALSE;
    sFight.beamCooldown = 160;
    sFight.teleportTimer = 200;
    sFight.flags = 0;
    fighter = 0;
    if ((sNextFightFlags >> 8) == PENDING_MAGIC)
    {
        sFight.flags = sNextFightFlags & 0x0F;
        fighter = (sNextFightFlags >> 4) & 3;
    }
    sNextFightFlags = 0;

    if (StoredGokuHp() == 0 && !(sFight.flags & FIGHT_FLAG_FUSION))
    {
        sFight.phase = FIGHT_NONE;
        return FALSE;
    }
    HideFollowerForFight();

    if (gSpecialVar_0x8006 != 0)
    {
        sFight.enemyLocalId = gSpecialVar_0x8006;
        sFight.enemyObjId = GetObjectEventIdByLocalIdAndMap(sFight.enemyLocalId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    }
    else
    {
        s16 x, y;
        sFight.enemyObjId = OBJECT_EVENTS_COUNT;
        if (FindSpawnTile(&x, &y))
        {
            sFight.enemyLocalId = LOCALID_DBZ_ENEMY;
            sFight.enemyObjId = SpawnSpecialObjectEventParameterized(k->graphicsId, MOVEMENT_TYPE_FACE_DOWN, LOCALID_DBZ_ENEMY,
                                                                     x, y, p->currentElevation);   // coords already include MAP_OFFSET
            sFight.spawned = TRUE;
        }
    }
    if (sFight.enemyObjId >= OBJECT_EVENTS_COUNT)
    {
        // nowhere to fight: call it off quietly
        sFight.phase = FIGHT_NONE;
        return FALSE;
    }

    if (sFight.kind == DBZ_ENEMY_FIGHTER)
        StringCopy(sFight.name, sFighterNames[fighter]);
    else if (k->name != NULL)
        StringCopy(sFight.name, k->name);
    else if (TRAINER_BATTLE_PARAM.opponentA != 0)
    {
        StringCopyN(sFight.name, GetTrainerStructFromId(TRAINER_BATTLE_PARAM.opponentA)->trainerName, TRAINER_NAME_LENGTH);
        sFight.name[TRAINER_NAME_LENGTH] = EOS;
    }
    else
        StringCopy(sFight.name, sText_RedRibbon);

    sFight.leader = (sFight.kind == DBZ_ENEMY_TRAINER && TRAINER_BATTLE_PARAM.opponentA != 0
                  && GetTrainerStructFromId(TRAINER_BATTLE_PARAM.opponentA)->trainerClass == TRAINER_CLASS_LEADER);
    hpBase = 20 + 7 * sFight.level;
    sFight.enemyMaxHp = (u32)hpBase * k->hpPercent / 100 * (sFight.leader ? 13 : 10) / 10 * DiffEnemyHp() / 100;
    sFight.enemyHp = sFight.enemyMaxHp;
    sFight.enemyPL = BasePowerLevel(sFight.level) * k->powerPercent / 100 * (sFight.leader ? 110 : 100) / 100;
    sFight.expPool = (u32)sFight.level * k->expPerLevel * (sFight.leader ? 2 : 1);
    if (sFight.flags & FIGHT_FLAG_CHAMBER)
        sFight.expPool *= 3;
    if (sFight.flags & FIGHT_FLAG_FUSION)
    {
        sFused = PENDING_MAGIC;
        sFight.gokuMaxHp = DBZ_GokuMaxHp() * 2;
        sFight.gokuHp = sFight.gokuMaxHp;
    }
    else
    {
        sFight.gokuMaxHp = DBZ_GokuMaxHp();
        sFight.gokuHp = StoredGokuHp();
    }

    sFight.savedRangeX = Enemy()->range.rangeX;
    sFight.savedRangeY = Enemy()->range.rangeY;
    Enemy()->range.rangeX = 0;
    Enemy()->range.rangeY = 0;
    ObjectEventClearHeldMovementIfActive(Enemy());

    sFight.startPX = p->currentCoords.x;
    sFight.startPY = p->currentCoords.y;
    sFight.startEX = Enemy()->currentCoords.x;
    sFight.startEY = Enemy()->currentCoords.y;
    sFight.arenaX0 = p->currentCoords.x - ARENA_HALF_W;
    sFight.arenaX1 = p->currentCoords.x + ARENA_HALF_W;
    sFight.arenaY0 = p->currentCoords.y - ARENA_HALF_H;
    sFight.arenaY1 = p->currentCoords.y + ARENA_HALF_H;

    sChamberHidden = 0;
    if (sFight.flags & FIGHT_FLAG_CHAMBER)
    {
        u8 i;
        for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        {
            if (gObjectEvents[i].active && !gObjectEvents[i].invisible && i != gPlayerAvatar.objectEventId && i != sFight.enemyObjId)
            {
                gObjectEvents[i].invisible = TRUE;
                sChamberHidden |= 1 << i;
            }
        }
    }
    sFight.phase = FIGHT_INTRO;
    sFight.timer = 0;
    DBZ_ResetFieldInputState();
    CreateTask(Task_Fight, 70);
    return TRUE;
}

void DBZ_StartFight(void)
{
    StartFightInternal(FALSE);
}

// For fights inside a running event script (rival VEGETA): returns TRUE if the fight started, then the
// script does `waitstate` and continues with VAR_RESULT = fight result once it's over.
void DBZ_StartFightAndWait(void)
{
    if (StartFightInternal(TRUE))
    {
        UnfreezeObjectEvents();
        UnlockPlayerFieldControls();
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

// ------------------------------------------------------------------ trainers / ambushes / gym leaders / Vegeta
u16 DBZ_CanFightTrainer(void)
{
    if (!DBZ_GokuCanFight())
        return FALSE;
    if (TRAINER_BATTLE_PARAM.opponentA == 0)
        return FALSE;
    return GetObjectEventIdByLocalIdAndMap(gSpecialVar_LastTalked, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup) != OBJECT_EVENTS_COUNT;
}

void DBZ_SetupTrainerFight(void)
{
    gSpecialVar_0x8004 = DBZ_ENEMY_TRAINER;
    gSpecialVar_0x8005 = TrainerTopLevel(TRAINER_BATTLE_PARAM.opponentA);
    gSpecialVar_0x8006 = gSpecialVar_LastTalked;
}

static bool8 IsRivalGfx(u8 gfx)
{
    return (gfx >= OBJ_EVENT_GFX_MAY_NORMAL && gfx <= OBJ_EVENT_GFX_MAY_FIELD_MOVE)
        || (gfx >= OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL && gfx <= OBJ_EVENT_GFX_RIVAL_MAY_FIELD_MOVE);
}

// after a rival battle: VAR_0x8004..6 for a fight with Vegeta (the rival object next to Goku, if any)
// VAR_0x8007 = stage (badge count) for his lines
void DBZ_SetupVegetaFight(void)
{
    u8 i, best = OBJECT_EVENTS_COUNT;
    s16 bestDist = 99;
    struct ObjectEvent *p = Player();
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *o = &gObjectEvents[i];
        s16 d;
        if (!o->active || o->invisible || i == gPlayerAvatar.objectEventId || !IsRivalGfx(o->graphicsId))
            continue;
        d = abs(o->currentCoords.x - p->currentCoords.x) + abs(o->currentCoords.y - p->currentCoords.y);
        if (d < bestDist)
        {
            bestDist = d;
            best = i;
        }
    }
    gSpecialVar_0x8004 = DBZ_ENEMY_VEGETA;
    gSpecialVar_0x8005 = (TRAINER_BATTLE_PARAM.opponentA != 0 ? TrainerTopLevel(TRAINER_BATTLE_PARAM.opponentA) : DBZ_GokuLevel()) + 3;
    if (gSpecialVar_0x8005 < DBZ_GokuLevel())
        gSpecialVar_0x8005 = DBZ_GokuLevel();
    gSpecialVar_0x8006 = (best != OBJECT_EVENTS_COUNT && bestDist <= 6) ? gObjectEvents[best].localId : 0;
    gSpecialVar_0x8007 = 0;
    for (i = 0; i < 8; i++)
        if (FlagGet(FLAG_BADGE01_GET + i))
            gSpecialVar_0x8007++;
    gSpecialVar_Result = DBZ_GokuCanFight();
}

// ------------------------------------------------------------------ bosses and the finale
// story scenes: put the opponent back where the cutscene had him, facing Goku (call behind a fade)
void DBZ_RestoreFightPositions(void)
{
    struct ObjectEvent *p = Player();
    if (!sFight.spawned && sFight.enemyObjId < OBJECT_EVENTS_COUNT && gObjectEvents[sFight.enemyObjId].active
     && (gObjectEvents[sFight.enemyObjId].currentCoords.x != sFight.startEX || gObjectEvents[sFight.enemyObjId].currentCoords.y != sFight.startEY))
    {
        struct ObjectEvent *e = &gObjectEvents[sFight.enemyObjId];
        ObjectEventClearHeldMovementIfActive(e);
        MoveObjectEventToMapCoords(e, sFight.startEX - MAP_OFFSET, sFight.startEY - MAP_OFFSET);
    }
    if (!sFight.spawned && sFight.enemyObjId < OBJECT_EVENTS_COUNT)
    {
        struct ObjectEvent *e = &gObjectEvents[sFight.enemyObjId];
        ObjectEventTurn(e, DirTo(e->currentCoords.x, e->currentCoords.y, p->currentCoords.x, p->currentCoords.y));
        ObjectEventTurn(p, DirTo(p->currentCoords.x, p->currentCoords.y, e->currentCoords.x, e->currentCoords.y));
    }
}
#define LOCALID_DBZ_ALLY 239

static u8 PartyTopLevel(void)
{
    u8 i, best = 1;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
        {
            u8 l = GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL);
            if (l > best)
                best = l;
        }
    }
    return best;
}

static u8 FindNearestWithGfx(u8 gfx, u8 maxDist)
{
    u8 i, best = OBJECT_EVENTS_COUNT;
    s16 bestDist = 99;
    struct ObjectEvent *p = Player();
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *o = &gObjectEvents[i];
        s16 d;
        if (!o->active || o->invisible || i == gPlayerAvatar.objectEventId || o->graphicsId != gfx)
            continue;
        if (o->localId == gSpecialVar_LastTalked)
            return i;   // the one Goku is talking to
        d = abs(o->currentCoords.x - p->currentCoords.x) + abs(o->currentCoords.y - p->currentCoords.y);
        if (d < bestDist)
        {
            bestDist = d;
            best = i;
        }
    }
    return bestDist <= maxDist ? best : OBJECT_EVENTS_COUNT;
}

// VAR_0x800A = boss kind -> VAR_0x8004..6 for DBZ_StartFightAndWait; VAR_RESULT = the fight can happen
void DBZ_SetupBossFight(void)
{
    u8 kind = gSpecialVar_0x800A, obj, level, add;
    if (kind >= ARRAY_COUNT(sEnemyKinds))
        kind = DBZ_ENEMY_KID_BUU;
    obj = FindNearestWithGfx(sEnemyKinds[kind].graphicsId, 8);
    // pitched at Goku, or a little under the team if Goku has fallen behind
    level = DBZ_GokuLevel();
    if (PartyTopLevel() > level + 4)
        level = PartyTopLevel() - 4;
    add = kind == DBZ_ENEMY_GENERAL_BLUE ? 1 : (kind == DBZ_ENEMY_DR_GERO ? 2 : 3);
    level = level + add > 100 ? 100 : level + add;
    gSpecialVar_0x8004 = kind;
    gSpecialVar_0x8005 = level;
    gSpecialVar_0x8006 = obj != OBJECT_EVENTS_COUNT ? gObjectEvents[obj].localId : 0;
    gSpecialVar_Result = TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT)
                      && (((sNextFightFlags >> 8) == PENDING_MAGIC && (sNextFightFlags & FIGHT_FLAG_FUSION)) || !DBZ_GokuIsKO());
}

// trainers whose rematch is a boss fight instead of an ordinary fistfight (until the boss is beaten)
u16 DBZ_TrainerIsPendingBoss(void)
{
    if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_TABITHA_MT_CHIMNEY && !FlagGet(FLAG_DBZ_BOSS_BLUE))
        return TRUE;
    return FALSE;
}

// the finale: Vegeta flies in next to Goku
void DBZ_SpawnVegetaAlly(void)
{
    s16 x, y;
    struct ObjectEvent *p = Player();
    if (!FindSpawnTile(&x, &y))
    {
        x = p->currentCoords.x;
        y = p->currentCoords.y + 1;
    }
    SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_RIVAL_MAY_NORMAL, MOVEMENT_TYPE_FACE_DOWN, LOCALID_DBZ_ALLY,
                                         x, y, p->currentElevation);
    x = GetObjectEventIdByLocalIdAndMap(LOCALID_DBZ_ALLY, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    if (x < OBJECT_EVENTS_COUNT)
    {
        ObjectEventTurn(&gObjectEvents[x], DirTo(gObjectEvents[x].currentCoords.x, gObjectEvents[x].currentCoords.y, p->currentCoords.x, p->currentCoords.y));
        ObjectEventTurn(p, DirTo(p->currentCoords.x, p->currentCoords.y, gObjectEvents[x].currentCoords.x, gObjectEvents[x].currentCoords.y));
    }
}

// after a white-out: make sure the player sprite's palette matches its graphics (fades restore old colours)
void DBZ_RefreshPlayerGfx(void)
{
    ObjectEventSetGraphicsId(Player(), DBZ_GetPlayerNormalGfx());
    ObjectEventTurn(Player(), Player()->facingDirection);
}

// Potara: Vegeta and Goku become VEGITO (call it while the screen is white)
void DBZ_StartFusion(void)
{
    sFused = PENDING_MAGIC;
    RemoveObjectEventByLocalIdAndMap(LOCALID_DBZ_ALLY, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    ObjectEventSetGraphicsId(Player(), DBZ_GetPlayerNormalGfx());
}

// gym scripts call this right after the badge; the fight starts once the script lets Goku go
void DBZ_QueueGymFight(void)
{
    VarSet(VAR_DBZ_PENDING_FIGHT, gSpecialVar_LastTalked + 1);
}

void DBZ_PrepareAmbush(void)
{
    s16 lvl = DBZ_GokuLevel() - 2 + (Random() % 4);
    if (lvl < 2)
        lvl = 2;
    if (lvl > 100)
        lvl = 100;
    gSpecialVar_0x8004 = (Random() % 3) ? DBZ_ENEMY_SAIYAN_SOLDIER : DBZ_ENEMY_MAJIN_SOLDIER;
    gSpecialVar_0x8005 = lvl;
    gSpecialVar_0x8006 = 0;
    TRAINER_BATTLE_PARAM.opponentA = 0;
    StringCopy(gStringVar1, sEnemyKinds[gSpecialVar_0x8004].name);
}

#define AMBUSH_MIN_STEPS 120

bool8 DBZ_TryAmbush(void)
{
    static const u16 sChance[4] = {0, 600, 300, 140};
    u8 freq = DBZ_OptAmbush();
    if (sAmbushSteps < 0xFFFF)
        sAmbushSteps++;
    if (freq == 0 || DBZ_IsFighting() || !FlagGet(FLAG_SYS_POKEMON_GET) || StoredGokuHp() == 0)
        return FALSE;
    if (!IsMapTypeOutdoors(gMapHeader.mapType) || !TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT))
        return FALSE;
    if (VarGet(VAR_REPEL_STEP_COUNT) != 0)   // a KI HIDER keeps the soldiers off Goku's trail too
        return FALSE;
    if (sAmbushSteps < AMBUSH_MIN_STEPS || (Random() % sChance[freq]) != 0)
        return FALSE;
    sAmbushSteps = 0;
    ScriptContext_SetupScript(EventScript_DBZ_Ambush);
    return TRUE;
}

// ------------------------------------------------------------------ knocked out
// Goku comes to at the last place he rested with half his HP (scripts fade out first, then waitstate)
void DBZ_WarpToLastHeal(void)
{
    DBZ_SetGokuHp(DBZ_GokuMaxHp() / 2);
    sKoNotice = PENDING_MAGIC;
    SetWarpDestinationToLastHealLocation();
    DoWarp();
    ResetInitialPlayerAvatarState();
}

// story fights can't send Goku away mid-scene: he gets back up with 1 HP
void DBZ_ReviveGokuWeak(void)
{
    if (StoredGokuHp() == 0)
        DBZ_SetGokuHp(1);
}

void DBZ_QueueFightEvolveCheck(void)
{
    sEvolvePending = PENDING_MAGIC;
    sEvolveCheck = 0;
}

// things that should pop up once Goku is free to move again
bool8 DBZ_TryStartPendingFieldEvent(void)
{
    u16 pending = VarGet(VAR_DBZ_PENDING_FIGHT);
    if (DBZ_IsFighting())
        return FALSE;
    if (DBZ_TryStartPendingInstantTransmission())
        return TRUE;
    if (sKoNotice == PENDING_MAGIC)
    {
        sKoNotice = 0;
        ScriptContext_SetupScript(EventScript_DBZ_WokeUp);
        return TRUE;
    }
    if (sEvolvePending == PENDING_MAGIC)
    {
        sEvolvePending = 0;
        ScriptContext_SetupScript(EventScript_DBZ_EvolveCheck);
        return TRUE;
    }
    if (pending != 0)
    {
        VarSet(VAR_DBZ_PENDING_FIGHT, 0);
        gSpecialVar_LastTalked = pending - 1;
        ScriptContext_SetupScript(EventScript_DBZ_GymFight);
        return TRUE;
    }
    if (VarGet(VAR_DBZ_GOKU_ANNOUNCED) != 0 && VarGet(VAR_DBZ_GOKU_ANNOUNCED) != DBZ_GokuLevel())
    {
        VarSet(VAR_DBZ_GOKU_ANNOUNCED, DBZ_GokuLevel());
        ConvertIntToDecimalStringN(gStringVar1, DBZ_GokuLevel(), STR_CONV_MODE_LEFT_ALIGN, 3);
        ScriptContext_SetupScript(EventScript_DBZ_LevelUpNotice);
        return TRUE;
    }
    return FALSE;
}

// ------------------------------------------------------------------ results
static void AddReport(u8 type, u8 partyId, u8 level, u16 move)
{
    if (sReportCount >= MAX_REPORTS)
        return;
    sReports[sReportCount].type = type;
    sReports[sReportCount].partyId = partyId;
    sReports[sReportCount].level = level;
    sReports[sReportCount].move = move;
    sReportCount++;
}

static void GiveMonFightExp(u8 partyId, u32 amount)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partyId];
    u16 species = GetMonData(mon, MON_DATA_SPECIES);
    u8 level = GetMonData(mon, MON_DATA_LEVEL);
    u32 exp, maxExp;

    if (species == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG) || level >= MAX_LEVEL)
        return;
    exp = GetMonData(mon, MON_DATA_EXP) + amount;
    maxExp = gExperienceTables[gSpeciesInfo[species].growthRate][MAX_LEVEL];
    if (exp > maxExp)
        exp = maxExp;
    SetMonData(mon, MON_DATA_EXP, &exp);
    while (level < MAX_LEVEL && exp >= gExperienceTables[gSpeciesInfo[species].growthRate][level + 1])
    {
        u16 move;
        bool8 first = TRUE;
        level++;
        SetMonData(mon, MON_DATA_LEVEL, &level);
        CalculateMonStats(mon);
        AddReport(DBZ_REPORT_LEVEL, partyId, level, 0);
        while ((move = MonTryLearningNewMove(mon, first)) != MOVE_NONE)
        {
            first = FALSE;
            if (move == MON_HAS_MAX_MOVES)
                AddReport(DBZ_REPORT_NO_ROOM, partyId, level, gMoveToLearn);
            else if (move != MON_ALREADY_KNOWS_MOVE)
                AddReport(DBZ_REPORT_LEARNED, partyId, level, move);
        }
    }
}

// STR_VAR_1 = Goku EXP, STR_VAR_2 = EXP per Pokemon, STR_VAR_3 = Goku level; VAR_RESULT = Goku levels gained
void DBZ_FightAwardExp(void)
{
    u32 goku, mons;
    u8 i, gained;
    u16 hpBefore = StoredGokuHp();
    u16 maxBefore = DBZ_GokuMaxHp();

    if (DBZ_GravityStepsLeft() != 0)
        sFight.expPool *= 2;   // Capsule Corp. gravity training
    goku = sFight.expPool;
    mons = sFight.expPool * 2 / 5;   // overworld fights lean toward Goku
    sReportCount = 0;
    sReportPos = 0;
    sEvolveCheck = 0;
    if (mons == 0)
        mons = 1;
    gained = GokuGainExp(goku);
    // a level-up brings the new max HP with it (and keeps the same wounds)
    if (gained)
        DBZ_SetGokuHp(hpBefore + (DBZ_GokuMaxHp() - maxBefore));
    VarSet(VAR_DBZ_GOKU_ANNOUNCED, DBZ_GokuLevel());
    for (i = 0; i < PARTY_SIZE; i++)
        GiveMonFightExp(i, mons);
    ConvertIntToDecimalStringN(gStringVar1, goku, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, mons, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, DBZ_GokuLevel(), STR_CONV_MODE_LEFT_ALIGN, 3);
    gSpecialVar_Result = gained;
}

// fills STR_VAR_1 (mon nickname), STR_VAR_2 (level or move); returns report type
u16 DBZ_NextFightReport(void)
{
    u8 type;
    if (sReportPos >= sReportCount)
        return DBZ_REPORT_NONE;
    type = sReports[sReportPos].type;
    GetMonData(&gParties[B_TRAINER_PLAYER][sReports[sReportPos].partyId], MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
    if (type == DBZ_REPORT_LEVEL)
        ConvertIntToDecimalStringN(gStringVar2, sReports[sReportPos].level, STR_CONV_MODE_LEFT_ALIGN, 3);
    else
        StringCopy(gStringVar2, GetMoveName(sReports[sReportPos].move));
    sReportPos++;
    return type;
}

static EWRAM_DATA u8 sEvolveParty = 0;
static EWRAM_DATA u16 sEvolveTarget = 0;
static EWRAM_DATA bool32 sEvolveCanStop = 0;

// finds the next party member that wants to evolve after a fight; TRUE if there is one
u16 DBZ_TryEvolveAfterFight(void)
{
    while (sEvolveCheck < PARTY_SIZE)
    {
        u8 i = sEvolveCheck++;
        u16 target;
        if (GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
            continue;
        target = GetEvolutionTargetSpecies(&gParties[B_TRAINER_PLAYER][i], EVO_MODE_NORMAL, ITEM_NONE, NULL, &sEvolveCanStop, CHECK_EVO);
        if (target != SPECIES_NONE)
        {
            sEvolveParty = i;
            sEvolveTarget = target;
            return TRUE;
        }
    }
    return FALSE;
}

// script fades to black first, then calls this and waits (waitstate)
void DBZ_StartFightEvolution(void)
{
    CleanupOverworldWindowsAndTilemaps();
    gCB2_AfterEvolution = CB2_ReturnToFieldContinueScriptPlayMapMusic;
    GetEvolutionTargetSpecies(&gParties[B_TRAINER_PLAYER][sEvolveParty], EVO_MODE_NORMAL, ITEM_NONE, NULL, &sEvolveCanStop, DO_EVO);
    BeginEvolutionScene(&gParties[B_TRAINER_PLAYER][sEvolveParty], sEvolveTarget, sEvolveCanStop, sEvolveParty);
}

// STR_VAR_1 level, STR_VAR_2 EXP to next level, STR_VAR_3 HP now / max
void DBZ_ShowGokuStatus(void)
{
    u8 *end;
    ConvertIntToDecimalStringN(gStringVar1, DBZ_GokuLevel(), STR_CONV_MODE_LEFT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar2, DBZ_GokuExpToNext(), STR_CONV_MODE_LEFT_ALIGN, 7);
    end = ConvertIntToDecimalStringN(gStringVar3, StoredGokuHp(), STR_CONV_MODE_LEFT_ALIGN, 4);
    *end++ = CHAR_SLASH;
    ConvertIntToDecimalStringN(end, DBZ_GokuMaxHp(), STR_CONV_MODE_LEFT_ALIGN, 4);
}

// ================================================================== Shenron's other wishes
// VAR_0x8005: 1 Goku's power (+5 levels), 2 team power (+3 levels each), 3 zeni. STR_VAR_1 = amount.
void DBZ_GrantPowerWish(void)
{
    u8 i;
    sReportCount = 0;
    sReportPos = 0;
    sEvolveCheck = 0;
    switch (gSpecialVar_0x8005)
    {
    case 1:
    {
        u8 level = DBZ_GokuLevel();
        u8 target = level + 5 > DBZ_GOKU_MAX_LEVEL ? DBZ_GOKU_MAX_LEVEL : level + 5;
        u32 need = GokuExpForLevel(target) > GokuExp() ? GokuExpForLevel(target) - GokuExp() : 0;
        GokuGainExp(need);
        VarSet(VAR_DBZ_GOKU_ANNOUNCED, DBZ_GokuLevel());
        DBZ_HealGoku();
        ConvertIntToDecimalStringN(gStringVar1, DBZ_GokuLevel(), STR_CONV_MODE_LEFT_ALIGN, 3);
        break;
    }
    case 2:
        for (i = 0; i < PARTY_SIZE; i++)
        {
            struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
            u16 species = GetMonData(mon, MON_DATA_SPECIES);
            u8 level, target;
            u32 exp;
            if (species == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
                continue;
            level = GetMonData(mon, MON_DATA_LEVEL);
            target = level + 3 > MAX_LEVEL ? MAX_LEVEL : level + 3;
            exp = gExperienceTables[gSpeciesInfo[species].growthRate][target];
            if (exp > GetMonData(mon, MON_DATA_EXP))
                GiveMonFightExp(i, exp - GetMonData(mon, MON_DATA_EXP));
        }
        break;
    default:
        AddMoney(&gSaveBlock1Ptr->money, 50000);
        ConvertIntToDecimalStringN(gStringVar1, 50000, STR_CONV_MODE_LEFT_ALIGN, 6);
        break;
    }
}

// ================================================================== World Martial Arts Tournament
// VAR_0x800A = round (1 SPOPOVICH by hand, 3 VEGETA by hand) -> VAR_0x8004..6, next-fight flags
void DBZ_SetupTourneyFight(void)
{
    u8 level = DBZ_GokuLevel();
    if (PartyTopLevel() > level)
        level = PartyTopLevel();
    if (gSpecialVar_0x800A >= 3)
    {
        gSpecialVar_0x8004 = DBZ_ENEMY_VEGETA;
        gSpecialVar_0x8005 = level + 4;
        gSpecialVar_0x8006 = FindNearestWithGfx(OBJ_EVENT_GFX_RIVAL_MAY_NORMAL, 6) != OBJECT_EVENTS_COUNT
                           ? gObjectEvents[FindNearestWithGfx(OBJ_EVENT_GFX_RIVAL_MAY_NORMAL, 6)].localId : 0;
        sNextFightFlags = 0;
    }
    else
    {
        gSpecialVar_0x8004 = DBZ_ENEMY_FIGHTER;
        gSpecialVar_0x8005 = level + 1;
        gSpecialVar_0x8006 = 0;
        sNextFightFlags = (PENDING_MAGIC << 8) | (0 << 4);   // SPOPOVICH
    }
    gSpecialVar_Result = DBZ_GokuCanFight();
}

// ================================================================== Hyperbolic Time Chamber
// between waves: get back 40% of max HP
void DBZ_RestGoku(void)
{
    u16 max = DBZ_GokuMaxHp(), hp = StoredGokuHp();
    hp += max * 2 / 5;
    DBZ_SetGokuHp(hp > max ? max : hp);
}

// VAR_0x800A = wave (0..4) -> a TIME SPIRIT sparring partner, stronger each wave; triple EXP
void DBZ_SetupChamberFight(void)
{
    u8 level = DBZ_GokuLevel();
    if (PartyTopLevel() > level)
        level = PartyTopLevel();
    level += 2 + gSpecialVar_0x800A * 3;
    if (level > 100)
        level = 100;
    gSpecialVar_0x8004 = DBZ_ENEMY_SPARRING;
    gSpecialVar_0x8005 = level;
    gSpecialVar_0x8006 = 0;
    sNextFightFlags = (PENDING_MAGIC << 8) | FIGHT_FLAG_CHAMBER;
    gSpecialVar_Result = DBZ_GokuCanFight();
}
