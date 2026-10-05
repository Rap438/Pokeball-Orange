// PokeBall Orange: Dragon Balls, Dragon Radar and Shenron.
//
// Seven balls are scattered over on-foot reachable spots across Hoenn when a new game starts
// (one per map). They are injected into the current map's object templates on load, so no map
// data changes. The Dragon Radar shows the remaining balls on the Hoenn map. With all seven in
// the bag, using one outdoors summons Shenron right on the overworld, who grants any Pokemon from
// the wish list. Afterwards the balls turn to stone and re-scatter after DBZ_DB_STONE_STEPS steps.
#include "global.h"
#include "dbz.h"
#include "bg.h"
#include "data.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item.h"
#include "list_menu.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon.h"
#include "random.h"
#include "region_map.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_pokemon_sprites.h"
#include "window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/items.h"
#include "constants/map_types.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainer_types.h"
#include "constants/vars.h"

#include "data/dbz_dragonball_spots.h"
#include "data/dbz_wish_list.h"

extern const u8 EventScript_DBZ_DragonBall[];

EWRAM_DATA bool8 gDBZNimbusFly = FALSE;

#define NUM_SPOTS ARRAY_COUNT(sDragonBallSpots)
#define NUM_WISHES ARRAY_COUNT(sWishableSpecies)
#define DEAD_TEMPLATE_POS (-1000)

static bool8 IsDragonBallLocalId(u8 localId)
{
    return localId >= LOCALID_DBZ_DRAGON_BALL && localId < LOCALID_DBZ_DRAGON_BALL + DBZ_DB_COUNT;
}

// ------------------------------------------------------------------ scattering
static u16 CurrentMapId(void)
{
    return (gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum;
}

void DBZ_ScatterDragonBalls(void)
{
    u16 picked[DBZ_DB_COUNT];
    u8 i, j;
    u16 tries;

    for (i = 0; i < DBZ_DB_COUNT; i++)
    {
        u16 idx = Random() % NUM_SPOTS;
        for (tries = 0; tries < 500; tries++)
        {
            bool8 clash = FALSE;
            for (j = 0; j < i; j++)
            {
                if (sDragonBallSpots[picked[j]].mapId == sDragonBallSpots[idx].mapId)
                    clash = TRUE;
            }
            if (!clash)
                break;
            idx = Random() % NUM_SPOTS;
        }
        picked[i] = idx;
        VarSet(VAR_DBZ_DB_SPOT_1 + i, idx + 1);
    }
    VarSet(VAR_DBZ_DB_STONE_STEPS, 0);
}

u8 DBZ_CountDragonBallsInBag(void)
{
    u8 i, n = 0;
    for (i = 0; i < DBZ_DB_COUNT; i++)
    {
        if (CheckBagHasItem(ITEM_DRAGON_BALL_1 + i, 1))
            n++;
    }
    return n;
}

static bool8 NeedsInitialScatter(void)
{
    u8 i;
    if (VarGet(VAR_DBZ_DB_STONE_STEPS) != 0)
        return FALSE;
    for (i = 0; i < DBZ_DB_COUNT; i++)
    {
        if (VarGet(VAR_DBZ_DB_SPOT_1 + i) != 0)
            return FALSE;
    }
    return DBZ_CountDragonBallsInBag() == 0;
}

void DBZ_DragonBallStepUpdate(void)
{
    u16 steps = VarGet(VAR_DBZ_DB_STONE_STEPS);
    if (steps != 0)
    {
        steps--;
        VarSet(VAR_DBZ_DB_STONE_STEPS, steps);
        if (steps == 0)
            DBZ_ScatterDragonBalls();
    }
}

// ------------------------------------------------------------------ object templates
static void InitBallTemplate(struct ObjectEventTemplate *t, u8 ball, const struct DragonBallSpot *spot)
{
    CpuFill32(0, t, sizeof(*t));
    t->localId = LOCALID_DBZ_DRAGON_BALL + ball;
    t->graphicsId = OBJ_EVENT_GFX_DRAGON_BALL;
    t->x = spot->x;
    t->y = spot->y;
    t->elevation = spot->elevation;
    t->movementType = MOVEMENT_TYPE_NONE;
    t->trainerType = TRAINER_TYPE_NONE;
    t->script = EventScript_DBZ_DragonBall;
    t->flagId = 0;
}

// Called after the map header's templates have been copied into the save block.
void DBZ_InjectDragonBalls(void)
{
    u8 i, count;
    u16 mapId;
    struct ObjectEventTemplate *templates = gSaveBlock1Ptr->objectEventTemplates;

    if (gMapHeader.events == NULL)
        return;
    if (NeedsInitialScatter())
        DBZ_ScatterDragonBalls();

    count = gMapHeader.events->objectEventCount;
    mapId = CurrentMapId();
    for (i = 0; i < DBZ_DB_COUNT && count < OBJECT_EVENT_TEMPLATES_COUNT; i++)
    {
        u16 v = VarGet(VAR_DBZ_DB_SPOT_1 + i);
        if (v == 0 || v == DBZ_DB_COLLECTED || v > NUM_SPOTS)
            continue;
        if (sDragonBallSpots[v - 1].mapId != mapId)
            continue;
        InitBallTemplate(&templates[count++], i, &sDragonBallSpots[v - 1]);
    }
}

u8 DBZ_GetCurrentMapObjectEventCount(void)
{
    u8 count = gMapHeader.events->objectEventCount;
    while (count < OBJECT_EVENT_TEMPLATES_COUNT && IsDragonBallLocalId(gSaveBlock1Ptr->objectEventTemplates[count].localId))
        count++;
    return count;
}

// Script pointers aren't trustworthy after loading a save (ROM may have changed), so re-point ours.
void DBZ_RestoreDragonBallScripts(void)
{
    u8 i;
    u8 start = gMapHeader.events->objectEventCount;
    u8 end = DBZ_GetCurrentMapObjectEventCount();
    for (i = start; i < end; i++)
        gSaveBlock1Ptr->objectEventTemplates[i].script = EventScript_DBZ_DragonBall;
}

// ------------------------------------------------------------------ pickup specials
u16 DBZ_GetTalkedDragonBallItem(void)
{
    if (!IsDragonBallLocalId(gSpecialVar_LastTalked))
        return ITEM_DRAGON_BALL_1;
    return ITEM_DRAGON_BALL_1 + (gSpecialVar_LastTalked - LOCALID_DBZ_DRAGON_BALL);
}

void DBZ_CollectTalkedDragonBall(void)
{
    u8 i, ball;
    struct ObjectEventTemplate *templates = gSaveBlock1Ptr->objectEventTemplates;

    if (!IsDragonBallLocalId(gSpecialVar_LastTalked))
        return;
    ball = gSpecialVar_LastTalked - LOCALID_DBZ_DRAGON_BALL;
    VarSet(VAR_DBZ_DB_SPOT_1 + ball, DBZ_DB_COLLECTED);
    // the template can't be hidden by a flag, so park it where it can never come into view
    for (i = 0; i < OBJECT_EVENT_TEMPLATES_COUNT; i++)
    {
        if (templates[i].localId == gSpecialVar_LastTalked)
        {
            templates[i].x = DEAD_TEMPLATE_POS;
            templates[i].y = DEAD_TEMPLATE_POS;
        }
    }
}

u16 DBZ_CountDragonBalls(void)
{
    return DBZ_CountDragonBallsInBag();
}

// ------------------------------------------------------------------ debug
void DBZ_Debug_WarpToDragonBall(void)
{
    u8 i;
    // test harness hook: VAR_TEMP_D = map id + 1, VAR_TEMP_E/F = x/y
    if (VarGet(VAR_TEMP_D) != 0)
    {
        u16 map = VarGet(VAR_TEMP_D) - 1;
        SetWarpDestination(map >> 8, map & 0xFF, WARP_ID_NONE, VarGet(VAR_TEMP_E), VarGet(VAR_TEMP_F));
        DoWarp();
        ResetInitialPlayerAvatarState();
        return;
    }
    for (i = 0; i < DBZ_DB_COUNT; i++)
    {
        u16 v = VarGet(VAR_DBZ_DB_SPOT_1 + i);
        if (v != 0 && v != DBZ_DB_COLLECTED && v <= NUM_SPOTS)
        {
            const struct DragonBallSpot *spot = &sDragonBallSpots[v - 1];
            SetWarpDestination(spot->mapId >> 8, spot->mapId & 0xFF, WARP_ID_NONE, spot->x, spot->y + 1);
            DoWarp();
            ResetInitialPlayerAvatarState();
            return;
        }
    }
    ScriptContext_Enable();
}

void DBZ_Debug_GiveSixBalls(void)
{
    u8 i, given = 0;
    for (i = 0; i < DBZ_DB_COUNT && given < 6; i++)
    {
        if (VarGet(VAR_DBZ_DB_SPOT_1 + i) != DBZ_DB_COLLECTED)
        {
            VarSet(VAR_DBZ_DB_SPOT_1 + i, DBZ_DB_COLLECTED);
            AddBagItem(ITEM_DRAGON_BALL_1 + i, 1);
            given++;
        }
    }
    AddBagItem(ITEM_DRAGON_RADAR, 1);
    AddBagItem(ITEM_NIMBUS, 1);
}

// ================================================================== Dragon Radar screen
#define TAG_RADAR_BLIP 0x2F10

enum {
    RADAR_WIN_TITLE,
    RADAR_WIN_INFO,
};

enum {
    TAG_RADAR_PLAYER_ICON = 0x2F11,
    TAG_RADAR_CURSOR,
};

static EWRAM_DATA struct {
    MainCallback callback;
    struct RegionMap regionMap;
    u16 state;
} *sRadar = NULL;

static const u32 sRadarBlipGfx[] = INCGFX_U32("graphics/dbz/radar_blip.png", ".4bpp");
static const u16 sRadarBlipPal[] = INCGFX_U16("graphics/dbz/radar_blip.png", ".gbapal");

static const struct SpriteSheet sRadarBlipSheet = { sRadarBlipGfx, 2 * 32, TAG_RADAR_BLIP };
static const struct SpritePalette sRadarBlipPalette = { sRadarBlipPal, TAG_RADAR_BLIP };

static const struct OamData sOam_RadarBlip = {
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 1,
};

static const union AnimCmd sAnim_RadarBlip[] = {
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_FRAME(1, 12),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_RadarBlip[] = { sAnim_RadarBlip };

static const struct SpriteTemplate sSpriteTemplate_RadarBlip = {
    .tileTag = TAG_RADAR_BLIP,
    .paletteTag = TAG_RADAR_BLIP,
    .oam = &sOam_RadarBlip,
    .anims = sAnims_RadarBlip,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct BgTemplate sRadarBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    }, {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 1,
        .priority = 2,
        .baseTile = 0
    }
};

static const struct WindowTemplate sRadarWindowTemplates[] = {
    [RADAR_WIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 1,
        .width = 12,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [RADAR_WIN_INFO] = {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 17,
        .width = 12,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 25
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sText_DragonRadar[] = _("DRAGON RADAR");
static const u8 sText_RadarFound[] = _("FOUND: {STR_VAR_1}/7");
static const u8 sText_RadarStone[] = _("NO SIGNAL…");

static void CB2_Radar(void);
static void VBlankCB_Radar(void);

void DBZ_OpenDragonRadar(void (*callback)(void))
{
    SetVBlankCallback(NULL);
    sRadar = AllocZeroed(sizeof(*sRadar));
    sRadar->callback = callback;
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(1, sRadarBgTemplates, ARRAY_COUNT(sRadarBgTemplates));
    InitWindows(sRadarWindowTemplates);
    DeactivateAllTextPrinters();
    LoadUserWindowBorderGfx(0, 0x27, BG_PLTT_ID(13));
    ClearScheduledBgCopiesToVram();
    SetMainCallback2(CB2_Radar);
    SetVBlankCallback(VBlankCB_Radar);
}

static void VBlankCB_Radar(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CreateRadarBlips(void)
{
    u8 i;
    LoadSpriteSheet(&sRadarBlipSheet);
    LoadSpritePalette(&sRadarBlipPalette);
    for (i = 0; i < DBZ_DB_COUNT; i++)
    {
        u16 v = VarGet(VAR_DBZ_DB_SPOT_1 + i);
        const struct DragonBallSpot *spot;
        const struct MapHeader *header;
        const struct RegionMapLocation *entry;
        s16 x, y, fx, fy;

        if (v == 0 || v == DBZ_DB_COLLECTED || v > NUM_SPOTS)
            continue;
        spot = &sDragonBallSpots[v - 1];
        header = Overworld_GetMapHeaderByGroupAndId(spot->mapId >> 8, spot->mapId & 0xFF);
        entry = &gRegionMapEntries[header->regionMapSectionId];
        fx = (spot->x * entry->width) / header->mapLayout->width;
        fy = (spot->y * entry->height) / header->mapLayout->height;
        // region map cells are 8px; the map is drawn 1 cell right and 2 cells down
        x = 8 * (entry->x + 1 + fx) + 4;
        y = 8 * (entry->y + 2 + fy) + 4;
        CreateSprite(&sSpriteTemplate_RadarBlip, x, y, 0);
    }
}

static void TintRadarMap(void)
{
    // region map art lives in BG palettes 7-9 (8bpp); give it the radar's green glow
    TintPalette_CustomTone(&gPlttBufferUnfaded[BG_PLTT_ID(7)], 48, 96, 300, 150);
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(7)], &gPlttBufferFaded[BG_PLTT_ID(7)], 48 * 2);
}

static void PrintRadarInfo(void)
{
    u8 found = DBZ_CountDragonBallsInBag();
    FillWindowPixelBuffer(RADAR_WIN_INFO, PIXEL_FILL(1));
    if (VarGet(VAR_DBZ_DB_STONE_STEPS) != 0)
    {
        AddTextPrinterParameterized(RADAR_WIN_INFO, FONT_NORMAL, sText_RadarStone, 0, 1, 0, NULL);
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, found, STR_CONV_MODE_LEFT_ALIGN, 1);
        StringExpandPlaceholders(gStringVar4, sText_RadarFound);
        AddTextPrinterParameterized(RADAR_WIN_INFO, FONT_NORMAL, gStringVar4, 0, 1, 0, NULL);
    }
    CopyWindowToVram(RADAR_WIN_INFO, COPYWIN_FULL);
}

static void CB2_Radar(void)
{
    u8 offset;
    switch (sRadar->state)
    {
    case 0:
        InitRegionMap(&sRadar->regionMap, FALSE);
        CreateRegionMapPlayerIcon(TAG_RADAR_PLAYER_ICON, TAG_RADAR_PLAYER_ICON);
        CreateRadarBlips();
        TintRadarMap();
        sRadar->state++;
        break;
    case 1:
        DrawStdFrameWithCustomTileAndPalette(RADAR_WIN_TITLE, FALSE, 0x27, 0xd);
        FillWindowPixelBuffer(RADAR_WIN_TITLE, PIXEL_FILL(1));
        offset = GetStringCenterAlignXOffset(FONT_NORMAL, sText_DragonRadar, 12 * 8);
        AddTextPrinterParameterized(RADAR_WIN_TITLE, FONT_NORMAL, sText_DragonRadar, offset, 1, 0, NULL);
        DrawStdFrameWithCustomTileAndPalette(RADAR_WIN_INFO, FALSE, 0x27, 0xd);
        PrintRadarInfo();
        ScheduleBgCopyTilemapToVram(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        PlaySE(SE_POKENAV_ON);
        sRadar->state++;
        break;
    case 2:
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(0);
        ShowBg(2);
        sRadar->state++;
        break;
    case 3:
        if (!gPaletteFade.active)
            sRadar->state++;
        break;
    case 4:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            PlaySE(SE_POKENAV_OFF);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sRadar->state++;
        }
        break;
    case 5:
        if (!gPaletteFade.active)
        {
            FreeRegionMapIconResources();
            SetMainCallback2(sRadar->callback);
            TRY_FREE_AND_SET_NULL(sRadar);
            FreeAllWindowBuffers();
            return;
        }
        break;
    }
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    DoScheduledBgTilemapCopiesToVram();
}

// ================================================================== Shenron
#define TAG_SHENRON     0x2F12
#define TAG_WISH_PIC    0x2F13
#define TAG_WISH_ARROWS 0x2F14

static const u32 sShenronGfx[] = INCGFX_U32("graphics/dbz/shenron.png", ".4bpp");
static const u16 sShenronPal[] = INCGFX_U16("graphics/dbz/shenron.png", ".gbapal");
static const struct SpriteSheet sShenronSheet = { sShenronGfx, 64 * 64 / 2, TAG_SHENRON };
static const struct SpritePalette sShenronPalette = { sShenronPal, TAG_SHENRON };

static const struct OamData sOam_Shenron = {
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 2, // behind Goku and the map's top layer, so he rises out of the scenery
};

static const union AffineAnimCmd sAffineAnim_ShenronRise[] = {
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(16, 16, 0, 31),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_ShenronLeave[] = {
    AFFINEANIMCMD_FRAME(512, 512, 0, 0),
    AFFINEANIMCMD_FRAME(-16, -16, 0, 31),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_Shenron[] = {
    sAffineAnim_ShenronRise,
    sAffineAnim_ShenronLeave,
};

static const struct SpriteTemplate sSpriteTemplate_Shenron = {
    .tileTag = TAG_SHENRON,
    .paletteTag = TAG_SHENRON,
    .oam = &sOam_Shenron,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_Shenron,
    .callback = SpriteCallbackDummy,
};

// note: EWRAM is not initialised at boot; these are set before use
static EWRAM_DATA u8 sShenronSpriteId = 0;
static EWRAM_DATA u16 sWishPicSpriteId = 0;
static EWRAM_DATA struct ListMenuItem *sWishItems = NULL;
static EWRAM_DATA u16 sWishScroll = 0;
static EWRAM_DATA u16 sWishRow = 0;
static EWRAM_DATA u16 sWishArrowScroll = 0;

#define SHENRON_X 120
#define SHENRON_Y 44
#define DARK_BG_PALS 0x1FFF   // BG palettes 0-12 (map tiles), leaves the message box alone

#define tState data[0]
#define tTimer data[1]

static void Task_ShenronAppear(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    switch (task->tState)
    {
    case 0:
        PlaySE(SE_THUNDER);
        DBZ_BlendPalettes(DARK_BG_PALS, 6, RGB(2, 4, 6));
        task->tTimer = 0;
        task->tState++;
        break;
    case 1: // the sky goes dark
        task->tTimer++;
        DBZ_BlendPalettes(DARK_BG_PALS, 6 + task->tTimer / 2, RGB(2, 4, 6));
        if (task->tTimer >= 14)
        {
            task->tState++;
            task->tTimer = 0;
        }
        break;
    case 2: // golden flash, the dragon rises
        DBZ_BlendPalettes(DARK_BG_PALS, 12, RGB(31, 28, 8));
        PlaySE(SE_THUNDER);
        LoadSpriteSheet(&sShenronSheet);
        LoadSpritePalette(&sShenronPalette);
        sShenronSpriteId = CreateSprite(&sSpriteTemplate_Shenron, SHENRON_X, SHENRON_Y, 255);
        StartSpriteAffineAnim(&gSprites[sShenronSpriteId], 0);
        task->tState++;
        break;
    case 3:
        if (++task->tTimer == 4)
            DBZ_BlendPalettes(DARK_BG_PALS, 13, RGB(2, 4, 6));
        if (task->tTimer >= 40)
        {
            PlayCry_Normal(SPECIES_RAYQUAZA, 0);
            task->tState++;
        }
        break;
    case 4:
        if (IsCryFinished())
        {
            DestroyTask(taskId);
            ScriptContext_Enable();
        }
        break;
    }
}

void DBZ_ShenronAppear(void)
{
    CreateTask(Task_ShenronAppear, 80);
}

static void Task_ShenronDepart(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    switch (task->tState)
    {
    case 0:
        PlaySE(SE_THUNDER);
        if (sShenronSpriteId != MAX_SPRITES)
            StartSpriteAffineAnim(&gSprites[sShenronSpriteId], 1);
        task->tState++;
        break;
    case 1:
        if (++task->tTimer >= 34)
        {
            if (sShenronSpriteId != MAX_SPRITES)
            {
                FreeOamMatrix(gSprites[sShenronSpriteId].oam.matrixNum);
                DestroySprite(&gSprites[sShenronSpriteId]);
                sShenronSpriteId = MAX_SPRITES;
            }
            FreeSpriteTilesByTag(TAG_SHENRON);
            FreeSpritePaletteByTag(TAG_SHENRON);
            DBZ_BlendPalettes(DARK_BG_PALS, 16, RGB_WHITE);
            PlaySE(SE_M_REFLECT);
            task->tTimer = 16;
            task->tState++;
        }
        break;
    case 2: // fade the light back to normal
        task->tTimer--;
        DBZ_BlendPalettes(DARK_BG_PALS, task->tTimer, RGB_WHITE);
        if (task->tTimer == 0)
        {
            DestroyTask(taskId);
            ScriptContext_Enable();
        }
        break;
    }
}

void DBZ_ShenronDepart(void)
{
    CreateTask(Task_ShenronDepart, 80);
}

// ------------------------------------------------------------------ wish list
#define tListTaskId data[2]
#define tWindowId   data[3]
#define tArrowTask  data[4]

#define WISH_WIN_LEFT   19
#define WISH_WIN_TOP    1
#define WISH_WIN_WIDTH  10
#define WISH_WIN_HEIGHT 12
#define WISH_PIC_X      44
#define WISH_PIC_Y      60

static void Task_WishInput(u8 taskId);

static void ShowWishPic(u16 species)
{
    if (sWishPicSpriteId != 0xFFFF)
        FreeAndDestroyMonPicSprite(sWishPicSpriteId);
    // the engine loads a pic palette under the species as its tag, so use that as the sprite tag too
    sWishPicSpriteId = CreateMonPicSprite(species, FALSE, 0x8000, TRUE, WISH_PIC_X, WISH_PIC_Y, 0, species);
    if (sWishPicSpriteId != 0xFFFF)
        gSprites[sWishPicSpriteId].oam.priority = 0;
}

static void WishMoveCursor(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (!onInit)
        PlaySE(SE_SELECT);
    ShowWishPic(sWishableSpecies[itemIndex]);
}

void DBZ_ChooseWish(void)
{
    u16 i;
    u8 taskId, windowId;
    struct WindowTemplate winTemplate;
    struct ListMenuTemplate menu;

    LockPlayerFieldControls();
    sWishPicSpriteId = 0xFFFF;
    sWishItems = Alloc(NUM_WISHES * sizeof(struct ListMenuItem));
    for (i = 0; i < NUM_WISHES; i++)
    {
        sWishItems[i].name = GetSpeciesName(sWishableSpecies[i]);
        sWishItems[i].id = i;
    }
    winTemplate = CreateWindowTemplate(0, WISH_WIN_LEFT, WISH_WIN_TOP, WISH_WIN_WIDTH, WISH_WIN_HEIGHT, 15, 0x64);
    windowId = AddWindow(&winTemplate);
    SetStandardWindowBorderStyle(windowId, FALSE);

    menu.items = sWishItems;
    menu.moveCursorFunc = WishMoveCursor;
    menu.itemPrintFunc = NULL;
    menu.totalItems = NUM_WISHES;
    menu.maxShowed = WISH_WIN_HEIGHT / 2;
    menu.windowId = windowId;
    menu.header_X = 0;
    menu.item_X = 8;
    menu.cursor_X = 0;
    menu.upText_Y = 1;
    menu.cursorPal = 2;
    menu.fillValue = 1;
    menu.cursorShadowPal = 3;
    menu.lettersSpacing = 0;
    menu.itemVerticalPadding = 0;
    menu.scrollMultiple = LIST_MULTIPLE_SCROLL_L_R;
    menu.fontId = FONT_NORMAL;
    menu.cursorKind = CURSOR_BLACK_ARROW;

    taskId = CreateTask(Task_WishInput, 80);
    gTasks[taskId].tWindowId = windowId;
    gTasks[taskId].tListTaskId = ListMenuInit(&menu, sWishScroll, sWishRow);
    sWishArrowScroll = sWishScroll;
    gTasks[taskId].tArrowTask = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP,
        (WISH_WIN_LEFT + WISH_WIN_WIDTH / 2) * 8, WISH_WIN_TOP * 8 - 4, (WISH_WIN_TOP + WISH_WIN_HEIGHT) * 8 + 4,
        NUM_WISHES - menu.maxShowed, TAG_WISH_ARROWS, TAG_WISH_ARROWS, &sWishArrowScroll);
    CopyWindowToVram(windowId, COPYWIN_FULL);
    ScheduleBgCopyTilemapToVram(0);
}

static void CloseWishList(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    DestroyListMenuTask(task->tListTaskId, &sWishScroll, &sWishRow);
    RemoveScrollIndicatorArrowPair(task->tArrowTask);
    if (sWishPicSpriteId != 0xFFFF)
    {
        FreeAndDestroyMonPicSprite(sWishPicSpriteId);
        sWishPicSpriteId = 0xFFFF;
    }
    TRY_FREE_AND_SET_NULL(sWishItems);
    ClearStdWindowAndFrameToTransparent(task->tWindowId, TRUE);
    RemoveWindow(task->tWindowId);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void Task_WishInput(u8 taskId)
{
    s32 input = ListMenu_ProcessInput(gTasks[taskId].tListTaskId);
    ListMenuGetScrollAndRow(gTasks[taskId].tListTaskId, &sWishArrowScroll, NULL);
    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        gSpecialVar_Result = SPECIES_NONE;
        CloseWishList(taskId);
        break;
    default:
        PlaySE(SE_SELECT);
        gSpecialVar_Result = sWishableSpecies[input];
        CloseWishList(taskId);
        break;
    }
}

static u8 HighestPartyLevel(void)
{
    u8 i, best = 5;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES);
        if (species != SPECIES_NONE && species != SPECIES_EGG)
        {
            u8 lvl = GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL);
            if (lvl > best)
                best = lvl;
        }
    }
    return best;
}

// VAR_0x8006 = species. Returns MON_GIVEN_TO_PARTY / MON_GIVEN_TO_PC / MON_CANT_GIVE.
u16 DBZ_GrantWish(void)
{
    u8 result;
    u16 species = gSpecialVar_0x8006;
    u16 dexNum;

    result = ScriptGiveMon(species, HighestPartyLevel(), ITEM_NONE);
    if (result == MON_CANT_GIVE)
        return result;

    dexNum = SpeciesToNationalPokedexNum(species);
    if (dexNum != 0)
    {
        GetSetPokedexFlag(dexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(dexNum, FLAG_SET_CAUGHT);
    }
    DBZ_ConsumeDragonBalls();
    return result;
}

// after a wish: the balls turn to stone and scatter
void DBZ_ConsumeDragonBalls(void)
{
    u8 i;
    for (i = 0; i < DBZ_DB_COUNT; i++)
    {
        RemoveBagItem(ITEM_DRAGON_BALL_1 + i, 1);
        VarSet(VAR_DBZ_DB_SPOT_1 + i, 0);
    }
    VarSet(VAR_DBZ_DB_STONE_STEPS, DBZ_DB_STONE_STEPS);
    if (VarGet(VAR_DBZ_WISHES) < 999)
        VarSet(VAR_DBZ_WISHES, VarGet(VAR_DBZ_WISHES) + 1);
}
