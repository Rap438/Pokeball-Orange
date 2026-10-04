// PokeBall Orange overworld style: soft drop shadows under characters and time-of-day lighting.
#include "global.h"
#include "dbz.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "gpu_regs.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "palette.h"
#include "rtc.h"
#include "sprite.h"
#include "constants/map_types.h"
#include "constants/rgb.h"

// ================================================================== drop shadows
#define TAG_DBZ_SHADOW_S 0x2F30
#define TAG_DBZ_SHADOW_L 0x2F31
#define TAG_DBZ_SHADOW_PAL 0x2F30

static const u32 sShadowSGfx[] = INCGFX_U32("graphics/dbz/shadow_s.png", ".4bpp");
static const u32 sShadowLGfx[] = INCGFX_U32("graphics/dbz/shadow_l.png", ".4bpp");
extern const u16 gDBZHudPalette[];   // shared: index 1 is the shadow colour

static const struct SpriteSheet sShadowSheetS = { sShadowSGfx, 16 * 8 / 2, TAG_DBZ_SHADOW_S };
static const struct SpriteSheet sShadowSheetL = { sShadowLGfx, 32 * 8 / 2, TAG_DBZ_SHADOW_L };
static const struct SpritePalette sShadowPalette = { gDBZHudPalette, TAG_DBZ_SHADOW_PAL };

static const struct OamData sOam_ShadowS = {
    .objMode = ST_OAM_OBJ_BLEND,
    .shape = SPRITE_SHAPE(16x8),
    .size = SPRITE_SIZE(16x8),
    .priority = 2,
};
static const struct OamData sOam_ShadowL = {
    .objMode = ST_OAM_OBJ_BLEND,
    .shape = SPRITE_SHAPE(32x8),
    .size = SPRITE_SIZE(32x8),
    .priority = 2,
};

static void SpriteCB_DbzShadow(struct Sprite *sprite);

static const struct SpriteTemplate sShadowTemplateS = {
    .tileTag = TAG_DBZ_SHADOW_S, .paletteTag = TAG_DBZ_SHADOW_PAL, .oam = &sOam_ShadowS,
    .anims = gDummySpriteAnimTable, .images = NULL, .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_DbzShadow,
};
static const struct SpriteTemplate sShadowTemplateL = {
    .tileTag = TAG_DBZ_SHADOW_L, .paletteTag = TAG_DBZ_SHADOW_PAL, .oam = &sOam_ShadowL,
    .anims = gDummySpriteAnimTable, .images = NULL, .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_DbzShadow,
};

// EWRAM isn't cleared on soft reset; every id is validated before use
static EWRAM_DATA u8 sShadowSprite[OBJECT_EVENTS_COUNT] = {0};

#define sObjId   data[0]
#define sYOffset data[1]

static void SpriteCB_DbzShadow(struct Sprite *sprite)
{
    struct ObjectEvent *obj = &gObjectEvents[sprite->sObjId];
    struct Sprite *linked;
    if (!obj->active || obj->spriteId >= MAX_SPRITES)
    {
        sprite->invisible = TRUE;
        return;
    }
    linked = &gSprites[obj->spriteId];
    sprite->x = linked->x + linked->x2;
    sprite->y = linked->y + sprite->sYOffset;
    sprite->x2 = 0;
    sprite->y2 = 0;
    sprite->oam.priority = linked->oam.priority;
    sprite->subpriority = linked->subpriority + 1;
    sprite->invisible = (linked->invisible && !(sprite->sObjId == gPlayerAvatar.objectEventId && DBZ_PlayerOverlayActive())) || sprite->data[7];
}

static bool8 WantsShadow(u8 i)
{
    struct ObjectEvent *obj = &gObjectEvents[i];
    const struct ObjectEventGraphicsInfo *info;
    u8 behavior;

    if (!DBZ_OptShadows())
        return FALSE;
    if (!obj->active || obj->spriteId >= MAX_SPRITES || !gSprites[obj->spriteId].inUse)
        return FALSE;
    if (obj->invisible && !(i == gPlayerAvatar.objectEventId && DBZ_PlayerOverlayActive()))
        return FALSE;
    if (obj->hasShadow)   // the game's own jump shadow is showing
        return FALSE;
    info = GetObjectEventGraphicsInfo(obj->graphicsId);
    if (info->inanimate || info->width > 32)
        return FALSE;
    if (i == gPlayerAvatar.objectEventId && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING | PLAYER_AVATAR_FLAG_UNDERWATER))
        return FALSE;
    behavior = obj->currentMetatileBehavior;
    if (MetatileBehavior_IsPokeGrass(behavior) || MetatileBehavior_IsLongGrass(behavior)
     || MetatileBehavior_IsSurfableWaterOrUnderwater(behavior) || MetatileBehavior_IsReflective(behavior)
     || MetatileBehavior_IsDeepSand(behavior))
        return FALSE;
    return TRUE;
}

static bool8 ShadowValid(u8 i)
{
    u8 id = sShadowSprite[i];
    return id < MAX_SPRITES && gSprites[id].inUse && gSprites[id].callback == SpriteCB_DbzShadow && gSprites[id].sObjId == i;
}

void DBZ_UpdateShadows(void)
{
    u8 i;
    if (!IsMapTypeOutdoors(gMapHeader.mapType) && gMapHeader.mapType != MAP_TYPE_INDOOR && gMapHeader.mapType != MAP_TYPE_UNDERGROUND)
        return;
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        bool8 want = WantsShadow(i);
        bool8 valid = ShadowValid(i);
        if (!want)
        {
            if (valid)
                gSprites[sShadowSprite[i]].data[7] = 1;   // hidden
            if (!gObjectEvents[i].active)
            {
                if (valid)
                    DestroySprite(&gSprites[sShadowSprite[i]]);
                sShadowSprite[i] = MAX_SPRITES;
            }
            continue;
        }
        if (valid)
            gSprites[sShadowSprite[i]].data[7] = 0;
        if (!valid)
        {
            const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(gObjectEvents[i].graphicsId);
            bool8 large = info->width > 16;
            u8 id;
            if (GetSpriteTileStartByTag(large ? TAG_DBZ_SHADOW_L : TAG_DBZ_SHADOW_S) == 0xFFFF)
                LoadSpriteSheet(large ? &sShadowSheetL : &sShadowSheetS);
            if (IndexOfSpritePaletteTag(TAG_DBZ_SHADOW_PAL) == 0xFF)
                LoadSpritePalette(&sShadowPalette);
            id = CreateSprite(large ? &sShadowTemplateL : &sShadowTemplateS, 0, 0, 0);
            sShadowSprite[i] = id;
            if (id != MAX_SPRITES)
            {
                gSprites[id].coordOffsetEnabled = TRUE;
                gSprites[id].sObjId = i;
                gSprites[id].sYOffset = info->height / 2 - 3;
                SpriteCB_DbzShadow(&gSprites[id]);
            }
        }
    }
    // shadows are semi-transparent: blend them over the map layers (unless weather is using the blender)
    if (DBZ_OptShadows() && GetGpuReg(REG_OFFSET_BLDCNT) == 0)
    {
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_BD);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(7, 12));
    }
}

#undef sObjId
#undef sYOffset

// ================================================================== time of day
enum {
    TOD_DAY,
    TOD_MORNING,
    TOD_EVENING,
    TOD_NIGHT,
};

// per-channel multipliers (/256) and additions
static const u16 sTodTint[][6] = {
    [TOD_DAY]     = {256, 256, 256, 0, 0, 0},
    [TOD_MORNING] = {256, 242, 222, 1, 0, 0},
    [TOD_EVENING] = {256, 222, 184, 1, 0, 0},
    [TOD_NIGHT]   = {128, 140, 190, 0, 0, 2},
};

static u8 TimeOfDay(void)
{
    u8 h;
    u16 force = VarGet(VAR_DBZ_TOD_OVERRIDE);
    if (force >= 1 && force <= 4)
        return force - 1;
    if (RtcGetErrorStatus() & RTC_ERR_FLAG_MASK)
        return TOD_DAY;
    RtcCalcLocalTime();
    h = gLocalTime.hours;
    if (h >= 20 || h < 5)
        return TOD_NIGHT;
    if (h < 8)
        return TOD_MORNING;
    if (h >= 17)
        return TOD_EVENING;
    return TOD_DAY;
}

static bool8 MapGetsDaylight(void)
{
    return IsMapTypeOutdoors(gMapHeader.mapType);
}

static u16 TintColor(u16 c, const u16 *t)
{
    u32 r = (c & 31) * t[0] / 256 + t[3];
    u32 g = ((c >> 5) & 31) * t[1] / 256 + t[4];
    u32 b = ((c >> 10) & 31) * t[2] / 256 + t[5];
    if (r > 31) r = 31;
    if (g > 31) g = 31;
    if (b > 31) b = 31;
    return r | (g << 5) | (b << 10);
}

// Tints palette entries [offset, offset+count) in both palette buffers.
void DBZ_ApplyTimeTint(u16 offset, u16 count)
{
    u8 tod;
    u16 i;
    if (!MapGetsDaylight() || !DBZ_OptDayNight())
        return;
    tod = TimeOfDay();
    if (tod == TOD_DAY)
        return;
    for (i = 0; i < count; i++)
    {
        gPlttBufferUnfaded[offset + i] = TintColor(gPlttBufferUnfaded[offset + i], sTodTint[tod]);
        gPlttBufferFaded[offset + i] = gPlttBufferUnfaded[offset + i];
    }
}

// Field-effect palettes can be "loaded" again while already resident, so only tint them once.
static EWRAM_DATA u16 sObjTintMark[16][2] = {0};

void DBZ_ApplyTimeTintToObjSlotOnce(u8 slot)
{
    u16 off = OBJ_PLTT_ID(slot);
    if (sObjTintMark[slot][0] == (gPlttBufferUnfaded[off + 1] | 0x8000) && sObjTintMark[slot][1] == gPlttBufferUnfaded[off + 2])
        return;
    DBZ_ApplyTimeTint(off, 16);
    sObjTintMark[slot][0] = gPlttBufferUnfaded[off + 1] | 0x8000;
    sObjTintMark[slot][1] = gPlttBufferUnfaded[off + 2];
}
