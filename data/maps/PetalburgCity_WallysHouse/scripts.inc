PetalburgCity_WallysHouse_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PetalburgCity_WallysHouse_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PetalburgCity_WallysHouse_OnWarp
	.byte 0

PetalburgCity_WallysHouse_OnWarp:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther
	.2byte 0

PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther::
	turnobject LOCALID_PLAYER, DIR_EAST
	turnobject LOCALID_WALLYS_HOUSE_WALLYS_DAD, DIR_WEST
	end

PetalburgCity_WallysHouse_OnFrame:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_GiveHMSurf
	.2byte 0

PetalburgCity_WallysHouse_EventScript_GiveHMSurf::
	lockall
	msgbox PetalburgCity_WallysHouse_Text_PleaseExcuseUs, MSGBOX_DEFAULT
	giveitem ITEM_HM_SURF
	setflag FLAG_RECEIVED_HM_SURF
	msgbox PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces, MSGBOX_DEFAULT
	setvar VAR_PETALBURG_CITY_STATE, 5
	releaseall
	end

PetalburgCity_WallysHouse_EventScript_WallysDad::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf
	goto_if_set FLAG_THANKED_FOR_PLAYING_WITH_WALLY, PetalburgCity_WallysHouse_EventScript_PlayedWithWally
	msgbox PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally, MSGBOX_DEFAULT
	setflag FLAG_THANKED_FOR_PLAYING_WITH_WALLY
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad::
	msgbox PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_PlayedWithWally::
	msgbox PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_WallysMom::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf
	msgbox PetalburgCity_WallysHouse_Text_WallyWasReallyHappy, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally:
	.string "You're…\n"
	.string "Ah, you must be {PLAYER}{KUN}, yes?\p"
	.string "Thank you for spending time with UUB\n"
	.string "earlier.\p"
	.string "He's been frail and sickly since he\n"
	.string "was a baby.\p"
	.string "We're sending him to stay with my\n"
	.string "brother, his uncle, in VERDANTURF TOWN.\p"
	.string "The air there is much cleaner than\n"
	.string "it is here.\p"
	.string "What's that? Where's UUB?\n"
	.string "He's already gone ahead, our UUB.\p"
	.string "I wonder how far he's gotten by now?$"

PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing:
	.string "I wonder how our UUB is getting along?$"

PetalburgCity_WallysHouse_Text_PleaseExcuseUs:
	.string "{PLAYER}{KUN}! Forgive us for dragging you\n"
	.string "all the way here like this.\p"
	.string "Our UUB has grown very healthy since\n"
	.string "he went to VERDANTURF TOWN.\p"
	.string "We owe it all to you!\p"
	.string "When UUB left town, you helped him\n"
	.string "catch a POKéMON, didn't you?\p"
	.string "That made UUB so happy. I could\n"
	.string "feel something stirring in that boy.\p"
	.string "And not just UUB. As his father, it\n"
	.string "made me happy too.\p"
	.string "Happy that he found such a good\n"
	.string "friend in you.\p"
	.string "This isn't a bribe or anything, but\n"
	.string "I'd really like you to have this.$"

PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces:
	.string "If your POKéMON knows SURF, you can\n"
	.string "cross the water and reach all sorts\l"
	.string "of places.$"

PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon:
	.string "UUB will be home soon.\n"
	.string "I can't wait to see him.$"

PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity:
	.string "Oh? You met UUB in\n"
	.string "EVER GRANDE CITY?\p"
	.string "Oh, {PLAYER}{KUN}, you're joking.\p"
	.string "He may be healthy now, but he can't\n"
	.string "travel somewhere that far all alone.$"

PetalburgCity_WallysHouse_Text_WallyWasReallyHappy:
	.string "UUB was overjoyed when he told us he\n"
	.string "caught a POKéMON.\p"
	.string "It's been ages since I saw him smile\n"
	.string "like that.$"

PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling:
	.string "Please keep this a secret from my\n"
	.string "husband…\p"
	.string "But our UUB left VERDANTURF TOWN\n"
	.string "without telling anyone.\p"
	.string "You know, UUB may be frail, but\n"
	.string "he has a surprisingly strong will.\p"
	.string "I'm sure he'll come back safe and\n"
	.string "sound one day!$"

