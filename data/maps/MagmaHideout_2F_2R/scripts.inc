MagmaHideout_2F_2R_MapScripts::
	.byte 0

MagmaHideout_2F_2R_EventScript_Grunt15::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_15, MagmaHideout_2F_2R_Text_Grunt15Intro, MagmaHideout_2F_2R_Text_Grunt15Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt15PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt6::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_6, MagmaHideout_2F_2R_Text_Grunt6Intro, MagmaHideout_2F_2R_Text_Grunt6Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt6PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt7::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_7, MagmaHideout_2F_2R_Text_Grunt7Intro, MagmaHideout_2F_2R_Text_Grunt7Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt7PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt8::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_8, MagmaHideout_2F_2R_Text_Grunt8Intro, MagmaHideout_2F_2R_Text_Grunt8Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt8PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_Text_Grunt15Intro:
	.string "I've got nothing against you. I'm just\n"
	.string "following orders! Sir!$"

MagmaHideout_2F_2R_Text_Grunt15Defeat:
	.string "I may have lost, but…$"

MagmaHideout_2F_2R_Text_Grunt15PostBattle:
	.string "We dug up something beyond belief! And\n"
	.string "we got the BLUE ORB!\p"
	.string "Now all that's left is for KID BUU to…\n"
	.string "Fufufu… Fwahahaha!$"

MagmaHideout_2F_2R_Text_Grunt6Intro:
	.string "I can't stand the heat. Maybe I should\n"
	.string "have asked to join the SEA squad under\l"
	.string "DR. GERO instead…$"

MagmaHideout_2F_2R_Text_Grunt6Defeat:
	.string "Yeah, maybe I'm not cut out for the\n"
	.string "LAND squad…$"

MagmaHideout_2F_2R_Text_Grunt6PostBattle:
	.string "Don't you miss the sea when you're\n"
	.string "stuck in a place like this?$"

MagmaHideout_2F_2R_Text_Grunt7Intro:
	.string "Sometimes you can feel tremors in here.\p"
	.string "Is it the volcano rumbling? Or is it\n"
	.string "GROU…\p"
	.string "Whoops! No, no, forget I said anything!$"

MagmaHideout_2F_2R_Text_Grunt7Defeat:
	.string "You battle like a volcano! Fiery!$"

MagmaHideout_2F_2R_Text_Grunt7PostBattle:
	.string "You just got lucky this time. It doesn't\n"
	.string "matter.\p"
	.string "RED RIBBON ARMY's goal is about to be\n"
	.string "reached!$"

MagmaHideout_2F_2R_Text_Grunt8Intro:
	.string "One of our guys was freaking out\n"
	.string "because he lost his RR EMBLEM…\p"
	.string "Wait a minute! Was it you who found it?$"

MagmaHideout_2F_2R_Text_Grunt8Defeat:
	.string "I can hardly believe this…$"

MagmaHideout_2F_2R_Text_Grunt8PostBattle:
	.string "I've got a feeling our plan is going to\n"
	.string "fail, and KID BUU will not be happy\l"
	.string "about that…$"

