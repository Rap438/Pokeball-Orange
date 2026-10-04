FortreeCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

FortreeCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_FORTREE_CITY
	end

FortreeCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_FORTREE_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

FortreeCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox FortreeCity_PokemonCenter_1F_Text_GoToSafariZone, MSGBOX_NPC
	end

FortreeCity_PokemonCenter_1F_EventScript_Man::
	msgbox FortreeCity_PokemonCenter_1F_Text_RecordCornerIsNeat, MSGBOX_NPC
	end

FortreeCity_PokemonCenter_1F_EventScript_Boy::
	msgbox FortreeCity_PokemonCenter_1F_Text_DoYouKnowAboutPokenav, MSGBOX_NPC
	end

FortreeCity_PokemonCenter_1F_Text_GoToSafariZone:
	.string "Hey, kid. Are you filling out a POKéDEX?\p"
	.string "Hmm… Then head for the SAFARI ZONE.\n"
	.string "Plenty of rare POKéMON there. That's my\l"
	.string "advice.$"

FortreeCity_PokemonCenter_1F_Text_RecordCornerIsNeat:
	.string "Have you tried the RECORD CORNER yet?\p"
	.string "It mixes and matches the records of\n"
	.string "TRAINERS.\p"
	.string "I don't quite know how it works, but it's\n"
	.string "a blast!$"

FortreeCity_PokemonCenter_1F_Text_DoYouKnowAboutPokenav:
	.string "Oh, wow, you have a POKéNAV! Just like\n"
	.string "mine!\p"
	.string "Do you know about the POKéNAV's MATCH\n"
	.string "CALL system?\p"
	.string "Access it, and you can chat with\n"
	.string "registered TRAINERS anytime.\p"
	.string "It also shows you which TRAINERS want\n"
	.string "a rematch with you.\p"
	.string "It's really nifty! Those CAPSULE CORP.\n"
	.string "geniuses can build anything!$"

