MossdeepCity_Mart_MapScripts::
	.byte 0

MossdeepCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart MossdeepCity_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
MossdeepCity_Mart_Pokemart:
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_NET_BALL
	.2byte ITEM_DIVE_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	pokemartlistend

MossdeepCity_Mart_EventScript_Woman::
	msgbox MossdeepCity_Mart_Text_ReviveIsFantastic, MSGBOX_NPC
	end

MossdeepCity_Mart_EventScript_Boy::
	msgbox MossdeepCity_Mart_Text_MaxRepelLastsLongest, MSGBOX_NPC
	end

MossdeepCity_Mart_EventScript_Sailor::
	msgbox MossdeepCity_Mart_Text_NetAndDiveBallsRare, MSGBOX_NPC
	end

MossdeepCity_Mart_Text_ReviveIsFantastic:
	.string "DENDE'S HEAL is fantastic! It's like a\n"
	.string "touch from the young Guardian himself.\p"
	.string "Give it to a fainted POKéMON, and it\n"
	.string "springs right back up.\p"
	.string "But be careful, it doesn't restore the\n"
	.string "used-up PP of moves.$"

MossdeepCity_Mart_Text_MaxRepelLastsLongest:
	.string "KI VOID hides your ki so well that every\n"
	.string "weak POKéMON keeps its distance.\p"
	.string "Out of all the KI HIDER sprays, it lasts\n"
	.string "the longest.$"

MossdeepCity_Mart_Text_NetAndDiveBallsRare:
	.string "NET BALLS and DIVE BALLS are rare POKé\n"
	.string "BALLS made only in MOSSDEEP.\p"
	.string "A NET BALL works well on BUG-type and\n"
	.string "WATER-type POKéMON.\p"
	.string "A DIVE BALL works best on POKéMON\n"
	.string "found at the bottom of the sea.$"

