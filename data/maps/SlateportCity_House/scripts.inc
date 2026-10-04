SlateportCity_House_MapScripts::
	.byte 0

SlateportCity_House_EventScript_PokefanM::
	msgbox SlateportCity_House_Text_NatureToDoWithStatGains, MSGBOX_NPC
	end

SlateportCity_House_EventScript_Girl::
	msgbox SlateportCity_House_Text_MustBeGoingToBattleTent, MSGBOX_NPC
	end

SlateportCity_House_Text_NatureToDoWithStatGains:
	.string "My POKéMON has a HASTY nature, so its\n"
	.string "SPEED is higher than my others'.\p"
	.string "I bet a POKéMON's nature has something\n"
	.string "to do with how its stats grow.$"

SlateportCity_House_Text_MustBeGoingToBattleTent:
	.string "You're a TRAINER, aren't you? I can tell\n"
	.string "by the way you carry yourself.\p"
	.string "Since you came all the way to SLATEPORT\n"
	.string "CITY, you must be headed for the\l"
	.string "BATTLE TENT.$"

