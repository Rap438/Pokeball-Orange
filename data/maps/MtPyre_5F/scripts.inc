MtPyre_5F_MapScripts::
	.byte 0

@ Seems like the scripts for the 4F and 5F were swapped

MtPyre_4F_EventScript_Tasha::
	trainerbattle_single TRAINER_TASHA, MtPyre_4F_Text_TashaIntro, MtPyre_4F_Text_TashaDefeat
	msgbox MtPyre_4F_Text_TashaPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_4F_Text_TashaIntro:
	.string "I adore anything that makes the blood\n"
	.string "run cold… I can't help it…\p"
	.string "When I'm up here…\n"
	.string "I shiver with delight…$"

MtPyre_4F_Text_TashaDefeat:
	.string "Losing, I dislike…$"

MtPyre_4F_Text_TashaPostBattle:
	.string "I want to see dreadful things…\n"
	.string "I can't leave…\p"
	.string "Stay…\n"
	.string "Won't you stay with me?$"

