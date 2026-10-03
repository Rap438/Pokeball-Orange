#ifndef GUARD_DBZ_H
#define GUARD_DBZ_H

#include "field_control_avatar.h"

#include "constants/dbz.h"

#define OBJ_EVENT_GFX_GOKU_SSJ   OBJ_EVENT_GFX_UNUSED_NATU_DOLL
#define OBJ_EVENT_GFX_GOKU_SSJ2  OBJ_EVENT_GFX_UNUSED_MAGNEMITE_DOLL
#define OBJ_EVENT_GFX_GOKU_SSJ3  OBJ_EVENT_GFX_UNUSED_SQUIRTLE_DOLL
#define OBJ_EVENT_GFX_DRAGON_BALL OBJ_EVENT_GFX_UNUSED_WOOPER_DOLL

u8 DBZ_GetForm(void);
u8 DBZ_GetMaxForm(void);
u8 DBZ_GetPlayerNormalGfx(void);
bool8 DBZ_HandleFieldInput(struct FieldInput *input);
void DBZ_ResetFieldInputState(void);
bool8 DBZ_IsChargingBlast(void);

// specials
u16 DBZ_GetFormSpecial(void);
u16 DBZ_GetNextForm(void);
void DBZ_StartAura(void);
void DBZ_ApplyNextForm(void);
void DBZ_FireBlast(void);

// Dragon Balls
extern bool8 gDBZNimbusFly;
void DBZ_ScatterDragonBalls(void);
void DBZ_InjectDragonBalls(void);
u8 DBZ_GetCurrentMapObjectEventCount(void);
void DBZ_RestoreDragonBallScripts(void);
void DBZ_DragonBallStepUpdate(void);
u8 DBZ_CountDragonBallsInBag(void);
void DBZ_OpenDragonRadar(void (*callback)(void));
u16 DBZ_GetTalkedDragonBallItem(void);
void DBZ_CollectTalkedDragonBall(void);
u16 DBZ_CountDragonBalls(void);
void DBZ_ShenronAppear(void);
void DBZ_ShenronDepart(void);
void DBZ_ChooseWish(void);
u16 DBZ_GrantWish(void);
void DBZ_Debug_WarpToDragonBall(void);
void DBZ_Debug_GiveSixBalls(void);

#endif // GUARD_DBZ_H
