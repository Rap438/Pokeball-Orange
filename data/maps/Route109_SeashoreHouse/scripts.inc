Route109_SeashoreHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route109_SeashoreHouse_OnTransition
	.byte 0

Route109_SeashoreHouse_OnTransition:
	setflag FLAG_LANDMARK_SEASHORE_HOUSE
	end

Route109_SeashoreHouse_EventScript_Owner::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_6_SODA_POP, Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop
	goto_if_set FLAG_DEFEATED_SEASHORE_HOUSE, Route109_SeashoreHouse_EventScript_DefeatedTrainers
	goto_if_set FLAG_TEMP_2, Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction
	msgbox Route109_SeashoreHouse_Text_SeashoreHouseIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_2
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction::
	msgbox Route109_SeashoreHouse_Text_ShowMeSomeHotMatches, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_DefeatedTrainers::
	msgbox Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles, MSGBOX_DEFAULT
	giveitem ITEM_SODA_POP, 6
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_BagFull
	setflag FLAG_RECEIVED_6_SODA_POP
	release
	end

Route109_SeashoreHouse_EventScript_BagFull::
	msgbox Route109_SeashoreHouse_Text_BagFull, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop::
	showmoneybox 0, 0
	msgbox Route109_SeashoreHouse_Text_WantToBuySodaPop, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route109_SeashoreHouse_EventScript_BuySodaPop
	msgbox Route109_SeashoreHouse_Text_ThatsTooBad, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_BuySodaPop::
	checkmoney 300
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughMoney
	checkitemspace ITEM_SODA_POP
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughSpace
	msgbox Route109_SeashoreHouse_Text_HereYouGo, MSGBOX_DEFAULT
	removemoney 300
	updatemoneybox
.if OW_SHOW_ITEM_DESCRIPTIONS != OW_ITEM_DESCRIPTIONS_OFF
	hidemoneybox
	giveitem ITEM_SODA_POP
.else
	giveitem ITEM_SODA_POP
	hidemoneybox
.endif
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughMoney::
	msgbox Route109_SeashoreHouse_Text_NotEnoughMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughSpace::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_Dwayne::
	trainerbattle_single TRAINER_DWAYNE, Route109_SeashoreHouse_Text_DwayneIntro, Route109_SeashoreHouse_Text_DwayneDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_DwaynePostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Johanna::
	trainerbattle_single TRAINER_JOHANNA, Route109_SeashoreHouse_Text_JohannaIntro, Route109_SeashoreHouse_Text_JohannaDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_JohannaPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Simon::
	trainerbattle_single TRAINER_SIMON, Route109_SeashoreHouse_Text_SimonIntro, Route109_SeashoreHouse_Text_SimonDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_SimonPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_CheckTrainersCompletion::
	goto_if_not_defeated TRAINER_DWAYNE, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_JOHANNA, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_SIMON, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	setflag FLAG_DEFEATED_SEASHORE_HOUSE
	release
	end

Route109_SeashoreHouse_EventScript_TrainersNotCompleted::
	release
	end

Route109_SeashoreHouse_Text_SeashoreHouseIntro:
	.string "I'm the owner of the SEASHORE HOUSE.\n"
	.string "But you can call me MR. SEA!\p"
	.string "What I love most is a fiery POKéMON\n"
	.string "battle.\p"
	.string "Let me see your ki burn hot!\p"
	.string "Defeat every TRAINER here and I'll\n"
	.string "reward your efforts.$"

Route109_SeashoreHouse_Text_ShowMeSomeHotMatches:
	.string "Show me some hot matches!\p"
	.string "That's the whole reason I run this\n"
	.string "SEASHORE HOUSE!$"

Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles:
	.string "You're scorching!\n"
	.string "Those battles blazed!\l"
	.string "I couldn't be more satisfied!\p"
	.string "As thanks for the blazing show, I want\n"
	.string "you to take these.\p"
	.string "Half a dozen bottles of SODA POP!$"

Route109_SeashoreHouse_Text_BagFull:
	.string "Oh, but your BAG is stuffed full.\n"
	.string "I'll hang on to these for you.$"

Route109_SeashoreHouse_Text_WantToBuySodaPop:
	.string "Want some SODA POP?\n"
	.string "POKéMON love it!\p"
	.string "Just ¥300 a bottle!\n"
	.string "Go on, buy some!$"

Route109_SeashoreHouse_Text_HereYouGo:
	.string "Here you go! Drink up and recharge!$"

Route109_SeashoreHouse_Text_NotEnoughMoney:
	.string "Looks like you're short on zeni.$"

Route109_SeashoreHouse_Text_ThatsTooBad:
	.string "No?\n"
	.string "That's a shame.$"

Route109_SeashoreHouse_Text_DwayneIntro:
	.string "If you want a battle in the SEASHORE\n"
	.string "HOUSE, you won't find a hotter fighter\l"
	.string "than me, matey!$"

Route109_SeashoreHouse_Text_DwayneDefeated:
	.string "Now that was a hot fight!\n"
	.string "I can take that loss, matey!$"

Route109_SeashoreHouse_Text_DwaynePostBattle:
	.string "Whenever I hit SLATEPORT, I look for\n"
	.string "a hot battle and an ice-cold SODA POP!$"

Route109_SeashoreHouse_Text_JohannaIntro:
	.string "Dull battles aren't worth the trouble.\p"
	.string "Fiery fights are what toughen up\n"
	.string "TRAINERS and POKéMON!$"

Route109_SeashoreHouse_Text_JohannaDefeated:
	.string "Hot stuff! My power level's dropping!$"

Route109_SeashoreHouse_Text_JohannaPostBattle:
	.string "Phew, I'm parched.\n"
	.string "Maybe I'll have a SODA POP.$"

Route109_SeashoreHouse_Text_SimonIntro:
	.string "I'll show you how great my POKéMON\n"
	.string "are, but don't cry!$"

Route109_SeashoreHouse_Text_SimonDefeated:
	.string "…I lost, but a fighter doesn't cry…$"

Route109_SeashoreHouse_Text_SimonPostBattle:
	.string "If one of my POKéMON knew the move\n"
	.string "that carries me across water on its\l"
	.string "back, I could ditch this inner tube.$"

