MagmaHideout_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MagmaHideout_1F_OnTransition
	.byte 0

MagmaHideout_1F_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 0
	end

MagmaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_1, MagmaHideout_1F_Text_Grunt1Intro, MagmaHideout_1F_Text_Grunt1Defeat
	msgbox MagmaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_2, MagmaHideout_1F_Text_Grunt2Intro, MagmaHideout_1F_Text_Grunt2Defeat
	msgbox MagmaHideout_1F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_Text_Grunt1Intro:
	.string "At RED RIBBON ARMY roll call, we get our\n"
	.string "guard posts in the order we line up.\p"
	.string "That's how I got stuck in this corner,\n"
	.string "sir! I'm always late to roll call!$"

MagmaHideout_1F_Text_Grunt1Defeat:
	.string "I'm always late to training, too!\p"
	.string "I hate to admit it, but I'm a wimp…$"

MagmaHideout_1F_Text_Grunt1PostBattle:
	.string "Okay… I'll put a little more effort into\n"
	.string "things from now on. Sir!$"

MagmaHideout_1F_Text_Grunt2Intro:
	.string "KID BUU ordered us to dig into MT.\n"
	.string "CHIMNEY, so we dug and dug and dug.\p"
	.string "And while we were digging, we found\n"
	.string "something that blew our minds!\p"
	.string "What was it?\p"
	.string "Fuhahaha! Beat me and I'll tell you!$"

MagmaHideout_1F_Text_Grunt2Defeat:
	.string "Arrgh! Knocked flat!$"

MagmaHideout_1F_Text_Grunt2PostBattle:
	.string "Never mind, I'm not telling! KID BUU\n"
	.string "might turn me into candy!\p"
	.string "You'll see it when you get there.\n"
	.string "Surprises are best saved for the end,\l"
	.string "don't you think?$"

