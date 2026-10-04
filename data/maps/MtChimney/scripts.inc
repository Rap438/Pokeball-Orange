MtChimney_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MtChimney_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, MtChimney_OnTransition
	.byte 0

MtChimney_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 1
	end

MtChimney_OnResume:
	setstepcallback STEP_CB_ASH
	end

MtChimney_EventScript_Archie::
	lock
	faceplayer
	call_if_unset FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieGoStopTeamMagma
	call_if_set FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieBusyFighting
	closemessage
	applywaitmovement LOCALID_MT_CHIMNEY_ARCHIE, Common_Movement_FaceOriginalDirection
	setflag FLAG_EVIL_LEADER_PLEASE_STOP
	release
	end

MtChimney_EventScript_ArchieGoStopTeamMagma::
	msgbox MtChimney_Text_ArchieGoStopTeamMagma, MSGBOX_DEFAULT
	return

MtChimney_EventScript_ArchieBusyFighting::
	msgbox MtChimney_Text_ArchieIHaveMyHandsFull, MSGBOX_DEFAULT
	return

MtChimney_EventScript_Maxie::
	lockall
	playbgm MUS_ENCOUNTER_MAGMA, FALSE
	msgbox MtChimney_Text_MeteoriteWillActivateVolcano, MSGBOX_DEFAULT
	applywaitmovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_FacePlayer
	playse SE_PIN
	applywaitmovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_Delay48
	msgbox MtChimney_Text_MaxieIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_MAXIE_MT_CHIMNEY, MtChimney_Text_MaxieDefeat
	msgbox MtChimney_Text_MaxieYouHaventSeenLastOfMagma, MSGBOX_DEFAULT
	closemessage
	delay 30
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_MT_CHIMNEY_MAXIE
	removeobject LOCALID_MT_CHIMNEY_MAGMA_GRUNT_1
	removeobject LOCALID_MT_CHIMNEY_TABITHA
	removeobject LOCALID_MT_CHIMNEY_MAGMA_GRUNT_2
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA
	fadescreen FADE_FROM_BLACK
	setobjectxyperm LOCALID_MT_CHIMNEY_ARCHIE, 10, 12
	addobject LOCALID_MT_CHIMNEY_ARCHIE
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieApproachPlayerEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieApproachPlayerNorth
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	msgbox MtChimney_Text_ArchieThankYou, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieExitEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieExitNorth
	removeobject LOCALID_MT_CHIMNEY_ARCHIE
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA
	setflag FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY
	clearflag FLAG_HIDE_FALLARBOR_HOUSE_PROF_COZMO
	setflag FLAG_HIDE_METEOR_FALLS_1F_1R_COZMO
	clearflag FLAG_HIDE_MT_CHIMNEY_LAVA_COOKIE_LADY
	releaseall
	end

MtChimney_EventScript_ArchieApproachPlayerEast::
	applywaitmovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieApproachPlayerEast
	return

MtChimney_EventScript_ArchieApproachPlayerNorth::
	applywaitmovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieApproachPlayerNorth
	return

MtChimney_EventScript_ArchieExitEast::
	applywaitmovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieExitEast
	return

MtChimney_EventScript_ArchieExitNorth::
	applywaitmovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieExitNorth
	return

MtChimney_EventScript_LavaCookieLady::
	lock
	faceplayer
	showmoneybox 0, 0
	msgbox MtChimney_Text_LavaCookiesJust200, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_DeclineLavaCookie
	checkmoney 200
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_NotEnoughMoney
	msgbox MtChimney_Text_ThankYouDear, MSGBOX_DEFAULT
	checkitemspace ITEM_LAVA_COOKIE
	call_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RemoveMoney
	giveitem ITEM_LAVA_COOKIE
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_BagIsFull
	hidemoneybox
	release
	end

MtChimney_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_EventScript_RemoveMoney::
	removemoney 200
	updatemoneybox
	return

MtChimney_EventScript_DeclineLavaCookie::
	msgbox MtChimney_Text_OhFineThen, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_EventScript_NotEnoughMoney::
	msgbox MtChimney_Text_YouveNotGotTheMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_Movement_ArchieApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	step_end

MtChimney_Movement_ArchieExitEast:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_ArchieApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	step_end

MtChimney_Movement_ArchieExitNorth:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused1:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused2:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused3:
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused4:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused5:
	delay_16
	delay_16
	delay_16
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused6:
	delay_16
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused7:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused8:
	delay_16
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

MtChimney_Movement_Unused9:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused10:
	walk_down
	step_end

MtChimney_Movement_Unused11:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused12:
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused13:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused14:
	lock_facing_direction
	walk_fast_left
	unlock_facing_direction
	face_right
	step_end

MtChimney_Movement_Unused15:
	walk_left
	walk_in_place_faster_right
	delay_16
	step_end

MtChimney_EventScript_BusyAquaGrunt1::
	msgbox MtChimney_Text_MagmaOutnumbersUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt2::
	msgbox MtChimney_Text_LessHabitatForWaterPokemon, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt3::
	msgbox MtChimney_Text_MagmasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_Tabitha::
	trainerbattle_single TRAINER_TABITHA_MT_CHIMNEY, MtChimney_Text_TabithaIntro, MtChimney_Text_TabithaDefeat
	msgbox MtChimney_Text_TabithaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_2, MtChimney_Text_Grunt2Intro, MtChimney_Text_Grunt2Defeat
	msgbox MtChimney_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_BusyMagmaGrunt1::
	msgbox MtChimney_Text_TeamAquaAlwaysMessingWithPlans, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt2::
	msgbox MtChimney_Text_MeteoritesPackAmazingPower, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt3::
	msgbox MtChimney_Text_YouBetterNotMessWithUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt4::
	msgbox MtChimney_Text_AquasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt5::
	msgbox MtChimney_Text_DouseThemInFire, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt6::
	msgbox MtChimney_Text_KeepMakingMoreLand, MSGBOX_SIGN
	end

MtChimney_EventScript_MagmaPoochyena::
	msgbox MtChimney_Text_Bufoh, MSGBOX_SIGN
	end

MtChimney_EventScript_AquaPoochyena::
	msgbox MtChimney_Text_Bushaa, MSGBOX_SIGN
	end

MtChimney_EventScript_MeteoriteMachine::
	lockall
	goto_if_unset FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, MtChimney_EventScript_MachineOn
	goto_if_set FLAG_RECEIVED_METEORITE, MtChimney_EventScript_MachineOff
	msgbox MtChimney_Text_RemoveTheMeteorite, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_LeaveMeteoriteAlone
	msgbox MtChimney_Text_PlayerRemovedMeteorite, MSGBOX_DEFAULT
	giveitem ITEM_METEORITE
	setflag FLAG_RECEIVED_METEORITE
	releaseall
	end

MtChimney_EventScript_LeaveMeteoriteAlone::
	msgbox MtChimney_Text_PlayerLeftMeteorite, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOff::
	msgbox MtChimney_Text_MachineMakesNoResponse, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOn::
	msgbox MtChimney_Text_MeteoriteFittedOnMachine, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_RouteSign::
	msgbox MtChimney_Text_RouteSign, MSGBOX_SIGN
	end

MtChimney_EventScript_Shelby::
	trainerbattle_single TRAINER_SHELBY_1, MtChimney_Text_ShelbyIntro, MtChimney_Text_ShelbyDefeat, MtChimney_EventScript_DefeatedShelby
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RematchShelby
	msgbox MtChimney_Text_ShelbyPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_DefeatedShelby::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_ShelbyRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SHELBY_1
	release
	end

MtChimney_EventScript_RematchShelby::
	trainerbattle_rematch TRAINER_SHELBY_1, MtChimney_Text_ShelbyRematchIntro, MtChimney_Text_ShelbyRematchDefeat
	msgbox MtChimney_Text_ShelbyPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Melissa::
	trainerbattle_single TRAINER_MELISSA, MtChimney_Text_MelissaIntro, MtChimney_Text_MelissaDefeat
	msgbox MtChimney_Text_MelissaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sheila::
	trainerbattle_single TRAINER_SHEILA, MtChimney_Text_SheilaIntro, MtChimney_Text_SheilaDefeat
	msgbox MtChimney_Text_SheilaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Shirley::
	trainerbattle_single TRAINER_SHIRLEY, MtChimney_Text_ShirleyIntro, MtChimney_Text_ShirleyDefeat
	msgbox MtChimney_Text_ShirleyPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_1, MtChimney_Text_Grunt1Intro, MtChimney_Text_Grunt1Defeat
	msgbox MtChimney_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sawyer::
	trainerbattle_single TRAINER_SAWYER_1, MtChimney_Text_SawyerIntro, MtChimney_Text_SawyerDefeat, MtChimney_EventScript_SawyerDefeated
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_SawyerRematch
	msgbox MtChimney_Text_SawyerPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_SawyerDefeated::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_SawyerRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SAWYER_1
	release
	end

MtChimney_EventScript_SawyerRematch::
	trainerbattle_rematch TRAINER_SAWYER_1, MtChimney_Text_SawyerRematchIntro, MtChimney_Text_SawyerRematchDefeat
	msgbox MtChimney_Text_SawyerPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_Text_MeteoriteWillActivateVolcano:
	.string "The power inside the METEORITE…\p"
	.string "Boosted by this machine, MT. CHIMNEY's\n"
	.string "volcanic activity will surge all at once!\p"
	.string "The energy will build deep inside the\n"
	.string "crater and…\p"
	.string "Fufufu…$"

MtChimney_Text_MaxieIntro:
	.string "KID BUU: Huh? Who are you?\p"
	.string "… … … … … …\p"
	.string "Oh! DR. GERO whined about a kid who\n"
	.string "keeps getting in our way. Hee hee!\l"
	.string "That's you!\p"
	.string "You came to stop us? To stop my fun?\p"
	.string "My soldiers think this is all about\n"
	.string "making more land for people and\l"
	.string "POKéMON. Hee hee hee!\p"
	.string "I don't care! I want the power sleeping\n"
	.string "inside this mountain! GROUDON will wake\l"
	.string "up and everything will go boom!\p"
	.string "Oops! I told you too much!\p"
	.string "That's okay! I'll teach you what\n"
	.string "happens to kids who spoil my fun!$"

MtChimney_Text_MaxieDefeat:
	.string "What?!\p"
	.string "Me, KID BUU, beaten by a kid?!$"

MtChimney_Text_MaxieYouHaventSeenLastOfMagma:
	.string "KID BUU: Hee hee hee! That was fun, but\n"
	.string "I'm bored now. I'm leaving.\p"
	.string "Don't think this is the last you'll see\n"
	.string "of RED RIBBON ARMY!\p"
	.string "Even without the METEORITE, if we get\n"
	.string "that ORB… Hee hee hee!$"

MtChimney_Text_TabithaIntro:
	.string "Hehehe!\p"
	.string "So you came all the way up here!\p"
	.string "But you're too late! I already delivered\n"
	.string "the METEORITE from METEOR FALLS to\l"
	.string "KID BUU!$"

MtChimney_Text_TabithaDefeat:
	.string "Hehehe…\p"
	.string "Even though I lost, if KID BUU wakes\n"
	.string "that thing…$"

MtChimney_Text_TabithaPostBattle:
	.string "KID BUU, hurry! Pour the METEORITE's\n"
	.string "energy into it!\p"
	.string "Hehehe…$"

MtChimney_Text_Grunt2Intro:
	.string "We of RED RIBBON ARMY work hard for\n"
	.string "everyone's sake!\p"
	.string "If that thing's power made more land,\n"
	.string "there'd be more places to live.\p"
	.string "Everyone would be happy!$"

MtChimney_Text_Grunt2Defeat:
	.string "Huh? What do you mean I lost?$"

MtChimney_Text_Grunt2PostBattle:
	.string "KID BUU promised, “It will make\n"
	.string "everyone happy!”\p"
	.string "So why does everyone keep getting in\n"
	.string "our way?$"

MtChimney_Text_Grunt1Intro:
	.string "If there were more land, I could finally\n"
	.string "have a big house of my own!\p"
	.string "I'm going to build it on hardened lava!$"

MtChimney_Text_Grunt1Defeat:
	.string "My dream of a big house…$"

MtChimney_Text_Grunt1PostBattle:
	.string "Kids like you should be out playing in\n"
	.string "the surf, not wrecking people's dreams!$"

MtChimney_Text_TeamAquaAlwaysMessingWithPlans:
	.string "Those SEA squad pests! They always\n"
	.string "mess with our plans!$"

MtChimney_Text_MeteoritesPackAmazingPower:
	.string "METEORITES pack amazing power! KID BUU\n"
	.string "says so!$"

MtChimney_Text_YouBetterNotMessWithUs:
	.string "Don't you dare mess with us!\p"
	.string "We're waking that thing up for the good\n"
	.string "of everybody!$"

MtChimney_Text_AquasNameSimilar:
	.string "We're the LAND squad of RED RIBBON\n"
	.string "ARMY!\p"
	.string "They're the SEA squad of RED RIBBON\n"
	.string "ARMY!\p"
	.string "Same uniform, same army, and they still\n"
	.string "get in our way! It totally annoys me!$"

MtChimney_Text_DouseThemInFire:
	.string "Yeah! Burn them to a crisp!$"

MtChimney_Text_KeepMakingMoreLand:
	.string "We're going to keep making more land!\n"
	.string "Sir!$"

MtChimney_Text_Bufoh:
	.string "Bufoh! Bufoh!$"

MtChimney_Text_ArchieGoStopTeamMagma:
	.string "DR. GERO: Grr, {PLAYER}! I should have\n"
	.string "guessed you'd show up!\p"
	.string "See for yourself what KID BUU's LAND\n"
	.string "squad fanatics are up to!\p"
	.string "They're pumping the stolen METEORITE's\n"
	.string "power into the volcano!\p"
	.string "Doing that will make the volcano erupt,\n"
	.string "and ruin everything!$"

MtChimney_Text_ArchieIHaveMyHandsFull:
	.string "DR. GERO: Grrr…\p"
	.string "I want to stop that brat KID BUU, but I\n"
	.string "can't!\p"
	.string "Not when I have my hands full fighting\n"
	.string "three opponents at once!$"

MtChimney_Text_ArchieThankYou:
	.string "DR. GERO: {PLAYER}! I'll admit it, you were\n"
	.string "useful.\p"
	.string "Thanks to you, the LAND squad's\n"
	.string "reckless scheme has failed.\p"
	.string "But… whose side are you on, boy?\p"
	.string "Bah, it doesn't matter.\p"
	.string "My SEA squad will stay vigilant and keep\n"
	.string "chasing KID BUU's LAND squad.\p"
	.string "{PLAYER}, we will meet again!$"

MtChimney_Text_MagmaOutnumbersUs:
	.string "Darn… the LAND squad outnumbers us! We\n"
	.string "can't keep up with them!$"

MtChimney_Text_LessHabitatForWaterPokemon:
	.string "If they make more land, there'll be less\n"
	.string "room for WATER POKéMON!$"

MtChimney_Text_MagmasNameSimilar:
	.string "We're the SEA squad of RED RIBBON ARMY!\p"
	.string "They're the LAND squad of RED RIBBON\n"
	.string "ARMY!\p"
	.string "Same uniform, wrong squad! It burns me\n"
	.string "up!$"

MtChimney_Text_Bushaa:
	.string "Bushaa! Bushaa!$"

MtChimney_Text_LavaCookiesJust200:
	.string "LAVA COOKIES! The local specialty of\n"
	.string "MT. CHIMNEY!\p"
	.string "Try one, fresh baked. Just ¥200. They're\n"
	.string "big enough to fill even a hungry Saiyan!$"

MtChimney_Text_ThankYouDear:
	.string "Thank you, dear! Enjoy!$"

MtChimney_Text_YouveNotGotTheMoney:
	.string "Oh, dear. No money, no cookies, I'm\n"
	.string "afraid.$"

MtChimney_Text_OhFineThen:
	.string "Oh, fine then. Come back when you're\n"
	.string "hungry!$"

MtChimney_Text_MeteoriteFittedOnMachine:
	.string "A METEORITE is mounted on a strange\n"
	.string "machine…\p"
	.string "It looks like the machine is storing\n"
	.string "energy in the METEORITE.$"

MtChimney_Text_RemoveTheMeteorite:
	.string "A METEORITE is mounted on a strange\n"
	.string "machine…\p"
	.string "Do you want to remove the METEORITE?$"

MtChimney_Text_PlayerRemovedMeteorite:
	.string "{PLAYER} removed the METEORITE from\n"
	.string "the strange machine.$"

MtChimney_Text_PlayerLeftMeteorite:
	.string "{PLAYER} left the METEORITE right where\n"
	.string "it was.$"

MtChimney_Text_MachineMakesNoResponse:
	.string "This strange machine…\p"
	.string "It doesn't respond at all.$"

MtChimney_Text_RouteSign:
	.string "{DOWN_ARROW} JAGGED PATH\n"
	.string "LAVARIDGE TOWN AHEAD$"

MtChimney_Text_ShelbyIntro:
	.string "A dip in the hot springs has refreshed\n"
	.string "these tired old bones.\p"
	.string "Right now I feel strong enough to take\n"
	.string "on a Saiyan!$"

MtChimney_Text_ShelbyDefeat:
	.string "Oh, my goodness. Now, aren't you\n"
	.string "something!$"

MtChimney_Text_ShelbyPostBattle:
	.string "Well, well, I lost. I can't call myself an\n"
	.string "EXPERT now, can I?$"

MtChimney_Text_ShelbyRegister:
	.string "Thank you, child. It was fun, as if I\n"
	.string "were battling my own grandchild.\p"
	.string "Please come see me again for a rematch.$"

MtChimney_Text_ShelbyRematchIntro:
	.string "When your heart moves as one with your\n"
	.string "POKéMON's, why, there's nothing you\l"
	.string "can't achieve.\p"
	.string "That is the heart of martial arts.$"

MtChimney_Text_ShelbyRematchDefeat:
	.string "Oh, my goodness. Now, aren't you\n"
	.string "something!$"

MtChimney_Text_ShelbyPostRematch:
	.string "Perhaps your spirit has become one with\n"
	.string "the spirits of your POKéMON.$"

MtChimney_Text_MelissaIntro:
	.string "My ki is on fire, baby! I can't stand it!\n"
	.string "I have to battle!$"

MtChimney_Text_MelissaDefeat:
	.string "Ooh, that was a scorching-hot match!$"

MtChimney_Text_MelissaPostBattle:
	.string "The heat of MT. CHIMNEY fires me up,\n"
	.string "baby!$"

MtChimney_Text_SheilaIntro:
	.string "I finally made it to MT. CHIMNEY! I want\n"
	.string "my POKéMON to battle!$"

MtChimney_Text_SheilaDefeat:
	.string "The way you battle… It's like a MT.\n"
	.string "CHIMNEY eruption!$"

MtChimney_Text_SheilaPostBattle:
	.string "Like I said, I finally made it to MT.\n"
	.string "CHIMNEY. It would be a shame to do\l"
	.string "nothing but sightseeing…\p"
	.string "I want to get in some battles and buy\n"
	.string "COOKIES as souvenirs.$"

MtChimney_Text_ShirleyIntro:
	.string "Ever since I soaked in the hot springs,\n"
	.string "I've felt great!\p"
	.string "I'm sure I'm going to win!$"

MtChimney_Text_ShirleyDefeat:
	.string "Yowch! The soak is wearing off and I'm\n"
	.string "getting a chill.$"

MtChimney_Text_ShirleyPostBattle:
	.string "I'll have to take another dip in the hot\n"
	.string "springs. Want to join me?\p"
	.string "Just joking!$"

MtChimney_Text_SawyerIntro:
	.string "This is one fine mountain! Plenty of\n"
	.string "fired-up folks around for company!$"

MtChimney_Text_SawyerDefeat:
	.string "Oh, you're a real firebrand, too!$"

MtChimney_Text_SawyerPostBattle:
	.string "I think I need a dip in the LAVARIDGE\n"
	.string "hot springs with the locals!$"

MtChimney_Text_SawyerRegister:
	.string "I like little fireballs like you. Let me\n"
	.string "register you in my POKéNAV.$"

MtChimney_Text_SawyerRematchIntro:
	.string "I'm happily surrounded by fired-up\n"
	.string "folks around these parts. I won't lose!$"

MtChimney_Text_SawyerRematchDefeat:
	.string "Gosh, you're the same firebrand as ever!$"

MtChimney_Text_SawyerPostRematch:
	.string "Honestly, it really is hot here. I'm\n"
	.string "overdressed for these parts.$"

