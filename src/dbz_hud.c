// PokeBall Orange overworld HUD: a small panel in the bottom-left corner with Goku's head (hair shows
// the current Saiyan form), the selected special move + charge meter, his HP bar and power level.
// It is one 64x32 sprite whose pixels are redrawn only when something changes.
// Hidden while scripts/menus run and when turned off in the OPTION menu.
#include "global.h"
#include "dbz.h"
#include "dma3.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "main.h"
#include "overworld.h"
#include "script.h"
#include "sprite.h"

#include "data/dbz_hud.h"

#define TAG_DBZ_HUD      0x2F32
#define TAG_DBZ_HUD_PAL  0x2F30   // shared with the drop shadows (index 1 is the shadow colour)

static EWRAM_DATA u8 sHudTiles[32 * 32] = {0};   // 64x32 at 4bpp, 1D sprite layout (8 x 4 tiles)
static EWRAM_DATA u8 sHudSprite = 0;
static EWRAM_DATA struct {
    u16 hp, maxHp;
    u8 move, charge, form, valid;
    u32 pl;
} sHudLast = {0};

static void SpriteCB_Hud(struct Sprite *sprite) { }

static const struct OamData sOam_Hud = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(64x32),
    .size = SPRITE_SIZE(64x32),
    .priority = 0,
};

static const struct SpriteTemplate sHudTemplate = {
    .tileTag = TAG_DBZ_HUD, .paletteTag = TAG_DBZ_HUD_PAL, .oam = &sOam_Hud,
    .anims = gDummySpriteAnimTable, .images = NULL, .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Hud,
};

static void Px(u8 x, u8 y, u8 c)
{
    u8 *b;
    if (x >= 64 || y >= 32)
        return;
    b = &sHudTiles[((y >> 3) * 8 + (x >> 3)) * 32 + (y & 7) * 4 + ((x & 7) >> 1)];
    if (x & 1)
        *b = (*b & 0x0F) | (c << 4);
    else
        *b = (*b & 0xF0) | c;
}

static void Rect(u8 x, u8 y, u8 w, u8 h, u8 c)
{
    u8 i, j;
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++)
            Px(x + i, y + j, c);
}

static u8 GlyphIndex(u8 ch)
{
    u8 i;
    for (i = 0; sHudFontOrder[i] != 0; i++)
        if (sHudFontOrder[i] == ch)
            return i;
    return 0;
}

// ASCII text in the 3x5 font, with a drop shadow; returns the end x
static u8 Text(u8 x, u8 y, const char *s, u8 color)
{
    while (*s)
    {
        const u8 *g = sHudFont[GlyphIndex(*s)];
        u8 r, c;
        for (r = 0; r < 5; r++)
            for (c = 0; c < 3; c++)
                if (g[r] & (4 >> c))
                {
                    Px(x + c + 1, y + r + 1, 4);
                    Px(x + c, y + r, color);
                }
        x += 4;
        s++;
    }
    return x;
}

static u8 Number(u8 x, u8 y, u32 n, u8 color)
{
    char buf[11];
    u8 i = 10;
    buf[10] = 0;
    do
    {
        buf[--i] = '0' + n % 10;
        n /= 10;
    } while (n && i);
    return Text(x, y, &buf[i], color);
}

static void Bar(u8 x, u8 y, u8 w, u8 h, u16 cur, u16 max, u8 color)
{
    u16 fill = max ? (u32)w * cur / max : 0;
    if (cur && !fill)
        fill = 1;
    Rect(x - 1, y - 1, w + 2, h + 2, 4);
    Rect(x, y, w, h, 5);
    if (fill)
        Rect(x, y, fill, h, color);
}

static void DrawCharge(u8 charge)
{
    Bar(20, 11, 41, 3, charge, 16, charge >= 16 ? 10 : 9);
}

static void Draw(u16 hp, u16 maxHp, u8 move, u8 charge, u8 form, u32 pl)
{
    static const char *const sForms[] = {"BASE", "SSJ", "SSJ2", "SSJ3", "FUSE"};
    static const char *const sMoves[] = {"KI BLAST", "KAMEHAMEHA", "SPIRIT BOMB"};
    static const u8 sMoveColors[] = {10, 9, 3};
    u8 x, y, hpColor;

    for (y = 0; y < 32; y++)
        for (x = 0; x < 64; x++)
            Px(x, y, sHudPanel[y][x]);
    // round backdrop so black hair reads against the dark panel
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++)
            if ((x * 2 - 15) * (x * 2 - 15) + (y * 2 - 15) * (y * 2 - 15) <= 15 * 15)
                Px(2 + x, 1 + y, 9);
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++)
            if (sHudIcons[form][y][x])
                Px(2 + x, 1 + y, sHudIcons[form][y][x]);

    if (move > 2)
        move = 0;
    Text(move == 2 ? 18 : 20, 3, sMoves[move], sMoveColors[move]);
    DrawCharge(charge);

    hpColor = (hp * 5 <= maxHp) ? 8 : ((hp * 2 <= maxHp) ? 7 : 6);
    Text(2, 18, "HP", 3);
    Bar(12, 19, 49, 3, hp, maxHp, hpColor);

    x = Text(2, 25, "PL", 7);
    Number(x + 2, 25, pl, 3);
    Text(62 - 4 * (form == 1 ? 3 : 4), 25, sForms[form], form ? 12 : 3);
}

static bool8 HudValid(void)
{
    u8 id = sHudSprite;
    return id < MAX_SPRITES && gSprites[id].inUse && gSprites[id].callback == SpriteCB_Hud;
}

static bool8 HudWanted(void)
{
    if (!DBZ_OptHud() || gMain.callback2 != CB2_Overworld)
        return FALSE;
    if (!FlagGet(FLAG_SYS_POKEMON_GET))
        return FALSE;
    if (DBZ_IsFighting())
        return TRUE;     // the fight HUD sits at the top; this one stays at the bottom
    if (ScriptContext_IsEnabled() || ArePlayerFieldControlsLocked())
        return FALSE;
    return TRUE;
}

void DBZ_UpdateHud(void)
{
    u16 hp, maxHp;
    u8 move, charge, form;
    u32 pl;
    bool8 want = HudWanted();

    if (!want)
    {
        if (HudValid())
            gSprites[sHudSprite].invisible = TRUE;
        return;
    }
    if (!HudValid())
    {
        struct SpriteSheet sheet = { sHudTiles, sizeof(sHudTiles), TAG_DBZ_HUD };
        struct SpritePalette pal = { gDBZHudPalette, TAG_DBZ_HUD_PAL };
        if (IndexOfSpritePaletteTag(TAG_DBZ_HUD_PAL) == 0xFF)
            LoadSpritePalette(&pal);
        if (GetSpriteTileStartByTag(TAG_DBZ_HUD) == 0xFFFF)
            LoadSpriteSheet(&sheet);
        sHudSprite = CreateSprite(&sHudTemplate, 4 + 32, DISPLAY_HEIGHT - 4 - 16, 0);
        if (sHudSprite == MAX_SPRITES)
            return;
        sHudLast.valid = FALSE;
    }
    gSprites[sHudSprite].invisible = FALSE;

    hp = DBZ_GokuHp();
    maxHp = DBZ_GokuHpMaxNow();
    move = DBZ_GetSelectedMove();
    charge = DBZ_GetChargeLevel();
    form = DBZ_IsFused() ? 4 : DBZ_GetForm();
    pl = DBZ_CurrentPowerLevel();
    if (sHudLast.valid && sHudLast.hp == hp && sHudLast.maxHp == maxHp && sHudLast.move == move
     && sHudLast.charge == charge && sHudLast.form == form && sHudLast.pl == pl)
        return;
    if (sHudLast.valid && sHudLast.hp == hp && sHudLast.maxHp == maxHp && sHudLast.move == move
     && sHudLast.form == form && sHudLast.pl == pl)
    {
        // only the charge meter moved: redraw just that (a full redraw costs most of a frame)
        sHudLast.charge = charge;
        DrawCharge(charge);
        RequestDma3Copy(sHudTiles + 8 * 32, (void *)(OBJ_VRAM0 + (GetSpriteTileStartByTag(TAG_DBZ_HUD) + 8) * TILE_SIZE_4BPP), 8 * 32, 1);   // tile row 1
        return;
    }
    sHudLast.valid = TRUE;
    sHudLast.hp = hp;
    sHudLast.maxHp = maxHp;
    sHudLast.move = move;
    sHudLast.charge = charge;
    sHudLast.form = form;
    sHudLast.pl = pl;
    Draw(hp, maxHp, move, charge, form, pl);
    RequestDma3Copy(sHudTiles, (void *)(OBJ_VRAM0 + GetSpriteTileStartByTag(TAG_DBZ_HUD) * TILE_SIZE_4BPP), sizeof(sHudTiles), 1);   // 32-bit copy at the next VBlank
}
