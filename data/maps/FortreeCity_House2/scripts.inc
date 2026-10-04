FortreeCity_House2_MapScripts::
	.byte 0

FortreeCity_House2_EventScript_HiddenPowerGiver::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_HIDDEN_POWER, FortreeCity_House2_EventScript_ExplainHiddenPower
	call_if_unset FLAG_MET_HIDDEN_POWER_GIVER, FortreeCity_House2_EventScript_Greeting
	msgbox FortreeCity_House2_Text_CoinInWhichHand, MSGBOX_DEFAULT
	multichoice 21, 8, MULTI_RIGHTLEFT, TRUE
	switch VAR_RESULT
	case 1, FortreeCity_House2_EventScript_WrongGuess
	msgbox FortreeCity_House2_Text_CorrectTryAgainWhichHand, MSGBOX_DEFAULT
	multichoice 21, 8, MULTI_RIGHTLEFT, TRUE
	switch VAR_RESULT
	case 1, FortreeCity_House2_EventScript_WrongGuess
	msgbox FortreeCity_House2_Text_CorrectTryAgainWhichHand2, MSGBOX_DEFAULT
	multichoice 21, 8, MULTI_RIGHTLEFT, TRUE
	switch VAR_RESULT
	case 0, FortreeCity_House2_EventScript_WrongGuess
	msgbox FortreeCity_House2_Text_YourHiddenPowerHasAwoken, MSGBOX_DEFAULT
	giveitem ITEM_TM_HIDDEN_POWER
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_HIDDEN_POWER
	msgbox FortreeCity_House2_Text_ExplainHiddenPower, MSGBOX_DEFAULT
	release
	end

FortreeCity_House2_EventScript_Greeting::
	msgbox FortreeCity_House2_Text_HiddenPowersArousedByNature, MSGBOX_DEFAULT
	setflag FLAG_MET_HIDDEN_POWER_GIVER
	return

FortreeCity_House2_EventScript_ExplainHiddenPower::
	msgbox FortreeCity_House2_Text_ExplainHiddenPower, MSGBOX_DEFAULT
	release
	end

FortreeCity_House2_EventScript_WrongGuess::
	msgbox FortreeCity_House2_Text_YouGuessedWrong, MSGBOX_DEFAULT
	release
	end

FortreeCity_House2_Text_HiddenPowersArousedByNature:
	.string "People… POKéMON…\p"
	.string "The power sleeping inside each of us\n"
	.string "wakes up when we live close to nature…$"

FortreeCity_House2_Text_CoinInWhichHand:
	.string "Let this old woman see whether your\n"
	.string "hidden power has awoken…\p"
	.string "I hold a coin in my hand.\p"
	.string "Now tell me, have I palmed it in the\n"
	.string "right hand? Or in the left?$"

FortreeCity_House2_Text_CorrectTryAgainWhichHand:
	.string "Oh! Correct!\p"
	.string "Let us try again.\p"
	.string "In which hand is the coin hidden now?\n"
	.string "The right or the left?$"

FortreeCity_House2_Text_CorrectTryAgainWhichHand2:
	.string "Oh! Correct again!\p"
	.string "One more time, then.\p"
	.string "Which hand holds the coin?\n"
	.string "The right or the left?$"

FortreeCity_House2_Text_YourHiddenPowerHasAwoken:
	.string "Oh! Splendid! Your hidden power has\n"
	.string "awoken, just like a true fighter's ki!\p"
	.string "Here, take this and awaken the hidden\n"
	.string "power of your POKéMON, too.$"

FortreeCity_House2_Text_ExplainHiddenPower:
	.string "HIDDEN POWER is a move that changes\n"
	.string "with each POKéMON.\p"
	.string "No two hidden powers are quite alike.$"

FortreeCity_House2_Text_YouGuessedWrong:
	.string "Ohh, too bad.\n"
	.string "Wrong hand, young one.$"

