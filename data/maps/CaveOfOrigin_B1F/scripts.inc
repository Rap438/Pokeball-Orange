CaveOfOrigin_B1F_MapScripts::
	.byte 0

CaveOfOrigin_B1F_EventScript_Wallace::
	lock
	faceplayer
	msgbox CaveOfOrigin_B1F_Text_WallaceStory, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_WalkInPlaceFasterUp
	delay 60
	playse SE_PIN
	applywaitmovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_Delay48
	delay 30
	applywaitmovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_FacePlayer
	message CaveOfOrigin_B1F_Text_WhereIsRayquaza
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_WheresRayquaza::
	multichoice 0, 0, MULTI_WHERES_RAYQUAZA, FALSE
	switch VAR_RESULT
	case 0, CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin
	case 1, CaveOfOrigin_B1F_EventScript_AtMtPyre
	case 2, CaveOfOrigin_B1F_EventScript_AtSkyPillar
	case 3, CaveOfOrigin_B1F_EventScript_DontRemember
	goto CaveOfOrigin_B1F_EventScript_DontRemember
	end

CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin::
	message CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtMtPyre::
	message CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_DontRemember::
	message CaveOfOrigin_B1F_Text_CantYouRememberSomehow
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtSkyPillar::
	msgbox CaveOfOrigin_B1F_Text_WellHeadToSkyPillar, MSGBOX_DEFAULT
	closemessage
	playse SE_EXIT
	fadescreenspeed FADE_TO_BLACK, 4
	setflag FLAG_WALLACE_GOES_TO_SKY_PILLAR
	setvar VAR_SOOTOPOLIS_CITY_STATE, 3
	removeobject LOCALID_CAVE_OF_ORIGIN_WALLACE
	clearflag FLAG_HIDE_SKY_PILLAR_WALLACE
	fadescreen FADE_FROM_BLACK
	release
	end

CaveOfOrigin_B1F_Text_WallaceStory:
	.string "Ah, so you are {PLAYER}{KUN}?\p"
	.string "I've heard tales of your power. My\n"
	.string "daughter VIDEL never stops talking\l"
	.string "about you!\p"
	.string "I am MR. SATAN, the WORLD CHAMPION!\p"
	.string "I was once the GYM LEADER of\n"
	.string "SOOTOPOLIS, but something came up.\p"
	.string "So I left the GYM in the hands of my\n"
	.string "esteemed teacher, PICCOLO.\p"
	.string "… … … … … …\p"
	.string "… … … … … …\p"
	.string "GROUDON and KYOGRE, the two POKéMON\n"
	.string "wreaking havoc here, are said to be\l"
	.string "super-ancient POKéMON.\p"
	.string "But there aren't just two of them.\p"
	.string "There is one more out there somewhere.\p"
	.string "A super-ancient POKéMON named\n"
	.string "RAYQUAZA, a dragon of the sky as mighty\l"
	.string "as SHENRON himself.\p"
	.string "Legend says it was RAYQUAZA that\n"
	.string "calmed the two combatants in the\l"
	.string "distant past.\p"
	.string "But even the great MR. SATAN has no\n"
	.string "clue where RAYQUAZA is…$"

CaveOfOrigin_B1F_Text_WhereIsRayquaza:
	.string "MR. SATAN: {PLAYER}{KUN}, do you happen to\n"
	.string "know where RAYQUAZA is?\p"
	.string "If you do, tell me, quick!$"

CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin:
	.string "MR. SATAN: The CAVE OF ORIGIN?\p"
	.string "We're standing in it right now! You'll\n"
	.string "have to do better than that!\p"
	.string "Please, think hard about where\n"
	.string "RAYQUAZA could be.$"

CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat:
	.string "MR. SATAN: MT. PYRE?\p"
	.string "But when I spoke with the old lady up\n"
	.string "there, she never mentioned it.\p"
	.string "I can't believe she'd hide anything from\n"
	.string "the WORLD CHAMPION…\p"
	.string "{PLAYER}{KUN}, please think harder for me.$"

CaveOfOrigin_B1F_Text_CantYouRememberSomehow:
	.string "MR. SATAN: Huh? You don't remember?\p"
	.string "Hmm… That's a problem…\p"
	.string "Can't you try to remember?$"

CaveOfOrigin_B1F_Text_WellHeadToSkyPillar:
	.string "MR. SATAN: The SKY PILLAR?\p"
	.string "That's it!\p"
	.string "It has to be the SKY PILLAR!\p"
	.string "{PLAYER}{KUN}, there's no time to lose! We're\n"
	.string "heading to the SKY PILLAR right now!$"

