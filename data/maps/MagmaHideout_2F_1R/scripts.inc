MagmaHideout_2F_1R_MapScripts::
	.byte 0

MagmaHideout_2F_1R_EventScript_Grunt14::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_14, MagmaHideout_2F_1R_Text_Grunt14Intro, MagmaHideout_2F_1R_Text_Grunt14Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt14PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_3, MagmaHideout_2F_1R_Text_Grunt3Intro, MagmaHideout_2F_1R_Text_Grunt3Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_4, MagmaHideout_2F_1R_Text_Grunt4Intro, MagmaHideout_2F_1R_Text_Grunt4Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_EventScript_Grunt5::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_5, MagmaHideout_2F_1R_Text_Grunt5Intro, MagmaHideout_2F_1R_Text_Grunt5Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_Text_Grunt14Intro:
	.string "What, what, what?\p"
	.string "Only RED RIBBON ARMY soldiers are\n"
	.string "allowed in here!\p"
	.string "But you're not wearing the RED RIBBON\n"
	.string "uniform…\p"
	.string "Suspicious! Battle me!$"

MagmaHideout_2F_1R_Text_Grunt14Defeat:
	.string "Aiyiyi… My pride as a RED RIBBON\n"
	.string "soldier…$"

MagmaHideout_2F_1R_Text_Grunt14PostBattle:
	.string "If you can't stand the cold, you should\n"
	.string "join RED RIBBON ARMY. The base is nice\l"
	.string "and toasty.$"

MagmaHideout_2F_1R_Text_Grunt3Intro:
	.string "Halt right there!\p"
	.string "You don't seriously expect me to keep\n"
	.string "my mouth shut and let you waltz right\l"
	.string "past me?$"

MagmaHideout_2F_1R_Text_Grunt3Defeat:
	.string "Ooh wow! I give up!$"

MagmaHideout_2F_1R_Text_Grunt3PostBattle:
	.string "Maybe I should have just let you\n"
	.string "through without a fight…$"

MagmaHideout_2F_1R_Text_Grunt4Intro:
	.string "Aha! An intruder!$"

MagmaHideout_2F_1R_Text_Grunt4Defeat:
	.string "Graaah!$"

MagmaHideout_2F_1R_Text_Grunt4PostBattle:
	.string "I already lost.\p"
	.string "Do I really still have to keep marching\n"
	.string "around in a circle?$"

MagmaHideout_2F_1R_Text_Grunt5Intro:
	.string "Oh, oh! An intruder!$"

MagmaHideout_2F_1R_Text_Grunt5Defeat:
	.string "Mutter, mutter…$"

MagmaHideout_2F_1R_Text_Grunt5PostBattle:
	.string "Honestly, I'm no good at battling…$"

