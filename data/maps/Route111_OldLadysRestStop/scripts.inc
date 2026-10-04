Route111_OldLadysRestStop_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route111_OldLadysRestStop_OnTransition
	.byte 0

Route111_OldLadysRestStop_OnTransition:
	setflag FLAG_LANDMARK_OLD_LADY_REST_SHOP
	end

Route111_OldLadysRestStop_EventScript_OldLady::
	lock
	faceplayer
	msgbox Route111_OldLadysRestStop_Text_RestUpHere, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_OldLadysRestStop_EventScript_Rest
	goto_if_eq VAR_RESULT, NO, Route111_OldLadysRestStop_EventScript_DeclineRest
	end

Route111_OldLadysRestStop_EventScript_Rest::
	msgbox Route111_OldLadysRestStop_Text_TakeYourTimeRestUp, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox Route111_OldLadysRestStop_Text_StillTiredTakeAnotherRest, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_OldLadysRestStop_EventScript_Rest
	goto_if_eq VAR_RESULT, NO, Route111_OldLadysRestStop_EventScript_DeclineRest
	end

Route111_OldLadysRestStop_EventScript_DeclineRest::
	msgbox Route111_OldLadysRestStop_Text_DontNeedToBeShy, MSGBOX_DEFAULT
	release
	end

Route111_OldLadysRestStop_Text_RestUpHere:
	.string "Oh my, dear, dear.\n"
	.string "Your POKéMON look worn out.\p"
	.string "Won't you rest up here a while?\n"
	.string "Resting is part of training, you know.$"

Route111_OldLadysRestStop_Text_TakeYourTimeRestUp:
	.string "There now.\n"
	.string "Take your time and rest up!$"

Route111_OldLadysRestStop_Text_StillTiredTakeAnotherRest:
	.string "Oh my, dear, dear.\n"
	.string "Are your POKéMON still tired?\p"
	.string "Why not rest here a little longer?\n"
	.string "It's a lovely idea, don't you think?$"

Route111_OldLadysRestStop_Text_DontNeedToBeShy:
	.string "Is that so?\n"
	.string "Don't be shy about it, now.$"

