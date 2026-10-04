Route114_FossilManiacsHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route114_FossilManiacsHouse_OnTransition
	.byte 0

Route114_FossilManiacsHouse_OnTransition:
	setflag FLAG_LANDMARK_FOSSIL_MANIACS_HOUSE
	end

Route114_FossilManiacsHouse_EventScript_FossilManiacsBrother::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_DIG, Route114_FossilManiacsHouse_EventScript_ReceivedDig
	msgbox Route114_FossilManiacsHouse_Text_HaveThisToDigLikeMyBrother, MSGBOX_DEFAULT
	giveitem ITEM_TM_DIG
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_DIG
	release
	end

Route114_FossilManiacsHouse_EventScript_ReceivedDig::
	msgbox Route114_FossilManiacsHouse_Text_DigReturnsYouToEntrance, MSGBOX_DEFAULT
	release
	end

Route114_FossilManiacsHouse_EventScript_RockDisplay::
	msgbox Route114_FossilManiacsHouse_Text_RocksFillDisplayCase, MSGBOX_SIGN
	end

Route114_FossilManiacsHouse_EventScript_Bookshelf::
	msgbox Route114_FossilManiacsHouse_Text_CrammedWithBooks, MSGBOX_SIGN
	end

Route114_FossilManiacsHouse_Text_HaveThisToDigLikeMyBrother:
	.string "My big brother is the FOSSIL MANIAC…\n"
	.string "He's a really nice guy who loves\l"
	.string "FOSSILS…\p"
	.string "He loves digging holes, too…\n"
	.string "He dug this one all by himself…\p"
	.string "Take this, so you can DIG holes just\n"
	.string "like my big brother…$"

Route114_FossilManiacsHouse_Text_DigReturnsYouToEntrance:
	.string "If you use DIG inside a cave, you'll pop\n"
	.string "back out at the entrance…$"

Route114_FossilManiacsHouse_Text_RocksFillDisplayCase:
	.string "Oddly shaped rocks fill the display\n"
	.string "case…$"

Route114_FossilManiacsHouse_Text_CrammedWithBooks:
	.string "THE MAKEUP OF THE EARTH'S LAYERS…\n"
	.string "HOW RAIN CARVES THE LAND…\l"
	.string "STONES, SOIL, AND ROCK…\p"
	.string "The shelves are crammed with books.$"

