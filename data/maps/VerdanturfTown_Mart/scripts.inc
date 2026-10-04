VerdanturfTown_Mart_MapScripts::
	.byte 0

VerdanturfTown_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart VerdanturfTown_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
VerdanturfTown_Mart_Pokemart:
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_NEST_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_X_SPECIAL
	.2byte ITEM_FLUFFY_TAIL
	pokemartlistend

VerdanturfTown_Mart_EventScript_Boy::
	msgbox VerdanturfTown_Mart_Text_XSpecialIsCrucial, MSGBOX_NPC
	end

VerdanturfTown_Mart_EventScript_ExpertF::
	msgbox VerdanturfTown_Mart_Text_NoStrategyGuidesForBattleTent, MSGBOX_NPC
	end

VerdanturfTown_Mart_EventScript_Lass::
	msgbox VerdanturfTown_Mart_Text_NestBallOnWeakenedPokemon, MSGBOX_NPC
	end

VerdanturfTown_Mart_Text_XSpecialIsCrucial:
	.string "In any POKéMON match, KI CHARGE is\n"
	.string "crucial.\p"
	.string "It boosts the power of some moves,\n"
	.string "though only for a single battle.$"

VerdanturfTown_Mart_Text_NoStrategyGuidesForBattleTent:
	.string "Looks like they don't sell any winning\n"
	.string "strategy guides for the BATTLE TENT…\p"
	.string "I guess you have to rely on your own\n"
	.string "wits after all…$"

VerdanturfTown_Mart_Text_NestBallOnWeakenedPokemon:
	.string "The NEST BALL works better on weakened\n"
	.string "POKéMON.\p"
	.string "VERDANTURF is the only place you can\n"
	.string "buy it.$"

