MysteryGiftScript_OldSeaMap::
	setvaddress MysteryGiftScript_OldSeaMap
	lock
	faceplayer
	vgoto_if_set FLAG_RECEIVED_OLD_SEA_MAP, OldSeaMap_Obtained
	vgoto_if_set FLAG_CAUGHT_MEW, OldSeaMap_Obtained
	checkitem ITEM_OLD_SEA_MAP
	vgoto_if_eq VAR_RESULT, TRUE, OldSeaMap_Obtained
	vmessage sText_MysteryGiftOldSeaMapForYou
	waitmessage
	waitbuttonpress
	checkitemspace ITEM_OLD_SEA_MAP
	vgoto_if_eq VAR_RESULT, FALSE, OldSeaMap_NoBagSpace
	giveitem ITEM_OLD_SEA_MAP
	setflag FLAG_ENABLE_SHIP_FARAWAY_ISLAND
	setflag FLAG_RECEIVED_OLD_SEA_MAP
	vmessage sText_MysteryGiftOldSeaMapUseAtPort
	waitmessage
	waitbuttonpress
	release
	end

OldSeaMap_NoBagSpace:
	vmessage sText_MysteryGiftOldSeaMapBagFull
	waitmessage
	waitbuttonpress
	release
	end

OldSeaMap_Obtained:
	vmessage sText_MysteryGiftOldSeaMapThankYou
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftOldSeaMapForYou:
	.string "Thanks for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "Let me double-check--you're\n"
	.string "{PLAYER}, right?\p"
	.string "An OLD SEA MAP came in with\n"
	.string "your name on it.$"

sText_MysteryGiftOldSeaMapUseAtPort:
	.string "It looks like it's meant for the\n"
	.string "LILYCOVE CITY port.\p"
	.string "Why not go and find out what\n"
	.string "it's all about?$"

sText_MysteryGiftOldSeaMapThankYou:
	.string "Thanks for using the MYSTERY\n"
	.string "GIFT System.$"

sText_MysteryGiftOldSeaMapBagFull:
	.string "Oh, sorry, {PLAYER}.\n"
	.string "Your BAG's KEY ITEMS POCKET is full.\p"
	.string "Store something on your PC, then\n"
	.string "come back and pick this up.$"
