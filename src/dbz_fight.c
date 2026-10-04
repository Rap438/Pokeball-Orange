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
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
        {
            u8 l = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
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

// percent of Goku's max HP a healing item restores (0: does nothing for Goku); revives work only when KO'd
u16 DBZ_GokuItemHealPercent(u16 itemId)
{
    bool8 ko = (StoredGokuHp() == 0);
    switch (itemId)
    {
    case ITEM_REVIVE:       return ko ? 50 : 0;
    case ITEM_MAX_REVIVE:
    case ITEM_REVIVAL_HERB: return ko ? 100 : 0;
    }
    if (ko)
        return 0;
    switch (itemId)
    {
    case ITEM_POTION:        return 20;
    case ITEM_SUPER_POTION:  return 40;
    case ITEM_HYPER_POTION:  return 70;
    case ITEM_MAX_POTION:
    case ITEM_FULL_RESTORE:  return 100;
    case ITEM_FRESH_WATER:   return 40;
    case ITEM_SODA_POP:      return 50;
    case ITEM_LEMONADE:      return 60;
    case ITEM_MOOMOO_MILK:   return 75;
    case ITEM_ENERGY_POWDER: return 40;
    case ITEM_ENERGY_ROOT:   return 70;
    case ITEM_BERRY_JUICE:   return 20;
    case ITEM_ORAN_BERRY:    return 10;
    case ITEM_SITRUS_BERRY:  return 25;
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
};

struct DbzEnemyKind {
    const u8 *name;
    u8 graphicsId;
    u8 hpPercent;
    u8 powerPercent;
    u8 stepFrames;
    bool8 ranged;
    u8 expPerLevel;
};

static const u8 sText_RedRibbon[] = _("RED RIBBON");
static const u8 sText_RRSergeant[] = _("RR SERGEANT");
static const u8 sText_Vegeta[] = _("VEGETA");
static const u8 sText_Goku[] = _("GOKU");

static const struct DbzEnemyKind sEnemyKinds[] = {
    [DBZ_ENEMY_TRAINER]        = { NULL,              0,                           100, 100, 18, FALSE, 15 },
    [DBZ_ENEMY_SAIYAN_SOLDIER] = { sText_RedRibbon,   OBJ_EVENT_GFX_AQUA_MEMBER_M, 100, 100, 16, TRUE,  12 },
    [DBZ_ENEMY_MAJIN_SOLDIER]  = { sText_RRSergeant,  OBJ_EVENT_GFX_AQUA_MEMBER_F,   90, 108, 14, FALSE, 12 },
    [DBZ_ENEMY_VEGETA]         = { sText_Vegeta,      OBJ_EVENT_GFX_RIVAL_MAY_NORMAL, 150, 112, 11, TRUE,  30 },
};

#define ARENA_HALF_W 7
#define ARENA_HALF_H 5

// attack strengths in tenths of a percent of the target's max HP (before the power-level ratio)
#define HIT_PUNCH     100
#define HIT_KICK      120
#define HIT_KI        130
#define HIT_KAME      320
#define HIT_E_PUNCH   120
#define HIT_E_KI      130
#define CAP_NORMAL    30
#define CAP_KAME      40

// Goku's attack overlay (Buu's Fury fighter frames)
enum { ATK_DOWN_WIND, ATK_DOWN_HIT, ATK_UP_WIND, ATK_UP_HIT, ATK_SIDE_WIND, ATK_SIDE_HIT };
#define ATK_KICK 6    // kick frames follow the punch frames
#define ATK_FRAMES 14
#define ATK_HIT_AT 4
#define ATK_RECOVER_AT 10

static EWRAM_DATA struct {
    u8 phase;
    u8 kind;
    u8 level;
    u8 enemyObjId;
    u8 enemyLocalId;
    bool8 spawned;
    bool8 leader;
    bool8 waitScript;     // started from a script that is waiting (rival fights)
    bool8 enemySSJ;
    u8 hudWin;
    u8 aiState;
    u8 projSprite;
    u8 projDir;
    u8 projDist;
    u8 savedRangeX;
    u8 savedRangeY;
    u8 result;
    u8 atkTimer;          // > 0 while Goku's punch/kick plays
    u8 atkDir;
    u8 atkKick;
    u8 atkSprite;
    u8 combo;
    u8 comboWindow;
    u8 hitStop;
    u8 name[TRAINER_NAME_LENGTH + 1];
    u16 enemyHp, enemyMaxHp;
    u16 gokuHp, gokuMaxHp;
    u32 enemyPL;
    u16 timer, aiTimer, rangedCooldown;
    u16 gokuInvuln, enemyFlash, attackCooldown;
    s16 arenaX0, arenaY0, arenaX1, arenaY1;
    s16 projTileX, projTileY;
    u32 expPool;
    bool8 hudDirty;
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

bool8 DBZ_IsFighting(void)
{
    return sFight.phase == FIGHT_ACTIVE || sFight.phase == FIGHT_INTRO || sFight.phase == FIGHT_OUTRO;
}

u16 DBZ_GokuHp(void)
{
    if (DBZ_IsFighting())
        return sFight.gokuHp;
    return StoredGokuHp();
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

// ------------------------------------------------------------------ difficulty
static u16 DiffEnemyDamage(void) { static const u8 s[3] = {70, 100, 135}; return s[DBZ_OptDifficulty()]; }
static u16 DiffEnemyHp(void)     { static const u8 s[3] = {85, 100, 120}; return s[DBZ_OptDifficulty()]; }
static s8 DiffWindup(void)       { static const s8 s[3] = {6, 0, -3}; return s[DBZ_OptDifficulty()]; }

// opponents power up when Goku does (a little less than he does)
static u16 EnemyFormScale(void)
{
    u16 goku = FormPLPercent();
    u16 scale = 100 + (goku - 100) * 6 / 10;
    if (sFight.enemySSJ && scale < 150)
        scale = 150;
    return scale;
}

static u32 CurrentEnemyPL(void)
{
    return sFight.enemyPL * EnemyFormScale() / 100;
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

static void DrawHud(void)
{
    u8 win = sFight.hudWin;
    u16 w;
    FillWindowPixelBuffer(win, PIXEL_FILL(10));
    StringCopy(gStringVar1, sText_Goku);
    ConvertIntToDecimalStringN(gStringVar2, DBZ_GokuPowerLevel(), STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, sText_HudPL);
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
    struct WindowTemplate t = { .bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 3, .paletteNum = 13, .baseBlock = 0x1A0 };
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

// Vegeta keeps a golden glow once he goes Super Saiyan
static void RestoreEnemyPalette(void)
{
    if (sFight.enemySSJ)
        FlashObjectPalette(Enemy(), 5, RGB(31, 27, 6));
    else
        FlashObjectPalette(Enemy(), 0, RGB_WHITE);
}

static void SpriteCenter(struct ObjectEvent *obj, s16 *x, s16 *y)
{
    struct Sprite *s = &gSprites[obj->spriteId];
    *x = s->x + s->x2;
    *y = s->y + s->y2 + 4;
}

// ------------------------------------------------------------------ Goku's attack overlay sprite
#define TAG_DBZ_ATK_PAL 0x2F40

static const u32 sAtkGfx_Base[] = INCGFX_U32("graphics/dbz/punch/goku.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u32 sAtkGfx_Ssj[]  = INCGFX_U32("graphics/dbz/punch/goku_ssj.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u32 sAtkGfx_Ssj2[] = INCGFX_U32("graphics/dbz/punch/goku_ssj2.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u32 sAtkGfx_Ssj3[] = INCGFX_U32("graphics/dbz/punch/goku_ssj3.png", ".4bpp", "-mwidth 4 -mheight 4");
static const u16 sAtkPal_Base[] = INCGFX_U16("graphics/dbz/punch/goku.png", ".gbapal");
static const u16 sAtkPal_Ssj[]  = INCGFX_U16("graphics/dbz/punch/goku_ssj.png", ".gbapal");
static const u16 sAtkPal_Ssj2[] = INCGFX_U16("graphics/dbz/punch/goku_ssj2.png", ".gbapal");
static const u16 sAtkPal_Ssj3[] = INCGFX_U16("graphics/dbz/punch/goku_ssj3.png", ".gbapal");

#define ATK_FRAME(gfx, n) {.data = (const u8 *)(gfx) + (n) * 512, .size = 512}
#define ATK_FRAMESET(gfx) { ATK_FRAME(gfx, 0), ATK_FRAME(gfx, 1), ATK_FRAME(gfx, 2), ATK_FRAME(gfx, 3), ATK_FRAME(gfx, 4), ATK_FRAME(gfx, 5), \
                            ATK_FRAME(gfx, 6), ATK_FRAME(gfx, 7), ATK_FRAME(gfx, 8), ATK_FRAME(gfx, 9), ATK_FRAME(gfx, 10), ATK_FRAME(gfx, 11) }
static const struct SpriteFrameImage sAtkImages_Base[] = ATK_FRAMESET(sAtkGfx_Base);
static const struct SpriteFrameImage sAtkImages_Ssj[]  = ATK_FRAMESET(sAtkGfx_Ssj);
static const struct SpriteFrameImage sAtkImages_Ssj2[] = ATK_FRAMESET(sAtkGfx_Ssj2);
static const struct SpriteFrameImage sAtkImages_Ssj3[] = ATK_FRAMESET(sAtkGfx_Ssj3);

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

static const struct SpriteTemplate *const sAtkTemplates[DBZ_FORM_COUNT] = {
    &sAtkTemplate_Base, &sAtkTemplate_Ssj, &sAtkTemplate_Ssj2, &sAtkTemplate_Ssj3,
};
static const u16 *const sAtkPals[DBZ_FORM_COUNT] = { sAtkPal_Base, sAtkPal_Ssj, sAtkPal_Ssj2, sAtkPal_Ssj3 };

static bool8 AtkSpriteValid(void)
{
    u8 id = sFight.atkSprite;
    return id < MAX_SPRITES && gSprites[id].inUse && gSprites[id].template == sAtkTemplates[DBZ_GetForm()];
}

bool8 DBZ_PlayerOverlayActive(void)
{
    return DBZ_IsFighting() && sFight.atkTimer != 0 && AtkSpriteValid();
}

static void LoadAtkPalette(void)
{
    struct SpritePalette pal;
    u8 slot;
    FreeSpritePaletteByTag(TAG_DBZ_ATK_PAL);
    pal.data = sAtkPals[DBZ_GetForm()];
    pal.tag = TAG_DBZ_ATK_PAL;
    slot = LoadSpritePalette(&pal);
    if (slot != 0xFF)
        DBZ_ApplyTimeTint(OBJ_PLTT_ID(slot), 16);
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

static void StartAtkSprite(void)
{
    struct Sprite *ps = &gSprites[gPlayerAvatar.spriteId];
    u8 id;
    DestroyAtkSprite();
    if (IndexOfSpritePaletteTag(TAG_DBZ_ATK_PAL) == 0xFF)
        LoadAtkPalette();
    id = CreateSprite(sAtkTemplates[DBZ_GetForm()], ps->x, ps->y, ps->subpriority);
    sFight.atkSprite = id;
    if (id == MAX_SPRITES)
        return;
    gSprites[id].coordOffsetEnabled = TRUE;
    StartSpriteAnim(&gSprites[id], AtkFrame(sFight.atkDir, FALSE, sFight.atkKick));
    Player()->invisible = TRUE;
}

static void UpdateAtkSprite(void)
{
    struct Sprite *ps = &gSprites[gPlayerAvatar.spriteId];
    struct Sprite *s;
    s16 dx, dy, lunge = 0;
    if (!AtkSpriteValid())
        return;
    s = &gSprites[sFight.atkSprite];
    Vec(sFight.atkDir, &dx, &dy);
    if (sFight.atkTimer == ATK_FRAMES - ATK_HIT_AT)
        StartSpriteAnim(s, AtkFrame(sFight.atkDir, TRUE, sFight.atkKick));
    else if (sFight.atkTimer == ATK_FRAMES - ATK_RECOVER_AT)
        StartSpriteAnim(s, AtkFrame(sFight.atkDir, FALSE, sFight.atkKick));
    if (sFight.atkTimer <= ATK_FRAMES - ATK_HIT_AT && sFight.atkTimer > ATK_FRAMES - ATK_RECOVER_AT)
        lunge = 3;
    s->x = ps->x + dx * lunge;
    s->y = ps->y + dy * lunge;
    s->x2 = 0;
    s->y2 = 0;
    s->oam.priority = ps->oam.priority;
    s->subpriority = ps->subpriority ? ps->subpriority - 1 : 0;   // in front of whatever Goku is in front of
    s->invisible = sFight.gokuInvuln && (sFight.gokuInvuln % 4) >= 2;
}

// ------------------------------------------------------------------ damage
static void HitEnemy(u16 strength, u8 cap, u8 fromDir, bool8 knockback, s16 fxX, s16 fxY)
{
    u16 dmg;
    if (sFight.phase != FIGHT_ACTIVE)
        return;
    dmg = Damage(DBZ_GokuPowerLevel(), CurrentEnemyPL(), sFight.enemyMaxHp, strength, cap);
    sFight.enemyHp = dmg >= sFight.enemyHp ? 0 : sFight.enemyHp - dmg;
    sFight.enemyFlash = 10;
    sFight.hudDirty = TRUE;
    sFight.hitStop = 3;
    DBZ_SpawnImpactAt(fxX, fxY);
    PlaySE(strength >= HIT_KAME ? SE_M_EXPLOSION : (strength >= HIT_KICK ? SE_M_MEGA_KICK : SE_M_COMET_PUNCH));
    // getting hit interrupts the enemy's attack
    sFight.aiState = AI_RECOVER;
    sFight.aiTimer = 16;
    if (knockback && sFight.enemyHp > 0)
    {
        struct ObjectEvent *e = Enemy();
        if (!ObjectEventIsMovementOverridden(e) && EnemyCanStep(fromDir))
            ObjectEventSetHeldMovement(e, GetWalkFastMovementAction(fromDir));
    }
}

static void HitGoku(u16 strength)
{
    u16 dmg;
    s16 x, y;
    if (sFight.gokuInvuln || sFight.phase != FIGHT_ACTIVE)
        return;
    dmg = Damage(CurrentEnemyPL(), DBZ_GokuPowerLevel(), sFight.gokuMaxHp, strength, CAP_NORMAL);
    dmg = (u32)dmg * DiffEnemyDamage() / 100;
    if (dmg == 0)
        dmg = 1;
    sFight.gokuHp = dmg >= sFight.gokuHp ? 0 : sFight.gokuHp - dmg;
    sFight.gokuInvuln = 36;
    sFight.hudDirty = TRUE;
    PlaySE(SE_M_VITAL_THROW);
    SpriteCenter(Player(), &x, &y);
    DBZ_SpawnImpactAt(x, y);
}

// called by the blast task when a ki blast / Kamehameha lands during a fight
void DBZ_FightOnBlast(u8 hitType, u8 hitLocal, bool8 kame)
{
    s16 x, y;
    if (sFight.phase != FIGHT_ACTIVE)
        return;
    if (hitType == DBZ_HIT_OBJECT && hitLocal == sFight.enemyLocalId)
    {
        SpriteCenter(Enemy(), &x, &y);
        HitEnemy(kame ? HIT_KAME : HIT_KI, kame ? CAP_KAME : CAP_NORMAL, GetPlayerFacingDirection(), TRUE, x, y);
    }
}

// ------------------------------------------------------------------ enemy projectile
static void FireProjectile(u8 dir)
{
    s16 x, y;
    SpriteCenter(Enemy(), &x, &y);
    sFight.projSprite = DBZ_CreateKiSprite(x, y);
    if (sFight.projSprite == MAX_SPRITES)
        return;
    sFight.projDir = dir;
    sFight.projDist = 0;
    sFight.projTileX = Enemy()->currentCoords.x;
    sFight.projTileY = Enemy()->currentCoords.y;
    PlaySE(SE_M_SWIFT);
}

static void UpdateProjectile(void)
{
    s16 dx, dy, tx, ty;
    struct Sprite *s;
    if (sFight.projSprite == MAX_SPRITES)
        return;
    s = &gSprites[sFight.projSprite];
    Vec(sFight.projDir, &dx, &dy);
    s->x += dx * 4;
    s->y += dy * 4;
    sFight.projDist += 4;
    DBZ_AnimateKiSprite(sFight.projSprite, sFight.projDist / 4);
    tx = sFight.projTileX + dx * ((sFight.projDist + 8) / 16);
    ty = sFight.projTileY + dy * ((sFight.projDist + 8) / 16);
    if (tx == Player()->currentCoords.x && ty == Player()->currentCoords.y)
    {
        HitGoku(HIT_E_KI);
        DBZ_DestroyFxSprite(sFight.projSprite);
        sFight.projSprite = MAX_SPRITES;
        return;
    }
    if (sFight.projDist > 16 * 6 || MapGridGetCollisionAt(tx, ty))
    {
        DBZ_SpawnImpactAt(s->x, s->y);
        DBZ_DestroyFxSprite(sFight.projSprite);
        sFight.projSprite = MAX_SPRITES;
    }
}

// ------------------------------------------------------------------ enemy AI
static u8 WindupFrames(void)
{
    s16 f = (sFight.leader || sFight.kind == DBZ_ENEMY_VEGETA) ? 10 : 14;
    if (sFight.enemySSJ)
        f -= 2;
    f += DiffWindup();
    return f < 6 ? 6 : f;
}

static void EnemyAI(void)
{
    struct ObjectEvent *e = Enemy();
    struct ObjectEvent *p = Player();
    s16 ex = e->currentCoords.x, ey = e->currentCoords.y;
    s16 px = p->currentCoords.x, py = p->currentCoords.y;
    s16 dist = abs(px - ex) + abs(py - ey);
    u8 dir;
    bool8 vegeta = (sFight.kind == DBZ_ENEMY_VEGETA);

    if (sFight.aiTimer)
        sFight.aiTimer--;
    if (sFight.rangedCooldown)
        sFight.rangedCooldown--;

    // Vegeta goes Super Saiyan once hurt (from the 4th gym onward)
    if (vegeta && !sFight.enemySSJ && FlagGet(FLAG_BADGE04_GET) && sFight.enemyHp * 2 < sFight.enemyMaxHp)
    {
        sFight.enemySSJ = TRUE;
        sFight.enemyHp += sFight.enemyMaxHp / 5;
        PlaySE(SE_M_SWAGGER);
        DBZ_SpawnImpactAt(gSprites[e->spriteId].x, gSprites[e->spriteId].y);
        sFight.aiState = AI_RECOVER;
        sFight.aiTimer = 30;
        sFight.hudDirty = TRUE;
        RestoreEnemyPalette();
        return;
    }

    switch (sFight.aiState)
    {
    case AI_APPROACH:
        if (sFight.aiTimer || EnemyBusy())
            break;
        dir = DirTo(ex, ey, px, py);
        if (dist == 1)
        {
            ObjectEventTurn(e, dir);
            sFight.aiState = AI_WINDUP;
            sFight.aiTimer = WindupFrames();
            PlaySE(SE_M_SWAGGER2);
            break;
        }
        if (sEnemyKinds[sFight.kind].ranged && sFight.rangedCooldown == 0 && (ex == px || ey == py) && dist <= 5
         && (Random() % (vegeta ? 2 : 3)) == 0)
        {
            ObjectEventTurn(e, dir);
            FireProjectile(dir);
            sFight.rangedCooldown = vegeta ? (sFight.enemySSJ ? 40 : 55) : 80;
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
        if (dist <= 3 || vegeta)
            ObjectEventSetHeldMovement(e, GetWalkFastMovementAction(dir));
        else
            ObjectEventSetHeldMovement(e, GetWalkNormalMovementAction(dir));
        sFight.aiTimer = sEnemyKinds[sFight.kind].stepFrames - (sFight.leader ? 3 : 0);
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
                HitGoku(HIT_E_PUNCH);
            }
            else
            {
                PlaySE(SE_M_TAIL_WHIP);
            }
            sFight.aiState = AI_RECOVER;
            sFight.aiTimer = DBZ_OptDifficulty() == DBZ_DIFF_EASY ? 26 : (DBZ_OptDifficulty() == DBZ_DIFF_HARD ? 14 : 18);
        }
        break;
    case AI_RECOVER:
        if (sFight.aiTimer == 0)
            sFight.aiState = AI_APPROACH;
        break;
    }
}

// ------------------------------------------------------------------ player input during a fight
// returns 0: walk normally, 1: a script started (lock controls), 2: input used, stand still
u8 DBZ_HandleFightInput(struct FieldInput *input)
{
    if (sFight.phase != FIGHT_ACTIVE)
        return 2;            // intro/outro: Goku stands his ground
    if (DBZ_IsFightBlastActive() || sFight.atkTimer)
        return 2;
    // L / R go through the normal power handler (charging, specials, transforming)
    if (DBZ_HandleFieldInput(input))
        return ScriptContext_IsEnabled() ? 1 : 2;
    if (DBZ_IsChargingBlast())
        return 0;
    if (input->pressedAButton && sFight.attackCooldown == 0 && gPlayerAvatar.tileTransitionState == T_NOT_MOVING)
    {
        sFight.atkDir = GetPlayerFacingDirection();
        sFight.atkKick = (sFight.comboWindow != 0) ? !sFight.atkKick : FALSE;
        sFight.atkTimer = ATK_FRAMES;
        sFight.attackCooldown = ATK_FRAMES + 2;
        sFight.comboWindow = 0;
        StartAtkSprite();
        PlaySE(SE_M_TAIL_WHIP);
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
        // spark right at the fist
        SpriteCenter(p, &fx, &fy);
        fx += dx * 14;
        fy += dy * 12 - (sFight.atkDir == DIR_NORTH ? 6 : 0);
        HitEnemy(sFight.atkKick ? HIT_KICK : HIT_PUNCH, CAP_NORMAL, sFight.atkDir, sFight.atkKick, fx, fy);
    }
}

static void UpdateAttack(void)
{
    if (sFight.attackCooldown)
        sFight.attackCooldown--;
    if (sFight.comboWindow)
        sFight.comboWindow--;
    if (sFight.atkTimer == 0)
        return;
    if (sFight.atkTimer == ATK_FRAMES - ATK_HIT_AT)
        ResolveAttackHit();
    UpdateAtkSprite();
    if (--sFight.atkTimer == 0)
    {
        DestroyAtkSprite();
        sFight.comboWindow = 24;
    }
}

// ------------------------------------------------------------------ main task
static void EndFight(u8 taskId)
{
    struct ObjectEvent *e = Enemy();
    if (sFight.projSprite != MAX_SPRITES)
    {
        DBZ_DestroyFxSprite(sFight.projSprite);
        sFight.projSprite = MAX_SPRITES;
    }
    sFight.atkTimer = 0;
    DestroyAtkSprite();
    FreeSpritePaletteByTag(TAG_DBZ_ATK_PAL);
    sFight.enemySSJ = FALSE;
    FlashObjectPalette(e, 0, RGB_WHITE);
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
    // Goku keeps his wounds
    DBZ_SetGokuHp(sFight.gokuHp);
    if (sFight.result == DBZ_FIGHT_WON)
    {
        if (VarGet(VAR_DBZ_FIGHTS_WON) < 9999)
            VarSet(VAR_DBZ_FIGHTS_WON, VarGet(VAR_DBZ_FIGHTS_WON) + 1);
    }
    else if (VarGet(VAR_DBZ_FIGHTS_LOST) < 9999)
    {
        VarSet(VAR_DBZ_FIGHTS_LOST, VarGet(VAR_DBZ_FIGHTS_LOST) + 1);
    }
    sFight.phase = FIGHT_NONE;
    gSpecialVar_Result = sFight.result;
    gSpecialVar_0x8009 = sFight.waitScript;
    StringCopy(gStringVar1, sFight.name);
    DestroyTask(taskId);
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
        }
        BlendPalettes(PALETTES_BG, (sFight.timer < 8) ? (8 - sFight.timer) : 0, RGB_WHITE);
        if (++sFight.timer >= 30)
        {
            sFight.phase = FIGHT_ACTIVE;
            sFight.timer = 0;
            sFight.aiTimer = 16;
        }
        break;
    case FIGHT_ACTIVE:
        UpdateAttack();
        if (sFight.gokuInvuln)
        {
            sFight.gokuInvuln--;
            if (!sFight.atkTimer)
                Player()->invisible = (sFight.gokuInvuln % 4) >= 2;
            if (sFight.gokuInvuln == 0 && !sFight.atkTimer)
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
            if (sFight.hitStop)
                sFight.hitStop--;   // a short freeze on impact sells the hit
            else
                EnemyAI();
            UpdateProjectile();
        }
        if (sFight.hudDirty)
            DrawHud();
        if ((sFight.enemyHp == 0 || sFight.gokuHp == 0) && !DBZ_IsFightBlastActive() && !scriptRunning
         && gPlayerAvatar.tileTransitionState == T_NOT_MOVING && sFight.atkTimer == 0)
        {
            sFight.result = sFight.enemyHp == 0 ? DBZ_FIGHT_WON : DBZ_FIGHT_LOST;
            sFight.phase = FIGHT_OUTRO;
            sFight.timer = 0;
            PlaySE(sFight.result == DBZ_FIGHT_WON ? SE_M_EXPLOSION : SE_FAINT);
            Player()->invisible = FALSE;
        }
        break;
    case FIGHT_OUTRO:
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

// ------------------------------------------------------------------ starting a fight
static u8 TrainerTopLevel(u16 trainerId)
{
    const struct Trainer *t = &gTrainers[trainerId];
    u8 i, best = 5, lvl;
    for (i = 0; i < t->partySize; i++)
    {
        switch (t->partyFlags & (F_TRAINER_PARTY_CUSTOM_MOVESET | F_TRAINER_PARTY_HELD_ITEM))
        {
        case 0:                              lvl = t->party.NoItemDefaultMoves[i].lvl; break;
        case F_TRAINER_PARTY_CUSTOM_MOVESET: lvl = t->party.NoItemCustomMoves[i].lvl; break;
        case F_TRAINER_PARTY_HELD_ITEM:      lvl = t->party.ItemDefaultMoves[i].lvl; break;
        default:                             lvl = t->party.ItemCustomMoves[i].lvl; break;
        }
        if (lvl > best)
            best = lvl;
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

// VAR_0x8004 = enemy kind, VAR_0x8005 = level, VAR_0x8006 = local id of the opponent object (0: spawn one)
static bool8 StartFightInternal(bool8 waitScript)
{
    const struct DbzEnemyKind *k;
    struct ObjectEvent *p = Player();
    u16 hpBase;

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
    sFight.atkKick = FALSE;
    sFight.comboWindow = 0;
    sFight.attackCooldown = 0;
    sFight.hitStop = 0;
    sFight.gokuInvuln = 0;
    sFight.enemyFlash = 0;
    sFight.rangedCooldown = 50;
    sFight.aiState = AI_APPROACH;
    sFight.result = 0;

    if (StoredGokuHp() == 0)
    {
        sFight.phase = FIGHT_NONE;
        return FALSE;
    }

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

    if (k->name != NULL)
        StringCopy(sFight.name, k->name);
    else if (gTrainerBattleOpponent_A != 0)
        StringCopyN(sFight.name, gTrainers[gTrainerBattleOpponent_A].trainerName, TRAINER_NAME_LENGTH);
    else
        StringCopy(sFight.name, sText_RedRibbon);
    sFight.name[TRAINER_NAME_LENGTH] = EOS;

    sFight.leader = (sFight.kind == DBZ_ENEMY_TRAINER && gTrainerBattleOpponent_A != 0
                  && gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_LEADER);
    hpBase = 20 + 7 * sFight.level;
    sFight.enemyMaxHp = (u32)hpBase * k->hpPercent / 100 * (sFight.leader ? 13 : 10) / 10 * DiffEnemyHp() / 100;
    sFight.enemyHp = sFight.enemyMaxHp;
    sFight.enemyPL = BasePowerLevel(sFight.level) * k->powerPercent / 100 * (sFight.leader ? 110 : 100) / 100;
    sFight.expPool = (u32)sFight.level * k->expPerLevel * (sFight.leader ? 2 : 1);
    sFight.gokuMaxHp = DBZ_GokuMaxHp();
    sFight.gokuHp = StoredGokuHp();

    sFight.savedRangeX = Enemy()->range.rangeX;
    sFight.savedRangeY = Enemy()->range.rangeY;
    Enemy()->range.rangeX = 0;
    Enemy()->range.rangeY = 0;
    ObjectEventClearHeldMovementIfActive(Enemy());

    sFight.arenaX0 = p->currentCoords.x - ARENA_HALF_W;
    sFight.arenaX1 = p->currentCoords.x + ARENA_HALF_W;
    sFight.arenaY0 = p->currentCoords.y - ARENA_HALF_H;
    sFight.arenaY1 = p->currentCoords.y + ARENA_HALF_H;

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
    if (gTrainerBattleOpponent_A == 0)
        return FALSE;
    return GetObjectEventIdByLocalIdAndMap(gSpecialVar_LastTalked, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup) != OBJECT_EVENTS_COUNT;
}

void DBZ_SetupTrainerFight(void)
{
    gSpecialVar_0x8004 = DBZ_ENEMY_TRAINER;
    gSpecialVar_0x8005 = TrainerTopLevel(gTrainerBattleOpponent_A);
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
    gSpecialVar_0x8005 = (gTrainerBattleOpponent_A != 0 ? TrainerTopLevel(gTrainerBattleOpponent_A) : DBZ_GokuLevel()) + 3;
    if (gSpecialVar_0x8005 < DBZ_GokuLevel())
        gSpecialVar_0x8005 = DBZ_GokuLevel();
    gSpecialVar_0x8006 = (best != OBJECT_EVENTS_COUNT && bestDist <= 6) ? gObjectEvents[best].localId : 0;
    gSpecialVar_0x8007 = 0;
    for (i = 0; i < 8; i++)
        if (FlagGet(FLAG_BADGE01_GET + i))
            gSpecialVar_0x8007++;
    gSpecialVar_Result = DBZ_GokuCanFight();
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
    gTrainerBattleOpponent_A = 0;
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
    struct Pokemon *mon = &gPlayerParty[partyId];
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
    u32 goku = sFight.expPool;
    u32 mons = sFight.expPool * 2 / 5;   // overworld fights lean toward Goku
    u8 i, gained;
    u16 hpBefore = StoredGokuHp();
    u16 maxBefore = DBZ_GokuMaxHp();

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
    GetMonData(&gPlayerParty[sReports[sReportPos].partyId], MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
    if (type == DBZ_REPORT_LEVEL)
        ConvertIntToDecimalStringN(gStringVar2, sReports[sReportPos].level, STR_CONV_MODE_LEFT_ALIGN, 3);
    else
        StringCopy(gStringVar2, gMoveNames[sReports[sReportPos].move]);
    sReportPos++;
    return type;
}

static EWRAM_DATA u8 sEvolveParty = 0;
static EWRAM_DATA u16 sEvolveTarget = 0;

// finds the next party member that wants to evolve after a fight; TRUE if there is one
u16 DBZ_TryEvolveAfterFight(void)
{
    while (sEvolveCheck < PARTY_SIZE)
    {
        u8 i = sEvolveCheck++;
        u16 target;
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
            continue;
        target = GetEvolutionTargetSpecies(&gPlayerParty[i], EVO_MODE_NORMAL, ITEM_NONE);
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
    BeginEvolutionScene(&gPlayerParty[sEvolveParty], sEvolveTarget, TRUE, sEvolveParty);
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
