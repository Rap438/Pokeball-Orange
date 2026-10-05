FallarborTown_Mart_MapScripts::
	.byte 0

FallarborTown_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart FallarborTown_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
FallarborTown_Mart_Pokemart:
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_X_SP_ATK
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_GUARD_SPEC
	pokemartlistend

FallarborTown_Mart_EventScript_Woman::
	msgbox FallarborTown_Mart_Text_DecidingSkittyEvolve, MSGBOX_NPC
	end

FallarborTown_Mart_EventScript_PokefanM::
	msgbox FallarborTown_Mart_Text_SellNuggetIFound, MSGBOX_NPC
	end

FallarborTown_Mart_EventScript_Skitty::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SKITTY, CRY_MODE_NORMAL
	msgbox FallarborTown_Mart_Text_Skitty, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FallarborTown_Mart_Text_DecidingSkittyEvolve:
	.string "I can't decide whether to evolve my\n"
	.string "SKITTY or not.\p"
	.string "I only have to use this MOON STONE, but\n"
	.string "it's such a hard choice…\p"
	.string "It would get much stronger, kind of like\n"
	.string "a power-up to the next level.\p"
	.string "But it would look so different, too.$"

FallarborTown_Mart_Text_Skitty:
	.string "SKITTY: Meeyao?$"

FallarborTown_Mart_Text_SellNuggetIFound:
	.string "I found this ZENI POUCH on the road.\p"
	.string "I guess I'll sell it, since it's no good\n"
	.string "for anything else. At least it'll pay for\l"
	.string "lunch!$"

