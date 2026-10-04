#include "global.h"
#include "option_menu.h"
#include "bg.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "gba/m4a_internal.h"
#include "dbz.h"
#include "string_util.h"
#include "constants/rgb.h"

// PokeBall Orange: the OPTION menu is a scrolling list with the original settings plus the
// PokeBall Orange ones (stored in VAR_DBZ_OPTIONS, see src/dbz_options.c).
#define tMenuSelection data[0]
#define tScroll data[1]

enum
{
    MENUITEM_TEXTSPEED,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_DBZ_FIRST,                       // DBZ settings 0..10, in DBZ_GetOptionValue order
    MENUITEM_CANCEL = MENUITEM_DBZ_FIRST + 11,
    MENUITEM_COUNT,
};

#define VISIBLE_ROWS 7

enum
{
    WIN_HEADER,
    WIN_OPTIONS
};

static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(u8 row);
static void DrawHeaderText(u8 scroll);
static void DrawOptionMenuTexts(u8 scroll, u8 selection);
static void DrawBgWindowFrames(void);

static EWRAM_DATA u8 sOptionValues[MENUITEM_COUNT] = {0};

static const u8 sText_Opt_DayNight[] = _("DAY/NIGHT");
static const u8 sText_Opt_Shadows[] = _("SHADOWS");
static const u8 sText_Opt_Hud[] = _("GOKU HUD");
static const u8 sText_Opt_Difficulty[] = _("FIGHT LEVEL");
static const u8 sText_Opt_Ambush[] = _("AMBUSHES");
static const u8 sText_Opt_BattleBg[] = _("BATTLE BG");
static const u8 sText_Opt_Sparks[] = _("SSJ2 SPARKS");
static const u8 sText_Opt_PowerUp[] = _("POWER-UPS");
static const u8 sText_Opt_Hints[] = _("FORM HINTS");
static const u8 sText_Opt_Shiny[] = _("SHINY ODDS");
static const u8 sText_Opt_AutoRun[] = _("AUTO-RUN");

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = gText_TextSpeed,
    [MENUITEM_BATTLESCENE] = gText_BattleScene,
    [MENUITEM_BATTLESTYLE] = gText_BattleStyle,
    [MENUITEM_SOUND]       = gText_Sound,
    [MENUITEM_BUTTONMODE]  = gText_ButtonMode,
    [MENUITEM_FRAMETYPE]   = gText_Frame,
    [MENUITEM_DBZ_FIRST + 0] = sText_Opt_DayNight,
    [MENUITEM_DBZ_FIRST + 1] = sText_Opt_Shadows,
    [MENUITEM_DBZ_FIRST + 2] = sText_Opt_Hud,
    [MENUITEM_DBZ_FIRST + 3] = sText_Opt_Difficulty,
    [MENUITEM_DBZ_FIRST + 4] = sText_Opt_Ambush,
    [MENUITEM_DBZ_FIRST + 5] = sText_Opt_BattleBg,
    [MENUITEM_DBZ_FIRST + 6] = sText_Opt_Sparks,
    [MENUITEM_DBZ_FIRST + 7] = sText_Opt_PowerUp,
    [MENUITEM_DBZ_FIRST + 8] = sText_Opt_Hints,
    [MENUITEM_DBZ_FIRST + 9] = sText_Opt_Shiny,
    [MENUITEM_DBZ_FIRST + 10] = sText_Opt_AutoRun,
    [MENUITEM_CANCEL]      = gText_OptionMenuCancel,
};

static const u8 sText_V_Slow[] = _("SLOW");
static const u8 sText_V_Mid[] = _("MID");
static const u8 sText_V_Fast[] = _("FAST");
static const u8 sText_V_On[] = _("ON");
static const u8 sText_V_Off[] = _("OFF");
static const u8 sText_V_Shift[] = _("SHIFT");
static const u8 sText_V_Set[] = _("SET");
static const u8 sText_V_Mono[] = _("MONO");
static const u8 sText_V_Stereo[] = _("STEREO");
static const u8 sText_V_Normal[] = _("NORMAL");
static const u8 sText_V_LR[] = _("LR");
static const u8 sText_V_LA[] = _("L=A");
static const u8 sText_V_Easy[] = _("EASY");
static const u8 sText_V_Hard[] = _("HARD");
static const u8 sText_V_Rare[] = _("RARE");
static const u8 sText_V_Often[] = _("OFTEN");
static const u8 sText_V_Map[] = _("MAP VIEW");
static const u8 sText_V_Classic[] = _("CLASSIC");
static const u8 sText_V_8192[] = _("1/8192");
static const u8 sText_V_4096[] = _("1/4096");
static const u8 sText_V_1024[] = _("1/1024");
static const u8 sText_V_256[] = _("1/256");
static const u8 sText_V_Always[] = _("ALWAYS");
static const u8 sText_V_Type[] = _("TYPE ");
static const u8 sText_LeftArrow[] = _("<");
static const u8 sText_RightArrow[] = _(">");
static const u8 sText_ScrollHint[] = _("{UP_ARROW}{DOWN_ARROW}");

static const u8 *const sV_OnOff[] = {sText_V_On, sText_V_Off};
static const u8 *const sV_TextSpeed[] = {sText_V_Slow, sText_V_Mid, sText_V_Fast};
static const u8 *const sV_BattleStyle[] = {sText_V_Shift, sText_V_Set};
static const u8 *const sV_Sound[] = {sText_V_Mono, sText_V_Stereo};
static const u8 *const sV_Button[] = {sText_V_Normal, sText_V_LR, sText_V_LA};
static const u8 *const sV_Difficulty[] = {sText_V_Easy, sText_V_Normal, sText_V_Hard};
static const u8 *const sV_Ambush[] = {sText_V_Off, sText_V_Rare, sText_V_Normal, sText_V_Often};
static const u8 *const sV_BattleBg[] = {sText_V_Map, sText_V_Classic};
static const u8 *const sV_Shiny[] = {sText_V_8192, sText_V_4096, sText_V_1024, sText_V_256, sText_V_Always};

static u8 ValueCount(u8 item)
{
    switch (item)
    {
    case MENUITEM_TEXTSPEED:  return 3;
    case MENUITEM_BUTTONMODE: return 3;
    case MENUITEM_FRAMETYPE:  return WINDOW_FRAMES_COUNT;
    case MENUITEM_DBZ_FIRST + 3: return 3;
    case MENUITEM_DBZ_FIRST + 4: return 4;
    case MENUITEM_DBZ_FIRST + 9: return 5;
    case MENUITEM_CANCEL:     return 0;
    default:                  return 2;
    }
}

static const u8 *ValueText(u8 item, u8 v)
{
    switch (item)
    {
    case MENUITEM_TEXTSPEED:   return sV_TextSpeed[v];
    case MENUITEM_BATTLESCENE: return sV_OnOff[v];
    case MENUITEM_BATTLESTYLE: return sV_BattleStyle[v];
    case MENUITEM_SOUND:       return sV_Sound[v];
    case MENUITEM_BUTTONMODE:  return sV_Button[v];
    case MENUITEM_DBZ_FIRST + 3: return sV_Difficulty[v];
    case MENUITEM_DBZ_FIRST + 4: return sV_Ambush[v];
    case MENUITEM_DBZ_FIRST + 5: return sV_BattleBg[v];
    case MENUITEM_DBZ_FIRST + 9: return sV_Shiny[v];
    default:                   return sV_OnOff[v];
    }
}

static const u16 sOptionMenuText_Pal[] = INCGFX_U16("graphics/interface/option_menu_text.pal", ".gbapal");

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    [WIN_OPTIONS] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 5,
        .width = 26,
        .height = 14,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitOptionMenu(void)
{
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);
        InitWindows(sOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, BG_PLTT_ID(0), sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, BG_PLTT_ID(1), sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_HEADER);
        DrawHeaderText(0);
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
    {
        u8 i;
        sOptionValues[MENUITEM_TEXTSPEED] = gSaveBlock2Ptr->optionsTextSpeed;
        sOptionValues[MENUITEM_BATTLESCENE] = gSaveBlock2Ptr->optionsBattleSceneOff;
        sOptionValues[MENUITEM_BATTLESTYLE] = gSaveBlock2Ptr->optionsBattleStyle;
        sOptionValues[MENUITEM_SOUND] = gSaveBlock2Ptr->optionsSound;
        sOptionValues[MENUITEM_BUTTONMODE] = gSaveBlock2Ptr->optionsButtonMode;
        sOptionValues[MENUITEM_FRAMETYPE] = gSaveBlock2Ptr->optionsWindowFrameType;
        for (i = 0; i < 11; i++)
            sOptionValues[MENUITEM_DBZ_FIRST + i] = DBZ_GetOptionValue(i);
        PutWindowTilemap(WIN_OPTIONS);
        DrawOptionMenuTexts(0, 0);
        gMain.state++;
    }
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
    {
        u8 taskId = CreateTask(Task_OptionMenuFadeIn, 0);
        gTasks[taskId].tMenuSelection = 0;
        gTasks[taskId].tScroll = 0;
        HighlightOptionMenuItem(0);
        CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
        gMain.state++;
        break;
    }
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput;
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 count;

    if (JOY_NEW(A_BUTTON))
    {
        if (tMenuSelection == MENUITEM_CANCEL)
            gTasks[taskId].func = Task_OptionMenuSave;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
        return;
    }
    if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
    {
        if (JOY_REPEAT(DPAD_UP))
            tMenuSelection = (tMenuSelection > 0) ? tMenuSelection - 1 : MENUITEM_CANCEL;
        else
            tMenuSelection = (tMenuSelection < MENUITEM_CANCEL) ? tMenuSelection + 1 : 0;
        if (tMenuSelection < tScroll)
            tScroll = tMenuSelection;
        if (tMenuSelection >= tScroll + VISIBLE_ROWS)
            tScroll = tMenuSelection - VISIBLE_ROWS + 1;
        DrawOptionMenuTexts(tScroll, tMenuSelection);
        HighlightOptionMenuItem(tMenuSelection - tScroll);
        return;
    }
    count = ValueCount(tMenuSelection);
    if (count != 0 && JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        u8 v = sOptionValues[tMenuSelection];
        if (JOY_NEW(DPAD_RIGHT))
            v = (v + 1) % count;
        else
            v = (v == 0) ? count - 1 : v - 1;
        sOptionValues[tMenuSelection] = v;
        if (tMenuSelection == MENUITEM_SOUND)
            SetPokemonCryStereo(v);
        if (tMenuSelection == MENUITEM_FRAMETYPE)
        {
            LoadBgTiles(1, GetWindowFrameTilesPal(v)->tiles, 0x120, 0x1A2);
            LoadPalette(GetWindowFrameTilesPal(v)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        }
        DrawOptionMenuTexts(tScroll, tMenuSelection);
    }
}

static void Task_OptionMenuSave(u8 taskId)
{
    u8 i;
    gSaveBlock2Ptr->optionsTextSpeed = sOptionValues[MENUITEM_TEXTSPEED];
    gSaveBlock2Ptr->optionsBattleSceneOff = sOptionValues[MENUITEM_BATTLESCENE];
    gSaveBlock2Ptr->optionsBattleStyle = sOptionValues[MENUITEM_BATTLESTYLE];
    gSaveBlock2Ptr->optionsSound = sOptionValues[MENUITEM_SOUND];
    gSaveBlock2Ptr->optionsButtonMode = sOptionValues[MENUITEM_BUTTONMODE];
    gSaveBlock2Ptr->optionsWindowFrameType = sOptionValues[MENUITEM_FRAMETYPE];
    for (i = 0; i < 11; i++)
        DBZ_SetOptionValue(i, sOptionValues[MENUITEM_DBZ_FIRST + i]);

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void HighlightOptionMenuItem(u8 row)
{
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(16, DISPLAY_WIDTH - 16));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(row * 16 + 40, row * 16 + 56));
}

static void DrawHeaderText(u8 scroll)
{
    FillWindowPixelBuffer(WIN_HEADER, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_Option, 8, 1, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, sText_ScrollHint, 184, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(u8 scroll, u8 selection)
{
    static const u8 sColorsValue[] = {TEXT_COLOR_WHITE, TEXT_COLOR_GREEN, TEXT_COLOR_LIGHT_GREEN};
    static const u8 sColorsSelected[] = {TEXT_COLOR_WHITE, TEXT_COLOR_RED, TEXT_COLOR_LIGHT_RED};
    u8 row, buf[24];

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (row = 0; row < VISIBLE_ROWS; row++)
    {
        u8 item = scroll + row;
        u8 y = row * 16 + 1;
        const u8 *colors = (item == selection) ? sColorsSelected : sColorsValue;
        if (item >= MENUITEM_COUNT)
            break;
        AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenuItemsNames[item], 8, y, TEXT_SKIP_DRAW, NULL);
        if (ValueCount(item) == 0)
            continue;
        if (item == MENUITEM_FRAMETYPE)
        {
            u8 *end = StringCopy(buf, sText_V_Type);
            ConvertIntToDecimalStringN(end, sOptionValues[item] + 1, STR_CONV_MODE_LEFT_ALIGN, 2);
        }
        else
        {
            StringCopy(buf, ValueText(item, sOptionValues[item]));
        }
        AddTextPrinterParameterized3(WIN_OPTIONS, FONT_NORMAL, 106, y, colors, TEXT_SKIP_DRAW, sText_LeftArrow);
        AddTextPrinterParameterized3(WIN_OPTIONS, FONT_NORMAL, 152 - GetStringWidth(FONT_NORMAL, buf, 0) / 2, y, colors, TEXT_SKIP_DRAW, buf);
        AddTextPrinterParameterized3(WIN_OPTIONS, FONT_NORMAL, 192, y, colors, TEXT_SKIP_DRAW, sText_RightArrow);
    }
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  3,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  3, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  3,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  4, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
