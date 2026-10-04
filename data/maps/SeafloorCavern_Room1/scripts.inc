SeafloorCavern_Room1_MapScripts::
	.byte 0

SeafloorCavern_Room1_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_1, SeafloorCavern_Room1_Text_Grunt1Intro, SeafloorCavern_Room1_Text_Grunt1Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_2, SeafloorCavern_Room1_Text_Grunt2Intro, SeafloorCavern_Room1_Text_Grunt2Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_Text_Grunt1Intro:
	.string "We don't need kids around here!\n"
	.string "Go home already!$"

SeafloorCavern_Room1_Text_Grunt1Defeat:
	.string "I want to go home…$"

SeafloorCavern_Room1_Text_Grunt1PostBattle:
	.string "I want a promotion so I can boss the\n"
	.string "other soldiers around…$"

SeafloorCavern_Room1_Text_Grunt2Intro:
	.string "That submarine… It's so cramped inside.\n"
	.string "I'm sore all over!$"

SeafloorCavern_Room1_Text_Grunt2Defeat:
	.string "Losing makes me sore, too!$"

SeafloorCavern_Room1_Text_Grunt2PostBattle:
	.string "That submarine we swiped, man, what a\n"
	.string "brutal ride. It's way too tight in there!$"

