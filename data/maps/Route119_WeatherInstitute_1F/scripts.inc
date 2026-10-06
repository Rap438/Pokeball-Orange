Route119_WeatherInstitute_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route119_WeatherInstitute_1F_OnTransition
	.byte 0

Route119_WeatherInstitute_1F_OnTransition:
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_1F_EventScript_SetLittleBoyPos
	end

Route119_WeatherInstitute_1F_EventScript_SetLittleBoyPos::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_LITTLE_BOY, 0, 5
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_LITTLE_BOY, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_1F_EventScript_LittleBoy::
	lock
	faceplayer
	special GetPlayerBigGuyGirlString
	goto_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_1F_EventScript_LittleBoyTeamAquaHere
	msgbox Route119_WeatherInstitute_1F_Text_WowYoureStrong, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_LittleBoyTeamAquaHere::
	msgbox Route119_WeatherInstitute_1F_Text_EveryoneWentUpstairs, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_InstituteWorker1::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_1F_EventScript_StudyingRain
	setvar VAR_0x8004, 0
	call_if_set FLAG_DEFEATED_KYOGRE, Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated
	call_if_set FLAG_DEFEATED_GROUDON, Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated
	goto_if_eq VAR_0x8004, 2, Route119_WeatherInstitute_1F_EventScript_StudyingRain  @ Both defeated
	msgbox Route119_WeatherInstitute_1F_Text_NoticingAbnormalWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated::
	addvar VAR_0x8004, 1
	return

Route119_WeatherInstitute_1F_EventScript_StudyingRain::
	msgbox Route119_WeatherInstitute_1F_Text_ProfStudyingRain, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_InstituteWorker2::
	msgbox Route119_WeatherInstitute_1F_Text_WhatWereAquasUpTo, MSGBOX_NPC
	end

Route119_WeatherInstitute_1F_EventScript_Bed::
	lockall
	msgbox Route119_WeatherInstitute_1F_Text_TakeRestInBed, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	releaseall
	end

Route119_WeatherInstitute_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_1, Route119_WeatherInstitute_1F_Text_Grunt1Intro, Route119_WeatherInstitute_1F_Text_Grunt1Defeat
	msgbox Route119_WeatherInstitute_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_1F_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_4, Route119_WeatherInstitute_1F_Text_Grunt4Intro, Route119_WeatherInstitute_1F_Text_Grunt4Defeat
	msgbox Route119_WeatherInstitute_1F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_1F_Text_Grunt1Intro:
	.string "DR. GERO took a keen interest in the\n"
	.string "research going on here, so he sent the\l"
	.string "SEA squad!\p"
	.string "Sir, yes sir! Now quit meddling!$"

Route119_WeatherInstitute_1F_Text_Grunt1Defeat:
	.string "Dang it…\n"
	.string "Defeated by a guy in a gi, sir…$"

Route119_WeatherInstitute_1F_Text_Grunt1PostBattle:
	.string "DR. GERO always knows what's going on.\p"
	.string "But I'm just a soldier. How would I know\n"
	.string "what he's thinking?$"

Route119_WeatherInstitute_1F_Text_Grunt4Intro:
	.string "Huh? What's a guy in a gi doing here?\n"
	.string "Is this a field trip?$"

Route119_WeatherInstitute_1F_Text_Grunt4Defeat:
	.string "Wha?!\n"
	.string "I lost?!$"

Route119_WeatherInstitute_1F_Text_Grunt4PostBattle:
	.string "Oh, no… I'm going to catch an earful for\n"
	.string "losing to a guy in a gi…\p"
	.string "I'm taking a nap in that bed. Wake me\n"
	.string "never.$"

Route119_WeatherInstitute_1F_Text_EveryoneWentUpstairs:
	.string "Huh?! I dozed off for one minute, and\n"
	.string "everybody went upstairs!$"

Route119_WeatherInstitute_1F_Text_WowYoureStrong:
	.string "Wow, your power level must be off the\n"
	.string "charts!\p"
	.string "I wish I could be a POKéMON TRAINER like\n"
	.string "you!$"

Route119_WeatherInstitute_1F_Text_ProfStudyingRain:
	.string "The PROFESSOR loves rain. That's a fact.\p"
	.string "The seasons turn in HOENN, and the land\n"
	.string "changes color with each one. Another\l"
	.string "fact!\p"
	.string "Some POKéMON from faraway regions even\n"
	.string "visit as seasonal guests. Fact!\p"
	.string "But if it never stops raining, people\n"
	.string "are in trouble. That's a fact, too.\p"
	.string "So the PROFESSOR is studying whether\n"
	.string "rain can be put to good use.$"

Route119_WeatherInstitute_1F_Text_NoticingAbnormalWeather:
	.string "On the 2nd floor of the INSTITUTE, we\n"
	.string "study the weather patterns over the\l"
	.string "HOENN region.\p"
	.string "Lately we've been noticing sudden,\n"
	.string "isolated droughts and torrential rain…$"

Route119_WeatherInstitute_1F_Text_WhatWereAquasUpTo:
	.string "Hello! You saved us!\p"
	.string "What I still don't understand is why the\n"
	.string "RED RIBBON ARMY wanted our weather\l"
	.string "data so badly.$"

Route119_WeatherInstitute_1F_Text_TakeRestInBed:
	.string "There's a bed here…\n"
	.string "Time for a short rest.$"

