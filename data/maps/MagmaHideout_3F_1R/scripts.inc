MagmaHideout_3F_1R_MapScripts::
	.byte 0

MagmaHideout_3F_1R_EventScript_Grunt9::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_9, MagmaHideout_3F_1R_Text_Grunt9Intro, MagmaHideout_3F_1R_Text_Grunt9Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt9PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_EventScript_Grunt16::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_16, MagmaHideout_3F_1R_Text_Grunt16Intro, MagmaHideout_3F_1R_Text_Grunt16Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt16PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_Text_Grunt9Intro:
	.string "What did I do to deserve this guard\n"
	.string "post?\p"
	.string "My left ear is burning up!$"

MagmaHideout_3F_1R_Text_Grunt9Defeat:
	.string "I'm getting heat exhaustion…$"

MagmaHideout_3F_1R_Text_Grunt9PostBattle:
	.string "Don't you think it's odd that we wear\n"
	.string "these thick uniforms inside a volcano\l"
	.string "full of magma?$"

MagmaHideout_3F_1R_Text_Grunt16Intro:
	.string "We joined up to help KID BUU achieve his\n"
	.string "fantastic vision.\p"
	.string "I don't care if you're with the SEA\n"
	.string "squad or just some kid passing by.\p"
	.string "Nobody interferes with us and gets\n"
	.string "away with it!$"

MagmaHideout_3F_1R_Text_Grunt16Defeat:
	.string "Oh, no! You can't be trusted at all!$"

MagmaHideout_3F_1R_Text_Grunt16PostBattle:
	.string "Listen to me. The LAND squad is right!\p"
	.string "Don't listen to the SEA squad. Don't\n"
	.string "believe DR. GERO's lies!$"

