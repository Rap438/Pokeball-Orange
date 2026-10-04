MossdeepCity_House1_MapScripts::
	.byte 0

MossdeepCity_House1_EventScript_BlackBelt::
	lock
	faceplayer
	bufferleadmonspeciesname STR_VAR_1
	msgbox MossdeepCity_House1_Text_HmmYourPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, GetPokeblockNameByMonNature
	goto_if_eq VAR_RESULT, 0, MossdeepCity_House1_EventScript_NeutralNature
	msgbox MossdeepCity_House1_Text_ItLikesXPokeblocks, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House1_EventScript_NeutralNature::
	msgbox MossdeepCity_House1_Text_DoesntLikeOrDislikePokeblocks, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House1_EventScript_Woman::
	msgbox MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes, MSGBOX_NPC
	end

MossdeepCity_House1_Text_HmmYourPokemon:
	.string "Mmm!\n"
	.string "Your {STR_VAR_1}…$"

MossdeepCity_House1_Text_ItLikesXPokeblocks:
	.string "It likes {STR_VAR_1}S,\n"
	.string "doesn't it?\p"
	.string "No, I'm certain of it! It definitely likes\n"
	.string "{STR_VAR_1}S!$"

MossdeepCity_House1_Text_DoesntLikeOrDislikePokeblocks:
	.string "It doesn't seem to like or dislike any\n"
	.string "{POKEBLOCK}S.$"

MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes:
	.string "My husband can sense what kind of\n"
	.string "{POKEBLOCK}S a POKéMON likes at a glance.\l"
	.string "He says he's reading its ki.$"

