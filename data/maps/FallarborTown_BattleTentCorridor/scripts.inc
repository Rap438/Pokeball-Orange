FallarborTown_BattleTentCorridor_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, FallarborTown_BattleTentCorridor_OnFrame
	.byte 0

FallarborTown_BattleTentCorridor_OnFrame:
	map_script_2 VAR_TEMP_0, 0, FallarborTown_BattleTentCorridor_EventScript_EnterCorridor
	.2byte 0

FallarborTown_BattleTentCorridor_EventScript_EnterCorridor::
	lockall
	setvar VAR_TEMP_0, 1
	applymovement LOCALID_FALLARBOR_TENT_CORRIDOR_ATTENDANT, FallarborTown_BattleTentCorridor_Movement_WalkToDoor
	applymovement LOCALID_PLAYER, FallarborTown_BattleTentCorridor_Movement_WalkToDoor
	waitmovement 0
	opendoor 2, 1
	waitdooranim
	applymovement LOCALID_FALLARBOR_TENT_CORRIDOR_ATTENDANT, FallarborTown_BattleTentCorridor_Movement_AttendantEnterDoor
	applymovement LOCALID_PLAYER, FallarborTown_BattleTentCorridor_Movement_PlayerEnterDoor
	waitmovement 0
	closedoor 2, 1
	waitdooranim
	setvar VAR_0x8006, 0
	warp MAP_FALLARBOR_TOWN_BATTLE_TENT_BATTLE_ROOM, 4, 4
	waitstate
	releaseall
	end

FallarborTown_BattleTentCorridor_Movement_WalkToDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

FallarborTown_BattleTentCorridor_Movement_PlayerEnterDoor:
	walk_up
FallarborTown_BattleTentCorridor_Movement_AttendantEnterDoor:
	walk_up
	set_invisible
	step_end

@ Leftover text from when this was a Contest Hall in R/S
@ Unused
FallarborTown_ContestHall_Text_DoAllRightInPreliminary:
	.string "We always clear the preliminary round,\n"
	.string "but the appeals never go our way…\p"
	.string "Maybe I need to study what the other\n"
	.string "contestants are doing. Even a rival can\l"
	.string "teach you something…$"

@ Unused
FallarborTown_ContestHall_Text_MonAllTheseRibbons:
	.string "Check these out! My POKéMON won every\n"
	.string "one of these RIBBONS!\p"
	.string "Has your POKéMON earned any? You can\n"
	.string "look them over on your POKéNAV.$"

@ Unused
FallarborTown_ContestHall_Text_CantWinEverywhere:
	.string "I can't beat a single GYM LEADER…\p"
	.string "I can't win a CONTEST, either…\p"
	.string "I've trained on mountains, in deserts,\n"
	.string "under waterfalls, and it's all for\l"
	.string "nothing…$"

@ Unused
FallarborTown_ContestHall_Text_SuperRankStage:
	.string "POKéMON CONTESTS\n"
	.string "SUPER RANK STAGE - SHOW YOUR STYLE!$"

