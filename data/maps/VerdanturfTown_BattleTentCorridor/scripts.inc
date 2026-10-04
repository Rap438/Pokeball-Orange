VerdanturfTown_BattleTentCorridor_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VerdanturfTown_BattleTentCorridor_OnFrame
	.byte 0

VerdanturfTown_BattleTentCorridor_OnFrame:
	map_script_2 VAR_TEMP_0, 0, VerdanturfTown_BattleTentCorridor_EventScript_EnterCorridor
	.2byte 0

VerdanturfTown_BattleTentCorridor_EventScript_EnterCorridor::
	lockall
	setvar VAR_TEMP_0, 1
	applymovement LOCALID_VERDANTURF_TENT_CORRIDOR_ATTENDANT, VerdanturfTown_BattleTentCorridor_Movement_WalkToDoor
	applymovement LOCALID_PLAYER, VerdanturfTown_BattleTentCorridor_Movement_WalkToDoor
	waitmovement 0
	opendoor 2, 1
	waitdooranim
	applymovement LOCALID_VERDANTURF_TENT_CORRIDOR_ATTENDANT, VerdanturfTown_BattleTentCorridor_Movement_AttendantEnterDoor
	applymovement LOCALID_PLAYER, VerdanturfTown_BattleTentCorridor_Movement_PlayerEnterDoor
	waitmovement 0
	closedoor 2, 1
	waitdooranim
	setvar VAR_0x8006, 0
	warp MAP_VERDANTURF_TOWN_BATTLE_TENT_BATTLE_ROOM, 6, 5
	waitstate
	releaseall
	end

VerdanturfTown_BattleTentCorridor_Movement_WalkToDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VerdanturfTown_BattleTentCorridor_Movement_PlayerEnterDoor:
	walk_up
VerdanturfTown_BattleTentCorridor_Movement_AttendantEnterDoor:
	walk_up
	set_invisible
	step_end


@ Leftover text from when this was a Contest Hall in R/S
@ Unused
VerdanturfTown_ContestHall_Text_WhichContestYouEntering:
	.string "Which CONTEST are you entering?\n"
	.string "Want some advice?\p"
	.string "Take a CUTE CONTEST, for example. I\n"
	.string "don't think they judge you on cuteness\l"
	.string "alone in the first round.\p"
	.string "You need to find better ways to raise\n"
	.string "your POKéMON.$"

@ Unused
VerdanturfTown_ContestHall_Text_RaisedMonToBeCute:
	.string "I raised my POKéMON to be cute.\p"
	.string "Then I found out there's a CONTEST just\n"
	.string "for cuteness!\p"
	.string "I'm so glad I raised my POKéMON with\n"
	.string "loving care…$"

@ Unused
VerdanturfTown_ContestHall_Text_MyMonRules:
	.string "My POKéMON rules!\p"
	.string "It's cool, tough yet beautiful, cute,\n"
	.string "and smart. It's complete!\p"
	.string "I might as well go for the win in every\n"
	.string "single CONTEST.$"

@ Unused
VerdanturfTown_ContestHall_Text_NormalRankStage:
	.string "POKéMON CONTESTS\n"
	.string "NORMAL RANK STAGE - SHOW YOUR STYLE!$"

