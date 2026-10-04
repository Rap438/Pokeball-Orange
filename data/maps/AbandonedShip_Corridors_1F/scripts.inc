AbandonedShip_Corridors_1F_MapScripts::
	.byte 0

AbandonedShip_Corridors_1F_EventScript_Youngster::
	msgbox AbandonedShip_Corridors_1F_Text_IsntItFunHere, MSGBOX_NPC
	end

AbandonedShip_Corridors_1F_EventScript_Charlie::
	trainerbattle_single TRAINER_CHARLIE, AbandonedShip_Corridors_1F_Text_CharlieIntro, AbandonedShip_Corridors_1F_Text_CharlieDefeat
	msgbox AbandonedShip_Corridors_1F_Text_CharliePostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Corridors_1F_Text_CharlieIntro:
	.string "Hey, what's so funny about hauling my\n"
	.string "inner tube onto a ship? A fighter has to\l"
	.string "stay afloat!$"

AbandonedShip_Corridors_1F_Text_CharlieDefeat:
	.string "Whoa, your power level is off the\n"
	.string "charts!$"

AbandonedShip_Corridors_1F_Text_CharliePostBattle:
	.string "Throwing POKé BALLS while hanging onto\n"
	.string "an inner tube takes serious balance\l"
	.string "training!$"

AbandonedShip_Corridors_1F_Text_IsntItFunHere:
	.string "Isn't this place a blast?\p"
	.string "I get all fired up just standing here!$"
