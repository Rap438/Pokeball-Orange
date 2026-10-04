Route110_TrickHousePuzzle4_MapScripts::
	.byte 0

Route110_TrickHousePuzzle4_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_4_STATE, 0, Route110_TrickHousePuzzle4_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle4_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_4_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle4_EventScript_Cora::
	trainerbattle_single TRAINER_CORA, Route110_TrickHousePuzzle4_Text_CoraIntro, Route110_TrickHousePuzzle4_Text_CoraDefeat
	msgbox Route110_TrickHousePuzzle4_Text_CoraPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_EventScript_Yuji::
	trainerbattle_single TRAINER_YUJI, Route110_TrickHousePuzzle4_Text_YujiIntro, Route110_TrickHousePuzzle4_Text_YujiDefeat
	msgbox Route110_TrickHousePuzzle4_Text_YujiPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_EventScript_Paula::
	trainerbattle_single TRAINER_PAULA, Route110_TrickHousePuzzle4_Text_PaulaIntro, Route110_TrickHousePuzzle4_Text_PaulaDefeat
	msgbox Route110_TrickHousePuzzle4_Text_PaulaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_Text_WroteSecretCodeLockOpened:
	.string "{PLAYER} wrote the secret code on the\n"
	.string "door.\p"
	.string "“TRICK MASTER is cool.”\n"
	.string "… … … … … … … …\p"
	.string "The lock clicked open!$"

Route110_TrickHousePuzzle4_Text_CoraIntro:
	.string "Thinking this all through is too much\n"
	.string "bother. I only wanted to battle!$"

Route110_TrickHousePuzzle4_Text_CoraDefeat:
	.string "Even though I lost, battling is still my\n"
	.string "favorite thing!$"

Route110_TrickHousePuzzle4_Text_CoraPostBattle:
	.string "Wouldn't you agree? A real fighter goes\n"
	.string "anywhere there are TRAINERS to battle.$"

Route110_TrickHousePuzzle4_Text_YujiIntro:
	.string "Heh! Boulders like these, I can flick\n"
	.string "aside with one finger!$"

Route110_TrickHousePuzzle4_Text_YujiDefeat:
	.string "I can push boulders, but I can't solve\n"
	.string "the puzzle…$"

Route110_TrickHousePuzzle4_Text_YujiPostBattle:
	.string "Being brawny isn't enough… You have to\n"
	.string "use your head, too. Be brainy!$"

Route110_TrickHousePuzzle4_Text_PaulaIntro:
	.string "The TRICK HOUSE keeps getting trickier,\n"
	.string "doesn't it?$"

Route110_TrickHousePuzzle4_Text_PaulaDefeat:
	.string "Aaak!$"

Route110_TrickHousePuzzle4_Text_PaulaPostBattle:
	.string "Has anyone actually made it to the end?$"

