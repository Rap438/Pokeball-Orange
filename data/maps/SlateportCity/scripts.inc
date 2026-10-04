SlateportCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SlateportCity_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SlateportCity_OnFrame
	.byte 0

SlateportCity_OnTransition:
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 0
	call SlateportCity_EventScript_EnterSlateport
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_MovePeopleForSternInterview
	call_if_eq VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_SetReadyForScottScene
	end

SlateportCity_EventScript_EnterSlateport::
	setflag FLAG_VISITED_SLATEPORT_CITY
	setvar VAR_CONTEST_HALL_STATE, 0
	setflag FLAG_HIDE_SLATEPORT_CITY_CONTEST_REPORTER
	return

SlateportCity_EventScript_MovePeopleForSternInterview::
	setobjectxyperm LOCALID_SLATEPORT_CAPT_STERN, 28, 13
	setobjectxyperm LOCALID_SLATEPORT_OLD_WOMAN, 25, 13
	setobjectxyperm LOCALID_SLATEPORT_RICH_BOY, 25, 14
	setobjectxyperm LOCALID_SLATEPORT_COOK, 27, 16
	setobjectxyperm LOCALID_SLATEPORT_GIRL, 28, 16
	setobjectxyperm LOCALID_SLATEPORT_FAT_MAN, 29, 16
	setobjectxyperm LOCALID_SLATEPORT_MAN_1, 31, 14
	setobjectmovementtype LOCALID_SLATEPORT_CAPT_STERN, MOVEMENT_TYPE_FACE_DOWN
	setobjectmovementtype LOCALID_SLATEPORT_OLD_WOMAN, MOVEMENT_TYPE_FACE_DOWN_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_RICH_BOY, MOVEMENT_TYPE_FACE_UP_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_COOK, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_GIRL, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_FAT_MAN, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_MAN_1, MOVEMENT_TYPE_FACE_LEFT
	return

SlateportCity_EventScript_SetReadyForScottScene::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 30, SlateportCity_EventScript_MoveScottLeft
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 23, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_EventScript_MoveScottLeft::
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 22, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_OnFrame:
	map_script_2 VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_ScottScene
	.2byte 0

SlateportCity_EventScript_ScottScene::
	lockall
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceScott
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottApproachPlayer
	waitmovement 0
	msgbox SlateportCity_Text_YouDroveTeamAquaAway, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterLeft
	delay 60
	msgbox SlateportCity_Text_MaybeThisTrainer, MSGBOX_DEFAULT
	applywaitmovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterRight
	msgbox SlateportCity_Text_LetsRegisterEachOther, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SlateportCity_Text_RegisteredScott, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_SCOTT_MATCH_CALL
	msgbox SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExit
	removeobject LOCALID_SLATEPORT_SCOTT
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 10, 12
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_DOWN
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 2
	addvar VAR_SCOTT_STATE, 1
	releaseall
	end

SlateportCity_Movement_PlayerFaceScott:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ScottApproachPlayer:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_Movement_ScottExit:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_EventScript_EnergyGuru::
	lock
	faceplayer
	message SlateportCity_Text_EnergyGuruSellWhatYouNeed
	waitmessage
	pokemart SlateportCity_Pokemart_EnergyGuru
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_EnergyGuru:
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_CARBOS
	.2byte ITEM_ZINC
	.2byte ITEM_CALCIUM
	.2byte ITEM_HP_UP
	pokemartlistend

SlateportCity_EventScript_EffortRibbonWoman::
	lock
	faceplayer
	bufferleadmonspeciesname STR_VAR_1
	msgbox SlateportCity_Text_OhYourPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, LeadMonHasEffortRibbon
	call_if_eq VAR_RESULT, TRUE, SlateportCity_EventScript_MonHasEffortRibbon
	specialvar VAR_RESULT, Special_AreLeadMonEVsMaxedOut
	call_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_MonEVsNotMaxed
	msgbox SlateportCity_Text_PleaseGiveItThisEffortRibbon, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message SlateportCity_Text_ReceivedEffortRibbon
	waitfanfare
	msgbox SlateportCity_Text_PutEffortRibbonOnMon, MSGBOX_DEFAULT
	special GiveLeadMonEffortRibbon
	release
	end

SlateportCity_EventScript_MonEVsNotMaxed::
	msgbox SlateportCity_Text_GoForItLittleHarder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_MonHasEffortRibbon::
	msgbox SlateportCity_Text_EffortRibbonLooksGoodOnIt, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Cook::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_CookSternInterview
	msgbox SlateportCity_Text_SeaweedFullOfLife, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_CookSternInterview::
	msgbox SlateportCity_Text_CaptainComeBackWithBigFish, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWoman::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_OldWomanSternInterview
	msgbox SlateportCity_Text_HowTownIsBornAndGrows, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWomanSternInterview::
	msgbox SlateportCity_Text_CaptSternBeingInterviewed, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Girl::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_GirlSternInterview
	goto_if_set FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_GirlSecretBase
	msgbox SlateportCity_Text_SlateportWonderfulPlace, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSternInterview::
	msgbox SlateportCity_Text_InterviewerSoCool, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSecretBase::
	msgbox SlateportCity_Text_BuyBricksSoDecorWontGetDirty, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoy::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_RichBoySternInterview
	msgbox SlateportCity_Text_GoingToCompeteInBattleTent, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoySternInterview::
	msgbox SlateportCity_Text_SternSaysDiscoveredSomething, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_FatMan::
	goto_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_FatManSternInterview
	msgbox SlateportCity_Text_BushedHikingFromMauville, MSGBOX_NPC
	end

SlateportCity_EventScript_FatManSternInterview::
	msgbox SlateportCity_Text_AmIOnTV, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man1::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_Man1SternInterview
	msgbox SlateportCity_Text_EveryoneCallsHimCaptStern, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Man1SternInterview::
	msgbox SlateportCity_Text_CaptainsACelebrity, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1::
	lock
	faceplayer
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_EventScript_Woman1AquaGone
	msgbox SlateportCity_Text_WhatsLongLineOverThere, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1AquaGone::
	msgbox SlateportCity_Text_VisitedMuseumOften, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_BattleTentSign::
	msgbox SlateportCity_Text_BattleTentSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_SternsShipyardSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_SternsShipyardFerryComplete
	goto_if_set FLAG_BADGE07_GET, SlateportCity_EventScript_SternsShipyardNearsCompletion
	msgbox SlateportCity_Text_SternsShipyardWantedSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardNearsCompletion::
	msgbox SlateportCity_Text_SternsShipyardNearsCompletion, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardFerryComplete::
	msgbox SlateportCity_Text_SternsShipyardFerryComplete, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_PokemonFanClubSign::
	msgbox SlateportCity_Text_PokemonFanClubSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_OceanicMuseumSign::
	msgbox SlateportCity_Text_OceanicMuseumSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_CitySign::
	msgbox SlateportCity_Text_CitySign, MSGBOX_SIGN
	end

SlateportCity_EventScript_MarketSign::
	msgbox SlateportCity_Text_MarketSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_HarborSignFerryComplete
	msgbox SlateportCity_Text_HarborFerryUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_HarborSignFerryComplete::
	msgbox SlateportCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_NameRatersHouseSign::
	msgbox SlateportCity_Text_NameRatersHouseSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_Maniac::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_ManiacSternInterview
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

@ Unclear if the text here was meant to be different, but its not
SlateportCity_EventScript_ManiacSternInterview::
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman2::
	msgbox SlateportCity_Text_CantChangeTradeMonName, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor1::
	msgbox SlateportCity_Text_SeaIsSoWet, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor2::
	msgbox SlateportCity_Text_SinkOldBoats, MSGBOX_NPC
	end

SlateportCity_EventScript_PokefanF::
	msgbox SlateportCity_Text_BuyTooMuch, MSGBOX_NPC
	end

SlateportCity_EventScript_Man2::
	msgbox SlateportCity_Text_BattleTentBuiltRecently, MSGBOX_NPC
	end

SlateportCity_EventScript_AquaGrunt1::
	lock
	faceplayer
	msgbox SlateportCity_Text_QuitPushing, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_1, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt2::
	lock
	faceplayer
	msgbox SlateportCity_Text_AquaHasPolicy, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_2, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt3::
	lock
	faceplayer
	msgbox SlateportCity_Text_BossIsBrilliant, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_3, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt4::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhatsNewSchemeIWonder, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_4, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt5::
	lock
	faceplayer
	msgbox SlateportCity_Text_ShouldTakeItAll, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_5, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt6::
	lock
	faceplayer
	msgbox SlateportCity_Text_DontButtIn, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_6, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt7::
	lock
	faceplayer
	msgbox SlateportCity_Text_RemindsMeOfLongLineForGames, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_7, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt8::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhyAreWeLiningUp, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_8, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_AquaGrunt9::
	lock
	faceplayer
	playse SE_PIN
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_Delay48
	msgbox SlateportCity_Text_WhatDoYouWant, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	msgbox SlateportCity_Text_IllReadSignForYou, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	msgbox SlateportCity_Text_SaysSomethingLikeSeaIsEndless, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	applywaitmovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_Movement_DelayAquaGrunt:
	delay_16
	delay_16
	step_end

SlateportCity_EventScript_AquaGrunt10::
	msgbox SlateportCity_Text_ShouldveBroughtMyGameBoy, MSGBOX_SIGN
	end

SlateportCity_EventScript_AquaGrunt11::
	lock
	faceplayer
	msgbox SlateportCity_Text_HotSpringsAfterOperation, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GRUNT_11, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_EventScript_DollClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor_Dolls
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor_Dolls:
	.2byte DECOR_AZURILL_DOLL
	.2byte DECOR_MARILL_DOLL
	.2byte DECOR_SKITTY_DOLL
	pokemartlistend

SlateportCity_EventScript_ComeBackWithSecretPower::
	msgbox gText_ComeBackWithSecretPower, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_DecorClerk::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_ComeBackWithSecretPower
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor:
	.2byte DECOR_RED_BRICK
	.2byte DECOR_BLUE_BRICK
	.2byte DECOR_YELLOW_BRICK
	.2byte DECOR_RED_BALLOON
	.2byte DECOR_BLUE_BALLOON
	.2byte DECOR_YELLOW_BALLOON
	.2byte DECOR_C_LOW_NOTE_MAT
	.2byte DECOR_D_NOTE_MAT
	.2byte DECOR_E_NOTE_MAT
	.2byte DECOR_F_NOTE_MAT
	.2byte DECOR_G_NOTE_MAT
	.2byte DECOR_A_NOTE_MAT
	.2byte DECOR_B_NOTE_MAT
	.2byte DECOR_C_HIGH_NOTE_MAT
	pokemartlistend

SlateportCity_EventScript_PowerTMClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart SlateportCity_Pokemart_PowerTMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_PowerTMs:
	.2byte ITEM_TM_HIDDEN_POWER
	.2byte ITEM_TM_SECRET_POWER
	pokemartlistend

@ Scene with Capt Sterns interview and Team Aqua announcing plans to steal Submarine
SlateportCity_EventScript_CaptStern::
	lockall
	msgbox SlateportCity_Text_SternMoveAheadWithExploration, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_GabbyWonderfulThanksForInterview, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SLATEPORT_GABBY, Common_Movement_WalkInPlaceFasterRight
	delay 10
	applywaitmovement LOCALID_SLATEPORT_TY, Common_Movement_WalkInPlaceFasterDown
	delay 25
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternWatchGabbyAndTyExit
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceStern
	applymovement LOCALID_SLATEPORT_GABBY, SlateportCity_Movement_GabbyExit
	applymovement LOCALID_SLATEPORT_TY, SlateportCity_Movement_TyExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_GABBY
	removeobject LOCALID_SLATEPORT_TY
	msgbox SlateportCity_Text_SternWhewFirstInterview, MSGBOX_DEFAULT
	applywaitmovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	msgbox SlateportCity_Text_OhPlayerWeMadeDiscovery, MSGBOX_DEFAULT
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox SlateportCity_Text_AquaWillAssumeControlOfSubmarine, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_COOK, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_FAT_MAN, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_OLD_WOMAN, SlateportCity_Movement_OldWomanConcern
	applymovement LOCALID_SLATEPORT_RICH_BOY, Common_Movement_QuestionMark
	applymovement LOCALID_SLATEPORT_MAN_1, SlateportCity_Movement_ManConcern
	waitmovement 0
	applywaitmovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterDown
	msgbox SlateportCity_Text_SternWhatWasAllThat, MSGBOX_DEFAULT
	playse SE_PIN
	applywaitmovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_Delay48
	applywaitmovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	msgbox SlateportCity_Text_FromHarborTryingToTakeSub, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_PleaseComeWithMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternEnterHarbor
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerEnterHarbor
	waitmovement 0
	removeobject LOCALID_SLATEPORT_CAPT_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_CAPTAIN_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_SUBMARINE_SHADOW
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_AQUA_GRUNT
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_ARCHIE
	setvar VAR_SLATEPORT_CITY_STATE, 2
	warp MAP_SLATEPORT_CITY_HARBOR, 11, 14
	waitstate
	releaseall
	end

SlateportCity_Movement_OldWomanConcern:
	delay_16
	delay_16
	emote_question_mark
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ManConcern:
	emote_question_mark
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_GabbyExit:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_TyExit:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_Unused:
	walk_down
	walk_in_place_faster_up
	step_end

SlateportCity_Movement_SternEnterHarbor:
	walk_right
	walk_up
	walk_up
	set_invisible
	step_end

SlateportCity_Movement_SternWatchGabbyAndTyExit:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	walk_left
	step_end

SlateportCity_Movement_PlayerEnterHarbor:
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_right
	walk_up
	step_end

SlateportCity_Movement_PlayerFaceStern:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_EventScript_Ty::
	msgbox SlateportCity_Text_BigSmileForCamera, MSGBOX_SIGN
	end

SlateportCity_EventScript_Gabby::
	msgbox SlateportCity_Text_MostInvaluableExperience, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man3::
	msgbox SlateportCity_Text_WonderIfLighthouseStartlesPokemon, MSGBOX_NPC
	end

SlateportCity_EventScript_BerryPowderClerk::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POWDER_JAR, SlateportCity_EventScript_ReceivedPowderJar
	msgbox SlateportCity_Text_ExplainBerries, MSGBOX_DEFAULT
	giveitem ITEM_POWDER_JAR
	setflag FLAG_RECEIVED_POWDER_JAR
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ReceivedPowderJar::
	setvar VAR_0x8004, 1
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_ExplainBerryPowder
	msgbox SlateportCity_Text_BroughtMeSomeBerryPowder, MSGBOX_DEFAULT
	special DisplayBerryPowderVendorMenu
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ExplainBerryPowder::
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ChooseBerryPowderItem::
	message SlateportCity_Text_ExchangeWhatWithIt
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BERRY_POWDER_VENDOR
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, SlateportCity_EventScript_EnergyPowder
	case 1, SlateportCity_EventScript_EnergyRoot
	case 2, SlateportCity_EventScript_HealPowder
	case 3, SlateportCity_EventScript_RevivalHerb
	case 4, SlateportCity_EventScript_Protein
	case 5, SlateportCity_EventScript_Iron
	case 6, SlateportCity_EventScript_Carbos
	case 7, SlateportCity_EventScript_Calcium
	case 8, SlateportCity_EventScript_Zinc
	case 9, SlateportCity_EventScript_HPUp
	case 10, SlateportCity_EventScript_PPUp
	case 11, SlateportCity_EventScript_CancelPowderItemSelect
	case MULTI_B_PRESSED, SlateportCity_EventScript_CancelPowderItemSelect
	end

SlateportCity_EventScript_EnergyPowder::
	bufferitemname STR_VAR_1, ITEM_ENERGY_POWDER
	setvar VAR_0x8008, ITEM_ENERGY_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_EnergyRoot::
	bufferitemname STR_VAR_1, ITEM_ENERGY_ROOT
	setvar VAR_0x8008, ITEM_ENERGY_ROOT
	setvar VAR_0x8009, 80
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HealPowder::
	bufferitemname STR_VAR_1, ITEM_HEAL_POWDER
	setvar VAR_0x8008, ITEM_HEAL_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_RevivalHerb::
	bufferitemname STR_VAR_1, ITEM_REVIVAL_HERB
	setvar VAR_0x8008, ITEM_REVIVAL_HERB
	setvar VAR_0x8009, 300
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Protein::
	bufferitemname STR_VAR_1, ITEM_PROTEIN
	setvar VAR_0x8008, ITEM_PROTEIN
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Iron::
	bufferitemname STR_VAR_1, ITEM_IRON
	setvar VAR_0x8008, ITEM_IRON
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Carbos::
	bufferitemname STR_VAR_1, ITEM_CARBOS
	setvar VAR_0x8008, ITEM_CARBOS
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Calcium::
	bufferitemname STR_VAR_1, ITEM_CALCIUM
	setvar VAR_0x8008, ITEM_CALCIUM
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Zinc::
	bufferitemname STR_VAR_1, ITEM_ZINC
	setvar VAR_0x8008, ITEM_ZINC
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HPUp::
	bufferitemname STR_VAR_1, ITEM_HP_UP
	setvar VAR_0x8008, ITEM_HP_UP
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_PPUp::
	bufferitemname STR_VAR_1, ITEM_PP_UP
	setvar VAR_0x8008, ITEM_PP_UP
	setvar VAR_0x8009, 3000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_CancelPowderItemSelect::
	msgbox SlateportCity_Text_ComeBackToTradeBerryPowder, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_TryBuyBerryPowderItem::
	msgbox SlateportCity_Text_ExchangeBerryPowderForItem, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_EventScript_ChooseBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NotEnoughBerryPowder
	giveitem VAR_0x8008
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NoRoomForBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	special TakeBerryPowder
	special PrintPlayerBerryPowderAmount
	msgbox SlateportCity_Text_FineBerryPowderTradeSomethingElse, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_EventScript_ChooseBerryPowderItem
	msgbox SlateportCity_Text_WhenYouGetMoreBringItToMe, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NoRoomForBerryPowderItem::
	msgbox gText_TheBagIsFull, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NotEnoughBerryPowder::
	msgbox SlateportCity_Text_DontHaveEnoughBerryPowder, MSGBOX_DEFAULT
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ScottBattleTentScene::
	lockall
	applywaitmovement LOCALID_PLAYER, Common_Movement_FaceUp
	opendoor 10, 12
	waitdooranim
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PushPlayerDown
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitBattleTent
	waitmovement 0
	closedoor 10, 12
	waitdooranim
	msgbox SlateportCity_Text_TakingBattleTentChallenge, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitAfterBattleTent
	waitmovement 0
	removeobject LOCALID_SLATEPORT_SCOTT
	addvar VAR_SCOTT_STATE, 1
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	releaseall
	end

SlateportCity_Movement_PushPlayerDown:
	lock_facing_direction
	walk_down
	unlock_facing_direction
	step_end

SlateportCity_Movement_PlayerWatchScottExit:
	delay_16
	walk_in_place_faster_right
	step_end

SlateportCity_Movement_ScottExitBattleTent:
	delay_8
	walk_down
	step_end

SlateportCity_Movement_ScottExitAfterBattleTent:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_EventScript_BerryCrushRankingsSign::
	lockall
	special ShowBerryCrushRankings
	releaseall
	end

SlateportCity_Text_EnergyGuruSellWhatYouNeed:
	.string "Welcome, welcome! I'm the ENERGY GURU!\p"
	.string "Hungry? Worn out? Tell me what your\n"
	.string "body is craving, and I'll sell it to you,\l"
	.string "no questions asked!$"

SlateportCity_Text_OhYourPokemon:
	.string "Oh? Your {STR_VAR_1}…\p"
	.string "Its ki is practically crackling from\n"
	.string "over here! That's a fighter's POKéMON if\l"
	.string "I ever saw one.$"

SlateportCity_Text_PleaseGiveItThisEffortRibbon:
	.string "That POKéMON pushed past its limit, and\n"
	.string "then pushed past that, too!\p"
	.string "A warrior that stubborn has earned\n"
	.string "this. Please give it this EFFORT RIBBON.$"

SlateportCity_Text_ReceivedEffortRibbon:
	.string "{PLAYER} was awarded the EFFORT\n"
	.string "RIBBON.$"

SlateportCity_Text_PutEffortRibbonOnMon:
	.string "{PLAYER} fastened the EFFORT RIBBON\n"
	.string "onto {STR_VAR_1}.$"

SlateportCity_Text_GoForItLittleHarder:
	.string "It hasn't hit its limit yet. Train it a\n"
	.string "little harder, then come see me!\p"
	.string "Push it all the way, and I'll give your\n"
	.string "POKéMON something special.$"

SlateportCity_Text_EffortRibbonLooksGoodOnIt:
	.string "Ohh! Your {STR_VAR_1} is wearing the EFFORT\n"
	.string "RIBBON. Only the partner of a true\l"
	.string "warrior earns one of those!$"

SlateportCity_Text_WonderIfLighthouseStartlesPokemon:
	.string "That lighthouse beam reaches dozens of\n"
	.string "miles out over the water. Brighter than\l"
	.string "a KAMEHAMEHA at midnight!\p"
	.string "Do you think it startles the POKéMON\n"
	.string "swimming down there?$"

SlateportCity_Text_SeaweedFullOfLife:
	.string "Hey, look at this seaweed! It's fresh as\n"
	.string "can be.\p"
	.string "The stall owner swears it's so full of\n"
	.string "life that it'll make you hit harder. I\l"
	.string "don't buy it, but I'm taking a bag\l"
	.string "anyway!$"

SlateportCity_Text_HowTownIsBornAndGrows:
	.string "Clean water, good harvest. Good\n"
	.string "harvest, good food.\p"
	.string "And where there's good food, people\n"
	.string "gather and a market opens up.\p"
	.string "That's how a town is born, and that's\n"
	.string "how it grows.$"

SlateportCity_Text_SlateportWonderfulPlace:
	.string "Shopping with the sea breeze in my\n"
	.string "face…\p"
	.string "SLATEPORT is the best! I could eat my\n"
	.string "way down this whole market and still ask\l"
	.string "for seconds.$"

SlateportCity_Text_BuyBricksSoDecorWontGetDirty:
	.string "DOLLS and CUSHIONS get filthy if you\n"
	.string "leave them on the floor.\p"
	.string "So I'm buying BRICKS to set them on.\n"
	.string "They stay spotless, even after a rowdy\l"
	.string "training session!$"

SlateportCity_Text_GoingToCompeteInBattleTent:
	.string "Wroooar! My power level is through the\n"
	.string "roof! I'm taking on the BATTLE TENT!\p"
	.string "But first I need to go catch some\n"
	.string "POKéMON!$"

SlateportCity_Text_BushedHikingFromMauville:
	.string "Whew… I'm wiped out.\p"
	.string "I hoofed it all the way from MAUVILLE\n"
	.string "CITY, and SLATEPORT is way bigger than\l"
	.string "I figured!\p"
	.string "I should've taken my BIKE. Next time I'm\n"
	.string "popping a capsule.$"

SlateportCity_Text_EveryoneCallsHimCaptStern:
	.string "STERN built the MUSEUM, and he also\n"
	.string "leads an undersea exploration team.\p"
	.string "That's why everybody calls him CAPT.\n"
	.string "STERN.$"

SlateportCity_Text_WhatsLongLineOverThere:
	.string "Huh? What's with the long line over\n"
	.string "there?$"

SlateportCity_Text_VisitedMuseumOften:
	.string "I practically lived at the MUSEUM when I\n"
	.string "was a kid.\p"
	.string "I'd stare at the exhibits and dream\n"
	.string "about whatever was hiding down in the\l"
	.string "deep.$"

SlateportCity_Text_QuitPushing:
	.string "Hey, no cutting!\n"
	.string "The line starts back there, pal!$"

SlateportCity_Text_AquaHasPolicy:
	.string "The RED RIBBON ARMY always musters\n"
	.string "right at the operation site. Standard\l"
	.string "procedure! Sir!$"

SlateportCity_Text_BossIsBrilliant:
	.string "DR. GERO is a genius, sir. No question!\p"
	.string "But what would a scientist want with a\n"
	.string "MUSEUM all of a sudden?$"

SlateportCity_Text_WhatsNewSchemeIWonder:
	.string "What's the new scheme, I wonder?\p"
	.string "DR. GERO gets scary when things go\n"
	.string "wrong, and KID BUU is even scarier. I'd\l"
	.string "better not mess this one up…$"

SlateportCity_Text_ShouldTakeItAll:
	.string "If the MUSEUM has anything the doctor\n"
	.string "needs, we just grab all of it! Easy!$"

SlateportCity_Text_DontButtIn:
	.string "Hey, you!\n"
	.string "Wait your turn! No butting in!$"

SlateportCity_Text_RemindsMeOfLongLineForGames:
	.string "Man, look at this line.\p"
	.string "Takes me back to the day I queued up\n"
	.string "for WORLD MARTIAL ARTS TOURNAMENT\l"
	.string "tickets… I never got in, either.$"

SlateportCity_Text_WhyAreWeLiningUp:
	.string "Why are we paying and waiting in line\n"
	.string "like civilians? We're an army! We should\l"
	.string "just march right in!$"

SlateportCity_Text_WhatDoYouWant:
	.string "Huh?\n"
	.string "What do you want, kid? Move along!$"

SlateportCity_Text_IllReadSignForYou:
	.string "You want to read this sign?\n"
	.string "Fine, I'll read it out loud for you!$"

SlateportCity_Text_SaysSomethingLikeSeaIsEndless:
	.string "Let's see…\p"
	.string "Um… I think it says something like “the\n"
	.string "life in the sea is endless.”\p"
	.string "Yup. Pretty sure that's what it says.\n"
	.string "Sir.$"

SlateportCity_Text_ShouldveBroughtMyGameBoy:
	.string "Grumble…\p"
	.string "I should've packed some lunch for this\n"
	.string "line… My stomach is roaring louder than\l"
	.string "a SAIBAMAN…\p"
	.string "Grumble…$"

SlateportCity_Text_HotSpringsAfterOperation:
	.string "When this operation's over, the whole\n"
	.string "squad is going to a hot spring spa!\p"
	.string "That's what our colonel promised. I\n"
	.string "can't wait!$"

SlateportCity_Text_SeaIsSoWet:
	.string "The sea goes on forever…\p"
	.string "Do you think it's all the tears POKéMON\n"
	.string "have ever shed?$"

SlateportCity_Text_SinkOldBoats:
	.string "You know what they do with old ships\n"
	.string "too creaky to sail?\p"
	.string "They sink them in the sea, and POKéMON\n"
	.string "move in and make homes out of them.$"

SlateportCity_Text_BuyTooMuch:
	.string "Every time I come here I get carried\n"
	.string "away and buy way too much. My eyes are\l"
	.string "bigger than my wallet.$"

SlateportCity_Text_GetNameRaterToHelpYou:
	.string "Want to change a POKéMON's nickname?\n"
	.string "You'll need the NAME RATER to help you\l"
	.string "out.$"

SlateportCity_Text_CantChangeTradeMonName:
	.string "A POKéMON you got in a trade can't be\n"
	.string "renamed.\p"
	.string "The original TRAINER's bond with it is\n"
	.string "written right into that nickname.$"

SlateportCity_Text_BattleTentBuiltRecently:
	.string "They just built a BATTLE TENT here in\n"
	.string "SLATEPORT.\p"
	.string "GYMS are great, but the BATTLE TENT is\n"
	.string "awesome in its own way. It's like a\l"
	.string "pocket-sized tournament!\p"
	.string "Go dig up some tough POKéMON and give\n"
	.string "it a shot!$"

SlateportCity_Text_CaptSternBeingInterviewed:
	.string "I was hoping that crowd was for MR.\n"
	.string "SATAN, the World Champion, so I could\l"
	.string "get an autograph.\p"
	.string "But who's being interviewed? Isn't that\n"
	.string "CAPT. STERN?$"

SlateportCity_Text_InterviewerSoCool:
	.string "That lady interviewer is so cool and\n"
	.string "pretty.\p"
	.string "When I grow up, I'm going to be an\n"
	.string "international journalist!$"

SlateportCity_Text_SternSaysDiscoveredSomething:
	.string "CAPT. STERN says his team found\n"
	.string "something on the bottom of the sea.\p"
	.string "What could it be? I can't wait to find\n"
	.string "out!$"

SlateportCity_Text_CaptainComeBackWithBigFish:
	.string "What's going on here?\p"
	.string "Did the CAPTAIN come back from the\n"
	.string "ocean floor with a monster catch? I'm\l"
	.string "starving!$"

SlateportCity_Text_AmIOnTV:
	.string "Hey! Are you watching?\n"
	.string "Am I on TV?$"

SlateportCity_Text_CaptainsACelebrity:
	.string "A TV interview, right here in town!\n"
	.string "The CAPTAIN is a celebrity!$"

SlateportCity_Text_BigSmileForCamera:
	.string "TY: Okay, CAPT. STERN, give the camera a\n"
	.string "big smile!$"

SlateportCity_Text_MostInvaluableExperience:
	.string "GABBY: I see, I see. That must have been\n"
	.string "a priceless experience…$"

SlateportCity_Text_SternMoveAheadWithExploration:
	.string "CAPT. STERN: It was. We intend to push\n"
	.string "on with our exploration.$"

SlateportCity_Text_GabbyWonderfulThanksForInterview:
	.string "GABBY: Wonderful, CAPT. STERN! Thank\n"
	.string "you for taking time out of your busy\l"
	.string "schedule to talk to us.\p"
	.string "We hope we can interview you again when\n"
	.string "you have news of more discoveries!$"

SlateportCity_Text_SternWhewFirstInterview:
	.string "CAPT. STERN: Whew…\p"
	.string "That was my first time on TV. I was more\n"
	.string "nervous than in any storm at sea.$"

SlateportCity_Text_OhPlayerWeMadeDiscovery:
	.string "Oh! {PLAYER}{KUN}!\n"
	.string "You look stronger than ever!\p"
	.string "Our last seafloor expedition turned up\n"
	.string "something huge.\p"
	.string "We found an underwater cavern on ROUTE\n"
	.string "128.\p"
	.string "We think it's the home of a POKéMON\n"
	.string "that was said to have died out ages\l"
	.string "ago.$"

SlateportCity_Text_AquaWillAssumeControlOfSubmarine:
	.string "Kyahahaha…\n"
	.string "CAPT. STERN, I presume.\p"
	.string "This is DR. GERO of the RED RIBBON ARMY.\n"
	.string "Your submarine is now under our\l"
	.string "command!\p"
	.string "Objections are meaningless. You will\n"
	.string "cooperate fully.\p"
	.string "Kyahahaha… Watch closely, and see what\n"
	.string "the SEA squad has planned for the\l"
	.string "ocean!$"

SlateportCity_Text_SternWhatWasAllThat:
	.string "CAPT. STERN: What was that all about?\p"
	.string "It sounded like someone shouting\n"
	.string "through a megaphone…\p"
	.string "Where did it come from?$"

SlateportCity_Text_FromHarborTryingToTakeSub:
	.string "It's coming from the HARBOR!\p"
	.string "The submarine!\n"
	.string "They're trying to steal it!$"

SlateportCity_Text_PleaseComeWithMe:
	.string "{PLAYER}{KUN}!\n"
	.string "Please, come with me!$"

SlateportCity_Text_BattleTentSign:
	.string "BATTLE TENT SLATEPORT SITE\n"
	.string "“Find it! The ultimate POKéMON!”$"

SlateportCity_Text_SternsShipyardWantedSign:
	.string "STERN'S SHIPYARD\p"
	.string "“Wanted: Sailors who can handle any\n"
	.string "current. Big appetites welcome.”$"

SlateportCity_Text_SternsShipyardNearsCompletion:
	.string "STERN'S SHIPYARD\p"
	.string "“The ferry S.S. TIDAL is nearing\n"
	.string "completion, and will soon serve the\l"
	.string "ports of SLATEPORT and LILYCOVE.”$"

SlateportCity_Text_SternsShipyardFerryComplete:
	.string "STERN'S SHIPYARD\p"
	.string "“Boarding for the SLATEPORT-LILYCOVE\n"
	.string "ferry S.S. TIDAL is handled at the\l"
	.string "HARBOR.”$"

SlateportCity_Text_PokemonFanClubSign:
	.string "POKéMON FAN CLUB\n"
	.string "“Fans of every POKéMON, come on in!”$"

SlateportCity_Text_OceanicMuseumSign:
	.string "“The endless sea nourishes all life.”\p"
	.string "OCEANIC MUSEUM$"

SlateportCity_Text_CitySign:
	.string "SLATEPORT CITY\p"
	.string "“The port where people and POKéMON\n"
	.string "cross paths.”$"

SlateportCity_Text_MarketSign:
	.string "SLATEPORT MARKET “Rare finds you won't\n"
	.string "see anywhere else!”$"

SlateportCity_Text_HarborFerryUnderConstruction:
	.string "SLATEPORT HARBOR\p"
	.string "“The ferry S.S. TIDAL is still under\n"
	.string "construction at the SHIPYARD.\p"
	.string "“Service will begin shortly.”$"

SlateportCity_Text_HarborSign:
	.string "SLATEPORT HARBOR\p"
	.string "“Enjoy a relaxing cruise aboard the\n"
	.string "ferry S.S. TIDAL.”$"

SlateportCity_Text_NameRatersHouseSign:
	.string "NAME RATER'S HOUSE\n"
	.string "“POKéMON nicknames rated here.”$"

SlateportCity_Text_ExplainBerries:
	.string "When a wild POKéMON gets hurt, it heals\n"
	.string "itself by chewing on BERRIES.\p"
	.string "Did you know that?\p"
	.string "Someone saw that and got the idea to\n"
	.string "make medicine out of BERRIES.\p"
	.string "Did you know that?\p"
	.string "To turn BERRIES into medicine, you have\n"
	.string "to grind them into powder.\p"
	.string "Did you know that?\p"
	.string "You look interested in BERRIES! And a\n"
	.string "fighter like you will want the good\l"
	.string "stuff.\p"
	.string "I've got something just for you.$"

SlateportCity_Text_ExplainBerryPowder:
	.string "I recently had machines installed at\n"
	.string "POKéMON CENTERS for crushing BERRIES.\p"
	.string "The BERRY CRUSH machines are in the\n"
	.string "DIRECT CORNER upstairs.\p"
	.string "Did you know?\p"
	.string "Could you make BERRY POWDER for me with\n"
	.string "those machines?\p"
	.string "Bring me plenty of BERRY POWDER and I'll\n"
	.string "whip you up all sorts of medicine.$"

SlateportCity_Text_BroughtMeSomeBerryPowder:
	.string "Did you bring me some BERRY POWDER?$"

SlateportCity_Text_ExchangeWhatWithIt:
	.string "What would you like in exchange?$"

SlateportCity_Text_ExchangeBerryPowderForItem:
	.string "So, you'd trade your BERRY POWDER for\n"
	.string "one {STR_VAR_1}? Is that right?$"

SlateportCity_Text_DontHaveEnoughBerryPowder:
	.string "Oh, dear. You don't have enough BERRY\n"
	.string "POWDER for that.$"

SlateportCity_Text_FineBerryPowderTradeSomethingElse:
	.string "Now that's fine BERRY POWDER. It'll make\n"
	.string "excellent medicine.\p"
	.string "Would you like to trade more of your\n"
	.string "BERRY POWDER for something else?$"

SlateportCity_Text_WhenYouGetMoreBringItToMe:
	.string "Very well! When you've got more BERRY\n"
	.string "POWDER, bring it to me!$"

SlateportCity_Text_ComeBackToTradeBerryPowder:
	.string "Come back whenever you want to trade\n"
	.string "your BERRY POWDER for medicine.\p"
	.string "My bazaar is always open here.\p"
	.string "Did you know?$"

SlateportCity_Text_YouDroveTeamAquaAway:
	.string "ANNOUNCER: Huh? I'm sure I've seen you\n"
	.string "somewhere before.\p"
	.string "Have I introduced myself? If not, I'm\n"
	.string "the ANNOUNCER. I scout fighters for\l"
	.string "the BATTLE FRONTIER.\p"
	.string "I just watched the RED RIBBON ARMY\n"
	.string "come tearing out of here like their\l"
	.string "tails were on fire.\p"
	.string "Let me guess… you're the one who\n"
	.string "chased them off?$"

SlateportCity_Text_MaybeThisTrainer:
	.string "ANNOUNCER: Hmm… That power level…\n"
	.string "Maybe, just maybe, this TRAINER…$"

SlateportCity_Text_LetsRegisterEachOther:
	.string "ANNOUNCER: Yes! I think we're going to\n"
	.string "get along just fine.\p"
	.string "Let's register each other in our\n"
	.string "POKéNAVS.\p"
	.string "… … … … … …$"

SlateportCity_Text_RegisteredScott:
	.string "Registered the ANNOUNCER in the\n"
	.string "POKéNAV.$"

SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou:
	.string "ANNOUNCER: I'd love to tag along with\n"
	.string "you, but I have to keep an eye on the\l"
	.string "talent in other towns, too.\p"
	.string "So I'll roam around a bit more.\p"
	.string "See you around, {PLAYER}{KUN}!$"

SlateportCity_Text_TakingBattleTentChallenge:
	.string "ANNOUNCER: Oh, hey!\n"
	.string "If it isn't {PLAYER}{KUN}!\p"
	.string "{PLAYER}{KUN}, let me guess… you're here to\n"
	.string "take on the BATTLE TENT?\p"
	.string "Yes! Great idea! Really great idea!\p"
	.string "A fighter like you is a perfect fit for\n"
	.string "the BATTLE TENT!\p"
	.string "Give it everything you've got!$"
