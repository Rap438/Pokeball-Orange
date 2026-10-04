SlateportCity_SternsShipyard_1F_MapScripts::
	.byte 0

SlateportCity_SternsShipyard_1F_EventScript_Dock::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_SternsShipyard_1F_EventScript_FerryReady
	goto_if_set FLAG_BADGE07_GET, SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined
	goto_if_set FLAG_DELIVERED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_GoFindStern
	msgbox SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails, MSGBOX_DEFAULT
	applywaitmovement LOCALID_DOCK, Common_Movement_FacePlayer
	msgbox SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	setflag FLAG_DOCK_REJECTED_DEVON_GOODS
	setflag FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_FerryReady::
	applywaitmovement LOCALID_DOCK, Common_Movement_FacePlayer
	msgbox SlateportCity_SternsShipyard_1F_Text_FerryIsReady, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined::
	applywaitmovement LOCALID_DOCK, Common_Movement_FacePlayer
	msgbox SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_GoFindStern::
	applywaitmovement LOCALID_DOCK, Common_Movement_FacePlayer
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran::
	applywaitmovement LOCALID_DOCK, Common_Movement_FacePlayer
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist1::
	msgbox SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist2::
	msgbox SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Briney::
	msgbox SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails:
	.string "Umm… If this goes here, and that goes\n"
	.string "over there…\p"
	.string "Then where does this thing go? And what\n"
	.string "about that doohickey?\p"
	.string "Aaargh! I can't make heads or tails of\n"
	.string "this!$"

SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern:
	.string "Hm?\n"
	.string "Oh, hi, I'm DOCK.\p"
	.string "CAPT. STERN hired me to design a ferry.\p"
	.string "Oh! That parcel you're carrying… Is\n"
	.string "that CC GOODS?\p"
	.string "But, hmm… That won't do for me…\p"
	.string "CAPT. STERN went off somewhere. He said\n"
	.string "he had some work to do.\p"
	.string "Could I get you to find CAPT. STERN and\n"
	.string "deliver that to him?$"

SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern:
	.string "DOCK: Where could CAPT. STERN have gone\n"
	.string "off to?\p"
	.string "Could you find him and deliver that\n"
	.string "parcel?$"

SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran:
	.string "DOCK: Shipbuilding is an art.\p"
	.string "A lot of it can't be figured out just by\n"
	.string "calculating.\p"
	.string "I really could use advice from a veteran\n"
	.string "who knows the sea…$"

SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs:
	.string "DOCK: Hi! MR. BRINEY has joined us to\n"
	.string "lend a hand.\p"
	.string "Thanks to the veteran sailor, the ferry\n"
	.string "is steadily coming together.$"

SlateportCity_SternsShipyard_1F_Text_FerryIsReady:
	.string "DOCK: The ferry is finally ready!\p"
	.string "The new S.S. TIDAL is a true marvel of\n"
	.string "technology!\p"
	.string "But I have a feeling we can build\n"
	.string "something even better.\p"
	.string "You know, technology never stops\n"
	.string "marching forward.$"

SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock:
	.string "MR. BRINEY: Ah, {PLAYER}{KUN}!\n"
	.string "It's been too long!\p"
	.string "Aye, ever since I met you, this old sea\n"
	.string "dog's been feeling frisky!\p"
	.string "So I've decided to help DOCK build a\n"
	.string "ferry.\p"
	.string "Aye, a ferry can carry a whole lot of\n"
	.string "people, after all.\p"
	.string "But you know, that DOCK is really\n"
	.string "something special.\p"
	.string "With his knack for technology and my\n"
	.string "experience, I'm sure we can build one\l"
	.string "great ship, aye!$"

SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing:
	.string "The seasons, the weather, where the\n"
	.string "moon sits in the sky…\p"
	.string "All of it makes the sea change its\n"
	.string "expression.\p"
	.string "That's right!\n"
	.string "The sea is a living thing!$"

SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily:
	.string "I get seasick real easy.\n"
	.string "So I help out here on land instead.$"

