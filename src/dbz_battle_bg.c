// PokeBall Orange: battle backgrounds that show the overworld spot where the battle started.
// The 16x7 metatiles around Goku are copied (pre-composited, see tools/gen_battle_views.py) into BG3,
// with the two battle platforms shaded into the ground, and tinted for the time of day.
#include "global.h"
#include "seasons.h"
#include "dbz.h"
#include "battle.h"
#include "battle_anim.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "task.h"
#include "trig.h"
#include "constants/map_types.h"
#include "constants/rgb.h"

struct DbzBattleView {
    const struct Tileset *primary;
    const struct Tileset *secondary;
    const u32 *tiles;
    const u16 *palettes;        // 6 x 16 colours
    const u16 *metatiles;       // 1024 metatiles x 4 entries: tile index | palette << 12
    const u8 *darkLut;          // 6 x 16
    const u8 *lightLut;
};

extern const struct Tileset gTileset_General;

#include "data/dbz_battle_views.h"

#define VIEW_W 16   // metatiles (256 px, covers horizontal intro scrolling)
#define VIEW_H 7    // metatiles (112 px, the area above the text box)

// battle BG palette slots that nothing else in battle uses
static const u8 sPalSlots[6] = {2, 3, 4, 10, 11, 12};

// platform ellipses in BG3 pixel space (enemy, player)
static const s16 sPlatforms[2][4] = {
    {176, 64, 62, 15},
    {64, 116, 64, 16},
};

static const struct DbzBattleView *FindView(void)
{
    u8 i;
    const struct MapLayout *layout = gMapHeader.mapLayout;
    if (layout == NULL)
        return NULL;
    const struct DbzBattleView *views = sBattleViewsBySeason[Season_Get()];   // PokeBall Orange: seasonal art
    for (i = 0; i < NUM_BATTLE_VIEWS; i++)
        if (views[i].primary == layout->primaryTileset && views[i].secondary == layout->secondaryTileset)
            return &views[i];
    return NULL;
}

// 0 outside, 1 rim, 2 inside
static u8 PlatformAt(s16 x, s16 y)
{
    u8 i;
    for (i = 0; i < 2; i++)
    {
        s32 dx = x - sPlatforms[i][0], dy = y - sPlatforms[i][1];
        s32 rx = sPlatforms[i][2], ry = sPlatforms[i][3];
        s32 v = dx * dx * ry * ry + dy * dy * rx * rx;
        s32 r = rx * rx * ry * ry;
        if (v <= r)
        {
            // rim: outer ~2px band
            s32 rx2 = rx - 2, ry2 = ry - 1;
            if (dx * dx * ry2 * ry2 + dy * dy * rx2 * rx2 > rx2 * rx2 * ry2 * ry2)
                return 1;
            return 2;
        }
    }
    return 0;
}

static bool8 TileTouchesPlatform(s16 tx, s16 ty)
{
    u8 i;
    for (i = 0; i < 2; i++)
    {
        s16 x0 = sPlatforms[i][0] - sPlatforms[i][2], x1 = sPlatforms[i][0] + sPlatforms[i][2];
        s16 y0 = sPlatforms[i][1] - sPlatforms[i][3], y1 = sPlatforms[i][1] + sPlatforms[i][3];
        if (tx * 8 + 7 >= x0 && tx * 8 <= x1 && ty * 8 + 7 >= y0 && ty * 8 <= y1)
            return TRUE;
    }
    return FALSE;
}

// ------------------------------------------------------------------ living backgrounds
// Water and grass colours of the battle view gently ripple (computed from the unfaded palette, so fades,
// the time-of-day tint and move animations are left alone: it pauses while either is running).
static EWRAM_DATA bool8 sMapBgDrawn = FALSE;

static void Task_BgShimmer(u8 taskId);

// battle_main: once the battle's tasks are set up
void DBZ_StartBattleBgShimmer(void)
{
    if (sMapBgDrawn == TRUE && !FuncIsActiveTask(Task_BgShimmer))
        CreateTask(Task_BgShimmer, 1);
    sMapBgDrawn = FALSE;
}

static void Task_BgShimmer(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i, c;
    if (!gMain.inBattle)
    {
        DestroyTask(taskId);
        return;
    }
    if (++data[0] % 4 != 0 || gPaletteFade.active || gAnimScriptActive)
        return;
    data[1] += 3;
    for (i = 0; i < 6; i++)
    {
        u16 base = BG_PLTT_ID(sPalSlots[i]);
        for (c = 1; c < 16; c++)
        {
            u16 col = gPlttBufferUnfaded[base + c];
            s16 r = col & 31, g = (col >> 5) & 31, b = (col >> 10) & 31, d;
            if (b > r + 3 && b >= g)
                d = Sin((data[1] * 4 + c * 40) & 0xFF, 3);           // water: sparkle
            else if (g > r + 2 && g > b + 2)
                d = Sin((data[1] * 2 + c * 24 + i * 64) & 0xFF, 2);  // grass: sway
            else
                continue;
            r += d; g += d; b += d;
            r = r < 0 ? 0 : (r > 31 ? 31 : r);
            g = g < 0 ? 0 : (g > 31 ? 31 : g);
            b = b < 0 ? 0 : (b > 31 ? 31 : b);
            gPlttBufferFaded[base + c] = r | (g << 5) | (b << 10);
        }
    }
}

bool8 DBZ_TryDrawMapBattleBackground(void)
{
    const struct DbzBattleView *view;
    struct ObjectEvent *player;
    s16 mx, my, cx, cy;
    u16 *vramMap = (u16 *)BG_SCREEN_ADDR(26);
    u8 *vramTiles = (u8 *)BG_CHAR_ADDR(2);
    u16 next = 1;   // tile 0 stays blank
    u8 i;

    if (!DBZ_OptMapBattleBg())
        return FALSE;
    if (!IsMapTypeOutdoors(gMapHeader.mapType) && gMapHeader.mapType != MAP_TYPE_UNDERGROUND)
        return FALSE;
    view = FindView();
    if (view == NULL)
        return FALSE;
    player = &gObjectEvents[gPlayerAvatar.objectEventId];
    cx = player->currentCoords.x;
    cy = player->currentCoords.y;

    CpuFill32(0, vramTiles, 32);
    CpuFill16(0, vramMap, 32 * 64 * 2);
    for (my = 0; my < VIEW_H; my++)
    {
        for (mx = 0; mx < VIEW_W; mx++)
        {
            u16 mid = MapGridGetMetatileIdAt(cx - VIEW_W / 2 + mx, cy - (VIEW_H - 2) + my) & 0x3FF;
            u8 q;
            for (q = 0; q < 4; q++)
            {
                u16 e = view->metatiles[mid * 4 + q];
                s16 tx = mx * 2 + (q & 1), ty = my * 2 + (q >> 1);
                u8 pal;
                u32 buf[8];
                if (e == 0xFFFF)
                    continue;
                pal = e >> 12;
                CpuCopy32(&view->tiles[(e & 0xFFF) * 8], buf, 32);
                if (TileTouchesPlatform(tx, ty))
                {
                    // shade the ground disc the Pokemon stand on
                    u8 *px = (u8 *)buf;
                    u8 py, pxx;
                    for (py = 0; py < 8; py++)
                    {
                        for (pxx = 0; pxx < 8; pxx++)
                        {
                            u8 *b = &px[py * 4 + pxx / 2];
                            u8 c = (pxx & 1) ? (*b >> 4) : (*b & 0xF);
                            u8 where = PlatformAt(tx * 8 + pxx, ty * 8 + py);
                            if (where == 0)
                                continue;
                            c = (where == 2) ? view->darkLut[pal * 16 + c] : view->lightLut[pal * 16 + c];
                            if (pxx & 1)
                                *b = (*b & 0x0F) | (c << 4);
                            else
                                *b = (*b & 0xF0) | c;
                        }
                    }
                }
                CpuCopy32(buf, vramTiles + next * 32, 32);
                vramMap[ty * 32 + tx] = next | (sPalSlots[pal] << 12);
                vramMap[(ty + 32) * 32 + tx] = next | (sPalSlots[pal] << 12);   // second screen, same picture
                next++;
            }
        }
    }
    for (i = 0; i < 6; i++)
    {
        LoadPalette(&view->palettes[i * 16], BG_PLTT_ID(sPalSlots[i]), PLTT_SIZE_4BPP);
        // same time of day as the overworld the battle started on (engine day/night blend)
        if (MapHasNaturalLight(gMapHeader.mapType))
        {
            TimeMixPalettes(1u << sPalSlots[i], gPlttBufferUnfaded, gPlttBufferUnfaded, &gTimeBlend.startBlend, &gTimeBlend.endBlend, gTimeBlend.weight);
            CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(sPalSlots[i])], &gPlttBufferFaded[BG_PLTT_ID(sPalSlots[i])], PLTT_SIZE_4BPP);
        }
    }
    sMapBgDrawn = TRUE;
    return TRUE;
}

// ------------------------------------------------------------------ trainer power-up on their last Pokemon
static EWRAM_DATA bool8 sPowerUpShown = FALSE;

void DBZ_ResetTrainerPowerUp(void)
{
    sPowerUpShown = FALSE;
}

bool8 DBZ_ShouldTrainerPowerUp(u8 battler)
{
    u8 i, alive = 0;
    if (sPowerUpShown || !(gBattleTypeFlags & BATTLE_TYPE_TRAINER) || !DBZ_OptPowerUp())
        return FALSE;
    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_SAFARI))
        return FALSE;
    if (GetBattlerSide(battler) != B_SIDE_OPPONENT)
        return FALSE;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&gParties[B_TRAINER_OPPONENT_A][i], MON_DATA_SPECIES);
        if (species != SPECIES_NONE && !GetMonData(&gParties[B_TRAINER_OPPONENT_A][i], MON_DATA_IS_EGG) && GetMonData(&gParties[B_TRAINER_OPPONENT_A][i], MON_DATA_HP) != 0)
            alive++;
    }
    if (alive != 1)
        return FALSE;
    sPowerUpShown = TRUE;
    return TRUE;
}

// ------------------------------------------------------------------ battle menu frame colours
// The FIGHT/BAG and move menus use the player's window frame; recolour it by brightness into the
// PokeBall Orange look (navy panel, orange/gold rim) whatever frame type is selected.
void DBZ_RestyleBattleMenuFrame(u16 offset)
{
    u8 i;
    for (i = 1; i < 16; i++)
    {
        u16 c = gPlttBufferUnfaded[offset + i];
        u16 lum = ((c & 31) * 3 + ((c >> 5) & 31) * 6 + ((c >> 10) & 31)) / 10;
        u16 n;
        if (lum >= 28)
            n = RGB(2, 3, 9);          // panel
        else if (lum >= 19)
            n = RGB(31, 20, 4);        // gold rim
        else if (lum >= 11)
            n = RGB(25, 12, 0);        // orange rim
        else
            n = RGB(1, 1, 4);          // outline
        gPlttBufferUnfaded[offset + i] = n;
        gPlttBufferFaded[offset + i] = n;
    }
}
