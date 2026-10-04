Route110_TrickHousePuzzle6_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route110_TrickHousePuzzle6_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route110_TrickHousePuzzle6_OnWarp
	.byte 0

Route110_TrickHousePuzzle6_OnTransition:
	special RotatingGate_InitPuzzle
	end

Route110_TrickHousePuzzle6_OnWarp:
	map_script_2 VAR_TEMP_0, VAR_TEMP_0, Route110_TrickHousePuzzle6_EventScript_InitPuzzle
	.2byte 0

@ NOTE: This rotating gate puzzle makes use of VAR_TEMP_0 - VAR_TEMP_5
Route110_TrickHousePuzzle6_EventScript_InitPuzzle::
	special RotatingGate_InitPuzzleAndGraphics
	end

Route110_TrickHousePuzzle6_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_6_STATE, 0, Route110_TrickHousePuzzle6_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle6_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_6_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle6_EventScript_Sophia::
	trainerbattle_single TRAINER_SOPHIA, Route110_TrickHousePuzzle6_Text_SophiaIntro, Route110_TrickHousePuzzle6_Text_SophiaDefeat
	msgbox Route110_TrickHousePuzzle6_Text_SophiaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle6_EventScript_Benny::
	trainerbattle_single TRAINER_BENNY, Route110_TrickHousePuzzle6_Text_BennyIntro, Route110_TrickHousePuzzle6_Text_BennyDefeat
	msgbox Route110_TrickHousePuzzle6_Text_BennyPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle6_EventScript_Sebastian::
	trainerbattle_single TRAINER_SEBASTIAN, Route110_TrickHousePuzzle6_Text_SebastianIntro, Route110_TrickHousePuzzle6_Text_SebastianDefeat
	msgbox Route110_TrickHousePuzzle6_Text_SebastianPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle6_Text_WroteSecretCodeLockOpened:
	.string "{PLAYER} wrote the secret code on the\n"
	.string "door.\p"
	.string "“TRICK MASTER is my life.”\n"
	.string "… … … … … … … …\p"
	.string "The lock clicked open!$"

Route110_TrickHousePuzzle6_Text_SophiaIntro:
	.string "When I heard about a strange house, I\n"
	.string "had to check it out.$"

Route110_TrickHousePuzzle6_Text_SophiaDefeat:
	.string "I've discovered a tough TRAINER!$"

Route110_TrickHousePuzzle6_Text_SophiaPostBattle:
	.string "I'm having a great time exploring this\n"
	.string "place.\p"
	.string "It's a challenge worth repeating!$"

Route110_TrickHousePuzzle6_Text_BennyIntro:
	.string "Maybe I could get my BIRD POKéMON to\n"
	.string "fly over the wall…$"

Route110_TrickHousePuzzle6_Text_BennyDefeat:
	.string "Gwaaah! I blew it!$"

Route110_TrickHousePuzzle6_Text_BennyPostBattle:
	.string "Ehehehe… I guess I lost because I was\n"
	.string "trying to cheat.$"

Route110_TrickHousePuzzle6_Text_SebastianIntro:
	.string "These rotating doors are making me\n"
	.string "dizzy…$"

Route110_TrickHousePuzzle6_Text_SebastianDefeat:
	.string "Everything's spinning around and\n"
	.string "around. I can't take it anymore…$"

Route110_TrickHousePuzzle6_Text_SebastianPostBattle:
	.string "You don't seem affected at all.\n"
	.string "Or is that just your poker face?$"

