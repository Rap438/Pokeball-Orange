OldaleTown_House2_MapScripts::
	.byte 0

OldaleTown_House2_EventScript_Woman::
	msgbox OldaleTown_House2_Text_PokemonLevelUp, MSGBOX_NPC
	end

OldaleTown_House2_EventScript_Man::
	msgbox OldaleTown_House2_Text_YoullGoFurtherWithStrongPokemon, MSGBOX_NPC
	end

OldaleTown_House2_Text_PokemonLevelUp:
	.string "When POKéMON battle, they gain\n"
	.string "experience, level up and grow stronger.\p"
	.string "Just like martial artists do!$"

OldaleTown_House2_Text_YoullGoFurtherWithStrongPokemon:
	.string "The stronger your POKéMON get, the\n"
	.string "farther from home you can safely go.$"

