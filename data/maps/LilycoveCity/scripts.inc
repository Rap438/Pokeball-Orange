LilycoveCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LilycoveCity_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, LilycoveCity_OnLoad
	.byte 0

LilycoveCity_OnTransition:
	setflag FLAG_VISITED_LILYCOVE_CITY
	setvar VAR_CONTEST_HALL_STATE, 0
	setflag FLAG_HIDE_LILYCOVE_CONTEST_HALL_REPORTER
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	call Common_EventScript_SetupRivalGfxId
	end

LilycoveCity_OnLoad:
	call_if_unset FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_SetWailmerMetatiles
	end

LilycoveCity_EventScript_SetWailmerMetatiles::
	setmetatile 76, 12, METATILE_Lilycove_Wailmer0, TRUE
	setmetatile 77, 12, METATILE_Lilycove_Wailmer1, TRUE
	setmetatile 76, 13, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 77, 13, METATILE_Lilycove_Wailmer3, TRUE
	setmetatile 76, 14, METATILE_Lilycove_Wailmer0_Alt, TRUE
	setmetatile 77, 14, METATILE_Lilycove_Wailmer1_Alt, TRUE
	setmetatile 76, 15, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 77, 15, METATILE_Lilycove_Wailmer3, TRUE
	setmetatile 77, 16, METATILE_Lilycove_Wailmer0_Alt, TRUE
	setmetatile 78, 16, METATILE_Lilycove_Wailmer1_Alt, TRUE
	setmetatile 77, 17, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 78, 17, METATILE_Lilycove_Wailmer3, TRUE
	return

LilycoveCity_EventScript_BerryGentleman::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_LILYCOVE_RECEIVED_BERRY, LilycoveCity_EventScript_ReceivedBerry
	msgbox LilycoveCity_Text_BerrySuitsYou, MSGBOX_DEFAULT
	giverandomberry BERRY_ID_CHERI, BERRY_ID_SITRUS
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_DAILY_LILYCOVE_RECEIVED_BERRY
	msgbox LilycoveCity_Text_BecauseYoureTrainer, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_ReceivedBerry::
	msgbox LilycoveCity_Text_PokeblocksSuitPokemon, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Man3::
	msgbox LilycoveCity_Text_ContestHallInTown, MSGBOX_NPC
	end

LilycoveCity_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_EventScript_GirlAquaGone
	msgbox LilycoveCity_Text_StrangeCaveInCove, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_GirlAquaGone::
	msgbox LilycoveCity_Text_GoingToMoveDeleterForHMs, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_RichBoy::
	msgbox LilycoveCity_Text_ImFromKanto, MSGBOX_NPC
	end

LilycoveCity_EventScript_Sailor2::
	lock
	faceplayer
	goto_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_Sailor2AquaGone
	msgbox LilycoveCity_Text_TeamAquaBeenTrainingWailmer, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Sailor2AquaGone::
	msgbox LilycoveCity_Text_SomeonePuntedTeamAquaOut, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Woman2::
	lock
	faceplayer
	goto_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_Woman2AquaGone
	msgbox LilycoveCity_Text_SomeoneStoleMyPokemon, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Woman2AquaGone::
	msgbox LilycoveCity_Text_MissingPokemonCameBack, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Man2::
	msgbox LilycoveCity_Text_ImArtDealer, MSGBOX_NPC
	end

LilycoveCity_EventScript_ExpertM1::
	msgbox LilycoveCity_Text_SeaRemainsForeverYoung, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ExpertF::
	msgbox LilycoveCity_Text_SixtyYearsAgoHusbandProposed, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ExpertM2::
	lock
	faceplayer
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_EventScript_ExpertM2AquaGone
	msgbox LilycoveCity_Text_TeamAquaRenovatedCavern, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_ExpertM2AquaGone::
	msgbox LilycoveCity_Text_TeamAquaLotGoneForGood, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Sailor1::
	msgbox LilycoveCity_Text_HeardTowerCalledSkyPillar, MSGBOX_NPC
	end

LilycoveCity_EventScript_FatMan::
	msgbox LilycoveCity_Text_SawTallTowerOnRoute131, MSGBOX_NPC
	end

LilycoveCity_EventScript_Man1::
	lockall
	msgbox LilycoveCity_Text_JustArrivedAndSawRarePokemon, MSGBOX_NPC
	applymovement LOCALID_LILYCOVE_MAN_1, Common_Movement_FaceOriginalDirection
	end

LilycoveCity_EventScript_Woman1::
	lockall
	msgbox LilycoveCity_Text_HoneymoonVowToSeeRarePokemon, MSGBOX_NPC
	applymovement LOCALID_LILYCOVE_WOMAN_1, Common_Movement_FaceOriginalDirection
	end

LilycoveCity_EventScript_CitySign::
	msgbox LilycoveCity_Text_CitySign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ContestHallSign::
	msgbox LilycoveCity_Text_ContestHallSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MotelSign::
	msgbox LilycoveCity_Text_MotelSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MuseumSign::
	lockall
	specialvar VAR_0x8004, CountPlayerMuseumPaintings
	switch VAR_0x8004
	case 0, LilycoveCity_EventScript_MuseumSignNoPaintings
	msgbox LilycoveCity_Text_MuseumSignPlayersExhibit, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_MuseumSignNoPaintings::
	msgbox LilycoveCity_Text_MuseumSign, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_HarborSignFerryReady
	msgbox LilycoveCity_Text_HarborSignUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_HarborSignFerryReady::
	msgbox LilycoveCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_TrainerFanClubSign::
	msgbox LilycoveCity_Text_TrainerFanClubSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_DepartmentStoreSign::
	msgbox LilycoveCity_Text_DepartmentStoreSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MoveDeletersHouseSign::
	msgbox LilycoveCity_Text_MoveDeletersHouseSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_WailmerTrainerGrunt::
	lockall
	goto_if_set FLAG_MET_WAILMER_TRAINER, LilycoveCity_EventScript_MetWailmerTrainer
	msgbox LilycoveCity_Text_WailmerLeapOutOfWater, MSGBOX_DEFAULT
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	msgbox LilycoveCity_Text_GetLostMessingUpTraining, MSGBOX_DEFAULT
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	setflag FLAG_MET_WAILMER_TRAINER
	releaseall
	end

LilycoveCity_EventScript_MetWailmerTrainer::
	msgbox LilycoveCity_Text_WailmerLeapOutOfWater, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_AquaGrunt1::
	msgbox LilycoveCity_Text_MovedLootIntoHideoutToday, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt2::
	msgbox LilycoveCity_Text_ChanceToDoBigThings, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt3::
	msgbox LilycoveCity_Text_DontGoNearCaveInCove, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt4::
	msgbox LilycoveCity_Text_IfWorldBecomesOurs, MSGBOX_NPC
	end

LilycoveCity_EventScript_Rival::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LilycoveCity_EventScript_May
	goto_if_eq VAR_RESULT, FEMALE, LilycoveCity_EventScript_Brendan
	end

LilycoveCity_EventScript_May::
	playbgm MUS_ENCOUNTER_MAY, TRUE
	call_if_set FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_MayAskToBattleAgain
	call_if_unset FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_MayAskToBattle
	goto_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DeclineMayBattle
	msgbox LilycoveCity_Text_MayWontBeBeaten, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, LilycoveCity_EventScript_BattleMayTreecko
	case 1, LilycoveCity_EventScript_BattleMayTorchic
	case 2, LilycoveCity_EventScript_BattleMayMudkip
	end

LilycoveCity_EventScript_MayAskToBattleAgain::
	msgbox LilycoveCity_Text_MayBattleMe, MSGBOX_YESNO
	return

LilycoveCity_EventScript_MayAskToBattle::
	msgbox LilycoveCity_Text_MayShoppingLetsBattle, MSGBOX_YESNO
	return

LilycoveCity_EventScript_DeclineMayBattle::
	setflag FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE
	msgbox LilycoveCity_Text_MayNotRaisingPokemon, MSGBOX_DEFAULT
	savebgm MUS_DUMMY
	fadedefaultbgm
	release
	end

LilycoveCity_EventScript_Brendan::
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_set FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_BrendanAskToBattleAgain
	call_if_unset FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_BrendanAskToBattle
	goto_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DeclineBrendanBattle
	msgbox LilycoveCity_Text_BrendanWontBeBeaten, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, LilycoveCity_EventScript_BattleBrendanTreecko
	case 1, LilycoveCity_EventScript_BattleBrendanTorchic
	case 2, LilycoveCity_EventScript_BattleBrendanMudkip
	end

LilycoveCity_EventScript_BrendanAskToBattleAgain::
	msgbox LilycoveCity_Text_BrendanBattleMe, MSGBOX_YESNO
	return

LilycoveCity_EventScript_BrendanAskToBattle::
	msgbox LilycoveCity_Text_BrendanShoppingLetsBattle, MSGBOX_YESNO
	return

LilycoveCity_EventScript_DeclineBrendanBattle::
	setflag FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE
	msgbox LilycoveCity_Text_BrendanNoConfidence, MSGBOX_DEFAULT
	savebgm MUS_DUMMY
	fadedefaultbgm
	release
	end

LilycoveCity_EventScript_BattleMayTreecko::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_TREECKO, LilycoveCity_Text_MayDefeat
	call EventScript_DBZ_VegetaFight
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleMayTorchic::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_TORCHIC, LilycoveCity_Text_MayDefeat
	call EventScript_DBZ_VegetaFight
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleMayMudkip::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_MUDKIP, LilycoveCity_Text_MayDefeat
	call EventScript_DBZ_VegetaFight
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleBrendanTreecko::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_TREECKO, LilycoveCity_Text_BrendanDefeat
	call EventScript_DBZ_VegetaFight
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_BattleBrendanTorchic::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_TORCHIC, LilycoveCity_Text_BrendanDefeat
	call EventScript_DBZ_VegetaFight
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_BattleBrendanMudkip::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_MUDKIP, LilycoveCity_Text_BrendanDefeat
	call EventScript_DBZ_VegetaFight
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_DefeatedMay::
	msgbox LilycoveCity_Text_MayGoingBackToLittleroot, MSGBOX_DEFAULT
	setvar VAR_RESULT, FALSE
	call_if_set FLAG_BADGE06_GET, LilycoveCity_EventScript_CheckFinalBadge
	call_if_eq VAR_RESULT, FALSE, LilycoveCity_EventScript_MayCollectBadges
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_EventScript_MayPokemonLeague
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_BEDROOM
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_PICHU_DOLL
	goto LilycoveCity_EventScript_RivalFlyAway
	end

LilycoveCity_EventScript_CheckFinalBadge::
	goto_if_set FLAG_BADGE08_GET, LilycoveCity_EventScript_HasFinalBadge
	return

LilycoveCity_EventScript_HasFinalBadge::
	setvar VAR_RESULT, TRUE
	return

LilycoveCity_EventScript_MayCollectBadges::
	msgbox LilycoveCity_Text_MayYouGoingToCollectBadges, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_MayPokemonLeague::
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_MayBattleFrontier
	msgbox LilycoveCity_Text_MayYouGoingToPokemonLeague, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_MayBattleFrontier::
	msgbox LilycoveCity_Text_MayYouGoingToBattleFrontier, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_DefeatedBrendan::
	msgbox LilycoveCity_Text_BrendanGoingBackToLittleroot, MSGBOX_DEFAULT
	setvar VAR_RESULT, FALSE
	call_if_set FLAG_BADGE06_GET, LilycoveCity_EventScript_CheckFinalBadge
	call_if_eq VAR_RESULT, FALSE, LilycoveCity_EventScript_BrendanCollectBadges
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_EventScript_BrendanPokemonLeague
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_SWABLU_DOLL
	goto LilycoveCity_EventScript_RivalFlyAway
	end

LilycoveCity_EventScript_BrendanCollectBadges::
	msgbox LilycoveCity_Text_BrendanYouGoingToCollectBadges, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_BrendanPokemonLeague::
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_BrendanBattleFrontier
	msgbox LilycoveCity_Text_BrendanYouGoingToPokemonLeague, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_BrendanBattleFrontier::
	msgbox LilycoveCity_Text_BrendanYouGoingToBattleFrontier, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_RivalFlyAway::
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	delay 50
	setfieldeffectargument 0, 1
	dofieldeffect FLDEFF_NPCFLY_OUT
	delay 15
	removeobject VAR_LAST_TALKED
	waitfieldeffect FLDEFF_NPCFLY_OUT
	savebgm MUS_DUMMY
	fadedefaultbgm
	setflag FLAG_MET_RIVAL_LILYCOVE
	release
	end

LilycoveCity_EventScript_SchoolKidM::
	lock
	faceplayer
	msgbox LilycoveCity_Text_DoYouKnowAboutBerryBlender, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, LilycoveCity_EventScript_KnowAboutBerryBlender
	call_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DontKnowAboutBerryBlender
	release
	end

LilycoveCity_EventScript_KnowAboutBerryBlender::
	msgbox LilycoveCity_Text_FasterSpinBetterPokeblocks, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_DontKnowAboutBerryBlender::
	msgbox LilycoveCity_Text_ExplainBerryBlender, MSGBOX_DEFAULT
	return

LilycoveCity_Text_MayShoppingLetsBattle:
	.string "VEGETA: Hmph. Kakarot{KUN}.\n"
	.string "Shopping, are you?\p"
	.string "Bulma sent me out to haul home a pile of\n"
	.string "DOLLS and POSTERS. Don't get ideas.\p"
	.string "I've also caught a strong team since\n"
	.string "LITTLEROOT, and I train them properly.\p"
	.string "You, on the other hand… I wonder if\n"
	.string "you've been training yours at all,\l"
	.string "Kakarot{KUN}.\p"
	.string "Fight me. Show me what your POKéMON can\n"
	.string "do, Kakarot{KUN}!$"

LilycoveCity_Text_MayNotRaisingPokemon:
	.string "VEGETA: Tch. Backing down, Kakarot{KUN}?\p"
	.string "Haven't you been training your\n"
	.string "POKéMON? That's a pathetic showing for\l"
	.string "a Saiyan…$"

LilycoveCity_Text_MayBattleMe:
	.string "VEGETA: Come on, Kakarot! Fight me, so I\n"
	.string "can measure your POKéMON's power!$"

LilycoveCity_Text_MayWontBeBeaten:
	.string "VEGETA: The POKéMON I trained will\n"
	.string "never fall to yours, Kakarot{KUN}!$"

LilycoveCity_Text_MayDefeat:
	.string "… … … … … … … …\p"
	.string "I remember our fight on ROUTE 103,\n"
	.string "Kakarot{KUN}.\p"
	.string "That fight is what made you this\n"
	.string "strong, isn't it, Kakarot{KUN}? Hmph…$"

LilycoveCity_Text_MayGoingBackToLittleroot:
	.string "VEGETA: I'm going back to LITTLEROOT\n"
	.string "soon.\p"
	.string "I've caught a decent group of POKéMON\n"
	.string "and my POKéDEX is coming along. I'll\l"
	.string "show the old turtle hermit, then train\l"
	.string "in Bulma's gravity room.$"

LilycoveCity_Text_MayYouGoingToCollectBadges:
	.string "Kakarot{KUN}, what's your plan?\p"
	.string "Collect every GYM BADGE and march on\n"
	.string "the POKéMON LEAGUE?\p"
	.string "Fine. While you're running around for\n"
	.string "BADGES, I'll finish my POKéDEX and head\l"
	.string "for the LEAGUE myself. I'll get there\l"
	.string "first, Kakarot{KUN}!\p"
	.string "Hmph. See you.$"

LilycoveCity_Text_MayYouGoingToPokemonLeague:
	.string "Kakarot{KUN}, what's your plan?\p"
	.string "Are you marching on the POKéMON\n"
	.string "LEAGUE?\p"
	.string "Or will you try to complete your\n"
	.string "POKéDEX?\p"
	.string "I may have lost to you today, but I'm\n"
	.string "moving on to the LEAGUE, and I'll finish\l"
	.string "my POKéDEX before you do!\p"
	.string "Hmph. See you.$"

LilycoveCity_Text_MayYouGoingToBattleFrontier:
	.string "Kakarot{KUN}, what's your plan?\p"
	.string "Are you taking on the BATTLE FRONTIER?\p"
	.string "Or will you try to complete your\n"
	.string "POKéDEX?\p"
	.string "I may have lost to you today, but I'll\n"
	.string "finish my POKéDEX before you do!\p"
	.string "Hmph. See you.$"

LilycoveCity_Text_BrendanShoppingLetsBattle:
	.string "VEGETA: Hmph. It's you, Kakarot.\p"
	.string "Bulma has me running errands. No, I am\n"
	.string "NOT buying DOLLS.\p"
	.string "Anyway, it's been a while.\p"
	.string "Want to settle who's been training\n"
	.string "their POKéMON better?$"

LilycoveCity_Text_BrendanNoConfidence:
	.string "VEGETA: What's the matter?\n"
	.string "Lost your nerve, Kakarot?$"

LilycoveCity_Text_BrendanBattleMe:
	.string "VEGETA: Fight me and we'll see who's\n"
	.string "been training their POKéMON better!$"

LilycoveCity_Text_BrendanWontBeBeaten:
	.string "VEGETA: Hmph. You know I won't lose to a\n"
	.string "low-class warrior like you, Kakarot.$"

LilycoveCity_Text_BrendanDefeat:
	.string "Tch…\n"
	.string "You've trained hard.\p"
	.string "That stings a little. I had a head start\n"
	.string "on you, and a prince's pride on top of\l"
	.string "it…$"

LilycoveCity_Text_BrendanGoingBackToLittleroot:
	.string "VEGETA: I…\n"
	.string "I plan to head home to LITTLEROOT.\p"
	.string "You know I'm helping the old hermit with\n"
	.string "his POKéDEX research. It's coming\l"
	.string "together decently, so I should report\l"
	.string "in.$"

LilycoveCity_Text_BrendanYouGoingToCollectBadges:
	.string "Kakarot, what's your plan?\p"
	.string "Collect every GYM BADGE and take on the\n"
	.string "POKéMON LEAGUE?\p"
	.string "Maybe I'll do that, too…\p"
	.string "Well, whatever. Don't slack off!$"

LilycoveCity_Text_BrendanYouGoingToPokemonLeague:
	.string "Kakarot, what's your plan?\p"
	.string "You're strong enough now. Are you\n"
	.string "marching on the POKéMON LEAGUE?\p"
	.string "Heh, give me a bit more training and my\n"
	.string "POKéMON will surpass yours, Kakarot.\p"
	.string "I'm moving on toward the LEAGUE myself.\n"
	.string "Don't slack off!$"

LilycoveCity_Text_BrendanYouGoingToBattleFrontier:
	.string "Kakarot, what's your plan?\p"
	.string "Are you taking on the BATTLE FRONTIER?\p"
	.string "Or will you try to complete your\n"
	.string "POKéDEX?\p"
	.string "You're strong enough that the BATTLE\n"
	.string "FRONTIER might suit you.\p"
	.string "Heh, give me a bit more training and I'll\n"
	.string "surpass you, Kakarot.\p"
	.string "Maybe I'll go to the BATTLE FRONTIER\n"
	.string "myself.\p"
	.string "Well, whatever. Don't slack off!$"

LilycoveCity_Text_MovedLootIntoHideoutToday:
	.string "Fufufu…\p"
	.string "We hauled even more loot into our\n"
	.string "secret SEA BASE today…\p"
	.string "Wh-who are you?!\n"
	.string "I was just talking to myself!$"

LilycoveCity_Text_ChanceToDoBigThings:
	.string "I'm only a grunt, so I have no idea what\n"
	.string "KID BUU is thinking…\p"
	.string "But as a soldier of the RED RIBBON ARMY,\n"
	.string "I know I'll get my chance to do big\l"
	.string "things!\p"
	.string "Wh-who are you?!\n"
	.string "I was just talking to myself!$"

LilycoveCity_Text_DontGoNearCaveInCove:
	.string "Hey, you!\p"
	.string "Don't go near the cave in the cove!\p"
	.string "Why? You don't need a reason! I'm a\n"
	.string "grown-up in uniform, so just do as I\l"
	.string "say!$"

LilycoveCity_Text_IfWorldBecomesOurs:
	.string "When this whole wide world belongs to\n"
	.string "the RED RIBBON ARMY, it'll be a happier\l"
	.string "place for POKéMON, too.\p"
	.string "At least, that's what DR. GERO says.$"

LilycoveCity_Text_WailmerLeapOutOfWater:
	.string "There! WAILMER!\n"
	.string "Leap out of the water now! Higher!$"

LilycoveCity_Text_GetLostMessingUpTraining:
	.string "Huh? What do you want?\p"
	.string "You're wrecking our training session,\n"
	.string "so, like, could you scram?$"

LilycoveCity_Text_ContestHallInTown:
	.string "There's a POKéMON CONTEST HALL right\n"
	.string "here in this town.\p"
	.string "That means well-trained POKéMON will\n"
	.string "come from all over the country.\p"
	.string "My heart is pounding with excitement!$"

LilycoveCity_Text_StrangeCaveInCove:
	.string "Have you seen that strange cave in the\n"
	.string "cove at the edge of town?$"

LilycoveCity_Text_GoingToMoveDeleterForHMs:
	.string "I'm going to teach my POKéMON some new\n"
	.string "moves so I can enter a CONTEST.\p"
	.string "First, I'll visit the MOVE DELETER and\n"
	.string "have it forget its HM moves.$"

LilycoveCity_Text_ImFromKanto:
	.string "I came all the way from KANTO.\p"
	.string "The HOENN region is gorgeous, with all\n"
	.string "its water and green hills.\p"
	.string "Huh? Wait a second…\p"
	.string "Could there be rare POKéMON that live\n"
	.string "only around here?$"

LilycoveCity_Text_TeamAquaBeenTrainingWailmer:
	.string "Those RED RIBBON ARMY soldiers have\n"
	.string "been drilling their WAILMER in the cove.\p"
	.string "We SAILORS can't get our boats out to\n"
	.string "sea with them in the way!$"

LilycoveCity_Text_SomeonePuntedTeamAquaOut:
	.string "Ahoy, did you hear?\p"
	.string "Somebody punted the RED RIBBON ARMY\n"
	.string "right out of here!\p"
	.string "That cleared the WAILMER out of the\n"
	.string "cove, so we can sail again.\p"
	.string "People say they saw a golden flash over\n"
	.string "the water…$"

LilycoveCity_Text_SomeoneStoleMyPokemon:
	.string "I was taking a nice nap at the seaside\n"
	.string "inn, lulled by the waves…\p"
	.string "When somebody stole my POKéMON!\p"
	.string "Who's the thief? The RED RIBBON ARMY?\n"
	.string "It's the RED RIBBON ARMY, isn't it?$"

LilycoveCity_Text_MissingPokemonCameBack:
	.string "My missing POKéMON…\n"
	.string "It came back, and I never even noticed!$"

LilycoveCity_Text_ImArtDealer:
	.string "I'm an ART DEALER.\p"
	.string "I buy and sell fine art, paintings most\n"
	.string "of all.\p"
	.string "I hear the MUSEUM here has magnificent\n"
	.string "works on display…$"

LilycoveCity_Text_SeaRemainsForeverYoung:
	.string "Even as we grow old and wrinkled, the\n"
	.string "sea stays young, shining with the\l"
	.string "brilliance of life…\p"
	.string "Fwohoho fwohohohoho…$"

LilycoveCity_Text_SixtyYearsAgoHusbandProposed:
	.string "My husband proposed to me right here,\n"
	.string "sixty years ago.\p"
	.string "The sea is just as beautiful as ever.\p"
	.string "Mufufufu mufufufufufu…$"

LilycoveCity_Text_TeamAquaRenovatedCavern:
	.string "Hm, you know about the odd cavern in\n"
	.string "this cove?\p"
	.string "It used to be a natural formation, but\n"
	.string "then that RED RIBBON ARMY bunch\l"
	.string "showed up and started their\l"
	.string "renovations.\p"
	.string "They call themselves a “friendly\n"
	.string "research society”!\p"
	.string "But what they do and what they say\n"
	.string "don't match at all!$"

LilycoveCity_Text_TeamAquaLotGoneForGood:
	.string "That cave in the cove…\p"
	.string "I didn't notice at first, but it's\n"
	.string "completely empty now.\p"
	.string "That RED RIBBON ARMY bunch must have\n"
	.string "cleared out for good.$"

LilycoveCity_Text_CitySign:
	.string "LILYCOVE CITY\p"
	.string "“Where the land ends and the sea\n"
	.string "begins.”$"

LilycoveCity_Text_ContestHallSign:
	.string "POKéMON CONTEST HALL\n"
	.string "“Where TRAINERS gather to shine!”$"

LilycoveCity_Text_MotelSign:
	.string "COVE LILY MOTEL\p"
	.string "“A good night's rest, right by the sea.”$"

LilycoveCity_Text_MuseumSign:
	.string "LILYCOVE MUSEUM\p"
	.string "“A masterpiece collection of POKéMON\n"
	.string "art is on exhibit!”$"

LilycoveCity_Text_MuseumSignPlayersExhibit:
	.string "LILYCOVE MUSEUM\p"
	.string "“{PLAYER}'s POKéMON Collection is now on\n"
	.string "exhibit!”$"

LilycoveCity_Text_HarborSignUnderConstruction:
	.string "LILYCOVE CITY HARBOR\p"
	.string "“The ferry S.S. TIDAL is still being\n"
	.string "built in SLATEPORT CITY.\p"
	.string "“Service will begin shortly.”$"

LilycoveCity_Text_HarborSign:
	.string "LILYCOVE CITY HARBOR\p"
	.string "“Come enjoy a lovely cruise aboard the\n"
	.string "ferry S.S. TIDAL.”$"

LilycoveCity_Text_TrainerFanClubSign:
	.string "POKéMON TRAINER FAN CLUB\p"
	.string "The names of fighters are scribbled all\n"
	.string "over the sign…$"

LilycoveCity_Text_DepartmentStoreSign:
	.string "LILYCOVE DEPARTMENT STORE\p"
	.string "“Packed with great goods and\n"
	.string "excitement!\p"
	.string "“The place to find whatever you need!”$"

LilycoveCity_Text_MoveDeletersHouseSign:
	.string "MOVE DELETER'S HOUSE\p"
	.string "“Unwanted POKéMON moves, erased.”$"

LilycoveCity_Text_DoYouKnowAboutBerryBlender:
	.string "Have you heard about the BERRY\n"
	.string "BLENDER machine?$"

LilycoveCity_Text_FasterSpinBetterPokeblocks:
	.string "Everybody has to work together to make\n"
	.string "it spin faster.\p"
	.string "The faster you spin it, the better the\n"
	.string "{POKEBLOCK}S that come out, or so I hear.$"

LilycoveCity_Text_ExplainBerryBlender:
	.string "It makes a kind of treat called a\n"
	.string "{POKEBLOCK}.\p"
	.string "Feed a good {POKEBLOCK} to a POKéMON and\n"
	.string "its condition will improve a lot.$"

LilycoveCity_Text_HeardTowerCalledSkyPillar:
	.string "I heard there's a tower out on the sea\n"
	.string "routes somewhere.\p"
	.string "They call it the SKY PILLAR. Old legends\n"
	.string "say a dragon sleeps up there.$"

LilycoveCity_Text_SawTallTowerOnRoute131:
	.string "I saw a tall tower somewhere around\n"
	.string "ROUTE 131.\p"
	.string "Could that be…?$"

LilycoveCity_Text_JustArrivedAndSawRarePokemon:
	.string "We just got here for our honeymoon.\p"
	.string "We saw a DRAGON-type POKéMON soaring\n"
	.string "way up in the sky.\p"
	.string "Do amazing POKéMON like that live in the\n"
	.string "HOENN region?$"

LilycoveCity_Text_HoneymoonVowToSeeRarePokemon:
	.string "On our honeymoon, we promised each\n"
	.string "other we'd see as many rare POKéMON as\l"
	.string "we could.\p"
	.string "And we spotted a rare one the moment we\n"
	.string "arrived!\p"
	.string "We are so happy!$"

