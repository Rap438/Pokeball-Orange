SootopolisCity_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SootopolisCity_Gym_1F_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, SootopolisCity_Gym_1F_OnResume
	map_script MAP_SCRIPT_ON_LOAD, SootopolisCity_Gym_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, SootopolisCity_Gym_1F_OnTransition
	.byte 0

SootopolisCity_Gym_1F_OnTransition:
	setvar VAR_ICE_STEP_COUNT, 1
	end

SootopolisCity_Gym_1F_OnResume:
	setstepcallback STEP_CB_SOOTOPOLIS_ICE
	end

SootopolisCity_Gym_1F_OnLoad:
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special SetSootopolisGymCrackedIceMetatiles
	end

SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles::
	goto_if_lt VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_StopCheckingStairs  @ All stairs ice
	goto_if_lt VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_OpenFirstStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs
	setmetatile 8, 4, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 5, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs::
	setmetatile 8, 10, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 11, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstStairs::
	setmetatile 8, 15, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 16, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_StopCheckingStairs::
	return

SootopolisCity_Gym_1F_OnFrame:
	map_script_2 VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs
	map_script_2 VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs
	map_script_2 VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs
	map_script_2 VAR_ICE_STEP_COUNT, 0, SootopolisCity_Gym_1F_EventScript_FallThroughIce
	.2byte 0

SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_FallThroughIce::
	lockall
	delay 20
	applywaitmovement LOCALID_PLAYER, SootopolisCity_Gym_1F_Movement_FallThroughIce
	playse SE_FALL
	delay 60
	warphole MAP_SOOTOPOLIS_CITY_GYM_B1F
	waitstate
	end

SootopolisCity_Gym_1F_Movement_FallThroughIce:
	set_invisible
	step_end

SootopolisCity_Gym_1F_EventScript_Juan::
	trainerbattle_single TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanIntro, SootopolisCity_Gym_1F_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, SootopolisCity_Gym_1F_EventScript_JuanRematch
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeated::
	message SootopolisCity_Gym_1F_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_SOOTOPOLIS_GYM
	setflag FLAG_BADGE08_GET
	special DBZ_QueueGymFight
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_WALLACE
	setvar VAR_SOOTOPOLIS_CITY_STATE, 6
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1
	setvar VAR_0x8008, 8
	call Common_EventScript_SetGymTrainers
	call SootopolisCity_Gym_1F_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SootopolisCity_Gym_1F_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	return

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	release
	end

SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge::
	msgbox SootopolisCity_Gym_1F_Text_GoGetFortreeBadge, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanRematch::
	trainerbattle_rematch_double TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanPreRematch, SootopolisCity_Gym_1F_Text_JuanRematchDefeat, SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons
	msgbox SootopolisCity_Gym_1F_Text_JuanPostRematch, MSGBOX_AUTOCLOSE
	end

SootopolisCity_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SOOTOPOLIS_GYM, SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory
	msgbox SootopolisCity_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox SootopolisCity_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_GymStatueCertified::
	msgbox SootopolisCity_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_EventScript_GymStatue::
	msgbox SootopolisCity_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_Text_GymGuideAdvice:
	.string "Yo! How's it going, CHAMPION-bound\n"
	.string "{PLAYER}?\p"
	.string "SOOTOPOLIS's GYM LEADER PICCOLO is a\n"
	.string "master of WATER-type POKéMON.\p"
	.string "And to get to PICCOLO, an icy floor will\n"
	.string "slow you down…\p"
	.string "Sorry, but that's all the advice I have\n"
	.string "for you.\p"
	.string "The rest of the way, you have to go for\n"
	.string "it yourself!$"

SootopolisCity_Gym_1F_Text_GymGuidePostVictory:
	.string "Yow! You beat even PICCOLO, the best in\n"
	.string "all HOENN!\p"
	.string "Okay! Check out your TRAINER CARD.\p"
	.string "If you've gotten all the BADGES, you're\n"
	.string "set for the POKéMON LEAGUE challenge!$"

SootopolisCity_Gym_1F_Text_JuanIntro:
	.string "So. You have come.\p"
	.string "I am PICCOLO. Once I was your rival. Now\n"
	.string "I stand here as SOOTOPOLIS's GYM\l"
	.string "LEADER.\p"
	.string "That loud fool MR. SATAN calls me his\n"
	.string "teacher. All I taught him was how to\l"
	.string "stay alive.\p"
	.string "I gave up this GYM once and left it in\n"
	.string "his hands.\p"
	.string "But a reason arose for me to return.\p"
	.string "Enough talk.\p"
	.string "Water, shaped by my POKéMON and my own\n"
	.string "two hands, will show you what true\l"
	.string "discipline is!$"

@ NOTE: This defeat text actually causes a buffer overflow. It's about 50 bytes too long for
@ the gDisplayedStringBattle buffer that it's put into, and it stomps all over the gBattleTextBuffs
@ after, as well as the otherwise unused array after that. One wonders if that's the reason for
@ the existence of that unused array of ints.
SootopolisCity_Gym_1F_Text_JuanDefeat:
	.string "Hmph… Impressive. You are the winner.\p"
	.string "I sense in you a brilliant skill that will\n"
	.string "overcome all.\p"
	.string "But compared with me, or even MR. SATAN,\n"
	.string "you still lack discipline.\p"
	.string "Perhaps I should lend you my cape and\n"
	.string "turban?\p"
	.string "… … … … … …\p"
	.string "… … … … … …\p"
	.string "Hah. A joke.\p"
	.string "Instead of my clothes, take this, the\n"
	.string "RAIN BADGE!$"

SootopolisCity_Gym_1F_Text_ReceivedRainBadge:
	.string "{PLAYER} received the RAIN BADGE from\n"
	.string "PICCOLO.$"

SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis:
	.string "The RAIN BADGE ensures all your\n"
	.string "POKéMON will obey your every command.\p"
	.string "It also lets you use the HM move\n"
	.string "WATERFALL to climb walls of cascading\l"
	.string "water.\p"
	.string "And so you never forget the battle we\n"
	.string "shared, take this…$"

SootopolisCity_Gym_1F_Text_ExplainWaterPulse:
	.string "The TECHNICAL MACHINE I gave you\n"
	.string "contains WATER PULSE.\p"
	.string "When used, it will occasionally confuse\n"
	.string "the target with ultrasonic waves.\p"
	.string "… … … … … …$"

SootopolisCity_Gym_1F_Text_RegisteredJuan:
	.string "Registered GYM LEADER PICCOLO in the\n"
	.string "POKéNAV.$"

SootopolisCity_Gym_1F_Text_JuanPostBattle:
	.string "TRAINERS who have gathered all the GYM\n"
	.string "BADGES of HOENN must head for the\l"
	.string "ultimate destination.\p"
	.string "The POKéMON LEAGUE.\p"
	.string "Travel to the easternmost reaches of\n"
	.string "HOENN, to the island EVER GRANDE.\p"
	.string "There you will find the POKéMON LEAGUE.\n"
	.string "Go. Train harder.$"

SootopolisCity_Gym_1F_Text_GoGetFortreeBadge:
	.string "Only one BADGE remains to be obtained\n"
	.string "in HOENN.\p"
	.string "If you wish to challenge the POKéMON\n"
	.string "LEAGUE, you must earn the last BADGE at\l"
	.string "the GYM in FORTREE.\p"
	.string "The leader there strikes poses, but do\n"
	.string "not underestimate him.$"

SootopolisCity_Gym_1F_Text_GymStatue:
	.string "SOOTOPOLIS CITY POKéMON GYM$"

SootopolisCity_Gym_1F_Text_GymStatueCertified:
	.string "SOOTOPOLIS CITY POKéMON GYM\p"
	.string "PICCOLO'S CERTIFIED TRAINERS:\p"
	.string "{PLAYER}$"

SootopolisCity_Gym_1F_Text_JuanPreRematch:
	.string "PICCOLO: Hmph. This GYM had returned to\n"
	.string "its usual calm…\p"
	.string "But the storm has come back to test us\n"
	.string "again!\p"
	.string "Very well.\p"
	.string "I will gladly spar with you as often as\n"
	.string "you wish!$"

SootopolisCity_Gym_1F_Text_JuanRematchDefeat:
	.string "Hmph. You win.\p"
	.string "You have defeated me again!$"

SootopolisCity_Gym_1F_Text_JuanPostRematch:
	.string "PICCOLO: If I told you to become my\n"
	.string "student, you would refuse, I am sure.\p"
	.string "I would like to give you my cape. But\n"
	.string "again, you would refuse.\p"
	.string "That, my friend, is the sign of a true\n"
	.string "warrior!$"

SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons:
	.string "PICCOLO: Hmph. This GYM had returned to\n"
	.string "its usual calm…\p"
	.string "But the storm has come back to test us\n"
	.string "again!\p"
	.string "Very well. I will gladly spar with you…\p"
	.string "No. You have with you but one POKéMON.\p"
	.string "Return with two, perhaps more, POKéMON.$"

