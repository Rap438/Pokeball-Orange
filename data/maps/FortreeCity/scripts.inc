FortreeCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, FortreeCity_OnResume
	.byte 0

FortreeCity_OnTransition:
	setflag FLAG_VISITED_FORTREE_CITY
	end

FortreeCity_OnResume:
	setstepcallback STEP_CB_FORTREE_BRIDGE
	end

FortreeCity_EventScript_Man::
	msgbox FortreeCity_Text_SawGiganticPokemonInSky, MSGBOX_NPC
	end

FortreeCity_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_KECLEON_FLED_FORTREE, FortreeCity_EventScript_WomanGymAccessible
	msgbox FortreeCity_Text_SomethingBlockingGym, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_WomanGymAccessible::
	msgbox FortreeCity_Text_ThisTimeIllBeatWinona, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_Girl::
	msgbox FortreeCity_Text_TreesGrowByDrinkingRainwater, MSGBOX_NPC
	end

FortreeCity_EventScript_OldMan::
	msgbox FortreeCity_Text_EveryoneHealthyAndLively, MSGBOX_NPC
	end

FortreeCity_EventScript_Boy::
	msgbox FortreeCity_Text_BugPokemonComeThroughWindow, MSGBOX_NPC
	end

FortreeCity_EventScript_GameboyKid::
	msgbox FortreeCity_Text_PokemonThatEvolveWhenTraded, MSGBOX_NPC
	end

FortreeCity_EventScript_CitySign::
	msgbox FortreeCity_Text_CitySign, MSGBOX_SIGN
	end

FortreeCity_EventScript_GymSign::
	msgbox FortreeCity_Text_GymSign, MSGBOX_SIGN
	end

FortreeCity_EventScript_Kecleon::
	lock
	faceplayer
	checkitem ITEM_DEVON_SCOPE
	goto_if_eq VAR_RESULT, TRUE, FortreeCity_EventScript_AskUseDevonScope
	msgbox FortreeCity_Text_SomethingUnseeable, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_AskUseDevonScope::
	msgbox FortreeCity_Text_UnseeableUseDevonScope, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FortreeCity_EventScript_UseDevonScope
	release
	end

FortreeCity_EventScript_UseDevonScope::
	msgbox FortreeCity_Text_UsedDevonScopePokemonFled, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Movement_KecleonAppears
	waitse
	playmoncry SPECIES_KECLEON, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	applywaitmovement VAR_LAST_TALKED, FortreeCity_Movement_KecleonFlee
	removeobject VAR_LAST_TALKED
	setflag FLAG_KECLEON_FLED_FORTREE
	release
	end

FortreeCity_Movement_KecleonFlee:
	walk_right
	step_end

FortreeCity_Text_SawGiganticPokemonInSky:
	.string "Nobody believes me, but I saw it. A\n"
	.string "gigantic, snaking shape coiling across\l"
	.string "the sky like a dragon out of a legend.\p"
	.string "It was heading toward ROUTE 131.\p"
	.string "By the way… Sniff…\n"
	.string "You smell a little scorched, you know.\p"
	.string "Were you fighting inside a volcano or\n"
	.string "something?$"

FortreeCity_Text_SomethingBlockingGym:
	.string "I'm dying to challenge the GYM, but\n"
	.string "something is blocking the way.\p"
	.string "I trained so hard on ROUTE 120 for this,\n"
	.string "and I can't even sense what's in the\l"
	.string "way!$"

FortreeCity_Text_ThisTimeIllBeatWinona:
	.string "My best POKéMON and I trained all week\n"
	.string "for this.\p"
	.string "This time we're going to beat the GREAT\n"
	.string "SAIYAMAN!$"

FortreeCity_Text_TreesGrowByDrinkingRainwater:
	.string "Rain soaks into the soil, and the trees\n"
	.string "drink it up and grow tall.\p"
	.string "FORTREE CITY only exists because we\n"
	.string "have both good water and good soil.$"

FortreeCity_Text_EveryoneHealthyAndLively:
	.string "Our homes sit up in the trees.\p"
	.string "Maybe that's why everyone here is so\n"
	.string "healthy and lively.\p"
	.string "Why, even I feel thirty years younger!\n"
	.string "This air is as good as a SENZU SPROUT.$"

FortreeCity_Text_BugPokemonComeThroughWindow:
	.string "Living in the treetops is fine.\p"
	.string "But now and then a BUG POKéMON climbs\n"
	.string "in through the window. It startles me\l"
	.string "every time, like a SAIBAMAN jumping out\l"
	.string "of the bushes!$"

FortreeCity_Text_PokemonThatEvolveWhenTraded:
	.string "I heard some POKéMON only evolve when\n"
	.string "you trade them to someone else!\p"
	.string "I guess swapping partners shakes them\n"
	.string "up, like sparring with a new rival.$"

FortreeCity_Text_SomethingUnseeable:
	.string "Something invisible is in the way.\n"
	.string "I can't make it out.$"

FortreeCity_Text_UnseeableUseDevonScope:
	.string "Something invisible is in the way.\p"
	.string "Want to use the SCOUTER?$"

FortreeCity_Text_UsedDevonScopePokemonFled:
	.string "{PLAYER} put on the SCOUTER.\p"
	.string "It beeped, and an invisible POKéMON\n"
	.string "flickered into view!\p"
	.string "The startled POKéMON bolted away!$"

FortreeCity_Text_CitySign:
	.string "FORTREE CITY “The city in the trees,\n"
	.string "where people and nature live as one.”$"

FortreeCity_Text_GymSign:
	.string "FORTREE CITY POKéMON GYM\n"
	.string "LEADER: GREAT SAIYAMAN\p"
	.string "“Fighting for justice on the wings of\n"
	.string "BIRD POKéMON!”$"

