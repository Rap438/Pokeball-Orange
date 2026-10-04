// PokeBall Orange: talking portraits. A Buu's Fury style 64x64 portrait sits above the field message box
// while someone from the cast is talking. The speaker comes from a "NAME:" at the start of the message,
// or else from the look of the person Goku is talking to.
#include "global.h"
#include "dbz.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_message_box.h"
#include "main.h"
#include "overworld.h"
#include "palette.h"
#include "script.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "constants/event_objects.h"
#include "constants/characters.h"

#include "data/dbz_portraits.h"

#define TAG_PORTRAIT 0x2F50

static EWRAM_DATA u8 sPortraitSprite = 0;
static EWRAM_DATA u8 sPortraitShown = 0;   // portrait id + 1 (EWRAM isn't cleared at boot: checked with the sprite)

static void SpriteCB_Portrait(struct Sprite *sprite) { }

static const struct OamData sOam_Portrait = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 0,
};

static const struct SpriteTemplate sPortraitTemplate = {
    .tileTag = TAG_PORTRAIT, .paletteTag = TAG_PORTRAIT, .oam = &sOam_Portrait,
    .anims = gDummySpriteAnimTable, .images = NULL, .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Portrait,
};

// ------------------------------------------------------------------ who is talking
static const u8 sName_Vegeta[] = _("VEGETA:");
static const u8 sName_Vegito[] = _("VEGITO:");
static const u8 sName_KidBuu[] = _("KID BUU:");
static const u8 sName_Gero[] = _("DR. GERO:");
static const u8 sName_Satan[] = _("MR. SATAN:");
static const u8 sName_Announcer[] = _("ANNOUNCER:");
static const u8 sName_Dende[] = _("DENDE:");
static const u8 sName_Jackie[] = _("JACKIE CHUN:");
static const u8 sName_Mask[] = _("MIGHTY MASK:");
static const u8 sName_ChiChi[] = _("CHI-CHI:");
static const u8 sName_Bulma[] = _("BULMA:");
static const u8 sName_Roshi[] = _("ROSHI:");
static const u8 sName_MasterRoshi[] = _("MASTER ROSHI:");
static const u8 sName_Krillin[] = _("KRILLIN:");
static const u8 sName_Piccolo[] = _("PICCOLO:");
static const u8 sName_Kai[] = _("SUPREME KAI:");
static const u8 sName_Uub[] = _("UUB:");
static const u8 sName_Brief[] = _("DR. BRIEF:");

static const struct { const u8 *name; u8 portrait; } sNamePortraits[] = {
    { sName_Vegeta, PORTRAIT_VEGETA },
    { sName_Vegito, PORTRAIT_VEGITO },
    { sName_KidBuu, PORTRAIT_KID_BUU },
    { sName_Gero, PORTRAIT_GERO },
    { sName_Satan, PORTRAIT_SATAN },
    { sName_Announcer, PORTRAIT_ANNOUNCER },
    { sName_Dende, PORTRAIT_DENDE },
    { sName_Jackie, PORTRAIT_ROSHI },
    { sName_Mask, PORTRAIT_GOTEN },
    { sName_ChiChi, PORTRAIT_CHICHI },
    { sName_Bulma, PORTRAIT_BULMA },
    { sName_Roshi, PORTRAIT_ROSHI },
    { sName_MasterRoshi, PORTRAIT_ROSHI },
    { sName_Krillin, PORTRAIT_KRILLIN },
    { sName_Piccolo, PORTRAIT_PICCOLO },
    { sName_Kai, PORTRAIT_KAI },
    { sName_Uub, PORTRAIT_UUB },
    { sName_Brief, PORTRAIT_BRIEF },
};

static const struct { u8 gfx; u8 portrait; } sGfxPortraits[] = {
    { OBJ_EVENT_GFX_MOM, PORTRAIT_CHICHI },
    { OBJ_EVENT_GFX_PROF_BIRCH, PORTRAIT_ROSHI },
    { OBJ_EVENT_GFX_NORMAN, PORTRAIT_KRILLIN },
    { OBJ_EVENT_GFX_STEVEN, PORTRAIT_KAI },
    { OBJ_EVENT_GFX_WALLY, PORTRAIT_UUB },
    { OBJ_EVENT_GFX_SCOTT, PORTRAIT_ANNOUNCER },
    { OBJ_EVENT_GFX_WALLACE, PORTRAIT_SATAN },
    { OBJ_EVENT_GFX_ROXANNE, PORTRAIT_YAMCHA },
    { OBJ_EVENT_GFX_BRAWLY, PORTRAIT_TIEN },
    { OBJ_EVENT_GFX_WATTSON, PORTRAIT_OX_KING },
    { OBJ_EVENT_GFX_FLANNERY, PORTRAIT_VIDEL },
    { OBJ_EVENT_GFX_WINONA, PORTRAIT_SAIYAMAN },
    { OBJ_EVENT_GFX_TATE, PORTRAIT_GOTEN },
    { OBJ_EVENT_GFX_LIZA, PORTRAIT_TRUNKS },
    { OBJ_EVENT_GFX_JUAN, PORTRAIT_PICCOLO },
    { OBJ_EVENT_GFX_SIDNEY, PORTRAIT_SUPER_BUU },
    { OBJ_EVENT_GFX_PHOEBE, PORTRAIT_BABA },
    { OBJ_EVENT_GFX_GLACIA, PORTRAIT_ANDROID18 },
    { OBJ_EVENT_GFX_DRAKE, PORTRAIT_DENDE },
    { OBJ_EVENT_GFX_MAXIE, PORTRAIT_KID_BUU },
    { OBJ_EVENT_GFX_ARCHIE, PORTRAIT_GERO },
    { OBJ_EVENT_GFX_UNUSED_PIKACHU_DOLL, PORTRAIT_BULMA },
    { OBJ_EVENT_GFX_RIVAL_MAY_NORMAL, PORTRAIT_VEGETA },
    { OBJ_EVENT_GFX_RIVAL_MAY_MACH_BIKE, PORTRAIT_VEGETA },
    { OBJ_EVENT_GFX_RIVAL_MAY_ACRO_BIKE, PORTRAIT_VEGETA },
    { OBJ_EVENT_GFX_RIVAL_MAY_SURFING, PORTRAIT_VEGETA },
    { OBJ_EVENT_GFX_RIVAL_MAY_FIELD_MOVE, PORTRAIT_VEGETA },
    { OBJ_EVENT_GFX_MAY_NORMAL, PORTRAIT_VEGETA },
    { OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL, PORTRAIT_VEGETA },
};

static bool8 StartsWith(const u8 *str, const u8 *prefix)
{
    while (*prefix != EOS)
    {
        if (*str++ != *prefix++)
            return FALSE;
    }
    return TRUE;
}

static u8 PortraitForText(const u8 *str)
{
    u8 i, obj;
    const u8 *name = gSaveBlock2Ptr->playerName;

    for (i = 0; i < ARRAY_COUNT(sNamePortraits); i++)
        if (StartsWith(str, sNamePortraits[i].name))
            return sNamePortraits[i].portrait;
    // "{PLAYER}: …" is Goku himself
    for (i = 0; i < PLAYER_NAME_LENGTH && name[i] != EOS && str[i] == name[i]; i++)
        ;
    if (i > 0 && (i == PLAYER_NAME_LENGTH || name[i] == EOS) && str[i] == CHAR_COLON)
        return DBZ_IsFused() ? PORTRAIT_VEGITO : PORTRAIT_GOKU;

    // otherwise: the person Goku is talking to, if they're one of the cast
    if (gSpecialVar_LastTalked == 0)
        return PORTRAIT_NONE;
    obj = GetObjectEventIdByLocalIdAndMap(gSpecialVar_LastTalked, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    if (obj >= OBJECT_EVENTS_COUNT)
        return PORTRAIT_NONE;
    for (i = 0; i < ARRAY_COUNT(sGfxPortraits); i++)
        if (gObjectEvents[obj].graphicsId == sGfxPortraits[i].gfx)
            return sGfxPortraits[i].portrait;
    return PORTRAIT_NONE;
}

// ------------------------------------------------------------------ showing it
static bool8 PortraitValid(void)
{
    return sPortraitSprite < MAX_SPRITES && gSprites[sPortraitSprite].inUse
        && gSprites[sPortraitSprite].callback == SpriteCB_Portrait;
}

void DBZ_HidePortrait(void)
{
    if (PortraitValid())
        DestroySprite(&gSprites[sPortraitSprite]);
    sPortraitSprite = MAX_SPRITES;
    sPortraitShown = 0;
    if (GetSpriteTileStartByTag(TAG_PORTRAIT) != 0xFFFF)
        FreeSpriteTilesByTag(TAG_PORTRAIT);
    if (IndexOfSpritePaletteTag(TAG_PORTRAIT) != 0xFF)
        FreeSpritePaletteByTag(TAG_PORTRAIT);
}

static void Task_Portrait(u8 taskId)
{
    // gone with the message box, or when the overworld isn't on screen
    if (!PortraitValid() || gMain.callback2 != CB2_Overworld || (!ScriptContext_IsEnabled() && !ArePlayerFieldControlsLocked()))
    {
        DBZ_HidePortrait();
        DestroyTask(taskId);
    }
}

void DBZ_PortraitForMessage(const u8 *str)
{
    u8 p = PortraitForText(str);
    u16 tile;

    if (p == PORTRAIT_NONE || gMain.callback2 != CB2_Overworld)
    {
        DBZ_HidePortrait();
        return;
    }
    if (PortraitValid() && sPortraitShown == p + 1)
        return;   // same speaker, keep it
    if (!PortraitValid())
    {
        struct SpriteSheet sheet = { sPortraits[p].gfx, 64 * 64 / 2, TAG_PORTRAIT };
        struct SpritePalette pal = { sPortraits[p].pal, TAG_PORTRAIT };
        if (GetSpriteTileStartByTag(TAG_PORTRAIT) == 0xFFFF && LoadSpriteSheet(&sheet) == 0)
            return;   // no room in VRAM right now: no portrait
        if (IndexOfSpritePaletteTag(TAG_PORTRAIT) == 0xFF && LoadSpritePalette(&pal) == 0xFF)
        {
            FreeSpriteTilesByTag(TAG_PORTRAIT);
            return;
        }
        sPortraitSprite = CreateSprite(&sPortraitTemplate, 8 + 32, 46 + 32, 0);
        if (sPortraitSprite == MAX_SPRITES)
        {
            DBZ_HidePortrait();
            return;
        }
        if (!FuncIsActiveTask(Task_Portrait))
            CreateTask(Task_Portrait, 90);
    }
    // swap in this speaker's pixels and colours
    tile = GetSpriteTileStartByTag(TAG_PORTRAIT);
    CpuCopy32(sPortraits[p].gfx, (void *)(OBJ_VRAM0 + tile * TILE_SIZE_4BPP), 64 * 64 / 2);
    LoadPalette(sPortraits[p].pal, OBJ_PLTT_ID(IndexOfSpritePaletteTag(TAG_PORTRAIT)), PLTT_SIZE_4BPP);
    sPortraitShown = p + 1;
}
