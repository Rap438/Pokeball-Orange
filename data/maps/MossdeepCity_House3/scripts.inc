MossdeepCity_House3_MapScripts::
	.byte 0

MossdeepCity_House3_EventScript_SuperRodFisherman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SUPER_ROD, MossdeepCity_House3_EventScript_ReceivedSuperRod
	msgbox MossdeepCity_House3_Text_YouWantSuperRod, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MossdeepCity_House3_EventScript_DeclineSuperRod
	msgbox MossdeepCity_House3_Text_SuperRodIsSuper, MSGBOX_DEFAULT
	giveitem ITEM_SUPER_ROD
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_SUPER_ROD
	msgbox MossdeepCity_House3_Text_TryDroppingRodInWater, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_EventScript_ReceivedSuperRod::
	msgbox MossdeepCity_House3_Text_GoAfterSeafloorPokemon, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_EventScript_DeclineSuperRod::
	msgbox MossdeepCity_House3_Text_DontYouLikeToFish, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_Text_YouWantSuperRod:
	.string "Hey there, TRAINER!\p"
	.string "A SUPER ROD really is super! Say what\n"
	.string "you like, this baby can reel in POKéMON\l"
	.string "all the way from the seafloor!\p"
	.string "And fish make a mighty fine meal, too.\p"
	.string "What do you think? You want it, don't\n"
	.string "you?$"

MossdeepCity_House3_Text_SuperRodIsSuper:
	.string "You bet, you bet!\n"
	.string "After all, a SUPER ROD is really super!$"

MossdeepCity_House3_Text_TryDroppingRodInWater:
	.string "If there's any water at all, drop in your\n"
	.string "ROD and see what bites!$"

MossdeepCity_House3_Text_DontYouLikeToFish:
	.string "Huh?\n"
	.string "Don't you like to fish?$"

MossdeepCity_House3_Text_GoAfterSeafloorPokemon:
	.string "Go after the seafloor POKéMON with\n"
	.string "that SUPER ROD.$"

