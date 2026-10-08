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
void DBZ_ConsumeDragonBalls(void);
void DBZ_PortraitForMessage(const u8 *str);
void DBZ_HidePortrait(void);
bool8 DBZ_HandleFieldInput(struct FieldInput *input);
void DBZ_ResetFieldInputState(void);
bool8 DBZ_IsChargingBlast(void);

// specials
u16 DBZ_GetFormSpecial(void);
u16 DBZ_GetNextForm(void);
void DBZ_StartAura(void);
void DBZ_ApplyNextForm(void);
void DBZ_FireBlast(void);
u16 DBZ_GetNewFormHint(void);

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

// fx frames (graphics/dbz/fx.png)
enum {
    DBZ_FX_KI_0, DBZ_FX_KI_1, DBZ_FX_BEAM_H0, DBZ_FX_BEAM_H1, DBZ_FX_BEAM_V0, DBZ_FX_BEAM_V1,
    DBZ_FX_HEAD_0, DBZ_FX_HEAD_1, DBZ_FX_IMPACT_0, DBZ_FX_IMPACT_1, DBZ_FX_CHARGE_0, DBZ_FX_CHARGE_1,
    DBZ_FX_SPARK_0, DBZ_FX_SPARK_1,
};
u8 DBZ_CreateFxSpriteFor(s16 x, s16 y, bool8 enemy);
void DBZ_SetFxSpriteFrame(u8 spriteId, u8 frame);
void DBZ_GokuHandsPos(u8 dir, s16 *x, s16 *y);
void DBZ_SpawnAuraSpark(bool8 red);
bool8 DBZ_FightTryBeamStruggle(void);

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
void DBZ_FightOnBlast(u8 hitType, u8 hitLocal, u8 kame);
bool8 DBZ_IsFused(void);
u32 DBZ_CurrentPowerLevel(void);
u16 DBZ_GokuHpMaxNow(void);
bool8 DBZ_IsBeamStruggling(void);
void DBZ_SetNextFightFlags(void);
void DBZ_EndFusion(void);
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

// Goku's health / stats (persistent between fights)
u16 DBZ_GokuMaxHp(void);
u16 DBZ_GokuHp(void);
void DBZ_SetGokuHp(u16 hp);
void DBZ_HealGoku(void);
bool8 DBZ_GokuIsKO(void);
u32 DBZ_GokuPowerLevel(void);
u16 DBZ_GokuPowerStat(void);
u16 DBZ_GokuDefenseStat(void);
u32 DBZ_GokuExpTotal(void);
u32 DBZ_GokuExpToNext(void);
u16 DBZ_GokuItemHealPercent(u16 itemId);
bool8 DBZ_UseItemOnGoku(u16 itemId);
bool8 DBZ_PlayerOverlayActive(void);
void DBZ_SetupVegetaFight(void);
void DBZ_StartFightAndWait(void);
void DBZ_WarpToLastHeal(void);
u16 DBZ_GokuCanFight(void);
void DBZ_ReviveGokuWeak(void);
void DBZ_QueueFightEvolveCheck(void);

// Goku's selected special move and the overworld HUD
u8 DBZ_GetSelectedMove(void);   // 0 ki blast, 1 Kamehameha
u8 DBZ_GetChargeLevel(void);    // 0..16
void DBZ_UpdateHud(void);
void DBZ_UpdateDragonBallTracker(void);
const u16 *DBZ_HudPalette(void);
u8 DBZ_GetStarterRegion(void);
void DBZ_SetStarterRegion(void);
void DBZ_AmbientMonCry(void);
enum Species DBZ_MaybeGuestSpecies(enum Species species, u32 area, u32 level);
struct Pokemon;
void DBZ_RemapRivalStarter(struct Pokemon *mon, u8 trainerClass);

// techniques
extern bool8 gDBZInstantTransmission;
bool8 DBZ_HasTechnique(u8 tech);
u16 DBZ_GetNewTechniqueHint(void);
u16 DBZ_ToggleHideKi(void);
bool8 DBZ_IsHidingKi(void);
u16 DBZ_GravityStepsLeft(void);
void DBZ_StartGravityTraining(void);
void DBZ_OnStep(void);
u16 DBZ_CanUseInstantTransmission(void);
void DBZ_QueueInstantTransmission(void);
bool8 DBZ_TryStartPendingInstantTransmission(void);
void DBZ_StartInstantTransmissionOut(void);
bool8 DBZ_InstantTransmissionOutActive(void);
void DBZ_StartInstantTransmissionIn(void);
bool8 DBZ_InstantTransmissionInActive(void);
bool8 DBZ_OptAutoRun(void);
bool8 DBZ_OptNimbus3D(void);

// PokeBall Orange settings
bool8 DBZ_OptDayNight(void);
bool8 DBZ_OptShadows(void);
bool8 DBZ_OptMapBattleBg(void);
bool8 DBZ_OptSparks(void);
bool8 DBZ_OptPowerUp(void);
bool8 DBZ_OptHints(void);
bool8 DBZ_OptHud(void);
u8 DBZ_OptAmbush(void);
u8 DBZ_OptDifficulty(void);
u8 DBZ_OptShiny(void);
#define DBZ_OPTION_COUNT 15
u8 DBZ_GetOptionValue(u8 id);
void DBZ_SetOptionValue(u8 id, u8 v);
bool32 DBZ_ExtraShinyRoll(void);

// overworld style
void DBZ_UpdateShadows(void);
void DBZ_UpdateFormFx(void);
bool8 DBZ_TryDrawMapBattleBackground(void);
void DBZ_StartBattleBgShimmer(void);
void DBZ_ResetTrainerPowerUp(void);
void DBZ_RestyleBattleMenuFrame(u16 offset);
bool8 DBZ_ShouldTrainerPowerUp(u8 battler);
void DBZ_ApplyTimeTint(u16 offset, u16 count);
s32 DBZ_TimeOverrideHour(void);
void DBZ_BlendPalettes(u32 selectedPalettes, u8 coeff, u32 color);
void DBZ_ApplyTimeTintToObjSlotOnce(u8 slot);

#endif // GUARD_DBZ_H
