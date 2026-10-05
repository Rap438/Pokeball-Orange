Route119_WeatherInstitute_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route119_WeatherInstitute_2F_OnTransition
	.byte 0

Route119_WeatherInstitute_2F_OnTransition:
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaHere
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 1, Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaGone
	call_if_set FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_2F_EventScript_SetScientistPosGameClear
	end

Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaHere::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, 1, 6
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaGone::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, 4, 6
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_2F_EventScript_SetScientistPosGameClear::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, 2, 2
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, MOVEMENT_TYPE_FACE_UP
	return

Route119_WeatherInstitute_2F_EventScript_Grunt5::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_5, Route119_WeatherInstitute_2F_Text_Grunt5Intro, Route119_WeatherInstitute_2F_Text_Grunt5Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_2, Route119_WeatherInstitute_2F_Text_Grunt2Intro, Route119_WeatherInstitute_2F_Text_Grunt2Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_3, Route119_WeatherInstitute_2F_Text_Grunt3Intro, Route119_WeatherInstitute_2F_Text_Grunt3Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Shelly::
	trainerbattle_single TRAINER_SHELLY_WEATHER_INSTITUTE, Route119_WeatherInstitute_2F_Text_ShellyIntro, Route119_WeatherInstitute_2F_Text_ShellyDefeat, Route119_WeatherInstitute_2F_EventScript_ShellyDefeated
	msgbox Route119_WeatherInstitute_2F_Text_ShellyPostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_ShellyDefeated::
	msgbox Route119_WeatherInstitute_2F_Text_ShellyPostBattle, MSGBOX_DEFAULT
	closemessage
	addobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_3
	applymovement LOCALID_WEATHER_INSTITUTE_2F_GRUNT_3, Route119_WeatherInstitute_2F_Movement_GruntApproachShelly
	applymovement LOCALID_PLAYER, Route119_WeatherInstitute_2F_Movement_ShovePlayerOutOfWay
	waitmovement 0
	msgbox Route119_WeatherInstitute_2F_Text_TeamMagmaJustPassedBy, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applywaitmovement LOCALID_WEATHER_INSTITUTE_2F_SHELLY, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_WEATHER_INSTITUTE_2F_SHELLY, Common_Movement_Delay48
	msgbox Route119_WeatherInstitute_2F_Text_WeHaveToHurryToMtPyre, MSGBOX_DEFAULT
	closemessage
	setvar VAR_WEATHER_INSTITUTE_STATE, 1
	clearflag FLAG_HIDE_WEATHER_INSTITUTE_2F_WORKERS
	fadedefaultbgm
	fadescreen FADE_TO_BLACK
	setflag FLAG_HIDE_ROUTE_119_TEAM_AQUA
	setflag FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_1
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_2
	removeobject LOCALID_WEATHER_INSTITUTE_2F_SHELLY
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_4
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_3
	applywaitmovement LOCALID_PLAYER, Route119_WeatherInstitute_2F_Movement_PlayerReturnToPosition
	fadescreen FADE_FROM_BLACK
	applywaitmovement LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, Route119_WeatherInstitute_2F_Movement_ScientistApproachPlayer
	copyobjectxytoperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST
	goto Route119_WeatherInstitute_2F_EventScript_ReceiveCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastform::
	msgbox Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemon, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CASTFORM_NORMAL
	givemon SPECIES_CASTFORM_NORMAL, 25, ITEM_MYSTIC_WATER
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC
	goto Common_EventScript_NoMoreRoomForPokemon
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty::
	call Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC::
	call Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route119_WeatherInstitute_2F_EventScript_SendCastformToPC
	call Common_EventScript_NameReceivedBoxMon
	goto Route119_WeatherInstitute_2F_EventScript_SendCastformToPC
	end

Route119_WeatherInstitute_2F_EventScript_SendCastformToPC::
	call Common_EventScript_TransferredToPC
	goto Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare::
	playfanfare MUS_OBTAIN_ITEM
	message Route119_WeatherInstitute_2F_Text_PlayerReceivedCastform
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_CASTFORM_NORMAL
	return

Route119_WeatherInstitute_2F_EventScript_ExplainCastform::
	msgbox Route119_WeatherInstitute_2F_Text_PokemonChangesWithWeather, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_CASTFORM
	release
	end

Route119_WeatherInstitute_2F_EventScript_ScientistMentionWeather::
	goto_if_set FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_2F_EventScript_TryStartAbnormalWeather
	msgbox Route119_WeatherInstitute_2F_Text_ChangingWeatherRidiculous, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_TryStartAbnormalWeather::
	setvar VAR_0x8004, 0
	call_if_set FLAG_DEFEATED_KYOGRE, Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated
	call_if_set FLAG_DEFEATED_GROUDON, Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated
	goto_if_eq VAR_0x8004, 2, Route119_WeatherInstitute_2F_EventScript_NoAbnormalWeather  @ Both defeated
	call_if_unset FLAG_TEMP_2, Route119_WeatherInstitute_2F_EventScript_CreateAbnormalWeather
	specialvar VAR_RESULT, GetAbnormalWeatherMapNameAndType
	goto_if_eq VAR_RESULT, 1, Route119_WeatherInstitute_2F_EventScript_KyogreWeather
	msgbox Route119_WeatherInstitute_2F_Text_GroudonWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_KyogreWeather::
	msgbox Route119_WeatherInstitute_2F_Text_KyogreWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_CreateAbnormalWeather::
	special CreateAbnormalWeatherEvent
	setflag FLAG_TEMP_2
	return

Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated::
	addvar VAR_0x8004, 1
	return

Route119_WeatherInstitute_2F_EventScript_NoAbnormalWeather::
	msgbox Route119_WeatherInstitute_2F_Text_NoAbnormalWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_Movement_GruntApproachShelly:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

Route119_WeatherInstitute_2F_Movement_ShovePlayerOutOfWay:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	ride_water_current_up
	walk_in_place_faster_down
	step_end

Route119_WeatherInstitute_2F_Movement_PlayerReturnToPosition:
	slide_down
	walk_in_place_faster_left
	step_end

Route119_WeatherInstitute_2F_Movement_ScientistApproachPlayer:
	walk_right
	walk_right
	walk_right
	step_end

Route119_WeatherInstitute_2F_EventScript_WeatherScientist::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CASTFORM, Route119_WeatherInstitute_2F_EventScript_ScientistMentionWeather
	goto Route119_WeatherInstitute_2F_EventScript_ReceiveCastform
	end

Route119_WeatherInstitute_2F_Text_Grunt2Intro:
	.string "The INSTITUTE created a POKéMON\n"
	.string "that's tied to the weather.\p"
	.string "DR. GERO wants it, so we're taking it!$"

Route119_WeatherInstitute_2F_Text_Grunt2Defeat:
	.string "Our plan is being ruined by one guy?!$"

Route119_WeatherInstitute_2F_Text_Grunt2PostBattle:
	.string "If the POKéMON they made here can\n"
	.string "control the weather freely, then the\l"
	.string "RED RIBBON ARMY definitely needs it for\l"
	.string "the, uh… the big plan!$"

Route119_WeatherInstitute_2F_Text_Grunt3Intro:
	.string "We are the RED RIBBON ARMY!\p"
	.string "Wherever something rare turns up, there\n"
	.string "we are!$"

Route119_WeatherInstitute_2F_Text_Grunt3Defeat:
	.string "Ouch! You got me!$"

Route119_WeatherInstitute_2F_Text_Grunt3PostBattle:
	.string "You've got no idea what the RED RIBBON\n"
	.string "ARMY is working toward!\p"
	.string "Stay out of our way!$"

Route119_WeatherInstitute_2F_Text_Grunt5Intro:
	.string "Don't tell me you're after that weather\n"
	.string "POKéMON, too?!\p"
	.string "No way! We got here first, so back off!$"

Route119_WeatherInstitute_2F_Text_Grunt5Defeat:
	.string "Aw, look at my poor POKéMON!$"

Route119_WeatherInstitute_2F_Text_Grunt5PostBattle:
	.string "Hmph, so what?\n"
	.string "What we really want isn't here!\p"
	.string "Ihihihihi…$"

Route119_WeatherInstitute_2F_Text_ShellyIntro:
	.string "Ahahahaha!\p"
	.string "You dare meddle in the RED RIBBON\n"
	.string "ARMY's affairs, right under COLONEL\l"
	.string "VIOLET's nose?\p"
	.string "You're either totally fearless,\n"
	.string "hopelessly ignorant, or both!\p"
	.string "You're so cute, you're disgusting! I'll\n"
	.string "put you down, kiddy!$"

Route119_WeatherInstitute_2F_Text_ShellyDefeat:
	.string "Ahahahaha!\n"
	.string "Disgustingly strong! Marvelous!$"

Route119_WeatherInstitute_2F_Text_ShellyPostBattle:
	.string "It's bad enough that the LAND squad\n"
	.string "keeps blundering around, begging KID\l"
	.string "BUU for attention, but now there's you!\p"
	.string "What makes you want to sniff around in\n"
	.string "our business, anyway?$"

Route119_WeatherInstitute_2F_Text_TeamMagmaJustPassedBy:
	.string "We have a situation here!\p"
	.string "A mob of RED RIBBON soldiers in red\n"
	.string "uniforms just marched past the WEATHER\l"
	.string "INSTITUTE.\p"
	.string "They seem to be heading for MT. PYRE!$"

Route119_WeatherInstitute_2F_Text_WeHaveToHurryToMtPyre:
	.string "What?!\p"
	.string "We can't waste any more time here! We\n"
	.string "have to hurry to MT. PYRE, too!\p"
	.string "Ahahahaha!\n"
	.string "LAND squad, just you wait!$"

Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemon:
	.string "Thank you!\n"
	.string "Thanks to you, we're safe!\p"
	.string "It may be an odd way to say thanks, but\n"
	.string "please take this POKéMON.$"

@ Unused
Route119_WeatherInstitute_2F_Text_NoRoomForPokemon:
	.string "Hm? Your party is full, so I can't hand\n"
	.string "over this POKéMON.$"

Route119_WeatherInstitute_2F_Text_PlayerReceivedCastform:
	.string "{PLAYER} received CASTFORM!$"

Route119_WeatherInstitute_2F_Text_PokemonChangesWithWeather:
	.string "That POKéMON changes shape to match\n"
	.string "the weather conditions.\p"
	.string "There are plenty of them in the\n"
	.string "INSTITUTE, so go ahead and take it.$"

Route119_WeatherInstitute_2F_Text_ChangingWeatherRidiculous:
	.string "I've studied rain for many years, and\n"
	.string "the idea that anyone could change the\l"
	.string "weather at will is absurd.\p"
	.string "Then again, I did once see a fighter\n"
	.string "scatter a thunderstorm with a single\l"
	.string "punch…$"

Route119_WeatherInstitute_2F_Text_GroudonWeather:
	.string "I monitor the weather across the HOENN\n"
	.string "region.\p"
	.string "Right now, a drought is being recorded\n"
	.string "around {STR_VAR_1}.\p"
	.string "Could it be that something is stirring\n"
	.string "near {STR_VAR_1}…$"

Route119_WeatherInstitute_2F_Text_KyogreWeather:
	.string "I monitor the weather across the HOENN\n"
	.string "region.\p"
	.string "Right now, torrential rain is being\n"
	.string "recorded over {STR_VAR_1}.\p"
	.string "Could it be that something is stirring\n"
	.string "near {STR_VAR_1}…$"

Route119_WeatherInstitute_2F_Text_NoAbnormalWeather:
	.string "No more abnormal weather is being\n"
	.string "reported.\p"
	.string "A little rain now and then is a blessing,\n"
	.string "truly.$"

