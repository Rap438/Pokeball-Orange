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

// fx helpers
bool8 DBZ_IsFightBlastActive(void);
void DBZ_SpawnImpactAt(s16 x, s16 y);
u8 DBZ_CreateKiSprite(s16 x, s16 y);
void DBZ_AnimateKiSprite(u8 spriteId, u8 t);
void DBZ_DestroyFxSprite(u8 spriteId);

// Goku's level + overworld fights
u8 DBZ_GokuLevel(void);
void DBZ_GokuGainExpFromBattle(u32 exp);
bool8 DBZ_IsFighting(void);
u8 DBZ_HandleFightInput(struct FieldInput *input);
bool8 DBZ_FightBlocksTile(struct ObjectEvent *objectEvent, s16 x, s16 y);
void DBZ_FightOnBlast(u8 hitType, u8 hitLocal, bool8 kame);
bool8 DBZ_TryStartPendingFieldEvent(void);
bool8 DBZ_TryAmbush(void);
void DBZ_StartFight(void);
void DBZ_PrepareAmbush(void);
u16 DBZ_CanFightTrainer(void);
void DBZ_SetupTrainerFight(void);
void DBZ_FightAwardExp(void);
u16 DBZ_NextFightReport(void);
u16 DBZ_TryEvolveAfterFight(void);
void DBZ_StartFightEvolution(void);
void DBZ_QueueGymFight(void);
void DBZ_ShowGokuStatus(void);
void DBZ_Debug_SetGokuLevel(void);

// overworld style
void DBZ_UpdateShadows(void);
void DBZ_ApplyTimeTint(u16 offset, u16 count);
void DBZ_ApplyTimeTintToObjSlotOnce(u8 slot);

#endif // GUARD_DBZ_H
