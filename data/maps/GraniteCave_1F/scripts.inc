GraniteCave_1F_MapScripts::
	.byte 0

GraniteCave_1F_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_FLASH, GraniteCave_1F_EventScript_ReceivedFlash
	msgbox GraniteCave_1F_Text_GetsDarkAheadHereYouGo, MSGBOX_DEFAULT
	giveitem ITEM_HM_FLASH
	setflag FLAG_RECEIVED_HM_FLASH
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_EventScript_ReceivedFlash::
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_Text_GetsDarkAheadHereYouGo:
	.string "Hey, you.\n"
	.string "It gets awfully dark ahead.\l"
	.string "Exploring in there will be tough.\p"
	.string "A calm fellow went through earlier…\n"
	.string "SUPREME KAI, he called himself.\p"
	.string "He knew how to use FLASH, so he ought\n"
	.string "to be fine, but…\p"
	.string "We HIKERS have a motto: help out\n"
	.string "whoever you meet on the trail.\p"
	.string "Here, I'll pass this on to you.$"

GraniteCave_1F_Text_ExplainFlash:
	.string "Teach the hidden move FLASH to a\n"
	.string "POKéMON and use it.\p"
	.string "It lights up even the pitch-black\n"
	.string "darkness of a cave.\p"
	.string "But to use it, you need the BADGE from\n"
	.string "DEWFORD's POKéMON GYM.$"

