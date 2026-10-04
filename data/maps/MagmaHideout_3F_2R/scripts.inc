MagmaHideout_3F_2R_MapScripts::
	.byte 0

MagmaHideout_3F_2R_EventScript_Grunt10::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_10, MagmaHideout_3F_2R_Text_Grunt10Intro, MagmaHideout_3F_2R_Text_Grunt10Defeat
	msgbox MagmaHideout_3F_2R_Text_Grunt10PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_2R_Text_Grunt10Intro:
	.string "I understand everything KID BUU says.\n"
	.string "But you know what?\p"
	.string "Digging up a super-ancient POKéMON and\n"
	.string "swiping someone's METEORITE…\p"
	.string "I think we're going a little too far.\n"
	.string "What do you think?$"

MagmaHideout_3F_2R_Text_Grunt10Defeat:
	.string "Yeah… I think we're doing something\n"
	.string "wrong somehow.$"

MagmaHideout_3F_2R_Text_Grunt10PostBattle:
	.string "You know, losing to you cleared my head.\p"
	.string "Next time I see KID BUU, I'm going to\n"
	.string "ask what we're really doing here. If he\l"
	.string "doesn't turn me into candy first…$"

