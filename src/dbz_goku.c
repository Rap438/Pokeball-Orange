// PokeBall Orange: Goku's techniques outside of battle.
// GOKU entry in the START menu: status, INSTANT TRANSMISSION (fly anywhere you've been, no Pokemon
// needed), HIDE KI (a reusable KI HIDER toggle). Techniques unlock with badges and are announced once.
// Also: Capsule Corp. GRAVITY ROOM training (double fight EXP for a while).
#include "global.h"
#include "dbz.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "main.h"
#include "overworld.h"
#include "palette.h"
#include "region_map.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/flags.h"
#include "constants/map_types.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/vars.h"

EWRAM_DATA bool8 gDBZInstantTransmission = FALSE;
#define PENDING_MAGIC 0xA5
static EWRAM_DATA u8 sPendingIT = 0;

// ------------------------------------------------------------------ techniques
bool8 DBZ_HasTechnique(u8 tech)
{
    switch (tech)
    {
    case DBZ_TECH_KAIOKEN:       return FlagGet(FLAG_BADGE05_GET);
    case DBZ_TECH_INSTANT_TRANS: return FlagGet(FLAG_BADGE06_GET);
    case DBZ_TECH_SPIRIT_BOMB:   return FlagGet(FLAG_BADGE08_GET);
    }
    return FALSE;
}

// after a badge: a technique that just unlocked and hasn't been announced yet (0 = none)
u16 DBZ_GetNewTechniqueHint(void)
{
    u16 seen = VarGet(VAR_DBZ_SEEN_FORMS);
    u8 t;
    for (t = DBZ_TECH_KAIOKEN; t <= DBZ_TECH_SPIRIT_BOMB; t++)
    {
        if (DBZ_HasTechnique(t) && !(seen & (0x80 << t)))
        {
            VarSet(VAR_DBZ_SEEN_FORMS, seen | (0x80 << t));
            return t;
        }
    }
    return 0;
}

// ------------------------------------------------------------------ hide ki / gravity training
u16 DBZ_ToggleHideKi(void)
{
    u16 misc = VarGet(VAR_DBZ_MISC) ^ DBZ_MISC_HIDE_KI;
    VarSet(VAR_DBZ_MISC, misc);
    if (misc & DBZ_MISC_HIDE_KI)
        VarSet(VAR_REPEL_STEP_COUNT, 200);
    else
        VarSet(VAR_REPEL_STEP_COUNT, 0);
    return (misc & DBZ_MISC_HIDE_KI) != 0;
}

bool8 DBZ_IsHidingKi(void)
{
    return (VarGet(VAR_DBZ_MISC) & DBZ_MISC_HIDE_KI) != 0;
}

u16 DBZ_GravityStepsLeft(void)
{
    return ((VarGet(VAR_DBZ_MISC) & DBZ_MISC_GRAVITY_MASK) >> DBZ_MISC_GRAVITY_SHIFT) * 2;
}

static void SetGravitySteps(u16 steps)
{
    u16 misc = VarGet(VAR_DBZ_MISC) & ~DBZ_MISC_GRAVITY_MASK;
    if (steps > 0x3FF * 2)
        steps = 0x3FF * 2;
    VarSet(VAR_DBZ_MISC, misc | ((steps / 2) << DBZ_MISC_GRAVITY_SHIFT));
}

// special: Dr. Brief's gravity room — 600 steps of 10x gravity training
void DBZ_StartGravityTraining(void)
{
    SetGravitySteps(600);
}

static EWRAM_DATA u8 sGravityHalfStep = 0;
void DBZ_OnStep(void)
{
    u16 g;
    if (DBZ_IsHidingKi() && VarGet(VAR_REPEL_STEP_COUNT) < 100)
        VarSet(VAR_REPEL_STEP_COUNT, 200);
    g = DBZ_GravityStepsLeft();
    if (g != 0 && (++sGravityHalfStep & 1) == 0)
        SetGravitySteps(g - 2);
}

// ------------------------------------------------------------------ instant transmission
// 0 not learned, 1 can't use here, 2 ok
u16 DBZ_CanUseInstantTransmission(void)
{
    if (!DBZ_HasTechnique(DBZ_TECH_INSTANT_TRANS))
        return 0;
    if (!Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) || DBZ_IsFighting())
        return 1;
    return 2;
}

void DBZ_QueueInstantTransmission(void)
{
    sPendingIT = PENDING_MAGIC;
}

static void Task_OpenITMap(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        gDBZNimbusFly = TRUE;          // the fly map returns straight to the field on cancel
        gDBZInstantTransmission = TRUE;
        SetMainCallback2(CB2_OpenFlyMap);
        DestroyTask(taskId);
    }
}

// called when Goku is free to move again after the GOKU menu
bool8 DBZ_TryStartPendingInstantTransmission(void)
{
    if (sPendingIT != PENDING_MAGIC)
        return FALSE;
    sPendingIT = 0;
    LockPlayerFieldControls();
    PlaySE(SE_M_TELEPORT);
    FadeScreen(FADE_TO_BLACK, 0);
    CreateTask(Task_OpenITMap, 0);
    return TRUE;
}

// Fly-out replacement: two fingers to the forehead, flicker, gone.
#define tTimer data[0]
static void Task_ITOut(u8 taskId)
{
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 *data = gTasks[taskId].data;
    if (tTimer == 0)
        PlaySE(SE_M_TELEPORT);
    if (tTimer < 24)
        player->invisible = (tTimer % 4) >= 2;
    else if (tTimer == 24)
    {
        player->invisible = TRUE;
        PlaySE(SE_WARP_OUT);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_WHITE);
    }
    else if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        return;
    }
    tTimer++;
}

void DBZ_StartInstantTransmissionOut(void)
{
    CreateTask(Task_ITOut, 80);
}

bool8 DBZ_InstantTransmissionOutActive(void)
{
    return FuncIsActiveTask(Task_ITOut);
}

static void Task_ITIn(u8 taskId)
{
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 *data = gTasks[taskId].data;
    if (tTimer == 0)
    {
        player->invisible = FALSE;
        PlaySE(SE_WARP_IN);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 12, 0, RGB_WHITE);
    }
    if (++tTimer > 20 && !gPaletteFade.active)
    {
        gDBZInstantTransmission = FALSE;
        DestroyTask(taskId);
    }
}

void DBZ_StartInstantTransmissionIn(void)
{
    CreateTask(Task_ITIn, 80);
}

bool8 DBZ_InstantTransmissionInActive(void)
{
    return FuncIsActiveTask(Task_ITIn);
}
#undef tTimer
