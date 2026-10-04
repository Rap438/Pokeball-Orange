FortreeCity_House4_MapScripts::
	.byte 0

FortreeCity_House4_EventScript_Woman::
	msgbox FortreeCity_House4_Text_BringsWorldCloserTogether, MSGBOX_NPC
	end

FortreeCity_House4_EventScript_Boy::
	lockall
	goto_if_set FLAG_RECEIVED_MENTAL_HERB, FortreeCity_House4_EventScript_ReceivedMentalHerb
	goto_if_set FLAG_WINGULL_DELIVERED_MAIL, FortreeCity_House4_EventScript_WingullReturned
	goto_if_set FLAG_WINGULL_SENT_ON_ERRAND, FortreeCity_House4_EventScript_WingullOnErrand
	msgbox FortreeCity_House4_Text_GoBirdPokemon, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_WINGULL_SENT_ON_ERRAND
	clearflag FLAG_HIDE_MOSSDEEP_CITY_HOUSE_2_WINGULL
	applywaitmovement LOCALID_FORTREE_HOUSE_WINGULL, FortreeCity_House4_Movement_WingullExit
	removeobject LOCALID_FORTREE_HOUSE_WINGULL
	releaseall
	end

FortreeCity_House4_EventScript_WingullOnErrand::
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	msgbox FortreeCity_House4_Text_AskedWingullToRunErrand, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_House4_EventScript_WingullReturned::
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	msgbox FortreeCity_House4_Text_WelcomeWingullTakeMentalHerb, MSGBOX_DEFAULT
	giveitem ITEM_MENTAL_HERB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MENTAL_HERB
	releaseall
	end

FortreeCity_House4_EventScript_ReceivedMentalHerb::
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	msgbox FortreeCity_House4_Text_FriendsFarAwayThanksToWingull, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_House4_Movement_WingullExit:
	walk_fast_down
	walk_fast_down
	walk_fast_right
	walk_in_place_faster_down
	delay_8
	step_end

FortreeCity_House4_EventScript_Wingull::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_WINGULL, CRY_MODE_NORMAL
	msgbox FortreeCity_House4_Text_Wingull, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FortreeCity_House4_Text_BringsWorldCloserTogether:
	.string "When you live and train alongside\n"
	.string "POKéMON, you end up with friends\l"
	.string "everywhere.\p"
	.string "And that brings the whole world closer\n"
	.string "together. Isn't that wonderful?$"

FortreeCity_House4_Text_GoBirdPokemon:
	.string "There!\n"
	.string "Fly, BIRD POKéMON!$"

FortreeCity_House4_Text_AskedWingullToRunErrand:
	.string "Heheh, I sent my WINGULL out to run an\n"
	.string "errand for me.$"

FortreeCity_House4_Text_WelcomeWingullTakeMentalHerb:
	.string "Welcome back, WINGULL!\p"
	.string "Hm? What's that you're carrying?\p"
	.string "A MENTAL HERB? You must have picked it\n"
	.string "up somewhere on your trip.\p"
	.string "I'm no TRAINER, so you may as well have\n"
	.string "it.$"

FortreeCity_House4_Text_FriendsFarAwayThanksToWingull:
	.string "Thanks to my WINGULL carrying my\n"
	.string "letters, I have friends in far-off\l"
	.string "towns.$"

FortreeCity_House4_Text_Wingull:
	.string "WINGULL: Pihyoh!$"

