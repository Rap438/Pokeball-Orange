RustboroCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RustboroCity_OnFrame
	.byte 0

RustboroCity_OnTransition:
	setflag FLAG_VISITED_RUSTBORO_CITY
	call Common_EventScript_SetupRivalGfxId
	call_if_eq VAR_RUSTBORO_CITY_STATE, 6, RustboroCity_EventScript_HideMapNamePopup
	getplayerxy VAR_TEMP_0, VAR_TEMP_1
	goto_if_eq VAR_RUSTBORO_CITY_STATE, 6, RustboroCity_EventScript_PositionScientistForExit
	end

RustboroCity_EventScript_PositionScientistForExit::
	goto_if_eq VAR_TEMP_0, 11, RustboroCity_EventScript_PositionScientistLeftExit
	setobjectxyperm LOCALID_RUSTBORO_SCIENTIST, 12, 15
	end

RustboroCity_EventScript_PositionScientistLeftExit::
	setobjectxyperm LOCALID_RUSTBORO_SCIENTIST, 11, 15
	end

RustboroCity_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

RustboroCity_OnFrame:
	map_script_2 VAR_RUSTBORO_CITY_STATE, 6, RustboroCity_EventScript_ScientistAddMatchCall
	.2byte 0

RustboroCity_EventScript_ScientistAddMatchCall::
	lockall
	setvar VAR_ROUTE104_STATE, 1
	applywaitmovement LOCALID_PLAYER, RustboroCity_Movement_PlayerWalkDown
	playse SE_EXIT
	delay 10
	addobject LOCALID_RUSTBORO_SCIENTIST
	applywaitmovement LOCALID_RUSTBORO_SCIENTIST, RustboroCity_Movement_ScientistApproachPlayer
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_SCIENTIST, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_SCIENTIST, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	applywaitmovement LOCALID_RUSTBORO_SCIENTIST, RustboroCity_Movement_ScientistWalkInPlaceDown
	msgbox RustboroCity_Text_DevelopedNewPokenavFeature, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HAS_MATCH_CALL
	applywaitmovement LOCALID_RUSTBORO_SCIENTIST, Common_Movement_WalkInPlaceFasterLeft
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 20
	applywaitmovement LOCALID_RUSTBORO_SCIENTIST, Common_Movement_WalkInPlaceFasterDown
	msgbox RustboroCity_Text_AddedMatchCallPleaseCallMrStone, MSGBOX_DEFAULT
	closemessage
	delay 20
	goto RustboroCity_EventScript_MatchCallTutorial

RustboroCity_EventScript_PleaseSelectPokenav::
	msgbox RustboroCity_Text_PleaseSelectPokenav, MSGBOX_DEFAULT
	closemessage
	delay 10
	goto RustboroCity_EventScript_MatchCallTutorial

RustboroCity_EventScript_MatchCallTutorial::
	setflag FLAG_ADDED_MATCH_CALL_TO_POKENAV
	special ScriptMenu_CreateStartMenuForPokenavTutorial
	switch VAR_RESULT
	case 0, RustboroCity_EventScript_PleaseSelectPokenav
	case 1, RustboroCity_EventScript_PleaseSelectPokenav
	case 2, RustboroCity_EventScript_PleaseSelectPokenav
	case 4, RustboroCity_EventScript_PleaseSelectPokenav
	case 5, RustboroCity_EventScript_PleaseSelectPokenav
	case 6, RustboroCity_EventScript_PleaseSelectPokenav
	case 7, RustboroCity_EventScript_PleaseSelectPokenav
	case MULTI_B_PRESSED, RustboroCity_EventScript_PleaseSelectPokenav
	special OpenPokenavForTutorial
	delay 20
	msgbox RustboroCity_Text_IdBetterGetBackToWork, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_RUSTBORO_SCIENTIST, RustboroCity_Movement_ScientistLeave
	playse SE_EXIT
	removeobject LOCALID_RUSTBORO_SCIENTIST
	setflag FLAG_HIDE_RUSTBORO_CITY_SCIENTIST
	setvar VAR_RUSTBORO_CITY_STATE, 7
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	releaseall
	end

RustboroCity_Movement_ScientistWalkInPlaceDown:
	walk_in_place_down
	step_end

RustboroCity_Movement_PlayerWalkDown:
	walk_down
	step_end

RustboroCity_Movement_ScientistApproachPlayer:
	walk_down
	delay_16
	step_end

@ Unused
RustboroCity_Movement_ScientistWalkAroundPlayer:
	delay_16
	walk_left
	walk_down
	walk_down
	walk_right
	walk_in_place_faster_up
	delay_16
	step_end

RustboroCity_Movement_ScientistLeave:
	walk_up
	step_end

RustboroCity_EventScript_FatMan::
	lock
	faceplayer
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_EventScript_FatManSawGrunt
	msgbox RustboroCity_Text_WeShortenItToDevon, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_FatManSawGrunt::
	msgbox RustboroCity_Text_SneakyLookingManWentAroundCorner, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_DevonEmployee2::
	lock
	faceplayer
	msgbox RustboroCity_Text_YoureNewAroundHere, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_Woman::
	msgbox RustboroCity_Text_GymLeaderIsntEasyWithFire, MSGBOX_NPC
	end

RustboroCity_EventScript_Man1::
	lock
	faceplayer
	goto_if_set FLAG_BADGE01_GET, RustboroCity_EventScript_Man1HaveBadge
	msgbox RustboroCity_Text_HaveYouChallengedGym, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_Man1HaveBadge::
	msgbox RustboroCity_Text_HeyThatsRustborosGymBadge, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_Boy2::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POKENAV, RustboroCity_EventScript_Boy2BrineyLeftTunnel
	msgbox RustboroCity_Text_MrBrineyWalksInTheTunnel, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_Boy2BrineyLeftTunnel::
	msgbox RustboroCity_Text_MrBrineyLovesPeeko, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_Twin::
	msgbox RustboroCity_Text_WowYouHavePokemon, MSGBOX_NPC
	end

RustboroCity_EventScript_NinjaBoy::
	msgbox RustboroCity_Text_CatchRarePokemonIfIGoToSchool, MSGBOX_SIGN
	end

RustboroCity_EventScript_TunnelSign::
	msgbox RustboroCity_Text_TunnelNearingCompletion, MSGBOX_SIGN
	end

RustboroCity_EventScript_DevonCorpSign::
	msgbox RustboroCity_Text_DevonCorpSign, MSGBOX_SIGN
	end

RustboroCity_EventScript_GymSign::
	msgbox RustboroCity_Text_GymSign, MSGBOX_SIGN
	end

@ Unused
RustboroCity_EventScript_DevonCorpBranchOfficeSign::
	msgbox RustboroCity_Text_DevonCorpBranchOfficeSign, MSGBOX_SIGN
	end

RustboroCity_EventScript_CitySign::
	msgbox RustboroCity_Text_CitySign, MSGBOX_SIGN
	end

RustboroCity_EventScript_TrainersSchoolSign::
	msgbox RustboroCity_Text_TrainersSchoolSign, MSGBOX_SIGN
	end

RustboroCity_EventScript_CuttersHouseSign::
	msgbox RustboroCity_Text_CuttersHouse, MSGBOX_SIGN
	end

RustboroCity_EventScript_LittleBoy::
	lock
	faceplayer
	msgbox RustboroCity_Text_PokemonCanChangeLookFromExp, MSGBOX_DEFAULT
	applywaitmovement LOCALID_RUSTBORO_LITTLE_BOY, Common_Movement_FaceOriginalDirection
	release
	end

RustboroCity_EventScript_LittleGirl::
	lock
	faceplayer
	msgbox RustboroCity_Text_PokemonChangeShape, MSGBOX_DEFAULT
	applywaitmovement LOCALID_RUSTBORO_LITTLE_GIRL, Common_Movement_FaceOriginalDirection
	release
	end

RustboroCity_EventScript_Man2::
	lock
	faceplayer
	msgbox RustboroCity_Text_TradePokemonGrowFast, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_StolenGoodsTrigger0::
	lockall
	setobjectxyperm LOCALID_RUSTBORO_DEVON_EMPLOYEE, 14, 21
	setobjectmovementtype LOCALID_RUSTBORO_DEVON_EMPLOYEE, MOVEMENT_TYPE_FACE_RIGHT
	setvar VAR_0x8004, 0
	goto RustboroCity_EventScript_StolenGoodsScene
	end

RustboroCity_EventScript_StolenGoodsTrigger1::
	lockall
	setobjectxyperm LOCALID_RUSTBORO_DEVON_EMPLOYEE, 14, 21
	setobjectmovementtype LOCALID_RUSTBORO_DEVON_EMPLOYEE, MOVEMENT_TYPE_FACE_RIGHT
	setvar VAR_0x8004, 1
	goto RustboroCity_EventScript_StolenGoodsScene
	end

RustboroCity_EventScript_StolenGoodsTrigger2::
	lockall
	setobjectxyperm LOCALID_RUSTBORO_DEVON_EMPLOYEE, 14, 21
	setobjectmovementtype LOCALID_RUSTBORO_DEVON_EMPLOYEE, MOVEMENT_TYPE_FACE_RIGHT
	setvar VAR_0x8004, 2
	goto RustboroCity_EventScript_StolenGoodsScene
	end

RustboroCity_EventScript_StolenGoodsTrigger3::
	lockall
	setobjectxyperm LOCALID_RUSTBORO_DEVON_EMPLOYEE, 14, 21
	setobjectmovementtype LOCALID_RUSTBORO_DEVON_EMPLOYEE, MOVEMENT_TYPE_FACE_RIGHT
	setvar VAR_0x8004, 3
	goto RustboroCity_EventScript_StolenGoodsScene
	end

RustboroCity_EventScript_StolenGoodsTrigger4::
	lockall
	setobjectxyperm LOCALID_RUSTBORO_DEVON_EMPLOYEE, 14, 21
	setobjectmovementtype LOCALID_RUSTBORO_DEVON_EMPLOYEE, MOVEMENT_TYPE_FACE_RIGHT
	setvar VAR_0x8004, 4
	goto RustboroCity_EventScript_StolenGoodsScene
	end

RustboroCity_EventScript_StolenGoodsScene::
	msgbox RustboroCity_Text_OutOfTheWay, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	addobject LOCALID_RUSTBORO_GRUNT
	addobject LOCALID_RUSTBORO_DEVON_EMPLOYEE
	applywaitmovement LOCALID_RUSTBORO_GRUNT, RustboroCity_Movement_GruntEscape
	removeobject LOCALID_RUSTBORO_GRUNT
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeChaseGrunt1
	msgbox RustboroCity_Text_WaitDontTakeMyGoods, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeChaseGrunt2
	fadedefaultbgm
	removeobject LOCALID_RUSTBORO_DEVON_EMPLOYEE
	setobjectxyperm LOCALID_RUSTBORO_DEVON_EMPLOYEE, 30, 10
	clearflag FLAG_HIDE_RUSTBORO_CITY_DEVON_EMPLOYEE_1
	setflag FLAG_DEVON_GOODS_STOLEN
	setvar VAR_RUSTBORO_CITY_STATE, 2
	setvar VAR_RUSTURF_TUNNEL_STATE, 2
	setvar VAR_ROUTE116_STATE, 1
	clearflag FLAG_HIDE_ROUTE_116_MR_BRINEY
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_PEEKO
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_AQUA_GRUNT
	setflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	setflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	releaseall
	end

@ Unused
RustboroCity_EventScript_ShadyCharacterTookOff::
	msgbox RustboroCity_Text_ShadyCharacterTookOffTowardsTunnel, MSGBOX_DEFAULT
	return

@ Unused
RustboroCity_EventScript_YouGotItThankYou::
	msgbox RustboroCity_Text_YouGotItThankYou, MSGBOX_DEFAULT
	return

@ Unknown, unused employee movements. Based on the differences in movement theyre for approaching an object (probably the player) in different positions
RustboroCity_EventScript_EmployeeApproachUp::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeApproachUp
	return

@ Unused, see above
RustboroCity_EventScript_EmployeeApproachLeft::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeApproachLeft
	return

@ Unused, see above
RustboroCity_EventScript_EmployeeApproachRight::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeApproachRight
	return

@ Unused, see above
RustboroCity_EventScript_EmployeeApproachDown::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeApproachDown
	return

@ Unused, similar movement to the above scripts
RustboroCity_EventScript_EmployeeApproachPlayerFar::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeApproachPlayerFar
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	return

@ Unused
RustboroCity_Movement_GruntEscapeExtended:
	walk_faster_right
	walk_faster_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

RustboroCity_Movement_GruntEscape:
	walk_faster_right
	walk_faster_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

RustboroCity_Movement_EmployeeChaseGrunt1:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

RustboroCity_Movement_EmployeeChaseGrunt2:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

@ Functionally unused
RustboroCity_Movement_EmployeeApproachUp:
	walk_down
	walk_right
	walk_right
	step_end

@ Functionally unused
RustboroCity_Movement_EmployeeApproachLeft:
	walk_down
	walk_down
	walk_right
	walk_right
	step_end

@ Functionally unused
RustboroCity_Movement_EmployeeApproachRight:
	walk_down
	walk_down
	walk_right
	walk_right
	step_end

@ Functionally unused
RustboroCity_Movement_EmployeeApproachDown:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	step_end

@ Functionally unused
RustboroCity_Movement_EmployeeApproachPlayerFar:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_down
	step_end

RustboroCity_EventScript_DevonEmployee1::
	lock
	faceplayer
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_EventScript_ReturnGoodsSpokeToEmployee
	msgbox RustboroCity_Text_ShadyCharacterTookOffTowardsTunnel, MSGBOX_DEFAULT
	release
	end

RustboroCity_EventScript_ReturnGoodsSpokeToEmployee::
	waitse
	setvar VAR_TEMP_1, 4
	goto RustboroCity_EventScript_ReturnGoods
	end

RustboroCity_EventScript_HelpGetGoodsTrigger0::
	lockall
	setvar VAR_TEMP_1, 0
	goto RustboroCity_EventScript_EmployeeAskToGetGoods
	end

RustboroCity_EventScript_HelpGetGoodsTrigger1::
	lockall
	setvar VAR_TEMP_1, 1
	goto RustboroCity_EventScript_EmployeeAskToGetGoods
	end

RustboroCity_EventScript_HelpGetGoodsTrigger2::
	lockall
	setvar VAR_TEMP_1, 2
	goto RustboroCity_EventScript_EmployeeAskToGetGoods
	end

RustboroCity_EventScript_HelpGetGoodsTrigger3::
	lockall
	setvar VAR_TEMP_1, 3
	goto RustboroCity_EventScript_EmployeeAskToGetGoods
	end

RustboroCity_EventScript_EmployeeAskToGetGoods::
	call_if_eq VAR_TEMP_1, 0, RustboroCity_EventScript_EmployeeFacePlayerUp1
	call_if_eq VAR_TEMP_1, 1, RustboroCity_EventScript_EmployeeFacePlayerLeft1
	call_if_eq VAR_TEMP_1, 2, RustboroCity_EventScript_EmployeeFacePlayerDown1
	call_if_eq VAR_TEMP_1, 3, RustboroCity_EventScript_EmployeeApproachPlayerDown1
	setflag FLAG_INTERACTED_WITH_DEVON_EMPLOYEE_GOODS_STOLEN
	setvar VAR_RUSTBORO_CITY_STATE, 3
	copyobjectxytoperm LOCALID_RUSTBORO_DEVON_EMPLOYEE
	msgbox RustboroCity_Text_HelpMeIWasRobbed, MSGBOX_DEFAULT
	releaseall
	end

@ The below movement scripts are either partially or fully duplicated by the movement scripts when the player returns the goods
RustboroCity_EventScript_EmployeeFacePlayerUp1::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	return

RustboroCity_EventScript_EmployeeFacePlayerLeft1::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterLeft
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_FaceRight
	return

RustboroCity_EventScript_EmployeeFacePlayerDown1::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	return

RustboroCity_EventScript_EmployeeApproachPlayerDown1::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeApproachPlayerDown
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	return

RustboroCity_Movement_EmployeeApproachPlayerDown:
	walk_down
	step_end

RustboroCity_EventScript_ReturnGoodsTrigger0::
	lockall
	setvar VAR_TEMP_1, 0
	goto RustboroCity_EventScript_ReturnGoods
	end

RustboroCity_EventScript_ReturnGoodsTrigger1::
	lockall
	setvar VAR_TEMP_1, 1
	goto RustboroCity_EventScript_ReturnGoods
	end

RustboroCity_EventScript_ReturnGoodsTrigger2::
	lockall
	setvar VAR_TEMP_1, 2
	goto RustboroCity_EventScript_ReturnGoods
	end

RustboroCity_EventScript_ReturnGoodsTrigger3::
	lockall
	setvar VAR_TEMP_1, 3
	goto RustboroCity_EventScript_ReturnGoods
	end

RustboroCity_EventScript_ReturnGoods::
	call_if_eq VAR_TEMP_1, 0, RustboroCity_EventScript_EmployeeFacePlayerUp2
	call_if_eq VAR_TEMP_1, 1, RustboroCity_EventScript_EmployeeFacePlayerLeft2
	call_if_eq VAR_TEMP_1, 2, RustboroCity_EventScript_EmployeeFacePlayerDown2
	call_if_eq VAR_TEMP_1, 3, RustboroCity_EventScript_EmployeeApproachPlayerDown2
	call_if_eq VAR_TEMP_1, 4, RustboroCity_EventScript_EmployeeFacePlayerRight
	msgbox RustboroCity_Text_YouGotItThankYou, MSGBOX_DEFAULT
	giveitem ITEM_GREAT_BALL
	call_if_eq VAR_RESULT, FALSE, RustboroCity_EventScript_BagFull
	msgbox RustboroCity_Text_PleaseComeWithMe, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RETURNED_DEVON_GOODS
	setflag FLAG_HIDE_RUSTBORO_CITY_DEVON_EMPLOYEE_1
	setvar VAR_RUSTBORO_CITY_STATE, 5
	delay 30
	warp MAP_RUSTBORO_CITY_DEVON_CORP_3F, 2, 2
	waitstate
	releaseall
	end

RustboroCity_EventScript_BagFull::
	msgbox RustboroCity_Text_YoureLoadedWithItems, MSGBOX_DEFAULT
	return

RustboroCity_EventScript_EmployeeFacePlayerUp2::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	return

RustboroCity_EventScript_EmployeeFacePlayerLeft2::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterRight
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_FaceLeft
	return

RustboroCity_EventScript_EmployeeFacePlayerDown2::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	return

RustboroCity_EventScript_EmployeeApproachPlayerDown2::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, RustboroCity_Movement_EmployeeApproachPlayerDown
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	return

RustboroCity_EventScript_EmployeeFacePlayerRight::
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterLeft
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_DEVON_EMPLOYEE, Common_Movement_Delay48
	return

RustboroCity_EventScript_Rival::
	lockall
	call_if_unset FLAG_MET_RIVAL_RUSTBORO, RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_FacePlayer
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_PlayRivalMusic::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, RustboroCity_EventScript_PlayMayMusic
	goto_if_eq VAR_RESULT, FEMALE, RustboroCity_EventScript_PlayBrendanMusic
	return

RustboroCity_EventScript_PlayMayMusic::
	playbgm MUS_ENCOUNTER_MAY, TRUE
	return

RustboroCity_EventScript_PlayBrendanMusic::
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	return

RustboroCity_EventScript_RivalTrigger0::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer0
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalTrigger1::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer1
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalTrigger2::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer2
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalTrigger3::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer3
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalTrigger4::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer4
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalTrigger5::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer5
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalTrigger6::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer6
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalTrigger7::
	lockall
	call RustboroCity_EventScript_PlayRivalMusic
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_WalkInPlaceFasterDown
	playse SE_PIN
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RUSTBORO_RIVAL, Common_Movement_Delay48
	applywaitmovement LOCALID_RUSTBORO_RIVAL, RustboroCity_Movement_RivalApproachPlayer7
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	goto RustboroCity_EventScript_RivalEncounter

RustboroCity_EventScript_RivalEncounter::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, RustboroCity_EventScript_MayEncounter
	goto_if_eq VAR_RESULT, FEMALE, RustboroCity_EventScript_BrendanEncounter
	end

RustboroCity_EventScript_MayEncounter::
	goto_if_set FLAG_DEFEATED_RIVAL_RUSTBORO, RustboroCity_EventScript_MayBrineyHint
	goto_if_set FLAG_MET_RIVAL_RUSTBORO, RustboroCity_EventScript_MayAskToBattle
	setflag FLAG_MET_RIVAL_RUSTBORO
	msgbox RustboroCity_Text_MayHiLetsRegister, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Text_RegisteredMay, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_RIVAL_MATCH_CALL
	setvar VAR_RUSTBORO_CITY_STATE, 8
	setvar VAR_ROUTE104_STATE, 2
	setvar VAR_0x8008, 0
	msgbox RustboroCity_Text_MayPassedBrineyWantToBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RustboroCity_EventScript_BattleMay
	msgbox RustboroCity_Text_MayOhHaventRaisedPokemonEnough, MSGBOX_DEFAULT
	call RustboroCity_EventScript_RestoreBgm
	releaseall
	end

RustboroCity_EventScript_MayAskToBattle::
	setvar VAR_0x8008, 1
	msgbox RustboroCity_Text_MayWantToBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RustboroCity_EventScript_BattleMay
	msgbox RustboroCity_Text_MayOhHaventRaisedPokemonEnough, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_EventScript_BattleMay::
	msgbox RustboroCity_Text_MayImNotGoingToLose, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, RustboroCity_EventScript_BattleMayTreecko
	case 1, RustboroCity_EventScript_BattleMayTorchic
	case 2, RustboroCity_EventScript_BattleMayMudkip
	end

RustboroCity_EventScript_MayBrineyHint::
	msgbox RustboroCity_Text_MayMrBrineyHint, MSGBOX_DEFAULT
	call_if_eq VAR_0x8008, 0, RustboroCity_EventScript_RestoreBgm
	releaseall
	end

RustboroCity_EventScript_RestoreBgm::
	savebgm MUS_DUMMY
	fadedefaultbgm
	return

RustboroCity_EventScript_BattleMayTreecko::
	trainerbattle_no_intro TRAINER_MAY_RUSTBORO_TREECKO, RustboroCity_Text_MayDefeat
	call EventScript_DBZ_VegetaFight
	setflag FLAG_DEFEATED_RIVAL_RUSTBORO
	goto RustboroCity_EventScript_MayBrineyHint
	end

RustboroCity_EventScript_BattleMayTorchic::
	trainerbattle_no_intro TRAINER_MAY_RUSTBORO_TORCHIC, RustboroCity_Text_MayDefeat
	call EventScript_DBZ_VegetaFight
	setflag FLAG_DEFEATED_RIVAL_RUSTBORO
	goto RustboroCity_EventScript_MayBrineyHint
	end

RustboroCity_EventScript_BattleMayMudkip::
	trainerbattle_no_intro TRAINER_MAY_RUSTBORO_MUDKIP, RustboroCity_Text_MayDefeat
	call EventScript_DBZ_VegetaFight
	setflag FLAG_DEFEATED_RIVAL_RUSTBORO
	goto RustboroCity_EventScript_MayBrineyHint
	end

RustboroCity_EventScript_BrendanEncounter::
	goto_if_set FLAG_DEFEATED_RIVAL_RUSTBORO, RustboroCity_EventScript_BrendanBrineyHint
	goto_if_set FLAG_MET_RIVAL_RUSTBORO, RustboroCity_EventScript_BrendanAskToBattle
	setflag FLAG_MET_RIVAL_RUSTBORO
	msgbox RustboroCity_Text_BrendanHiLetsRegister, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Text_RegisteredBrendan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_RIVAL_MATCH_CALL
	setvar VAR_RUSTBORO_CITY_STATE, 8
	setvar VAR_ROUTE104_STATE, 2
	msgbox RustboroCity_Text_BrendanPassedBrineyWantToBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RustboroCity_EventScript_BattleBrendan
	msgbox RustboroCity_Text_BrendanNoConfidenceInPokemon, MSGBOX_DEFAULT
	call RustboroCity_EventScript_RestoreBgm
	releaseall
	end

RustboroCity_EventScript_BrendanAskToBattle::
	msgbox RustboroCity_Text_BrendanWantToBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RustboroCity_EventScript_BattleBrendan
	msgbox RustboroCity_Text_BrendanNoConfidenceInPokemon, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_EventScript_BattleBrendan::
	msgbox RustboroCity_Text_BrendanIWontGoEasy, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, RustboroCity_EventScript_BattleBrendanTreecko
	case 1, RustboroCity_EventScript_BattleBrendanTorchic
	case 2, RustboroCity_EventScript_BattleBrendanMudkip
	end

RustboroCity_EventScript_BrendanBrineyHint::
	msgbox RustboroCity_Text_BrendanMrBrineyHint, MSGBOX_DEFAULT
	call_if_eq VAR_0x8008, 0, RustboroCity_EventScript_RestoreBgm
	releaseall
	end

RustboroCity_EventScript_BattleBrendanTreecko::
	trainerbattle_no_intro TRAINER_BRENDAN_RUSTBORO_TREECKO, RustboroCity_Text_BrendanDefeat
	call EventScript_DBZ_VegetaFight
	setflag FLAG_DEFEATED_RIVAL_RUSTBORO
	goto RustboroCity_EventScript_BrendanBrineyHint
	end

RustboroCity_EventScript_BattleBrendanTorchic::
	trainerbattle_no_intro TRAINER_BRENDAN_RUSTBORO_TORCHIC, RustboroCity_Text_BrendanDefeat
	call EventScript_DBZ_VegetaFight
	setflag FLAG_DEFEATED_RIVAL_RUSTBORO
	goto RustboroCity_EventScript_BrendanBrineyHint
	end

RustboroCity_EventScript_BattleBrendanMudkip::
	trainerbattle_no_intro TRAINER_BRENDAN_RUSTBORO_MUDKIP, RustboroCity_Text_BrendanDefeat
	call EventScript_DBZ_VegetaFight
	setflag FLAG_DEFEATED_RIVAL_RUSTBORO
	goto RustboroCity_EventScript_BrendanBrineyHint
	end

RustboroCity_Movement_RivalApproachPlayer0:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

RustboroCity_Movement_RivalApproachPlayer1:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

RustboroCity_Movement_RivalApproachPlayer2:
	walk_down
	walk_left
	walk_left
	walk_down
	step_end

RustboroCity_Movement_RivalApproachPlayer3:
	walk_down
	walk_left
	walk_down
	step_end

RustboroCity_Movement_RivalApproachPlayer4:
	walk_down
	walk_down
	step_end

RustboroCity_Movement_RivalApproachPlayer5:
	walk_down
	walk_right
	walk_down
	step_end

RustboroCity_Movement_RivalApproachPlayer6:
	walk_down
	walk_right
	walk_right
	walk_down
	step_end

RustboroCity_Movement_RivalApproachPlayer7:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	step_end

RustboroCity_EventScript_Boy1::
	msgbox RustboroCity_Text_YouCanHave2On2Battle, MSGBOX_NPC
	end

RustboroCity_Text_WeShortenItToDevon:
	.string "CAPSULE CORP.… That's the name on the\n"
	.string "sign, and nobody here ever says\l"
	.string "anything longer.\p"
	.string "That company makes all sorts of handy\n"
	.string "products. They even store houses in\l"
	.string "capsules!$"

RustboroCity_Text_SneakyLookingManWentAroundCorner:
	.string "Hm? A sneaky-looking man?\p"
	.string "Come to think of it, yes. Some shifty\n"
	.string "guy in a red uniform dashed around that\l"
	.string "corner.$"

RustboroCity_Text_HaveYouChallengedGym:
	.string "Have you taken the POKéMON GYM\n"
	.string "challenge?\p"
	.string "Once that shiny GYM BADGE is in your\n"
	.string "hand, I think TRAINERS finally realize\l"
	.string "what's expected of them.$"

RustboroCity_Text_HeyThatsRustborosGymBadge:
	.string "Hey, that's RUSTBORO's GYM BADGE!\p"
	.string "Of all the POKéMON GYM BADGES, I'd say\n"
	.string "RUSTBORO's is the coolest.$"

RustboroCity_Text_YoureNewAroundHere:
	.string "Oh? Who might you be? You're a new face\n"
	.string "around here.\p"
	.string "Did you just transfer into the POKéMON\n"
	.string "TRAINER'S SCHOOL?$"

RustboroCity_Text_GymLeaderIsntEasyWithFire:
	.string "I challenged the GYM LEADER, but…\p"
	.string "Winning with my FIRE-type POKéMON isn't\n"
	.string "going to be easy…\p"
	.string "FIRE-type POKéMON don't match up well\n"
	.string "against ROCK-type POKéMON…$"

RustboroCity_Text_MrBrineyWalksInTheTunnel:
	.string "The old sailor MR. BRINEY lives in a\n"
	.string "cottage by the sea.\p"
	.string "He likes to take walks through the\n"
	.string "tunnel every so often.$"

RustboroCity_Text_MrBrineyLovesPeeko:
	.string "The old sailor MR. BRINEY lives in a\n"
	.string "cottage by the sea.\p"
	.string "He said he was off to SLATEPORT to go\n"
	.string "shopping for his pet, PEEKO.\p"
	.string "That old sea dog must really love his\n"
	.string "PEEKO.$"

RustboroCity_Text_WowYouHavePokemon:
	.string "Wow, you've got POKéMON with you, too!\p"
	.string "When I grow up, I'm going to travel the\n"
	.string "world with POKéMON, just like you.$"

RustboroCity_Text_CatchRarePokemonIfIGoToSchool:
	.string "The POKéMON TRAINER'S SCHOOL!\p"
	.string "If I go to this school, will I be able to\n"
	.string "catch rare POKéMON easily?$"

RustboroCity_Text_PokemonCanChangeLookFromExp:
	.string "When a POKéMON gains experience in\n"
	.string "battles, it can sometimes change how it\l"
	.string "looks.$"

RustboroCity_Text_PokemonChangeShape:
	.string "A POKéMON changes shape?\n"
	.string "If one did that in front of me, I'd be\l"
	.string "shocked!$"

RustboroCity_Text_TradePokemonGrowFast:
	.string "A POKéMON you get in a trade grows\n"
	.string "fast.\p"
	.string "But if you don't have certain GYM\n"
	.string "BADGES, it might not obey you…$"

RustboroCity_Text_OutOfTheWay:
	.string "Out of the way!\n"
	.string "The RED RIBBON ARMY is coming through!$"

RustboroCity_Text_WaitDontTakeMyGoods:
	.string "Wait! Pleeeaaase!\p"
	.string "Don't take the CC GOODS!$"

RustboroCity_Text_HelpMeIWasRobbed:
	.string "Oh, it's you!\p"
	.string "You're that amazing TRAINER who helped\n"
	.string "me in PETALBURG WOODS!\p"
	.string "Help me! The RED RIBBON ARMY robbed me!\n"
	.string "I have to get the CC GOODS back!\p"
	.string "If I don't…\n"
	.string "I'm going to be in serious trouble.$"

RustboroCity_Text_ShadyCharacterTookOffTowardsTunnel:
	.string "That shady character in red… I'm pretty\n"
	.string "sure he ran off toward the tunnel over\l"
	.string "there.$"

RustboroCity_Text_YouGotItThankYou:
	.string "Oh! How did it go? The CC GOODS?\p"
	.string "You did it! You got them back!\p"
	.string "You really are a great TRAINER!\p"
	.string "I know! To thank you, here's another\n"
	.string "GREAT BALL!$"

RustboroCity_Text_YoureLoadedWithItems:
	.string "Your BAG is stuffed full.\n"
	.string "I can't give you this GREAT BALL.$"

RustboroCity_Text_PleaseComeWithMe:
	.string "Excuse me, please!\n"
	.string "Please come with me!$"

RustboroCity_Text_MayHiLetsRegister:
	.string "VEGETA: Hmph. There you are, KAKAROT{KUN}.\p"
	.string "So you had MATCH CALL put on your\n"
	.string "POKéNAV.\p"
	.string "Register me. I want to be able to reach\n"
	.string "you anytime I feel like a fight.\p"
	.string "… … … … … …$"

RustboroCity_Text_RegisteredMay:
	.string "VEGETA is now registered\n"
	.string "in your POKéNAV.$"

RustboroCity_Text_MayPassedBrineyWantToBattle:
	.string "VEGETA: By the way, I passed that old\n"
	.string "sailor, MR. BRINEY, in PETALBURG WOODS.\p"
	.string "He was probably heading home to his\n"
	.string "cottage by the sea.\p"
	.string "How is your POKéDEX coming, KAKAROT{KUN}?\n"
	.string "Mine is looking excellent, of course!\p"
	.string "So…\n"
	.string "How about a fight?$"

RustboroCity_Text_MayOhHaventRaisedPokemonEnough:
	.string "VEGETA: What's this?\p"
	.string "You haven't caught or trained many\n"
	.string "POKéMON?\p"
	.string "Pathetic! How do you expect to ever\n"
	.string "surpass me like that?$"

RustboroCity_Text_MayWantToBattle:
	.string "VEGETA: Well? Are you going to fight me\n"
	.string "or not?$"

RustboroCity_Text_MayImNotGoingToLose:
	.string "VEGETA: You've only just become a\n"
	.string "TRAINER, KAKAROT{KUN}. I refuse to lose to\l"
	.string "you!$"

RustboroCity_Text_MayDefeat:
	.string "Gah!\n"
	.string "You're stronger than I expected!$"

RustboroCity_Text_MayMrBrineyHint:
	.string "VEGETA: Oh, and that old man I just\n"
	.string "passed, MR. BRINEY…\p"
	.string "{PLAYER}{KUN}, you've only just moved here,\n"
	.string "so you wouldn't know this, but MR.\l"
	.string "BRINEY was once a revered seafarer.$"

RustboroCity_Text_BrendanHiLetsRegister:
	.string "VEGETA: Hmph, {PLAYER}.\p"
	.string "So you had MATCH CALL put on your\n"
	.string "POKéNAV.\p"
	.string "Register me. I want to be able to find\n"
	.string "you whenever I feel like a fight.\p"
	.string "… … … … … …$"

RustboroCity_Text_RegisteredBrendan:
	.string "VEGETA is now registered\n"
	.string "in your POKéNAV.$"

RustboroCity_Text_BrendanPassedBrineyWantToBattle:
	.string "VEGETA: By the way, {PLAYER}, I passed\n"
	.string "that old sailor, MR. BRINEY, in\l"
	.string "PETALBURG WOODS.\p"
	.string "I'd guess he was heading home to his\n"
	.string "cottage by the sea.\p"
	.string "{PLAYER}, how is your POKéDEX? Filled in\n"
	.string "any pages yet?\p"
	.string "Mine is flawless.\p"
	.string "Want to fight and test how far you've\n"
	.string "really come?$"

RustboroCity_Text_BrendanNoConfidenceInPokemon:
	.string "VEGETA: What's the matter? Don't you\n"
	.string "trust your own POKéMON?$"

RustboroCity_Text_BrendanWantToBattle:
	.string "VEGETA: Well? Let's fight. Right here,\n"
	.string "right now.$"

RustboroCity_Text_BrendanIWontGoEasy:
	.string "VEGETA: I know you've only just become\n"
	.string "a TRAINER, but I won't hold back!$"

RustboroCity_Text_BrendanDefeat:
	.string "Hmph…\n"
	.string "You're not bad.$"

RustboroCity_Text_BrendanMrBrineyHint:
	.string "VEGETA: That old man I just passed, MR.\n"
	.string "BRINEY… do you know him?\p"
	.string "I'd bet not, since you only just moved\n"
	.string "here, {PLAYER}, but MR. BRINEY was once\l"
	.string "a great sailor.$"

RustboroCity_Text_TunnelNearingCompletion:
	.string "“Timesaving tunnel nearing\n"
	.string "completion!”\p"
	.string "That's what the sign says, but someone\n"
	.string "has splashed a big red “X” across it…$"

RustboroCity_Text_DevonCorpSign:
	.string "CAPSULE CORP.\p"
	.string "“Anything you need, we capsule it.”$"

RustboroCity_Text_GymSign:
	.string "RUSTBORO CITY POKéMON GYM\n"
	.string "LEADER: YAMCHA\p"
	.string "“The ex-bandit who now teaches\n"
	.string "ROCK-solid lessons!”$"

RustboroCity_Text_DevonCorpBranchOfficeSign:
	.string "CAPSULE CORP. BRANCH OFFICE\p"
	.string "“Staff and authorized personnel only.\n"
	.string "Everyone else, please wait outside.”$"

RustboroCity_Text_CitySign:
	.string "RUSTBORO CITY\p"
	.string "“The city where nature and science\n"
	.string "meet.”$"

RustboroCity_Text_TrainersSchoolSign:
	.string "POKéMON TRAINER'S SCHOOL\p"
	.string "“We'll teach you everything about\n"
	.string "POKéMON!”$"

RustboroCity_Text_CuttersHouse:
	.string "CUTTER'S HOUSE$"

RustboroCity_Text_DevelopedNewPokenavFeature:
	.string "I've been working on an added feature\n"
	.string "for the POKéNAV…\p"
	.string "And it came out great!\p"
	.string "{PLAYER}, may I see your POKéNAV? The\n"
	.string "one our PRESIDENT gave you?$"

RustboroCity_Text_AddedMatchCallPleaseCallMrStone:
	.string "There you go, {PLAYER}!\p"
	.string "I added a new feature to your POKéNAV\n"
	.string "called MATCH CALL.\p"
	.string "With MATCH CALL, you can chat with\n"
	.string "anyone who's been registered in your\l"
	.string "POKéNAV.\p"
	.string "{PLAYER}, our PRESIDENT, DR. BRIEF,\n"
	.string "should already be registered in your\l"
	.string "POKéNAV.\p"
	.string "Go ahead and test it out. Please give\n"
	.string "our PRESIDENT a call.$"

RustboroCity_Text_PleaseSelectPokenav:
	.string "Please select the POKéNAV.$"

RustboroCity_Text_IdBetterGetBackToWork:
	.string "Okay, I'd better get back to work.\n"
	.string "Take care, {PLAYER}!$"

RustboroCity_Text_YouCanHave2On2Battle:
	.string "Did you know this?\p"
	.string "You can have a 2-on-2 battle even if\n"
	.string "you're not with another TRAINER.\p"
	.string "If you catch the eyes of two TRAINERS\n"
	.string "while you have two or more POKéMON,\l"
	.string "they'll both challenge you.\p"
	.string "Wouldn't it be amazing to beat two\n"
	.string "TRAINERS all by yourself?$"
