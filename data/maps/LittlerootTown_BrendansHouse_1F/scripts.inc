LittlerootTown_BrendansHouse_1F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, LittlerootTown_BrendansHouse_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_BrendansHouse_1F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_BrendansHouse_1F_OnFrame
	.byte 0

LittlerootTown_BrendansHouse_1F_OnLoad:
	call_if_lt VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_SetMovingBoxes
	call_if_set FLAG_RECEIVED_RUNNING_SHOES, LittlerootTown_BrendansHouse_1F_EventScript_CheckShowShoesManual
	end

LittlerootTown_BrendansHouse_1F_EventScript_SetMovingBoxes::
	setmetatile 5, 4, METATILE_BrendansMaysHouse_MovingBox_Open, TRUE
	setmetatile 5, 2, METATILE_BrendansMaysHouse_MovingBox_Closed, TRUE
	return

LittlerootTown_BrendansHouse_1F_EventScript_CheckShowShoesManual::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_1F_EventScript_ShowRunningShoesManual
	return

LittlerootTown_BrendansHouse_1F_EventScript_ShowRunningShoesManual::
	setmetatile 3, 7, METATILE_BrendansMaysHouse_BookOnTable, TRUE
	return

LittlerootTown_BrendansHouse_1F_OnTransition:
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 3, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToDoor
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 5, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToStairs
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToTV
	end

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToStairs::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 8, 4
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToTV::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 4, 5
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToDoor::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 9, 8
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

@ Many of the below scripts have no gender check because they assume youre in the correct house
@ The below SS Ticket script uses Mays house state by accident(?), but theyre both set identically after the intro
LittlerootTown_BrendansHouse_1F_OnFrame:
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 3, LittlerootTown_BrendansHouse_1F_EventScript_EnterHouseMovingIn
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 5, LittlerootTown_BrendansHouse_1F_EventScript_GoUpstairsToSetClock
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_PetalburgGymReport
	map_script_2 VAR_LITTLEROOT_HOUSES_STATE_MAY, 1, LittlerootTown_BrendansHouse_1F_EventScript_YoureNewNeighbor
	map_script_2 VAR_LITTLEROOT_HOUSES_STATE_MAY, 3, PlayersHouse_1F_EventScript_GetSSTicketAndSeeLatiTV
	.2byte 0

LittlerootTown_BrendansHouse_1F_EventScript_GoUpstairsToSetClock::
	lockall
	msgbox PlayersHouse_1F_Text_GoSetTheClock, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs
	waitmovement 0
	warp MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F, 7, 1
	waitstate
	releaseall
	end

LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs:
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_EnterHouseMovingIn::
	lockall
	setvar VAR_0x8004, LOCALID_PLAYERS_HOUSE_1F_MOM
	setvar VAR_0x8005, MALE
	goto PlayersHouse_1F_EventScript_EnterHouseMovingIn
	end

LittlerootTown_BrendansHouse_1F_EventScript_PetalburgGymReport::
	lockall
	setvar VAR_0x8004, MALE
	setvar VAR_0x8005, LOCALID_PLAYERS_HOUSE_1F_MOM
	goto PlayersHouse_1F_EventScript_PetalburgGymReportMale
	end

LittlerootTown_BrendansHouse_1F_EventScript_YoureNewNeighbor::
	lockall
	playse SE_PIN
	applywaitmovement LOCALID_RIVALS_HOUSE_1F_MOM, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RIVALS_HOUSE_1F_MOM, Common_Movement_Delay48
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_RIVALS_HOUSE_1F_MOM, LittlerootTown_BrendansHouse_1F_Movement_RivalMomApproach
	waitmovement 0
	special GetRivalSonDaughterString
	msgbox RivalsHouse_1F_Text_OhYoureTheNewNeighbor, MSGBOX_DEFAULT
	setflag FLAG_MET_RIVAL_MOM
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 2
	releaseall
	end

LittlerootTown_BrendansHouse_1F_Movement_RivalMomApproach:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_GoSeeRoom::
	lockall
	setvar VAR_0x8004, LOCALID_PLAYERS_HOUSE_1F_MOM
	setvar VAR_0x8005, MALE
	applywaitmovement VAR_0x8004, Common_Movement_WalkInPlaceFasterLeft
	goto PlayersHouse_1F_EventScript_MomGoSeeRoom
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival0::
	lockall
	setvar VAR_0x8008, 0
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival1::
	lockall
	setvar VAR_0x8008, 1
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival2::
	lockall
	setvar VAR_0x8008, 2
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival::
	playse SE_EXIT
	delay 10
	addobject LOCALID_RIVALS_HOUSE_1F_RIVAL
	delay 30
	playse SE_PIN
	applywaitmovement LOCALID_RIVALS_HOUSE_1F_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_RIVALS_HOUSE_1F_RIVAL, Common_Movement_Delay48
	call_if_ne VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_PlayerFaceBrendan
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_eq VAR_0x8008, 0, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer0
	call_if_eq VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer1
	call_if_eq VAR_0x8008, 2, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer2
	msgbox RivalsHouse_1F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_0x8008, 0, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs0
	call_if_eq VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs1
	call_if_eq VAR_0x8008, 2, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs2
	playse SE_EXIT
	removeobject LOCALID_RIVALS_HOUSE_1F_RIVAL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_BRENDAN
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	delay 30
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

LittlerootTown_BrendansHouse_1F_EventScript_PlayerFaceBrendan::
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer0::
	applywaitmovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer1::
	applywaitmovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer1
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer2::
	applywaitmovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer2
	return

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer0:
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer1:
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer2:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs0::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit0
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs0
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs1::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit1
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs1
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs2::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit2
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs2
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit0:
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit1:
	delay_16
	delay_8
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit2:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs0:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs1:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs2:
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	walk_up
	step_end

PlayersHouse_1F_Text_IsntItNiceInHere:
	.string "CHI-CHI: Well, {PLAYER}?\n"
	.string "It's cozy in here too, isn't it?$"

PlayersHouse_1F_Text_MoversPokemonGoSetClock:
	.string "The CAPSULE CORP. movers' POKéMON\n"
	.string "unpacked everything and even tidied\l"
	.string "up afterward. How wonderful!\p"
	.string "{PLAYER}, our room is upstairs.\n"
	.string "Go have a look, dear!\p"
	.string "I bought a new clock to mark our\n"
	.string "move here.\l"
	.string "Don't forget to set it!$"

PlayersHouse_1F_Text_ArentYouInterestedInRoom:
	.string "CHI-CHI: Well, {PLAYER}?\p"
	.string "Don't you want to see our new\n"
	.string "room?$"

PlayersHouse_1F_Text_GoSetTheClock:
	.string "CHI-CHI: {PLAYER}.\p"
	.string "Go set the clock upstairs, honey.$"

PlayersHouse_1F_Text_OhComeQuickly:
	.string "CHI-CHI: Oh! {PLAYER}, {PLAYER}!\n"
	.string "Hurry! Come here, quick!$"

PlayersHouse_1F_Text_MaybeDadWillBeOn:
	.string "CHI-CHI: Look! It's PETALBURG GYM!\n"
	.string "Maybe KRILLIN will show up!$"

PlayersHouse_1F_Text_ItsOverWeMissedHim:
	.string "CHI-CHI: Oh… It's over.\p"
	.string "I think KRILLIN was on for a second,\n"
	.string "but we missed him. What a shame.$"

PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor:
	.string "Oh, that's right.\n"
	.string "Your old master lives in this town!\p"
	.string "MASTER ROSHI has a POKéMON LAB right\n"
	.string "next door. Go say hello, dear.\p"
	.string "And if he starts being a pervert,\n"
	.string "you come straight home!$"

PlayersHouse_1F_Text_SeeYouHoney:
	.string "CHI-CHI: See you later, honey!$"

PlayersHouse_1F_Text_DidYouMeetProfBirch:
	.string "CHI-CHI: Did you say hello to\n"
	.string "MASTER ROSHI yet?$"

PlayersHouse_1F_Text_YouShouldRestABit:
	.string "CHI-CHI: How are you feeling, {PLAYER}?\n"
	.string "You look a little worn out.\p"
	.string "Come sit down and rest a while.$"

PlayersHouse_1F_Text_TakeCareHoney:
	.string "CHI-CHI: Take care out there, honey!$"

PlayersHouse_1F_Text_GotDadsBadgeHeresSomethingFromMom:
	.string "CHI-CHI: Oh? KRILLIN gave you that\n"
	.string "BADGE?\p"
	.string "Then here's a little something from\n"
	.string "me!$"

PlayersHouse_1F_Text_DontPushYourselfTooHard:
	.string "Don't push yourself too hard, dear.\n"
	.string "Home is always here for you.\p"
	.string "Go get 'em, honey!$"

PlayersHouse_1F_Text_IsThatAPokenav:
	.string "CHI-CHI: What's that, honey? A\n"
	.string "POKéNAV? Someone from CAPSULE CORP.\l"
	.string "gave it to you?\p"
	.string "Well then, how about registering me?\n"
	.string "That way you can't forget to call home!\p"
	.string "… … …$"

PlayersHouse_1F_Text_RegisteredMom:
	.string "Registered CHI-CHI\n"
	.string "in the POKéNAV.$"

PlayersHouse_1F_Text_Vigoroth1:
	.string "Fugiiiiih!$"

PlayersHouse_1F_Text_Vigoroth2:
	.string "Huggoh, uggo uggo…$"

PlayersHouse_1F_Text_ReportFromPetalburgGym:
	.string "REPORTER: …And that was our live report\n"
	.string "from outside KRILLIN's PETALBURG GYM.$"

PlayersHouse_1F_Text_TheresAMovieOnTV:
	.string "A martial arts movie is on TV.\p"
	.string "A man in a golden cape strikes a pose\n"
	.string "and shouts something about being the\l"
	.string "champion of the world.\p"
	.string "Better get going!$"

PlayersHouse_1F_Text_RunningShoesManual:
	.string "It's the instruction booklet for the\n"
	.string "RUNNING SHOES.\p"
	.string "“Hold the B Button while wearing your\n"
	.string "RUNNING SHOES to dash at top speed.\p"
	.string "“Lace them up and race into the\n"
	.string "great outdoors!”$"

PlayersHouse_1F_Text_TicketFromBrineyCameForYou:
	.string "KRILLIN: Hm?\p"
	.string "Hey, it's {PLAYER}!\p"
	.string "It's been a while, but you look…\n"
	.string "stronger, somehow.\p"
	.string "I can feel it, even without a SCOUTER.\n"
	.string "But don't count your old buddy out yet!\p"
	.string "Oh, right, I've got something for you.\n"
	.string "This came for you from a guy named\l"
	.string "MR. BRINEY.$"

PlayersHouse_1F_Text_PortsInSlateportLilycove:
	.string "KRILLIN: Hm, a TICKET for a ferry?\p"
	.string "If I remember right, there are ferry\n"
	.string "ports at SLATEPORT and LILYCOVE.$"

PlayersHouse_1F_Text_BetterGetBackToGym:
	.string "I'd better get back to PETALBURG GYM.\p"
	.string "CHI-CHI, thanks for the great meal!\n"
	.string "The GYM won't run itself.$"

PlayersHouse_1F_Text_DadShouldStayLonger:
	.string "CHI-CHI: Honestly, that KRILLIN…\p"
	.string "He stops by for the first time in ages,\n"
	.string "and all he talks about is POKéMON and\l"
	.string "training.\p"
	.string "He could at least stay for seconds.$"

PlayersHouse_1F_Text_IsThatABreakingStory:
	.string "CHI-CHI: Is that a breaking news\n"
	.string "story?$"

PlayersHouse_1F_Text_LatiEmergencyNewsFlash:
	.string "We interrupt this program with an\n"
	.string "emergency news flash!\p"
	.string "In various HOENN locales, there have\n"
	.string "been reports of a BZZT…colored\l"
	.string "POKéMON in flight.\p"
	.string "The identity of this POKéMON is\n"
	.string "currently unknown.\p"
	.string "Now, back to our regularly scheduled\n"
	.string "movie.$"

PlayersHouse_1F_Text_WhatColorDidTheySay:
	.string "CHI-CHI: {PLAYER}, did you catch that?\p"
	.string "What color did the newscaster say\n"
	.string "that POKéMON was?$"

PlayersHouse_1F_Text_StillUnknownPokemon:
	.string "CHI-CHI: My goodness, how about that!\n"
	.string "There are still POKéMON nobody knows.$"
