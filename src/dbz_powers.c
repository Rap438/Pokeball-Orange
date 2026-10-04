// PokeBall Orange: Goku's overworld powers.
// R            : power up to the next unlocked Super Saiyan form (wraps back to base)
// L            : fire the selected special (KI BLAST: tap, KAMEHAMEHA: hold to charge, then let go)
// hold L + R   : swap the selected special
// SSJ  + Kamehameha cuts trees, SSJ2 smashes rocks, SSJ3 pushes boulders.
#include "global.h"
#include "dbz.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_control_avatar.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "palette.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "util.h"
#include "constants/rgb.h"
#include "constants/event_objects.h"
#include "constants/field_effects.h"
#include "constants/flags.h"
#include "constants/songs.h"
#include "constants/vars.h"

extern const u8 EventScript_DBZ_PowerUp[];
extern const u8 EventScript_DBZ_Fire[];
extern const u8 EventScript_DBZ_Debug[];

#define TAG_DBZ_FX      0x2F00
#define TAG_DBZ_AURA    0x2F01
#define TAG_DBZ_FX_PAL  0x2F00

#define KAME_CHARGE_FRAMES 36
#define KI_RANGE   5
#define KAME_RANGE 7

enum {
    FX_KI_0, FX_KI_1, FX_BEAM_H0, FX_BEAM_H1, FX_BEAM_V0, FX_BEAM_V1,
    FX_HEAD_0, FX_HEAD_1, FX_IMPACT_0, FX_IMPACT_1, FX_CHARGE_0, FX_CHARGE_1,
    FX_SPARK_0, FX_SPARK_1, FX_COUNT
};

static const u32 sDbzFxGfx[] = INCGFX_U32("graphics/dbz/fx.png", ".4bpp", "-mwidth 2 -mheight 2");
static const u16 sDbzFxPal[] = INCGFX_U16("graphics/dbz/fx.png", ".gbapal");
static const u32 sDbzAuraGfx[] = INCGFX_U32("graphics/dbz/aura.png", ".4bpp", "-mwidth 4 -mheight 4");

static const struct SpriteSheet sDbzFxSheet = { sDbzFxGfx, FX_COUNT * 128, TAG_DBZ_FX };
static const struct SpriteSheet sDbzAuraSheet = { sDbzAuraGfx, 3 * 512, TAG_DBZ_AURA };
static const struct SpritePalette sDbzFxPalette = { sDbzFxPal, TAG_DBZ_FX_PAL };

static const struct OamData sOam_Fx16 = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 1,
};

static const struct OamData sOam_Aura32 = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 2,
};

static void SpriteCB_ChargeGlow(struct Sprite *sprite);

static const struct SpriteTemplate sSpriteTemplate_Fx = {
    .tileTag = TAG_DBZ_FX,
    .paletteTag = TAG_DBZ_FX_PAL,
    .oam = &sOam_Fx16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSpriteTemplate_Glow = {
    .tileTag = TAG_DBZ_FX,
    .paletteTag = TAG_DBZ_FX_PAL,
    .oam = &sOam_Fx16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_ChargeGlow,
};

static const struct SpriteTemplate sSpriteTemplate_Aura = {
    .tileTag = TAG_DBZ_AURA,
    .paletteTag = TAG_DBZ_FX_PAL,
    .oam = &sOam_Aura32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static EWRAM_DATA u8 sChargeFrames = 0;
static EWRAM_DATA u8 sGlowSpriteId = MAX_SPRITES;
static EWRAM_DATA u8 sHandlerTick = 0;
static EWRAM_DATA bool8 sFightBlast = FALSE;   // blast fired during an overworld fight (no script)

// ------------------------------------------------------------------ forms
u8 DBZ_GetForm(void)
{
    u16 form = VarGet(VAR_DBZ_FORM);
    if (form >= DBZ_FORM_COUNT)
        form = DBZ_FORM_BASE;
    if (form > DBZ_GetMaxForm())
        form = DBZ_GetMaxForm();
    return form;
}

u8 DBZ_GetMaxForm(void)
{
    if (FlagGet(FLAG_BADGE04_GET))
        return DBZ_FORM_SSJ3;
    if (FlagGet(FLAG_BADGE03_GET))
        return DBZ_FORM_SSJ2;
    if (FlagGet(FLAG_BADGE01_GET))
        return DBZ_FORM_SSJ;
    return DBZ_FORM_BASE;
}

static u8 FormToGfx(u8 form)
{
    switch (form)
    {
    case DBZ_FORM_SSJ:  return OBJ_EVENT_GFX_GOKU_SSJ;
    case DBZ_FORM_SSJ2: return OBJ_EVENT_GFX_GOKU_SSJ2;
    case DBZ_FORM_SSJ3: return OBJ_EVENT_GFX_GOKU_SSJ3;
    default:            return OBJ_EVENT_GFX_BRENDAN_NORMAL;
    }
}

u8 DBZ_GetPlayerNormalGfx(void)
{
    return FormToGfx(DBZ_GetForm());
}

u16 DBZ_GetFormSpecial(void)
{
    return DBZ_GetForm();
}

// returns the form R would switch to, or 0xFF if Goku can't transform yet
u16 DBZ_GetNextForm(void)
{
    u8 max = DBZ_GetMaxForm();
    u8 cur = DBZ_GetForm();
    if (max == DBZ_FORM_BASE)
        return 0xFF;
    if (cur >= max)
        return DBZ_FORM_BASE;
    return cur + 1;
}

// after a badge: returns a form that just became available and hasn't been hinted yet, else 0
u16 DBZ_GetNewFormHint(void)
{
    u8 max = DBZ_GetMaxForm();
    u16 seen = VarGet(VAR_DBZ_SEEN_FORMS);
    u8 f;
    for (f = DBZ_FORM_SSJ; f <= max; f++)
    {
        if (!(seen & (0x10 << f)))
        {
            VarSet(VAR_DBZ_SEEN_FORMS, seen | (0x10 << f));
            return DBZ_OptHints() ? f : 0;
        }
    }
    return 0;
}

// returns the form number the first time it is reached (for the intro message), else 0
u16 DBZ_ShouldShowFormIntro(void)
{
    u8 form = DBZ_GetForm();
    u16 seen = VarGet(VAR_DBZ_SEEN_FORMS);
    if (form == DBZ_FORM_BASE || (seen & (1 << form)))
        return 0;
    VarSet(VAR_DBZ_SEEN_FORMS, seen | (1 << form));
    return DBZ_OptHints() ? form : 0;
}

// ------------------------------------------------------------------ helpers
static void LoadFxGraphics(void)
{
    if (GetSpriteTileStartByTag(TAG_DBZ_FX) == 0xFFFF)
        LoadSpriteSheet(&sDbzFxSheet);
    if (IndexOfSpritePaletteTag(TAG_DBZ_FX_PAL) == 0xFF)
        LoadSpritePalette(&sDbzFxPalette);
}

static void LoadAuraGraphics(void)
{
    if (GetSpriteTileStartByTag(TAG_DBZ_AURA) == 0xFFFF)
        LoadSpriteSheet(&sDbzAuraSheet);
    if (IndexOfSpritePaletteTag(TAG_DBZ_FX_PAL) == 0xFF)
        LoadSpritePalette(&sDbzFxPalette);
}

static void FreeFxGraphicsIfUnused(void)
{
    u8 i;
    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse && (gSprites[i].template == &sSpriteTemplate_Fx
                               || gSprites[i].template == &sSpriteTemplate_Glow
                               || gSprites[i].template == &sSpriteTemplate_Aura))
            return;
    }
    FreeSpriteTilesByTag(TAG_DBZ_FX);
    FreeSpriteTilesByTag(TAG_DBZ_AURA);
    FreeSpritePaletteByTag(TAG_DBZ_FX_PAL);
}

static void SetFxFrame(struct Sprite *sprite, u8 frame)
{
    sprite->oam.tileNum = GetSpriteTileStartByTag(TAG_DBZ_FX) + frame * 4;
}

static struct Sprite *PlayerSprite(void)
{
    return &gSprites[gPlayerAvatar.spriteId];
}

static void DirToVec(u8 dir, s16 *dx, s16 *dy)
{
    *dx = 0; *dy = 0;
    switch (dir)
    {
    case DIR_SOUTH: *dy = 1; break;
    case DIR_NORTH: *dy = -1; break;
    case DIR_WEST:  *dx = -1; break;
    default:        *dx = 1; break;
    }
}

static u8 CreateFxSprite(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority)
{
    u8 spriteId = CreateSprite(template, x, y, subpriority);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].coordOffsetEnabled = TRUE;
        if (template == &sSpriteTemplate_Fx || template == &sSpriteTemplate_Glow)
            SetFxFrame(&gSprites[spriteId], FX_KI_0);
    }
    return spriteId;
}

// position of Goku's hands, in sprite space
static void HandsPos(u8 dir, s16 *x, s16 *y)
{
    struct Sprite *ps = PlayerSprite();
    s16 dx, dy;
    DirToVec(dir, &dx, &dy);
    *x = ps->x + ps->x2 + dx * 9;
    *y = ps->y + ps->y2 + 4 + dy * 8;
    if (dir == DIR_NORTH)
        *y -= 4;
}

// ------------------------------------------------------------------ field input
static void SpriteCB_ChargeGlow(struct Sprite *sprite)
{
    s16 x, y;
    if (++sHandlerTick > 3 || sChargeFrames == 0)
    {
        // the field input handler stopped running (script, warp...) -> cancel the charge
        sChargeFrames = 0;
        sGlowSpriteId = MAX_SPRITES;
        DestroySprite(sprite);
        FreeFxGraphicsIfUnused();
        return;
    }
    HandsPos(GetPlayerFacingDirection(), &x, &y);
    sprite->x = x;
    sprite->y = y;
    if (DBZ_GetSelectedMove() == 0)
        SetFxFrame(sprite, (sprite->data[0]++ / 4) % 2 ? FX_KI_1 : FX_KI_0);
    else if (sChargeFrames >= KAME_CHARGE_FRAMES)
        SetFxFrame(sprite, (sprite->data[0]++ / 4) % 2 ? FX_HEAD_1 : FX_HEAD_0);
    else
        SetFxFrame(sprite, (sprite->data[0]++ / 4) % 2 ? FX_CHARGE_1 : FX_CHARGE_0);
}

void DBZ_ResetFieldInputState(void)
{
    sChargeFrames = 0;
    // EWRAM isn't initialised at boot, so only trust the id if it really is our glow sprite
    if (sGlowSpriteId < MAX_SPRITES && gSprites[sGlowSpriteId].inUse && gSprites[sGlowSpriteId].callback == SpriteCB_ChargeGlow)
        DestroySprite(&gSprites[sGlowSpriteId]);
    sGlowSpriteId = MAX_SPRITES;
}

bool8 DBZ_IsChargingBlast(void)
{
    return sChargeFrames != 0;
}

u8 DBZ_GetSelectedMove(void)
{
    return VarGet(VAR_DBZ_MISC) & 1;
}

// 0..16 for the HUD meter (a ki blast is ready instantly)
u8 DBZ_GetChargeLevel(void)
{
    if (sChargeFrames == 0)
        return 0;
    if (DBZ_GetSelectedMove() == 0)
        return 16;
    if (sChargeFrames >= KAME_CHARGE_FRAMES)
        return 16;
    return sChargeFrames * 16 / KAME_CHARGE_FRAMES;
}

bool8 DBZ_IsFightBlastActive(void)
{
    return sFightBlast;
}

// ------------------------------------------------------------------ fx helpers for the fight engine
static void SpriteCB_Impact(struct Sprite *sprite)
{
    SetFxFrame(sprite, FX_IMPACT_0 + (sprite->data[0] / 3) % 2);
    if (++sprite->data[0] > 12)
    {
        DestroySprite(sprite);
        FreeFxGraphicsIfUnused();
    }
}

static void SpriteCB_Spark(struct Sprite *sprite)
{
    struct Sprite *ps = PlayerSprite();
    sprite->x = ps->x + ps->x2 + sprite->data[1];
    sprite->y = ps->y + ps->y2 + sprite->data[2];
    sprite->subpriority = ps->subpriority - 1;
    sprite->oam.priority = ps->oam.priority;
    SetFxFrame(sprite, FX_SPARK_0 + (sprite->data[0] / 2) % 2);
    sprite->invisible = ps->invisible;
    if (++sprite->data[0] > 7)
    {
        DestroySprite(sprite);
        FreeFxGraphicsIfUnused();
    }
}

// SSJ2 crackles with lightning while Goku walks around
static EWRAM_DATA u8 sSparkTimer = 0;
void DBZ_UpdateFormFx(void)
{
    u8 id;
    struct Sprite *ps;
    if (gPlayerAvatar.spriteId >= MAX_SPRITES || !gSprites[gPlayerAvatar.spriteId].inUse)
        return;
    if (DBZ_GetForm() != DBZ_FORM_SSJ2 || !TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT) || !DBZ_OptSparks())
        return;
    if (++sSparkTimer < 22 + (Random() % 30))
        return;
    sSparkTimer = 0;
    ps = PlayerSprite();
    LoadFxGraphics();
    id = CreateFxSprite(&sSpriteTemplate_Fx, ps->x, ps->y, 0);
    if (id == MAX_SPRITES)
        return;
    gSprites[id].data[0] = 0;
    gSprites[id].data[1] = (s16)(Random() % 17) - 8;
    gSprites[id].data[2] = (s16)(Random() % 20) - 14;
    gSprites[id].callback = SpriteCB_Spark;
    SpriteCB_Spark(&gSprites[id]);
    if ((Random() % 8) == 0)
        PlaySE(SE_M_THUNDERBOLT2);
}

void DBZ_SpawnImpactAt(s16 x, s16 y)
{
    u8 id;
    LoadFxGraphics();
    id = CreateFxSprite(&sSpriteTemplate_Fx, x, y, 0);
    if (id != MAX_SPRITES)
    {
        gSprites[id].data[0] = 0;
        gSprites[id].callback = SpriteCB_Impact;
        SetFxFrame(&gSprites[id], FX_IMPACT_0);
    }
}

u8 DBZ_CreateKiSprite(s16 x, s16 y)
{
    LoadFxGraphics();
    return CreateFxSprite(&sSpriteTemplate_Fx, x, y, 0);
}

void DBZ_AnimateKiSprite(u8 spriteId, u8 t)
{
    SetFxFrame(&gSprites[spriteId], (t / 2) % 2 ? FX_KI_1 : FX_KI_0);
}

void DBZ_DestroyFxSprite(u8 spriteId)
{
    if (spriteId < MAX_SPRITES && gSprites[spriteId].inUse)
        DestroySprite(&gSprites[spriteId]);
    FreeFxGraphicsIfUnused();
}

// input_field_1_0 = R pressed, input_field_1_1 = L pressed, input_field_1_2 = L held
bool8 DBZ_HandleFieldInput(struct FieldInput *input)
{
    s16 x, y;
    sHandlerTick = 0;

    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT))
    {
        if (sChargeFrames)
            DBZ_ResetFieldInputState();
        return FALSE;
    }

    if (sChargeFrames == 0)
    {
#ifdef DBZ_DEBUG
        if (input->input_field_1_3)
        {
            ScriptContext_SetupScript(EventScript_DBZ_Debug);
            return TRUE;
        }
#endif
        if (input->input_field_1_0)
        {
            ScriptContext_SetupScript(EventScript_DBZ_PowerUp);
            return TRUE;
        }
        if (input->input_field_1_1)
        {
            sChargeFrames = 1;
            LoadFxGraphics();
            HandsPos(GetPlayerFacingDirection(), &x, &y);
            sGlowSpriteId = CreateFxSprite(&sSpriteTemplate_Glow, x, y, 0);
            return FALSE;
        }
        return FALSE;
    }

    // hold L + tap R: swap the selected special move
    if (input->input_field_1_0)
    {
        VarSet(VAR_DBZ_MISC, VarGet(VAR_DBZ_MISC) ^ 1);
        DBZ_ResetFieldInputState();
        PlaySE(SE_SELECT);
        return FALSE;
    }

    if (input->input_field_1_2)
    {
        if (sChargeFrames < 250)
            sChargeFrames++;
        if (DBZ_GetSelectedMove() == 1)
        {
            if (sChargeFrames == 8)
                PlaySE(SE_M_CHARGE);
            if (sChargeFrames == KAME_CHARGE_FRAMES)
                PlaySE(SE_M_DETECT);
        }
        return FALSE;
    }

    // released: fire the selected move (an under-charged Kamehameha fizzles)
    if (DBZ_GetSelectedMove() == 1 && sChargeFrames < KAME_CHARGE_FRAMES)
    {
        DBZ_ResetFieldInputState();
        PlaySE(SE_FAILURE);
        return FALSE;
    }
    gSpecialVar_0x8004 = DBZ_GetSelectedMove();
    DBZ_ResetFieldInputState();
    if (DBZ_IsFighting())
    {
        sFightBlast = TRUE;
        DBZ_FireBlast();
        return TRUE;
    }
    ScriptContext_SetupScript(EventScript_DBZ_Fire);
    return TRUE;
}

// ------------------------------------------------------------------ transformation
#define tTimer   data[0]
#define tSprite  data[1]

static void Task_Aura(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    struct Sprite *ps = PlayerSprite();
    struct Sprite *aura = &gSprites[tSprite];
    u8 coeff;

    aura->x = ps->x + ps->x2;
    aura->y = ps->y + ps->y2 + 2;
    aura->subpriority = ps->subpriority + 1;
    aura->oam.tileNum = GetSpriteTileStartByTag(TAG_DBZ_AURA) + ((tTimer / 3) % 3) * 16;
    aura->invisible = (tTimer % 2) && tTimer > 40;

    coeff = (tTimer % 8) < 4 ? (tTimer % 8) * 2 : (8 - (tTimer % 8)) * 2;
    BlendPalette(OBJ_PLTT_ID(0), 16, coeff, RGB(31, 28, 8));

    if (++tTimer > 54)
    {
        BlendPalette(OBJ_PLTT_ID(0), 16, 0, RGB_WHITE);
        DestroySprite(aura);
        FreeFxGraphicsIfUnused();
        DestroyTask(taskId);
    }
}

void DBZ_StartAura(void)
{
    u8 taskId;
    struct Sprite *ps = PlayerSprite();
    LoadAuraGraphics();
    taskId = CreateTask(Task_Aura, 80);
    gTasks[taskId].tTimer = 0;
    gTasks[taskId].tSprite = CreateFxSprite(&sSpriteTemplate_Aura, ps->x, ps->y + 2, ps->subpriority + 1);
    PlaySE(SE_M_MEGA_KICK);
}

static void Task_FormFlash(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (tTimer < 8)
        BlendPalettes(PALETTES_ALL, 16 - tTimer * 2, RGB_WHITE);
    else
    {
        BlendPalettes(PALETTES_ALL, 0, RGB_WHITE);
        DestroyTask(taskId);
        ScriptContext_Enable();
        return;
    }
    tTimer++;
}

void DBZ_ApplyNextForm(void)
{
    u16 next = DBZ_GetNextForm();
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    u8 taskId;

    if (next == 0xFF)
        next = DBZ_FORM_BASE;
    VarSet(VAR_DBZ_FORM, next);
    ObjectEventSetGraphicsId(player, FormToGfx(next));
    ObjectEventTurn(player, player->facingDirection);
    PlaySE(next == DBZ_FORM_BASE ? SE_M_MINIMIZE : SE_M_SWAGGER);
    taskId = CreateTask(Task_FormFlash, 80);
    gTasks[taskId].tTimer = 0;
}

#undef tTimer
#undef tSprite

// ------------------------------------------------------------------ ki blast / kamehameha
#define tState     data[0]
#define tTimer     data[1]
#define tKame      data[2]
#define tDir       data[3]
#define tMaxLen    data[4]
#define tLen       data[5]
#define tHitType   data[6]
#define tHitLocal  data[7]
#define tHead      data[8]
#define tGlow      data[9]
#define tSegs      10   // data[10..15] segment sprite ids
#define NUM_SEGS   6

static void ComputeBlastPath(u8 dir, u8 maxTiles, s16 *lenPx, s16 *hitType, s16 *hitLocal)
{
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 x = player->currentCoords.x;
    s16 y = player->currentCoords.y;
    s16 dx, dy;
    u8 i, objId;

    DirToVec(dir, &dx, &dy);
    *hitType = DBZ_HIT_NOTHING;
    *hitLocal = 0;
    for (i = 1; i <= maxTiles; i++)
    {
        x += dx;
        y += dy;
        objId = GetObjectEventIdByXY(x, y);
        if (objId != OBJECT_EVENTS_COUNT && objId != gPlayerAvatar.objectEventId && !gObjectEvents[objId].invisible)
        {
            switch (gObjectEvents[objId].graphicsId)
            {
            case OBJ_EVENT_GFX_CUTTABLE_TREE:   *hitType = DBZ_HIT_TREE; break;
            case OBJ_EVENT_GFX_BREAKABLE_ROCK:  *hitType = DBZ_HIT_ROCK; break;
            case OBJ_EVENT_GFX_PUSHABLE_BOULDER: *hitType = DBZ_HIT_BOULDER; break;
            default:                            *hitType = DBZ_HIT_OBJECT; break;
            }
            *hitLocal = gObjectEvents[objId].localId;
            *lenPx = i * 16 - 6;
            return;
        }
        if (MapGridGetCollisionAt(x, y))
        {
            *hitType = DBZ_HIT_WALL;
            *lenPx = i * 16 - 10;
            return;
        }
    }
    *lenPx = maxTiles * 16;
}

static void BlastFinish(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i;
    if (tHead != MAX_SPRITES) DestroySprite(&gSprites[tHead]);
    if (tGlow != MAX_SPRITES) DestroySprite(&gSprites[tGlow]);
    for (i = 0; i < NUM_SEGS; i++)
        if (data[tSegs + i] != MAX_SPRITES)
            DestroySprite(&gSprites[data[tSegs + i]]);
    FreeFxGraphicsIfUnused();
    if (sFightBlast)
    {
        sFightBlast = FALSE;
        DBZ_FightOnBlast(tHitType, tHitLocal, tKame);
        DestroyTask(taskId);
        return;
    }
    gSpecialVar_Result = tHitType;
    gSpecialVar_LastTalked = tHitLocal;
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void PlaceAlongBeam(struct Sprite *sprite, u8 dir, s16 dist)
{
    s16 x, y, dx, dy;
    HandsPos(dir, &x, &y);
    DirToVec(dir, &dx, &dy);
    sprite->x = x + dx * dist;
    sprite->y = y + dy * dist;
}

static void Task_Blast(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i;
    s16 x, y;
    bool8 vertical = (tDir == DIR_NORTH || tDir == DIR_SOUTH);

    switch (tState)
    {
    case 0: // charge
        if (tTimer == 0)
        {
            HandsPos(tDir, &x, &y);
            tGlow = CreateFxSprite(&sSpriteTemplate_Fx, x, y, 0);
            PlaySE(tKame ? SE_M_CHARGE : SE_M_SWIFT);
        }
        if (tGlow != MAX_SPRITES)
            SetFxFrame(&gSprites[tGlow], (tTimer / 3) % 2 ? (tKame ? FX_HEAD_1 : FX_KI_1) : (tKame ? FX_HEAD_0 : FX_KI_0));
        if (++tTimer >= (tKame ? 8 : 4))
        {
            tTimer = 0;
            tState = 1;
            if (tGlow != MAX_SPRITES)
            {
                DestroySprite(&gSprites[tGlow]);
                tGlow = MAX_SPRITES;
            }
            HandsPos(tDir, &x, &y);
            tHead = CreateFxSprite(&sSpriteTemplate_Fx, x, y, 0);
            if (tKame)
            {
                PlaySE(SE_M_HYPER_BEAM);
                for (i = 0; i < NUM_SEGS; i++)
                {
                    data[tSegs + i] = CreateFxSprite(&sSpriteTemplate_Fx, x, y, 1);
                    if (data[tSegs + i] != MAX_SPRITES)
                        gSprites[data[tSegs + i]].invisible = TRUE;
                }
            }
        }
        break;
    case 1: // travel
        tLen += tKame ? 6 : 5;
        if (tLen > tMaxLen)
            tLen = tMaxLen;
        if (tHead != MAX_SPRITES)
        {
            PlaceAlongBeam(&gSprites[tHead], tDir, tLen);
            SetFxFrame(&gSprites[tHead], (tTimer / 2) % 2 ? (tKame ? FX_HEAD_1 : FX_KI_1) : (tKame ? FX_HEAD_0 : FX_KI_0));
        }
        if (tKame)
        {
            for (i = 0; i < NUM_SEGS; i++)
            {
                struct Sprite *seg;
                s16 d = 8 + i * 16;
                if (data[tSegs + i] == MAX_SPRITES)
                    continue;
                seg = &gSprites[data[tSegs + i]];
                seg->invisible = (d > tLen - 4);
                PlaceAlongBeam(seg, tDir, d);
                SetFxFrame(seg, (vertical ? FX_BEAM_V0 : FX_BEAM_H0) + ((tTimer / 2) % 2));
            }
        }
        tTimer++;
        if (tLen >= tMaxLen)
        {
            tState = 2;
            tTimer = 0;
            if (tHitType != DBZ_HIT_NOTHING)
                PlaySE(tHitType == DBZ_HIT_ROCK ? SE_M_ROCK_THROW : SE_M_EXPLOSION);
        }
        break;
    case 2: // hold + impact
        if (tHead != MAX_SPRITES)
            SetFxFrame(&gSprites[tHead], tHitType != DBZ_HIT_NOTHING ? (FX_IMPACT_0 + (tTimer / 4) % 2)
                                                                      : (tKame ? FX_HEAD_0 + (tTimer / 2) % 2 : FX_KI_0));
        if (tKame)
        {
            for (i = 0; i < NUM_SEGS; i++)
                if (data[tSegs + i] != MAX_SPRITES)
                    SetFxFrame(&gSprites[data[tSegs + i]], (vertical ? FX_BEAM_V0 : FX_BEAM_H0) + ((tTimer / 2) % 2));
        }
        if (++tTimer >= (tKame ? 22 : 10))
        {
            tState = 3;
            tTimer = 0;
        }
        break;
    case 3: // fade out: shrink the beam back toward Goku
        if (tKame)
        {
            for (i = 0; i < NUM_SEGS; i++)
                if (data[tSegs + i] != MAX_SPRITES && (NUM_SEGS - 1 - i) < tTimer)
                    gSprites[data[tSegs + i]].invisible = TRUE;
        }
        if (tHead != MAX_SPRITES)
            gSprites[tHead].invisible = (tTimer % 2);
        if (++tTimer > (tKame ? NUM_SEGS + 2 : 2))
            BlastFinish(taskId);
        break;
    }
}

void DBZ_FireBlast(void)
{
    u8 taskId = CreateTask(Task_Blast, 80);
    s16 *data = gTasks[taskId].data;
    u8 i;

    LoadFxGraphics();
    tState = 0;
    tTimer = 0;
    tKame = (gSpecialVar_0x8004 != 0);
    tDir = GetPlayerFacingDirection();
    tLen = 0;
    tHead = MAX_SPRITES;
    tGlow = MAX_SPRITES;
    for (i = 0; i < NUM_SEGS; i++)
        data[tSegs + i] = MAX_SPRITES;
    ComputeBlastPath(tDir, tKame ? KAME_RANGE : KI_RANGE, &tMaxLen, &tHitType, &tHitLocal);
    if (tKame && tMaxLen > NUM_SEGS * 16 + 8)
        tMaxLen = NUM_SEGS * 16 + 8;
}
