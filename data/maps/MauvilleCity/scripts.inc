MauvilleCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MauvilleCity_OnTransition
	.byte 0

MauvilleCity_OnTransition:
	setflag FLAG_VISITED_MAUVILLE_CITY
	clearflag FLAG_FORCE_MIRAGE_TOWER_VISIBLE
	clearflag FLAG_HIDE_SLATEPORT_MUSEUM_POPULATION
	setflag FLAG_SYS_TV_START
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	setvar VAR_MAUVILLE_GYM_STATE, 0
	call_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_MoveWattsonBackToGym
	end

MauvilleCity_EventScript_MoveWattsonBackToGym::
	clearflag FLAG_HIDE_MAUVILLE_GYM_WATTSON
	setflag FLAG_HIDE_MAUVILLE_CITY_WATTSON
	setflag FLAG_WATTSON_REMATCH_AVAILABLE
	return

MauvilleCity_EventScript_Boy::
	msgbox MauvilleCity_Text_NurseHurtMonBackToHealth, MSGBOX_NPC
	end

MauvilleCity_EventScript_Maniac::
	msgbox MauvilleCity_Text_AllSortsOfPeopleComeThrough, MSGBOX_NPC
	end

MauvilleCity_EventScript_Woman::
	msgbox MauvilleCity_Text_RydelVeryGenerous, MSGBOX_NPC
	end

MauvilleCity_EventScript_RichBoy::
	msgbox MauvilleCity_Text_PokemonCanJumpYouOnBike, MSGBOX_NPC
	end

MauvilleCity_EventScript_CitySign::
	msgbox MauvilleCity_Text_CitySign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GymSign::
	msgbox MauvilleCity_Text_GymSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_BikeShopSign::
	msgbox MauvilleCity_Text_BikeShopSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GameCornerSign::
	msgbox MauvilleCity_Text_GameCornerSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_SchoolKidM::
	lock
	faceplayer
	goto_if_set FLAG_TV_EXPLAINED, MauvilleCity_EventScript_TVExplained
	msgbox MauvilleCity_Text_ExplainTV, MSGBOX_DEFAULT
	setflag FLAG_TV_EXPLAINED
	release
	end

MauvilleCity_EventScript_TVExplained::
	msgbox MauvilleCity_Text_BeenCheckingOutTV, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallysUncle::
	lock
	faceplayer
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_UncleAskPlayerToBattleWally
	msgbox MauvilleCity_Text_UncleHesTooPeppy, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	release
	end

MauvilleCity_EventScript_UncleAskPlayerToBattleWally::
	msgbox MauvilleCity_Text_UncleCanYouBattleWally, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	release
	end

MauvilleCity_EventScript_Wally::
	lockall
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_WallyRequestBattleAgain
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	msgbox MauvilleCity_Text_WallyWantToChallengeGym, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_UncleYourePushingIt, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_WallyWeCanBeatAnyone, MSGBOX_DEFAULT
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	playse SE_PIN
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_Delay48
	msgbox MauvilleCity_Text_WallyWillYouBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_EventScript_BattleWallyPrompt::
	call_if_eq VAR_RESULT, YES, MauvilleCity_EventScript_BattleWally
	goto_if_eq VAR_RESULT, NO, MauvilleCity_EventScript_DeclineWallyBattle
	closemessage
	switch VAR_FACING
	case DIR_NORTH, MauvilleCity_EventScript_WallyAndUncleExitNorth
	case DIR_EAST, MauvilleCity_EventScript_WallyAndUncleExitEast
	end

MauvilleCity_EventScript_WallyAndUncleExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth1
	waitmovement 0
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	delay 30
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerFaceUncleNorth
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerNorth
	waitmovement 0
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_WallyAndUncleExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast1
	waitmovement 0
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	delay 30
	applywaitmovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerEast
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_DefeatedWally::
	removeobject LOCALID_MAUVILLE_WALLY
	removeobject LOCALID_MAUVILLE_WALLYS_UNCLE
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLYS_UNCLE
	setflag FLAG_DEFEATED_WALLY_MAUVILLE
	setvar VAR_WALLY_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottApproachPlayerNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottApproachPlayerEast
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	msgbox MauvilleCity_Text_ScottYouDidntHoldBack, MSGBOX_DEFAULT
	closemessage
	addvar VAR_SCOTT_STATE, 1
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottExitEast
	removeobject LOCALID_MAUVILLE_SCOTT
	releaseall
	end

MauvilleCity_EventScript_ScottApproachPlayerNorth::
	addobject LOCALID_MAUVILLE_SCOTT
	applywaitmovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerNorth
	return

MauvilleCity_EventScript_ScottApproachPlayerEast::
	setobjectxyperm LOCALID_MAUVILLE_SCOTT, 12, 13
	addobject LOCALID_MAUVILLE_SCOTT
	applywaitmovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerEast
	return

MauvilleCity_EventScript_ScottExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitNorth
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitEast
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitEast
	waitmovement 0
	return

MauvilleCity_EventScript_BattleWally::
	msgbox MauvilleCity_Text_WallyHereICome, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_MAUVILLE, MauvilleCity_Text_WallyDefeat
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	msgbox MauvilleCity_Text_WallyIllGoBackToVerdanturf, MSGBOX_DEFAULT
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	msgbox MauvilleCity_Text_ThankYouNotEnoughToBattle, MSGBOX_DEFAULT
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	msgbox MauvilleCity_Text_UncleNoNeedToBeDown, MSGBOX_DEFAULT
	return

MauvilleCity_EventScript_DeclineWallyBattle::
	setflag FLAG_DECLINED_WALLY_BATTLE_MAUVILLE
	msgbox MauvilleCity_Text_WallyMyUncleWontKnowImStrong, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallyRequestBattleAgain::
	applywaitmovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	msgbox MauvilleCity_Text_WallyPleaseBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_Movement_WallyExitNorth1:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	step_end

MauvilleCity_Movement_WallyExitEast1:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallyExitNorth2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_WallyExitEast2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth2:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast2:
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitNorth:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast1:
	delay_16
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth1:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_WallysUncleExitNorth1:
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

MauvilleCity_Movement_WallysUncleExitEast1:
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_PlayerFaceUncleNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerNorth:
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerEast:
	walk_right
	walk_up
	step_end

MauvilleCity_Movement_WallysUncleExitNorth2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallysUncleExitEast2:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_ScottApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

MauvilleCity_Movement_ScottExitNorth:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottExitEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_EventScript_Wattson::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_ReceivedThunderbolt
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_EventScript_CompletedNewMauville
	goto_if_set FLAG_GOT_BASEMENT_KEY_FROM_WATTSON, MauvilleCity_EventScript_BegunNewMauville
	msgbox MauvilleCity_Text_WattsonNeedFavorTakeKey, MSGBOX_DEFAULT
	giveitem ITEM_BASEMENT_KEY
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_BASEMENT_KEY_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_BegunNewMauville::
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Text_WattsonThanksTakeTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_THUNDERBOLT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_ReceivedThunderbolt::
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_RegisterWallyCall::
	lockall
	pokenavcall MauvilleCity_Text_WallyPokenavCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Text_RegisteredWally, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WALLY_MATCH_CALL
	clearflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	releaseall
	end

MauvilleCity_Text_UncleHesTooPeppy:
	.string "UNCLE: Ever since he started training\n"
	.string "with POKéMON, this boy's been full of\l"
	.string "pep, I'll give him that…\p"
	.string "But now he's a little too peppy for my\n"
	.string "old heart…$"

MauvilleCity_Text_WallyWantToChallengeGym:
	.string "UUB: Aww, UNCLE, please?\p"
	.string "I want to challenge this GYM and see\n"
	.string "how much stronger I've gotten.\p"
	.string "Please? Can I, please?$"

MauvilleCity_Text_UncleYourePushingIt:
	.string "UNCLE: Now hold on, UUB.\p"
	.string "Since you started training with\n"
	.string "POKéMON, you've grown a lot stronger. I\l"
	.string "can see that.\p"
	.string "But isn't it a bit much to walk up and\n"
	.string "challenge a GYM all of a sudden?$"

MauvilleCity_Text_WallyWeCanBeatAnyone:
	.string "UUB: I'm not pushing it.\p"
	.string "When I fight side by side with RALTS, we\n"
	.string "can beat anyone!$"

MauvilleCity_Text_WallyWillYouBattleMe:
	.string "UUB: Oh! Hi, {PLAYER}!\p"
	.string "I've gotten a lot stronger since we met.\p"
	.string "{PLAYER}, I want you and my UNCLE to see\n"
	.string "it with your own eyes.\p"
	.string "{PLAYER}, please, will you battle me?$"

MauvilleCity_Text_WallyMyUncleWontKnowImStrong:
	.string "UUB: Oh… If you won't fight me, {PLAYER},\n"
	.string "my UNCLE will never believe I've really\l"
	.string "gotten strong.$"

MauvilleCity_Text_UncleCanYouBattleWally:
	.string "UNCLE: {PLAYER}{KUN}, was it? On UUB's behalf,\n"
	.string "could I ask you to battle him, just this\l"
	.string "once?\p"
	.string "He's got a look in his eye I've never\n"
	.string "seen before. I don't think he'll listen\l"
	.string "to reason right now.$"

MauvilleCity_Text_WallyPleaseBattleMe:
	.string "UUB: {PLAYER}, please!\n"
	.string "Fight me, please.$"

MauvilleCity_Text_WallyHereICome:
	.string "UUB: {PLAYER}, thank you.\p"
	.string "Okay… I'm ready. Here I come!$"

MauvilleCity_Text_WallyDefeat:
	.string "UUB: … … … … … … …\p"
	.string "… … … … … … … …\p"
	.string "I… lost…$"

MauvilleCity_Text_WallyIllGoBackToVerdanturf:
	.string "UUB: UNCLE… I'll go back to VERDANTURF\n"
	.string "and train some more…$"

MauvilleCity_Text_ThankYouNotEnoughToBattle:
	.string "{PLAYER}, thank you.\n"
	.string "Being a TRAINER is tough, isn't it?\p"
	.string "Having POKéMON and making them fight\n"
	.string "isn't enough. Real strength comes from\l"
	.string "standing together with them.$"

MauvilleCity_Text_UncleNoNeedToBeDown:
	.string "UNCLE: UUB, there's no need to be so\n"
	.string "down on yourself.\p"
	.string "You gave it everything you had. That's\n"
	.string "how a fighter gets stronger and\l"
	.string "stronger.\p"
	.string "Come on, let's go home. Everyone's\n"
	.string "waiting for you.$"

MauvilleCity_Text_UncleVisitUsSometime:
	.string "UNCLE: {PLAYER}{KUN}, it just dawned on me.\n"
	.string "You're the TRAINER who looked out for\l"
	.string "UUB when he caught his first POKéMON,\l"
	.string "aren't you?\p"
	.string "Why don't you visit us in VERDANTURF\n"
	.string "sometime? I'm sure UUB would love it.\p"
	.string "Bring an appetite, too!$"


MauvilleCity_Text_WallyPokenavCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "UUB: Oh, hello, {PLAYER}!\p"
	.string "I wanted to tell you my uncle bought me\n"
	.string "a POKéNAV!\p"
	.string "Now I can call you anytime and tell you\n"
	.string "how my training is going, {PLAYER}!\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

MauvilleCity_Text_RegisteredWally:
	.string "Registered UUB\n"
	.string "in the POKéNAV.$"

MauvilleCity_Text_ScottYouDidntHoldBack:
	.string "ANNOUNCER: Hehe… I caught that match!\p"
	.string "You're friends with that boy UUB, aren't\n"
	.string "you?\p"
	.string "And you still didn't hold anything back.\n"
	.string "You beat him fair and square. Beautiful!\p"
	.string "Yeah! That's what a real POKéMON battle\n"
	.string "is all about!\p"
	.string "I'm a big fan of fighters like you!\p"
	.string "… … … … … …\p"
	.string "I'll be cheering for you from the front\n"
	.string "row!$"

MauvilleCity_Text_WattsonNeedFavorTakeKey:
	.string "OX-KING: Oh, {PLAYER}{KUN}! My favorite\n"
	.string "son-in-law! Look at you, brimming with\l"
	.string "energy! That's what I like to see,\l"
	.string "wahahahaha!\p"
	.string "Fine! I've decided! I've got a favor to\n"
	.string "ask, {PLAYER}{KUN}!\p"
	.string "Beneath MAUVILLE CITY there's an\n"
	.string "underground sector called NEW\l"
	.string "MAUVILLE. That's where I built our first\l"
	.string "power plant.\p"
	.string "{PLAYER}{KUN}, I'd like you to go down there\n"
	.string "and switch off the GENERATOR.\p"
	.string "The thing's been running wild lately.\n"
	.string "It's getting dangerous.\p"
	.string "Here, take this KEY. It opens the door\n"
	.string "to NEW MAUVILLE.$"

MauvilleCity_Text_WattsonWontBeChallenge:
	.string "OX-KING: Don't you worry about it. A\n"
	.string "fighter like you won't find it much of a\l"
	.string "challenge.\p"
	.string "The entrance to NEW MAUVILLE is just a\n"
	.string "short SURF away from ROUTE 110.\p"
	.string "That's that, then! You have my trust!\n"
	.string "Wahahahaha!$"

MauvilleCity_Text_WattsonThanksTakeTM:
	.string "OX-KING: Wahahahaha!\p"
	.string "I knew it, {PLAYER}{KUN}! I knew I made the\n"
	.string "right choice asking you!\p"
	.string "Here's my thanks - a TM containing\n"
	.string "THUNDERBOLT!\p"
	.string "Go on, take it! You've earned it!$"

MauvilleCity_Text_WattsonYoungTakeCharge:
	.string "OX-KING: Wahahahaha!\p"
	.string "Nothing makes me happier than watching\n"
	.string "the young ones step up and take\l"
	.string "charge!$"

MauvilleCity_Text_NurseHurtMonBackToHealth:
	.string "Battling is great and all…\p"
	.string "But when your POKéMON gets hurt, you\n"
	.string "have to nurse it back to health. Even\l"
	.string "the strongest fighters rest up.$"

MauvilleCity_Text_AllSortsOfPeopleComeThrough:
	.string "The roads of this town run north and\n"
	.string "south, east and west.\p"
	.string "Because of that, all sorts of people\n"
	.string "come through. Tourists, traders, even a\l"
	.string "few who look like they could flatten a\l"
	.string "mountain.$"

MauvilleCity_Text_RydelVeryGenerous:
	.string "Have you been to RYDEL'S CYCLES yet?\p"
	.string "RYDEL, the owner, is a very generous\n"
	.string "man. He gives BIKES away for free!$"

MauvilleCity_Text_PokemonCanJumpYouOnBike:
	.string "Even on a BIKE, watch where you ride!\p"
	.string "Wild POKéMON roam around out in the\n"
	.string "open. Run into one, and you're in for a\l"
	.string "battle!\p"
	.string "Keep your eyes peeled!$"

MauvilleCity_Text_CitySign:
	.string "MAUVILLE CITY\n"
	.string "“The bright city that OX-KING lit up!”$"

MauvilleCity_Text_GymSign:
	.string "MAUVILLE CITY POKéMON GYM\n"
	.string "LEADER: OX-KING\l"
	.string "“The jolly giant who powers the city!”$"

MauvilleCity_Text_BikeShopSign:
	.string "“Kick up the dust and rattle your\n"
	.string "soul!” RYDEL'S CYCLES$"

MauvilleCity_Text_GameCornerSign:
	.string "“Fun for every fighter, big or small!”\n"
	.string "MAUVILLE GAME CORNER$"

MauvilleCity_Text_ExplainTV:
	.string "Hey, do you ever watch TV?\p"
	.string "They've added a bunch of cool new shows\n"
	.string "lately.\p"
	.string "TV is amazing. I once saw a friend pop\n"
	.string "up on the screen out of nowhere.\p"
	.string "And sometimes they put you on TV\n"
	.string "without telling you!\p"
	.string "I wish I'd get on TV someday.\p"
	.string "That's why you should check out TVs\n"
	.string "whenever you can.$"

MauvilleCity_Text_BeenCheckingOutTV:
	.string "Hey, have you been watching the TVs\n"
	.string "lately?$"
