PetalburgCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PetalburgCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_PETALBURG_CITY
	call Common_EventScript_UpdateBrineyLocation
	end

PetalburgCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_PETALBURG_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_FatMan::
	msgbox PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Woman::
	lock
	faceplayer
	msgbox PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, IsStarterInParty
	goto_if_eq VAR_RESULT, TRUE, PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo::
	call_if_eq VAR_STARTER_MON, 0, PetalburgCity_PokemonCenter_1F_EventScript_SayTreeckoType
	call_if_eq VAR_STARTER_MON, 1, PetalburgCity_PokemonCenter_1F_EventScript_SayTorchicType
	call_if_eq VAR_STARTER_MON, 2, PetalburgCity_PokemonCenter_1F_EventScript_SayMudkipType
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayTreeckoType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_TreeckoIsGrassType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayTorchicType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_TorchicIsFireType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayMudkipType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_MudkipIsWaterType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem:
	.string "That PC POKéMON Storage System…\p"
	.string "Whoever built it has to be a genius\n"
	.string "on par with the CAPSULE CORP. crowd!$"

PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP:
	.string "My POKéMON ate an ORAN BERRY and\n"
	.string "recovered its HP right away!$"

PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon:
	.string "POKéMON come in many types.\p"
	.string "Every type has strengths and\n"
	.string "weaknesses against the others.\p"
	.string "Depending on the types in play,\n"
	.string "a battle can be easy or tough.$"

PetalburgCity_PokemonCenter_1F_Text_TreeckoIsGrassType:
	.string "Take your TREECKO, for example.\n"
	.string "It's a GRASS type.\p"
	.string "It's strong against WATER and\n"
	.string "GROUND types.\p"
	.string "But it's weak against FIRE-type\n"
	.string "POKéMON.$"

PetalburgCity_PokemonCenter_1F_Text_TorchicIsFireType:
	.string "Take your TORCHIC, for example.\n"
	.string "It's a FIRE type.\p"
	.string "It's strong against GRASS and\n"
	.string "BUG types.\p"
	.string "But it's weak against WATER-type\n"
	.string "POKéMON.$"

PetalburgCity_PokemonCenter_1F_Text_MudkipIsWaterType:
	.string "Take your MUDKIP, for example.\n"
	.string "It's a WATER type.\p"
	.string "It's strong against the FIRE type.\p"
	.string "But it's weak against GRASS-type\n"
	.string "and ELECTRIC-type POKéMON.$"

