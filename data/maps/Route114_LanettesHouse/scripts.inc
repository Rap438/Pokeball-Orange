Route114_LanettesHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route114_LanettesHouse_OnTransition
	.byte 0

Route114_LanettesHouse_OnTransition:
	setflag FLAG_LANDMARK_LANETTES_HOUSE
	end

Route114_LanettesHouse_EventScript_Lanette::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_DOLL_LANETTE, Route114_LanettesHouse_EventScript_OfferAdvice
	setflag FLAG_SYS_PC_LANETTE
	msgbox Route114_LanettesHouse_Text_EverythingClutteredKeepThis, MSGBOX_DEFAULT
	givedecoration DECOR_LOTAD_DOLL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowNoRoomForDecor
	setflag FLAG_RECEIVED_DOLL_LANETTE
	release
	end

Route114_LanettesHouse_EventScript_OfferAdvice::
	msgbox Route114_LanettesHouse_Text_OrganizeYourBoxes, MSGBOX_DEFAULT
	release
	end

Route114_LanettesHouse_EventScript_Notebook::
	lockall
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage1, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route114_LanettesHouse_EventScript_NotebookPage2
	msgbox Route114_LanettesHouse_Text_ClosedTheNotebook, MSGBOX_DEFAULT
	releaseall
	end

Route114_LanettesHouse_EventScript_NotebookPage2::
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage2, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Route114_LanettesHouse_EventScript_NotebookPage3
	releaseall
	end

Route114_LanettesHouse_EventScript_NotebookPage3::
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage3, MSGBOX_DEFAULT
	return

Route114_LanettesHouse_EventScript_PC::
	msgbox Route114_LanettesHouse_Text_EmailFromBill, MSGBOX_SIGN
	end

Route114_LanettesHouse_Text_EverythingClutteredKeepThis:
	.string "LANETTE: Oh! {PLAYER}{KUN}!\p"
	.string "Sorry about the mess… When I get\n"
	.string "absorbed in research, this is what\l"
	.string "happens…\p"
	.string "How embarrassing… Please keep this our\n"
	.string "secret, and take this in return.$"

Route114_LanettesHouse_Text_OrganizeYourBoxes:
	.string "Can I give you some advice about my\n"
	.string "POKéMON Storage System?\p"
	.string "Sort your BOXES so you always know\n"
	.string "which POKéMON is in which one.$"

Route114_LanettesHouse_Text_ResearchNotesPage1:
	.string "These are LANETTE's research notes.\n"
	.string "They cover BOXES.\p"
	.string "Each BOX should hold 30 POKéMON.\p"
	.string "Every TRAINER should be able to store\n"
	.string "420 POKéMON on the PC system.\p"
	.string "Keep reading?$"

Route114_LanettesHouse_Text_ResearchNotesPage2:
	.string "Add a marking system to make POKéMON\n"
	.string "easier to sort.\p"
	.string "Let each BOX's name and wallpaper be\n"
	.string "changed to suit the POKéMON stored\l"
	.string "inside.\p"
	.string "Keep reading?$"

Route114_LanettesHouse_Text_ResearchNotesPage3:
	.string "A stored POKéMON should be sent to the\n"
	.string "BOX that was inspected last.\p"
	.string "If that BOX is full, the POKéMON goes to\n"
	.string "the next BOX.\p"
	.string "In other words, whichever BOX you look\n"
	.string "at is automatically the one that\l"
	.string "receives POKéMON.$"

Route114_LanettesHouse_Text_ClosedTheNotebook:
	.string "{PLAYER} closed the notebook and put it\n"
	.string "back.$"

Route114_LanettesHouse_Text_EmailFromBill:
	.string "There's an e-mail from someone on the\n"
	.string "PC.\p"
	.string "“… … … … … … …\p"
	.string "“Your Storage System is more\n"
	.string "convenient than mine.\p"
	.string "“It's packed with user-friendly\n"
	.string "features that make it fun and useful.\p"
	.string "“I'm proud I had a hand in its\n"
	.string "development.\p"
	.string "“I hope you'll keep up your research on\n"
	.string "Storage Systems.\p"
	.string "“From BILL\n"
	.string "… … … … … … … …”$"

