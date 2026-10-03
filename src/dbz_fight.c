// PokeBall Orange: real-time DBZ fights on the overworld + Goku's own level/EXP.
//
// Controls during a fight: D-pad move (B to dash), A punch, tap L ki blast, hold L Kamehameha,
// R power up. Fights happen against trainers (talk to a beaten trainer), gym leaders (right after
// the badge), and Saiyan/Majin soldiers who ambush Goku on routes and in cities.
// Overworld fights lean EXP toward Goku; the party gets a share. Pokemon battles lean toward the
// party; Goku gets a share (see DBZ_GokuGainExpFromBattle, called from Cmd_getexp).
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
#include "fieldmap.h"
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

static u16 GokuMaxHp(void)  { return 30 + 8 * DBZ_GokuLevel(); }
static u16 GokuPower(void)  { return (4 + 2 * DBZ_GokuLevel()) * FormPowerPercent() / 100; }
static u16 GokuDefense(void){ return (2 + DBZ_GokuLevel()) * (100 + (FormPowerPercent() - 100) / 2) / 100; }

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

static const u8 sText_SaiyanSoldier[] = _("SAIYAN");
static const u8 sText_MajinSoldier[] = _("MAJIN");
static const u8 sText_Goku[] = _("GOKU");

static const struct DbzEnemyKind sEnemyKinds[] = {
    [DBZ_ENEMY_TRAINER]        = { NULL,                 0,                            100, 100, 22, FALSE, 15 },
    [DBZ_ENEMY_SAIYAN_SOLDIER] = { sText_SaiyanSoldier,  OBJ_EVENT_GFX_AQUA_MEMBER_M,  100, 105, 20, TRUE,  12 },
    [DBZ_ENEMY_MAJIN_SOLDIER]  = { sText_MajinSoldier,   OBJ_EVENT_GFX_AQUA_MEMBER_F,   90, 110, 18, FALSE, 12 },
};

#define ARENA_HALF_W 7
#define ARENA_HALF_H 5

static EWRAM_DATA struct {
    u8 phase;
    u8 kind;
    u8 level;
    u8 enemyObjId;
    u8 enemyLocalId;
    bool8 spawned;
    bool8 leader;
    u8 hudWin;
    u8 aiState;
    u8 projSprite;
    u8 projDir;
    u8 projDist;
    u8 savedRangeX;
    u8 savedRangeY;
    u8 result;
    u8 punchFrames;
    u8 name[TRAINER_NAME_LENGTH + 1];
    u16 enemyHp, enemyMaxHp;
    u16 gokuHp, gokuMaxHp;
    u16 enemyPower, enemyDefense;
    u16 timer, aiTimer, rangedCooldown;
    u16 gokuInvuln, enemyFlash, punchCooldown;
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

bool8 DBZ_IsFighting(void)
{
    return sFight.phase == FIGHT_ACTIVE || sFight.phase == FIGHT_INTRO || sFight.phase == FIGHT_OUTRO;
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

// ------------------------------------------------------------------ HUD
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
static const u8 sText_HudLv[] = _("{STR_VAR_1} Lv{STR_VAR_2}");

static void DrawHud(void)
{
    u8 win = sFight.hudWin;
    u16 w;
    FillWindowPixelBuffer(win, PIXEL_FILL(10));
    StringCopy(gStringVar1, sText_Goku);
    ConvertIntToDecimalStringN(gStringVar2, DBZ_GokuLevel(), STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, sText_HudLv);
    AddTextPrinterParameterized3(win, FONT_SMALL, 4, 0, sHudColors, TEXT_SKIP_DRAW, gStringVar4);
    DrawBar(win, 4, 13, 90, sFight.gokuHp, sFight.gokuMaxHp, sFight.gokuHp * 4 < sFight.gokuMaxHp ? 4 : 6);

    StringCopy(gStringVar1, sFight.name);
    ConvertIntToDecimalStringN(gStringVar2, sFight.level, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, sText_HudLv);
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
static u16 Damage(u16 power, u16 percent, u16 defense)
{
    s32 d = (s32)power * percent / 100;
    d = d * (90 + (Random() % 21)) / 100;
    d -= defense / 2;
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

static u8 Opposite(u8 dir)
{
    switch (dir)
    {
    case DIR_SOUTH: return DIR_NORTH;
    case DIR_NORTH: return DIR_SOUTH;
    case DIR_WEST:  return DIR_EAST;
    default:        return DIR_WEST;
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

static void SpriteCenter(struct ObjectEvent *obj, s16 *x, s16 *y)
{
    struct Sprite *s = &gSprites[obj->spriteId];
    *x = s->x + s->x2;
    *y = s->y + s->y2 + 4;
}

// ------------------------------------------------------------------ damage
static void HitEnemy(u16 percent, u8 fromDir, bool8 knockback)
{
    u16 dmg;
    s16 x, y;
    if (sFight.phase != FIGHT_ACTIVE)
        return;
    dmg = Damage(GokuPower(), percent, sFight.enemyDefense);
    sFight.enemyHp = dmg >= sFight.enemyHp ? 0 : sFight.enemyHp - dmg;
    sFight.enemyFlash = 10;
    sFight.hudDirty = TRUE;
    SpriteCenter(Enemy(), &x, &y);
    DBZ_SpawnImpactAt(x, y);
    PlaySE(percent >= 200 ? SE_M_EXPLOSION : SE_M_COMET_PUNCH);
    // getting hit interrupts the enemy's attack
    sFight.aiState = AI_RECOVER;
    sFight.aiTimer = 18;
    if (knockback && sFight.enemyHp > 0)
    {
        u8 away = Opposite(fromDir);
        struct ObjectEvent *e = Enemy();
        if (!ObjectEventIsMovementOverridden(e) && EnemyCanStep(fromDir))
        {
            ObjectEventSetHeldMovement(e, GetWalkFastMovementAction(fromDir));
            (void)away;
        }
    }
}

static void HitGoku(u16 percent)
{
    u16 dmg;
    if (sFight.gokuInvuln || sFight.phase != FIGHT_ACTIVE)
        return;
    dmg = Damage(sFight.enemyPower, percent, GokuDefense());
    sFight.gokuHp = dmg >= sFight.gokuHp ? 0 : sFight.gokuHp - dmg;
    sFight.gokuInvuln = 40;
    sFight.hudDirty = TRUE;
    PlaySE(SE_M_VITAL_THROW);
    SetCameraPanningCallback(NULL);
    {
        s16 x, y;
        SpriteCenter(Player(), &x, &y);
        DBZ_SpawnImpactAt(x, y);
    }
}

// called by the blast task when a ki blast / Kamehameha lands during a fight
void DBZ_FightOnBlast(u8 hitType, u8 hitLocal, bool8 kame)
{
    if (sFight.phase != FIGHT_ACTIVE)
        return;
    if (hitType == DBZ_HIT_OBJECT && hitLocal == sFight.enemyLocalId)
        HitEnemy(kame ? 300 : 140, GetPlayerFacingDirection(), TRUE);
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
        HitGoku(130);
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
static void EnemyAI(void)
{
    struct ObjectEvent *e = Enemy();
    struct ObjectEvent *p = Player();
    s16 ex = e->currentCoords.x, ey = e->currentCoords.y;
    s16 px = p->currentCoords.x, py = p->currentCoords.y;
    s16 dist = abs(px - ex) + abs(py - ey);
    u8 dir;

    if (sFight.aiTimer)
        sFight.aiTimer--;
    if (sFight.rangedCooldown)
        sFight.rangedCooldown--;

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
            sFight.aiTimer = sFight.leader ? 14 : 20;
            PlaySE(SE_M_SWAGGER2);
            break;
        }
        if (sEnemyKinds[sFight.kind].ranged && sFight.rangedCooldown == 0 && (ex == px || ey == py) && dist <= 5 && (Random() % 3) == 0)
        {
            ObjectEventTurn(e, dir);
            FireProjectile(dir);
            sFight.rangedCooldown = 90;
            sFight.aiState = AI_RECOVER;
            sFight.aiTimer = 24;
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
                    sFight.aiTimer = 8;
                    break;
                }
            }
        }
        ObjectEventSetHeldMovement(e, GetWalkNormalMovementAction(dir));
        sFight.aiTimer = sEnemyKinds[sFight.kind].stepFrames - (sFight.leader ? 4 : 0);
        break;
    case AI_WINDUP:
        FlashObjectPalette(e, (sFight.aiTimer % 4) < 2 ? 8 : 0, RGB_WHITE);
        if (sFight.aiTimer == 0)
        {
            FlashObjectPalette(e, 0, RGB_WHITE);
            ObjectEventSetHeldMovement(e, GetWalkInPlaceFastMovementAction(e->facingDirection));
            if (dist == 1 && DirTo(ex, ey, px, py) == e->facingDirection)
                HitGoku(100);
            else
                PlaySE(SE_M_TAIL_WHIP);
            sFight.aiState = AI_RECOVER;
            sFight.aiTimer = 26;
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
    struct ObjectEvent *p = Player();
    s16 dx, dy;
    u8 objId;

    if (sFight.phase != FIGHT_ACTIVE)
        return 2;            // intro/outro: Goku stands his ground
    if (DBZ_IsFightBlastActive() || sFight.punchFrames)
        return 2;
    // L / R go through the normal power handler (charging, Kamehameha, transforming)
    if (DBZ_HandleFieldInput(input))
        return ScriptContext_IsEnabled() ? 1 : 2;
    if (DBZ_IsChargingBlast())
        return 0;
    if (input->pressedAButton && sFight.punchCooldown == 0 && gPlayerAvatar.tileTransitionState == T_NOT_MOVING)
    {
        u8 dir = GetPlayerFacingDirection();
        sFight.punchFrames = 8;
        sFight.punchCooldown = 16;
        Vec(dir, &dx, &dy);
        objId = GetObjectEventIdByXY(p->currentCoords.x + dx, p->currentCoords.y + dy);
        if (objId == sFight.enemyObjId)
            HitEnemy(100, dir, (Random() % 3) == 0);
        else
            PlaySE(SE_M_TAIL_WHIP);
        return 2;
    }
    return 0;    // normal walking / dashing
}

static void UpdatePunchLunge(void)
{
    struct Sprite *s = &gSprites[gPlayerAvatar.spriteId];
    s16 dx, dy;
    if (sFight.punchFrames == 0)
        return;
    Vec(GetPlayerFacingDirection(), &dx, &dy);
    sFight.punchFrames--;
    if (sFight.punchFrames > 4)
    {
        s->x2 = dx * (8 - sFight.punchFrames) * 2;
        s->y2 = dy * (8 - sFight.punchFrames) * 2;
    }
    else
    {
        s->x2 = dx * sFight.punchFrames * 2;
        s->y2 = dy * sFight.punchFrames * 2;
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
    FlashObjectPalette(e, 0, RGB_WHITE);
    gSprites[gPlayerAvatar.spriteId].invisible = FALSE;
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
    UnfreezeObjectEvents();
    sFight.phase = FIGHT_NONE;
    gSpecialVar_Result = sFight.result;
    StringCopy(gStringVar1, sFight.name);
    DestroyTask(taskId);
    ScriptContext_SetupScript(EventScript_DBZ_FightEnd);
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
            sFight.aiTimer = 20;
        }
        break;
    case FIGHT_ACTIVE:
        if (sFight.punchCooldown)
            sFight.punchCooldown--;
        UpdatePunchLunge();
        if (sFight.gokuInvuln)
        {
            sFight.gokuInvuln--;
            gSprites[gPlayerAvatar.spriteId].invisible = (sFight.gokuInvuln % 4) >= 2;
            if (sFight.gokuInvuln == 0)
                gSprites[gPlayerAvatar.spriteId].invisible = FALSE;
        }
        if (sFight.enemyFlash)
        {
            sFight.enemyFlash--;
            FlashObjectPalette(Enemy(), (sFight.enemyFlash % 4) < 2 ? 12 : 0, RGB(31, 8, 8));
        }
        if (!scriptRunning)
        {
            FreezeObjectEventsExceptTwo(gPlayerAvatar.objectEventId, sFight.enemyObjId);
            EnemyAI();
            UpdateProjectile();
        }
        if (sFight.hudDirty)
            DrawHud();
        if ((sFight.enemyHp == 0 || sFight.gokuHp == 0) && !DBZ_IsFightBlastActive() && !scriptRunning
         && gPlayerAvatar.tileTransitionState == T_NOT_MOVING && sFight.punchFrames == 0)
        {
            sFight.result = sFight.enemyHp == 0 ? DBZ_FIGHT_WON : DBZ_FIGHT_LOST;
            sFight.phase = FIGHT_OUTRO;
            sFight.timer = 0;
            PlaySE(sFight.result == DBZ_FIGHT_WON ? SE_M_EXPLOSION : SE_FAINT);
            gSprites[gPlayerAvatar.spriteId].invisible = FALSE;
        }
        break;
    case FIGHT_OUTRO:
        if (sFight.result == DBZ_FIGHT_WON)
            Enemy()->invisible = (sFight.timer % 4) >= 2;
        else
            gSprites[gPlayerAvatar.spriteId].invisible = (sFight.timer % 4) >= 2;
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
void DBZ_StartFight(void)
{
    const struct DbzEnemyKind *k;
    struct ObjectEvent *p = Player();
    u16 hpBase;

    sFight.kind = gSpecialVar_0x8004 < ARRAY_COUNT(sEnemyKinds) ? gSpecialVar_0x8004 : DBZ_ENEMY_SAIYAN_SOLDIER;
    k = &sEnemyKinds[sFight.kind];
    sFight.level = gSpecialVar_0x8005 ? gSpecialVar_0x8005 : 5;
    sFight.projSprite = MAX_SPRITES;
    sFight.hudWin = WINDOW_NONE;
    sFight.spawned = FALSE;
    sFight.punchFrames = 0;
    sFight.punchCooldown = 0;
    sFight.gokuInvuln = 0;
    sFight.enemyFlash = 0;
    sFight.rangedCooldown = 60;
    sFight.aiState = AI_APPROACH;
    sFight.result = 0;

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
        return;
    }

    if (k->name != NULL)
        StringCopy(sFight.name, k->name);
    else if (gTrainerBattleOpponent_A != 0)
        StringCopyN(sFight.name, gTrainers[gTrainerBattleOpponent_A].trainerName, TRAINER_NAME_LENGTH);
    else
        StringCopy(sFight.name, sText_SaiyanSoldier);
    sFight.name[TRAINER_NAME_LENGTH] = EOS;

    sFight.leader = (sFight.kind == DBZ_ENEMY_TRAINER && gTrainerBattleOpponent_A != 0
                  && gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_LEADER);
    hpBase = 20 + 7 * sFight.level;
    sFight.enemyMaxHp = hpBase * k->hpPercent / 100 * (sFight.leader ? 13 : 10) / 10;
    sFight.enemyHp = sFight.enemyMaxHp;
    sFight.enemyPower = (3 + 2 * sFight.level) * k->powerPercent / 100;
    sFight.enemyDefense = 2 + sFight.level;
    sFight.expPool = (u32)sFight.level * k->expPerLevel * (sFight.leader ? 2 : 1);
    sFight.gokuMaxHp = GokuMaxHp();
    sFight.gokuHp = sFight.gokuMaxHp;

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
}

// ------------------------------------------------------------------ trainers / ambushes / gym leaders
u16 DBZ_CanFightTrainer(void)
{
    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT))
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

// gym scripts call this right after the badge; the fight starts once the script lets Goku go
void DBZ_QueueGymFight(void)
{
    VarSet(VAR_DBZ_PENDING_FIGHT, gSpecialVar_LastTalked + 1);
}

static u8 BadgeCount(void)
{
    u8 i, n = 0;
    for (i = 0; i < 8; i++)
        if (FlagGet(FLAG_BADGE01_GET + i))
            n++;
    return n;
}

void DBZ_PrepareAmbush(void)
{
    s16 lvl = DBZ_GokuLevel() - 2 + (Random() % 4);
    s16 floor = 2 + BadgeCount() * 2;
    if (lvl < floor)
        lvl = floor;
    if (lvl > 100)
        lvl = 100;
    gSpecialVar_0x8004 = (Random() & 1) ? DBZ_ENEMY_SAIYAN_SOLDIER : DBZ_ENEMY_MAJIN_SOLDIER;
    gSpecialVar_0x8005 = lvl;
    gSpecialVar_0x8006 = 0;
    gTrainerBattleOpponent_A = 0;
    StringCopy(gStringVar1, sEnemyKinds[gSpecialVar_0x8004].name);
}

#define AMBUSH_MIN_STEPS 120
#define AMBUSH_CHANCE    300

bool8 DBZ_TryAmbush(void)
{
    if (sAmbushSteps < 0xFFFF)
        sAmbushSteps++;
    if (DBZ_IsFighting() || !FlagGet(FLAG_SYS_POKEMON_GET))
        return FALSE;
    if (!IsMapTypeOutdoors(gMapHeader.mapType) || !TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT))
        return FALSE;
    if (VarGet(VAR_REPEL_STEP_COUNT) != 0)   // a KI HIDER keeps the soldiers off Goku's trail too
        return FALSE;
    if (sAmbushSteps < AMBUSH_MIN_STEPS || (Random() % AMBUSH_CHANCE) != 0)
        return FALSE;
    sAmbushSteps = 0;
    ScriptContext_SetupScript(EventScript_DBZ_Ambush);
    return TRUE;
}

// things that should pop up once Goku is free to move again
bool8 DBZ_TryStartPendingFieldEvent(void)
{
    u16 pending = VarGet(VAR_DBZ_PENDING_FIGHT);
    if (DBZ_IsFighting())
        return FALSE;
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

    sReportCount = 0;
    sReportPos = 0;
    sEvolveCheck = 0;
    if (mons == 0)
        mons = 1;
    gained = GokuGainExp(goku);
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

// STR_VAR_1 level, STR_VAR_2 EXP to next level, STR_VAR_3 max HP
void DBZ_ShowGokuStatus(void)
{
    u8 level = DBZ_GokuLevel();
    u32 next = level >= DBZ_GOKU_MAX_LEVEL ? 0 : GokuExpForLevel(level + 1) - GokuExp();
    ConvertIntToDecimalStringN(gStringVar1, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar2, next, STR_CONV_MODE_LEFT_ALIGN, 7);
    ConvertIntToDecimalStringN(gStringVar3, GokuMaxHp(), STR_CONV_MODE_LEFT_ALIGN, 4);
}
