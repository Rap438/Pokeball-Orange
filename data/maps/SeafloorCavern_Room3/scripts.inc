SeafloorCavern_Room3_MapScripts::
	.byte 0

SeafloorCavern_Room3_EventScript_Shelly::
	trainerbattle_single TRAINER_SHELLY_SEAFLOOR_CAVERN, SeafloorCavern_Room3_Text_ShellyIntro, SeafloorCavern_Room3_Text_ShellyDefeat
	msgbox SeafloorCavern_Room3_Text_ShellyPostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room3_EventScript_Grunt5::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_5, SeafloorCavern_Room3_Text_Grunt5Intro, SeafloorCavern_Room3_Text_Grunt5Defeat
	msgbox SeafloorCavern_Room3_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room3_Text_ShellyIntro:
	.string "Ahahahaha!\p"
	.string "How did you get here without a\n"
	.string "submarine? What an impressive brute!\p"
	.string "But… we can't have you meddling around\n"
	.string "here.\p"
	.string "And I do want payback for what\n"
	.string "happened at the WEATHER INSTITUTE…\p"
	.string "I'll give you a little taste of pain!\n"
	.string "Resign yourself to it!$"

SeafloorCavern_Room3_Text_ShellyDefeat:
	.string "Ahahahaha!\p"
	.string "Ouch!$"

SeafloorCavern_Room3_Text_ShellyPostBattle:
	.string "Ahahahaha!\n"
	.string "You're so darn strong.\p"
	.string "It's terribly disappointing that you're\n"
	.string "not one of us in RED RIBBON ARMY.\p"
	.string "You could have enjoyed the fabulous\n"
	.string "world DR. GERO has promised us…$"

SeafloorCavern_Room3_Text_Grunt5Intro:
	.string "For our dream to come true, we need the\n"
	.string "power of POKéMON.\p"
	.string "But meddlers like you use that same\n"
	.string "power to mess with us, even in a place\l"
	.string "like this!\p"
	.string "Things never go the way we need them\n"
	.string "to!$"

SeafloorCavern_Room3_Text_Grunt5Defeat:
	.string "Gwah!$"

SeafloorCavern_Room3_Text_Grunt5PostBattle:
	.string "You know, we don't dare question the\n"
	.string "motives of DR. GERO.\p"
	.string "But here you are, just some punk, going\n"
	.string "up against the DOCTOR himself.\p"
	.string "Maybe…\n"
	.string "You really are something…$"

