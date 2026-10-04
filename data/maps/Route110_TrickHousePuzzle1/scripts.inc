Route110_TrickHousePuzzle1_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Route110_TrickHousePuzzle1_OnLoad
	.byte 0

Route110_TrickHousePuzzle1_OnLoad:
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_1_STATE, 2, Route110_TrickHousePuzzle1_EventScript_OpenDoor
	end

Route110_TrickHousePuzzle1_EventScript_OpenDoor::
	setmetatile 13, 1, METATILE_TrickHousePuzzle_Stairs_Down, FALSE
	end

Route110_TrickHousePuzzle1_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_1_STATE, 0, Route110_TrickHousePuzzle1_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle1_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_1_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle1_EventScript_Sally::
	trainerbattle_single TRAINER_SALLY, Route110_TrickHousePuzzle1_Text_SallyIntro, Route110_TrickHousePuzzle1_Text_SallyDefeat
	msgbox Route110_TrickHousePuzzle1_Text_SallyPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_EventScript_Eddie::
	trainerbattle_single TRAINER_EDDIE, Route110_TrickHousePuzzle1_Text_EddieIntro, Route110_TrickHousePuzzle1_Text_EddieDefeat
	msgbox Route110_TrickHousePuzzle1_Text_EddiePostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_EventScript_Robin::
	trainerbattle_single TRAINER_ROBIN, Route110_TrickHousePuzzle1_Text_RobinIntro, Route110_TrickHousePuzzle1_Text_RobinDefeat
	msgbox Route110_TrickHousePuzzle1_Text_RobinPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_Text_WroteSecretCodeLockOpened::
	.string "{PLAYER} wrote the secret code on the\n"
	.string "door.\p"
	.string "“TRICK MASTER is fabulous.”\n"
	.string "… … … … … … … …\p"
	.string "The lock clicked open!$"

Route110_TrickHousePuzzle1_Text_SallyIntro:
	.string "I'll chop my way to victory with the CUT\n"
	.string "we just learned!$"

Route110_TrickHousePuzzle1_Text_SallyDefeat:
	.string "Why so serious?\n"
	.string "I was just having fun!$"

Route110_TrickHousePuzzle1_Text_SallyPostBattle:
	.string "I never get tired of chopping things\n"
	.string "up! Hack, slash, repeat!$"

Route110_TrickHousePuzzle1_Text_EddieIntro:
	.string "I stumbled into this weird house by\n"
	.string "accident…$"

Route110_TrickHousePuzzle1_Text_EddieDefeat:
	.string "And now I've lost…$"

Route110_TrickHousePuzzle1_Text_EddiePostBattle:
	.string "I lost my way, I lost a battle, and now\n"
	.string "I'm even more lost… I can't find the\l"
	.string "exit…$"

Route110_TrickHousePuzzle1_Text_RobinIntro:
	.string "Just who is the TRICK MASTER?$"

Route110_TrickHousePuzzle1_Text_RobinDefeat:
	.string "I lost because I was lost in thought!$"

Route110_TrickHousePuzzle1_Text_RobinPostBattle:
	.string "Whoa, you're strong! Are you secretly a\n"
	.string "World Tournament champion?$"

