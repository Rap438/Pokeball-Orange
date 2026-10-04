VictoryRoad_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VictoryRoad_1F_OnTransition
	.byte 0

VictoryRoad_1F_OnTransition:
	call_if_eq VAR_VICTORY_ROAD_1F_STATE, 1, VictoryRoad_1F_EventScript_SetEntranceWallyPos1
	call_if_eq VAR_VICTORY_ROAD_1F_STATE, 2, VictoryRoad_1F_EventScript_SetEntranceWallyPos2
	end

VictoryRoad_1F_EventScript_SetEntranceWallyPos1::
	setobjectxyperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, 2, 24
	setobjectmovementtype LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, MOVEMENT_TYPE_FACE_DOWN
	return

VictoryRoad_1F_EventScript_SetEntranceWallyPos2::
	setobjectxyperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, 3, 24
	setobjectmovementtype LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, MOVEMENT_TYPE_FACE_DOWN
	return

VictoryRoad_1F_EventScript_WallyBattleTrigger1::
	lockall
	setvar VAR_0x8008, 1
	addobject LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	applywaitmovement LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, VictoryRoad_1F_Movement_WallyApproachPlayer1
	goto VictoryRoad_1F_EventScript_WallyEntranceBattle
	end

VictoryRoad_1F_EventScript_WallyBattleTrigger2::
	lockall
	setvar VAR_0x8008, 2
	addobject LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	applywaitmovement LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, VictoryRoad_1F_Movement_WallyApproachPlayer2
	goto VictoryRoad_1F_EventScript_WallyEntranceBattle
	end

VictoryRoad_1F_EventScript_WallyEntranceBattle::
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	msgbox VictoryRoad_1F_Text_WallyNotGoingToLoseAnymore, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_VR_1, VictoryRoad_1F_Text_WallyEntranceDefeat
	msgbox VictoryRoad_1F_Text_WallyPostEntranceBattle, MSGBOX_DEFAULT
	clearflag FLAG_HIDE_VICTORY_ROAD_ENTRANCE_WALLY
	copyobjectxytoperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	setflag FLAG_DEFEATED_WALLY_VICTORY_ROAD
	copyvar VAR_VICTORY_ROAD_1F_STATE, VAR_0x8008
	releaseall
	end

VictoryRoad_1F_Movement_WallyApproachPlayer1:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

VictoryRoad_1F_Movement_WallyApproachPlayer2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

@ This Wally appears near the entrance once his battle is triggered and remains there until the Hall of Fame is entered
VictoryRoad_1F_EventScript_EntranceWally::
	msgbox VictoryRoad_1F_Text_WallyPostEntranceBattle, MSGBOX_NPC
	end

@ This Wally appears and remains at the exit after the Hall of Fame is entered
VictoryRoad_1F_EventScript_ExitWally::
	trainerbattle_single TRAINER_WALLY_VR_2, VictoryRoad_1F_Text_WallyIntro, VictoryRoad_1F_Text_WallyDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_1F_EventScript_RematchWally
	msgbox VictoryRoad_1F_Text_WallyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_RematchWally::
	trainerbattle_rematch TRAINER_WALLY_VR_2, VictoryRoad_1F_Text_WallyIntro, VictoryRoad_1F_Text_WallyDefeat
	msgbox VictoryRoad_1F_Text_WallyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Edgar::
	trainerbattle_single TRAINER_EDGAR, VictoryRoad_1F_Text_EdgarIntro, VictoryRoad_1F_Text_EdgarDefeat
	msgbox VictoryRoad_1F_Text_EdgarPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Albert::
	trainerbattle_single TRAINER_ALBERT, VictoryRoad_1F_Text_AlbertIntro, VictoryRoad_1F_Text_AlbertDefeat
	msgbox VictoryRoad_1F_Text_AlbertPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Hope::
	trainerbattle_single TRAINER_HOPE, VictoryRoad_1F_Text_HopeIntro, VictoryRoad_1F_Text_HopeDefeat
	msgbox VictoryRoad_1F_Text_HopePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Quincy::
	trainerbattle_single TRAINER_QUINCY, VictoryRoad_1F_Text_QuincyIntro, VictoryRoad_1F_Text_QuincyDefeat
	msgbox VictoryRoad_1F_Text_QuincyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Katelynn::
	trainerbattle_single TRAINER_KATELYNN, VictoryRoad_1F_Text_KatelynnIntro, VictoryRoad_1F_Text_KatelynnDefeat
	msgbox VictoryRoad_1F_Text_KatelynnPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_Text_WallyNotGoingToLoseAnymore:
	.string "UUB: Hi! {PLAYER}!\p"
	.string "I bet you're surprised to see me here!\p"
	.string "I made it all the way to VICTORY ROAD,\n"
	.string "and it's all thanks to you!\p"
	.string "{PLAYER}, losing to you that time made\n"
	.string "me stronger. Something deep inside me\l"
	.string "finally woke up!\p"
	.string "And I'm not going to lose anymore!\p"
	.string "I'm going to win! For the POKéMON who\n"
	.string "gave me courage and strength!\p"
	.string "Okay… Here I come!$"

VictoryRoad_1F_Text_WallyEntranceDefeat:
	.string "Whoa! {PLAYER}, you're still way stronger\n"
	.string "than me!$"

VictoryRoad_1F_Text_WallyPostEntranceBattle:
	.string "UUB: I couldn't beat you today, {PLAYER},\n"
	.string "but one of these days I'll catch up to\l"
	.string "you!$"

VictoryRoad_1F_Text_WallyIntro:
	.string "UUB: Hi! {PLAYER}!\p"
	.string "I've gotten even stronger since the\n"
	.string "last time! I wanted to show you,\l"
	.string "{PLAYER}!\p"
	.string "Okay… Here I come!$"

VictoryRoad_1F_Text_WallyDefeat:
	.string "Whoa! {PLAYER}, you're still way stronger\n"
	.string "than me!$"

VictoryRoad_1F_Text_WallyPostBattle:
	.string "UUB: I couldn't beat you this time,\n"
	.string "either… But one of these days, {PLAYER},\l"
	.string "I'm going to catch up to you…\p"
	.string "And then I'll challenge the POKéMON\n"
	.string "LEAGUE!$"

VictoryRoad_1F_Text_EdgarIntro:
	.string "I've gotten this far a couple of times,\n"
	.string "but the last stretch of this cave never\l"
	.string "ends…$"

VictoryRoad_1F_Text_EdgarDefeat:
	.string "My dream ends here again…$"

VictoryRoad_1F_Text_EdgarPostBattle:
	.string "You've got power to spare. Keep it\n"
	.string "rising all the way to CHAMPION! If\l"
	.string "anyone can, it's you!$"

VictoryRoad_1F_Text_AlbertIntro:
	.string "I didn't climb all this way just to lose\n"
	.string "now. Defeat isn't even in my vocabulary!$"

VictoryRoad_1F_Text_AlbertDefeat:
	.string "Impossible…\n"
	.string "I lost?$"

VictoryRoad_1F_Text_AlbertPostBattle:
	.string "I lost here…\p"
	.string "That means I'm not CHAMPION material\n"
	.string "yet…$"

VictoryRoad_1F_Text_HopeIntro:
	.string "This endless, brutal road earns its\n"
	.string "name. Only the strong walk out the\l"
	.string "other side.$"

VictoryRoad_1F_Text_HopeDefeat:
	.string "Your fighting style is fantastic…$"

VictoryRoad_1F_Text_HopePostBattle:
	.string "I can sense it in your ki. You have what\n"
	.string "it takes to become the CHAMPION.$"

VictoryRoad_1F_Text_QuincyIntro:
	.string "What is VICTORY ROAD, really?\n"
	.string "I'll tell you if you win!$"

VictoryRoad_1F_Text_QuincyDefeat:
	.string "Okay!\n"
	.string "Well fought!$"

VictoryRoad_1F_Text_QuincyPostBattle:
	.string "Getting through here in one piece is\n"
	.string "the final test for any TRAINER aiming\l"
	.string "to become the POKéMON CHAMPION.\p"
	.string "That's why it's called VICTORY ROAD.$"

VictoryRoad_1F_Text_KatelynnIntro:
	.string "I've got nothing to say to anyone who\n"
	.string "dares come this far. Fight me!$"

VictoryRoad_1F_Text_KatelynnDefeat:
	.string "Tch… How humiliating…$"

VictoryRoad_1F_Text_KatelynnPostBattle:
	.string "Hmph. Go right on ahead. Not like I care.$"
