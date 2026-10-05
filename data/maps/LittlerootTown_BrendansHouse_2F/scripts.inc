LittlerootTown_BrendansHouse_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_BrendansHouse_2F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_BrendansHouse_2F_OnWarp
	.byte 0

LittlerootTown_BrendansHouse_2F_OnTransition:
	call_if_lt VAR_LITTLEROOT_RIVAL_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_CheckSetReadyToMeetBrendan
	call_if_ge VAR_LITTLEROOT_RIVAL_STATE, 3, LittlerootTown_BrendansHouse_2F_EventScript_CheckShouldUpdateBrendanPos
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 4, PlayersHouse_2F_EventScript_BlockStairsUntilClockIsSet
	call SecretBase_EventScript_SetDecorationFlags
	setvar VAR_SECRET_BASE_INITIALIZED, 0
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckShouldUpdateBrendanPos::
	goto_if_set FLAG_MET_RIVAL_LILYCOVE, LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos
	goto_if_ge VAR_BIRCH_LAB_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_Ret
	goto LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos

LittlerootTown_BrendansHouse_2F_EventScript_TryUpdateBrendanPos::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_2F_EventScript_Ret
	@ Odd that the MaysHouse equivalent was used below instead
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_MaysHouse_2F_EventScript_Ret
	setobjectxyperm LOCALID_RIVALS_HOUSE_2F_RIVAL, 0, 2
	setobjectmovementtype LOCALID_RIVALS_HOUSE_2F_RIVAL, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_2F_EventScript_Ret::
	return

LittlerootTown_BrendansHouse_2F_EventScript_CheckSetReadyToMeetBrendan::
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, LittlerootTown_BrendansHouse_2F_EventScript_SetReadyToMeetBrendan
	return

LittlerootTown_BrendansHouse_2F_EventScript_SetReadyToMeetBrendan::
	setvar VAR_LITTLEROOT_RIVAL_STATE, 2
	return

LittlerootTown_BrendansHouse_2F_OnWarp:
	map_script_2 VAR_SECRET_BASE_INITIALIZED, 0, LittlerootTown_BrendansHouse_2F_EventScript_CheckInitDecor
	.2byte 0

LittlerootTown_BrendansHouse_2F_EventScript_CheckInitDecor::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, SecretBase_EventScript_InitDecorations
	end

LittlerootTown_BrendansHouse_2F_EventScript_RivalsPokeBall::
	lockall
	goto_if_eq VAR_LITTLEROOT_RIVAL_STATE, 2, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendan
	msgbox RivalsHouse_2F_Text_ItsRivalsPokeBall, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendan::
	delay 10
	addobject LOCALID_RIVALS_HOUSE_2F_RIVAL
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanEnters
	playse SE_PIN
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, Common_Movement_Delay48
	delay 10
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_eq VAR_FACING, DIR_NORTH, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanNorth
	call_if_eq VAR_FACING, DIR_SOUTH, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanSouth
	call_if_eq VAR_FACING, DIR_WEST, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanWest
	call_if_eq VAR_FACING, DIR_EAST, LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanEast
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanNorth::
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerNorth
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanNorth
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCNorth
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanSouth::
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerSouth
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanSouth
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCSouth
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanWest::
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerWest
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanWest
	applymovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCWest
	waitmovement 0
	return

LittlerootTown_BrendansHouse_2F_EventScript_MeetBrendanEast::
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerEast
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_RIVALS_HOUSE_2F_RIVAL, LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCEast
	return

LittlerootTown_BrendansHouse_2F_Movement_BrendanEnters:
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerNorth:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCNorth:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanNorth:
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerSouth:
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCSouth:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanSouth:
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerWest:
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCWest:
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanWest:
	delay_8
	delay_16
	walk_in_place_faster_up
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanApproachPlayerEast:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

LittlerootTown_BrendansHouse_2F_Movement_BrendanWalkToPCEast:
	walk_up
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

@ Unused, the player is already facing this direction so its unneeded
LittlerootTown_BrendansHouse_2F_Movement_PlayerWatchBrendanEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_2F_EventScript_PC::
	lockall
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_2F_EventScript_CheckPlayersPC
	goto_if_eq VAR_RESULT, FEMALE, LittlerootTown_BrendansHouse_2F_EventScript_CheckRivalsPC
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckPlayersPC::
	setvar VAR_0x8004, PC_LOCATION_BRENDANS_HOUSE
	special DoPCTurnOnEffect
	playse SE_PC_ON
	msgbox gText_PlayerHouseBootPC, MSGBOX_DEFAULT
	special BedroomPC
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_TurnOffPlayerPC::
	setvar VAR_0x8004, PC_LOCATION_BRENDANS_HOUSE
	playse SE_PC_OFF
	special DoPCTurnOffEffect
	releaseall
	end

LittlerootTown_BrendansHouse_2F_EventScript_CheckRivalsPC::
	msgbox gText_PokemonTrainerSchoolEmail, MSGBOX_DEFAULT
	releaseall
	end

PlayersHouse_2F_EventScript_Notebook::
	msgbox PlayersHouse_2F_Text_Notebook, MSGBOX_SIGN
	end

PlayersHouse_2F_EventScript_GameCube::
	msgbox PlayersHouse_2F_Text_ItsAGameCube, MSGBOX_SIGN
	end

PlayersHouse_2F_Text_ClockIsStopped:
	.string "The clock has stopped…\p"
	.string "Better set it and get it running!$"

PlayersHouse_2F_Text_HowDoYouLikeYourRoom:
	.string "CHI-CHI: {PLAYER}, how do you like our\n"
	.string "new room?\p"
	.string "Good! Everything's put away neatly!\p"
	.string "The movers finished carrying\n"
	.string "everything in downstairs, too.\p"
	.string "Those POKéMON movers are so handy!\p"
	.string "Oh, and make sure everything's\n"
	.string "there on your desk.$"

PlayersHouse_2F_Text_Notebook:
	.string "{PLAYER} flipped open the notebook.\p"
	.string "TRAINING RULE NO. 1\n"
	.string "Press START to open the MENU.\p"
	.string "TRAINING RULE NO. 2\n"
	.string "SAVE to record your progress.\p"
	.string "The remaining pages are blank…$"

Common_Text_LookCloserAtMap:
	.string "{PLAYER} studied the HOENN region map.\n"
	.string "So many places to train!$"

PlayersHouse_2F_Text_ItsAGameCube:
	.string "It's a Nintendo GameCube.\p"
	.string "A Game Boy Advance is hooked up to\n"
	.string "serve as the Controller.\p"
	.string "CHI-CHI would rather you got a job!$"
