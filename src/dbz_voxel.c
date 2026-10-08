// PokeBall Orange: voxel Flying Nimbus flight (prototype).
// After Goku takes off on the Nimbus, a 3D view of Hoenn flies him from where he was to where he picked on
// the fly map, then the normal arrival plays. The world is a 512x512 heightmap + colour map built from the
// region map (tools/pbo/gen_voxel_hoenn.py). Rendering is the classic "voxel space" heightmap ray caster:
// for each distance step, a line of samples across the screen, drawn front to back with a per-column
// y-buffer, into a Mode 4 (8bpp bitmap) page with double buffering. Columns are 2 pixels wide (one
// halfword write per row). The inner loop runs as ARM code from IWRAM.
#include "global.h"
#include "dbz.h"
#include "dbz_voxel.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "overworld.h"
#include "palette.h"
#include "region_map.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "trig.h"
#include "constants/region_map_sections.h"
#include "constants/rgb.h"
#include "constants/songs.h"

#include "data/dbz_voxel_data.h"

static const u16 sVoxelTerrain[] = INCBIN_U16("graphics/dbz/voxel/terrain.bin");

#define SCREEN_COLS     120                 // 2-pixel columns
#define SCREEN_ROWS     160
#define PAGE_HALFWORDS  (240 * 160 / 2)
#define DISPCNT_PAGE    0x0010              // Mode 4: show page 1 (0x0600A000)
#define NUM_STEPS       72
#define FAR_DISTANCE    260                 // texels
#define PROJ_SCALE      70                  // screen rows per height unit at distance 1 (x 1/z)
#define CRUISE_CLEARANCE 44
#define HORIZON_ROW     42
#define OBJ_TILE_BASE   512                 // bitmap modes: sprite tiles 0-511 overlap the frame buffer
#define WORLD_MASK      (VOXEL_WORLD_SIZE - 1)

struct VoxelFlight
{
    MainCallback done;
    s32 camX, camY;             // 16.16 texels
    s32 fromX, fromY, toX, toY; // texels
    s32 dirX, dirY;             // Q14 unit vector (forward)
    s32 camH;                   // 8.8 height units
    s32 speed;                  // 16.16 texels per vblank
    u32 lastVblank;
    u16 frames, vblanks;        // rendered frames / elapsed vblanks, for the frame-rate readout
    u8 page;
    u8 state;
    u8 bob;
    u16 zStep[NUM_STEPS];
    u16 invZ[NUM_STEPS];        // (PROJ_SCALE << 8) / z
    u8 fog[NUM_STEPS];          // palette offset for the step's fog level
    u16 skyRow[SCREEN_ROWS];    // sky colour pair per row for the current horizon
    s16 skyHorizon;
    u8 riderPal;
    bool8 riderWide;
};

static EWRAM_DATA struct VoxelFlight *sVoxel = NULL;
static EWRAM_DATA s16 sPendingFromX = 0, sPendingFromY = 0, sPendingToX = 0, sPendingToY = 0;
static EWRAM_DATA bool8 sPending = FALSE;
EWRAM_DATA u16 gDBZVoxelFrameRate = 0;   // frames per 10 seconds of the last flight (read by the test harness)

static void CB2_VoxelFlight(void);
static void VBlankCB_VoxelFlight(void);

// ------------------------------------------------------------------ setup from the fly map
static void MapsecToWorld(u16 mapsec, s16 *x, s16 *y)
{
    const struct RegionMapLocation *loc = &gRegionMapEntries[mapsec];
    *x = VOXEL_ORIGIN_X + ((loc->x + VOXEL_CURSOR_X_MIN) * 8 + loc->width * 4) * VOXEL_SCALE;
    *y = VOXEL_ORIGIN_Y + ((loc->y + VOXEL_CURSOR_Y_MIN) * 8 + loc->height * 4) * VOXEL_SCALE;
}

// region_map.c: a Nimbus flight was chosen on the fly map
void DBZ_VoxelFlight_Prepare(u16 fromMapsec, u16 toMapsec)
{
    sPending = FALSE;
    if (!DBZ_OptNimbus3D() || fromMapsec >= MAPSEC_NONE || toMapsec >= MAPSEC_NONE)
        return;
    MapsecToWorld(fromMapsec, &sPendingFromX, &sPendingFromY);
    MapsecToWorld(toMapsec, &sPendingToX, &sPendingToY);
    sPending = TRUE;
}

bool8 DBZ_VoxelFlight_IsPending(void)
{
    return sPending;
}

// ------------------------------------------------------------------ renderer
static u32 ISqrt(u32 n)
{
    u32 r = 0, b = 1u << 30;
    while (b > n)
        b >>= 2;
    while (b)
    {
        if (n >= r + b)
        {
            n -= r + b;
            r = (r >> 1) + b;
        }
        else
        {
            r >>= 1;
        }
        b >>= 2;
    }
    return r;
}

static inline u32 TerrainHeight(s32 x, s32 y)
{
    return sVoxelTerrain[((y & WORLD_MASK) << 9) | (x & WORLD_MASK)] >> 8;
}

ARM_FUNC __attribute__((section(".iwram.code"), noinline))
static void RenderVoxelFrame(u16 *page, const u16 *terrain, const struct VoxelFlight *v, s32 horizon)
{
    u8 ybuf[SCREEN_COLS];
    s32 i, k;
    s32 camH = v->camH >> 8;
    // forward and right vectors, Q14
    s32 fx = v->dirX, fy = v->dirY;
    s32 rx = -fy, ry = fx;

    for (i = 0; i < SCREEN_COLS; i++)
        ybuf[i] = SCREEN_ROWS;

    for (k = 0; k < NUM_STEPS; k++)
    {
        s32 z = v->zStep[k];
        s32 invZ = v->invZ[k];
        u32 fog = v->fog[k];
        // left edge of the view at distance z, and the step per column (16.16)
        s32 px = v->camX + ((z * (fx - rx)) << 2);
        s32 py = v->camY + ((z * (fy - ry)) << 2);
        s32 sx = (z * rx * 2 / SCREEN_COLS) << 2;
        s32 sy = (z * ry * 2 / SCREEN_COLS) << 2;
        u8 *yb = ybuf;
        u16 *col = page;

        for (i = 0; i < SCREEN_COLS; i++, px += sx, py += sy, yb++, col++)
        {
            u32 t = terrain[(((u32)py >> 16) & WORLD_MASK) << 9 | (((u32)px >> 16) & WORLD_MASK)];
            s32 top = (((camH - (s32)(t >> 8)) * invZ) >> 8) + horizon;
            u32 y = *yb;
            if (top < (s32)y)
            {
                u32 c = (t & 0xFF) + fog;
                u32 pair = c | (c << 8);
                u16 *p;
                if (top < 0)
                    top = 0;
                p = col + top * (240 / 2);
                *yb = top;
                for (; (u32)top < y; top++, p += 240 / 2)
                    *p = pair;
            }
        }
    }
    // sky above whatever the terrain left uncovered
    for (i = 0; i < SCREEN_COLS; i++)
    {
        u16 *p = page + i;
        const u16 *sky = v->skyRow;
        u32 y, end = ybuf[i];
        for (y = 0; y < end; y++, p += 240 / 2)
            *p = sky[y];
    }
}

// ------------------------------------------------------------------ flight
void DBZ_VoxelFlight_Start(MainCallback done)
{
    struct VoxelFlight *v;
    s32 dx, dy, len, k, z, dz;

    sPending = FALSE;
    SetMainCallback1(NULL);
    sVoxel = v = AllocZeroed(sizeof(*v));
    v->skyHorizon = -1;
    v->done = done;
    v->fromX = sPendingFromX;
    v->fromY = sPendingFromY;
    v->toX = sPendingToX;
    v->toY = sPendingToY;
    dx = v->toX - v->fromX;
    dy = v->toY - v->fromY;
    len = ISqrt(dx * dx + dy * dy);
    if (len == 0)
    {
        v->dirX = 0;
        v->dirY = -(1 << 14);
    }
    else
    {
        v->dirX = (dx << 14) / len;
        v->dirY = (dy << 14) / len;
    }
    // start a little behind the town, cross the distance in about 2-5 seconds
    v->camX = (v->fromX << 16) - v->dirX * 24 * 4;
    v->camY = (v->fromY << 16) - v->dirY * 24 * 4;
    v->speed = ((len + 40) << 16) / 200;
    if (v->speed < (3 << 16) / 2)
        v->speed = (3 << 16) / 2;
    if (v->speed > (4 << 16))
        v->speed = 4 << 16;
    v->camH = (TerrainHeight(v->camX >> 16, v->camY >> 16) + 24) << 8;

    for (k = 0, z = 2, dz = 256; k < NUM_STEPS; k++)
    {
        v->zStep[k] = z;
        v->invZ[k] = (PROJ_SCALE << 8) / z;
        v->fog[k] = VOXEL_NUM_BASE_COLOURS * min(3, z * 4 / FAR_DISTANCE);
        dz += 20;                  // steps grow with distance (8.8)
        z += dz >> 8;
    }
    SetMainCallback2(CB2_VoxelFlight);
}

static void FinishFlight(void)
{
    MainCallback done = sVoxel->done;
    if (sVoxel->vblanks)
        gDBZVoxelFrameRate = sVoxel->frames * 600 / sVoxel->vblanks;
    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    FREE_AND_SET_NULL(sVoxel);
    done();
}

static void VBlankCB_VoxelFlight(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void UpdateCamera(struct VoxelFlight *v, u32 elapsed)
{
    s32 x, y, ahead, ground, target, remain;
    v->camX += (v->dirX * (v->speed >> 8) >> 6) * (s32)elapsed;
    v->camY += (v->dirY * (v->speed >> 8) >> 6) * (s32)elapsed;
    x = v->camX >> 16;
    y = v->camY >> 16;
    // keep clear of the ground here and a little ahead, ease down near the destination
    ground = TerrainHeight(x, y);
    for (ahead = 8; ahead <= 64; ahead += 8)
    {
        u32 g = TerrainHeight(x + (v->dirX * ahead >> 14), y + (v->dirY * ahead >> 14));
        if (g > (u32)ground)
            ground = g;
    }
    remain = ISqrt((v->toX - x) * (v->toX - x) + (v->toY - y) * (v->toY - y));
    target = ground + (remain < 60 ? 20 + remain * (CRUISE_CLEARANCE - 20) / 60 : CRUISE_CLEARANCE);
    // climb quickly, sink slowly
    if ((target << 8) > v->camH)
        v->camH += ((target << 8) - v->camH) * (s32)min(elapsed, 4) / 5;
    else
        v->camH += ((target << 8) - v->camH) * (s32)min(elapsed, 4) / 16;
    v->bob += elapsed;
}

// Goku (back view) riding the Nimbus at the bottom of the screen. Tiles go straight into the upper half
// of sprite VRAM (the only part bitmap modes leave to sprites); OAM entries are written by hand.
extern const u32 gFieldEffectObjectPic_Bird[];   // PokeBall Orange: the Nimbus cloud

static void LoadRiderSprites(struct VoxelFlight *v)
{
    const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_STATE_NORMAL));
    u8 pal = LoadPlayerObjectEventPalette(gSaveBlock2Ptr->playerGender);
    u16 *obj = (u16 *)(OBJ_VRAM0 + OBJ_TILE_BASE * TILE_SIZE_4BPP);
    CpuCopy16(gFieldEffectObjectPic_Bird, obj, 32 * 32 / 2);
    // frame 1 of a walking sprite faces north (sheets stored as one entry use relative frames)
    if (info->images[0].relativeFrames)
        CpuCopy16((const u8 *)info->images[0].data + info->images[0].size, obj + 32 * 32 / 4, info->images[0].size);
    else
        CpuCopy16(info->images[1].data, obj + 32 * 32 / 4, info->images[1].size);
    v->riderPal = pal;
    v->riderWide = info->width >= 32;
}

static void UpdateRiderSprites(struct VoxelFlight *v)
{
    struct OamData *cloud = &gMain.oamBuffer[0], *goku = &gMain.oamBuffer[1];
    s32 bob = gSineTable[(v->bob * 4) & 0xFF] >> 6;   // +-4 px
    *cloud = (struct OamData){0};
    cloud->shape = SPRITE_SHAPE(32x32);
    cloud->size = SPRITE_SIZE(32x32);
    cloud->x = 104;
    cloud->y = 118 + bob;
    cloud->tileNum = OBJ_TILE_BASE;
    cloud->paletteNum = v->riderPal;
    *goku = (struct OamData){0};
    if (v->riderWide)
    {
        goku->shape = SPRITE_SHAPE(32x32);
        goku->size = SPRITE_SIZE(32x32);
        goku->x = 104;
    }
    else
    {
        goku->shape = SPRITE_SHAPE(16x32);
        goku->size = SPRITE_SIZE(16x32);
        goku->x = 112;
    }
    goku->y = 98 + bob;
    goku->tileNum = OBJ_TILE_BASE + 16;
    goku->paletteNum = v->riderPal;
}

static void UpdateSky(struct VoxelFlight *v, s32 horizon)
{
    s32 y;
    if (v->skyHorizon == horizon)
        return;
    v->skyHorizon = horizon;
    for (y = 0; y < SCREEN_ROWS; y++)
    {
        u32 c = VOXEL_SKY_FIRST + (y >= horizon ? 15 : (y * 15) / horizon);
        v->skyRow[y] = c | (c << 8);
    }
}

static void CB2_VoxelFlight(void)
{
    struct VoxelFlight *v = sVoxel;
    u32 now, elapsed;
    s32 x, y;

    switch (v->state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetHBlankCallback(NULL);
        ScanlineEffect_Stop();
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_FORCED_BLANK);
        DmaFill16(3, 0, (void *)VRAM, 0x14000);
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BG2PA, 0x100);
        SetGpuReg(REG_OFFSET_BG2PB, 0);
        SetGpuReg(REG_OFFSET_BG2PC, 0);
        SetGpuReg(REG_OFFSET_BG2PD, 0x100);
        SetGpuReg(REG_OFFSET_BG2X_L, 0);
        SetGpuReg(REG_OFFSET_BG2X_H, 0);
        SetGpuReg(REG_OFFSET_BG2Y_L, 0);
        SetGpuReg(REG_OFFSET_BG2Y_H, 0);
        SetGpuReg(REG_OFFSET_BG2CNT, 0);
        LoadPalette(sVoxelPalette, BG_PLTT_ID(0), sizeof(sVoxelPalette));
        LoadRiderSprites(v);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_VoxelFlight);
        v->page = 1;
        v->lastVblank = gMain.vblankCounter1;
        v->state++;
        // fall through: draw the first frame into the hidden page before the screen comes on
    case 1:
    case 2:
        now = gMain.vblankCounter1;
        elapsed = now - v->lastVblank;
        v->lastVblank = now;
        if (elapsed > 6)
            elapsed = 6;
        if (v->state == 1 && elapsed)
        {
            v->frames++;
            v->vblanks += elapsed;
        }
        UpdateCamera(v, elapsed);
        UpdateRiderSprites(v);
        x = HORIZON_ROW + (gSineTable[(v->bob * 2) & 0xFF] >> 7);
        UpdateSky(v, x);
        RenderVoxelFrame((u16 *)(VRAM + (v->page ? 0xA000 : 0)), sVoxelTerrain, v, x);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_4 | DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP
                                      | (v->page ? DISPCNT_PAGE : 0));
        v->page ^= 1;
        x = v->camX >> 16;
        y = v->camY >> 16;
        if (v->state == 1)
        {
            s32 dx = v->toX - x, dy = v->toY - y;
            // arrived (or passed it), or B: fade out
            if (JOY_NEW(B_BUTTON) || dx * v->dirX + dy * v->dirY < (12 << 14))
            {
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
                v->state = 2;
            }
        }
        else if (!gPaletteFade.active)
        {
            FinishFlight();
            return;
        }
        UpdatePaletteFade();
        break;
    }
}
