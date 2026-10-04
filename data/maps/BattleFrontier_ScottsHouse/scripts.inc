BattleFrontier_ScottsHouse_MapScripts::
	.byte 0

BattleFrontier_ScottsHouse_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_TEMP_4, BattleFrontier_ScottsHouse_EventScript_GivenBerry
	goto_if_set FLAG_TEMP_3, BattleFrontier_ScottsHouse_EventScript_GivenShield
	goto_if_set FLAG_TEMP_2, BattleFrontier_ScottsHouse_EventScript_GivenBattlePoints
	goto BattleFrontier_ScottsHouse_EventScript_CheckGiveItems
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveItems::
	goto_if_unset FLAG_SCOTT_GIVES_BATTLE_POINTS, BattleFrontier_ScottsHouse_EventScript_WelcomeToFrontier
	goto_if_unset FLAG_COLLECTED_ALL_SILVER_SYMBOLS, BattleFrontier_ScottsHouse_EventScript_CheckSilverSymbols
	goto_if_unset FLAG_COLLECTED_ALL_GOLD_SYMBOLS, BattleFrontier_ScottsHouse_EventScript_CheckGoldSymbols
	goto BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveShield::
	goto_if_unset FLAG_RECEIVED_SILVER_SHIELD, BattleFrontier_ScottsHouse_EventScript_CheckGiveSilverShield
	goto_if_unset FLAG_RECEIVED_GOLD_SHIELD, BattleFrontier_ScottsHouse_EventScript_CheckGiveGoldShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_CheckSilverSymbols::
	goto_if_unset FLAG_SYS_TOWER_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_DOME_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PALACE_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_ARENA_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_FACTORY_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PIKE_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PYRAMID_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	msgbox BattleFrontier_ScottsHouse_Text_YouveCollectedAllSilverSymbols, MSGBOX_DEFAULT
	giveitem ITEM_LANSAT_BERRY
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_BerryPocketFull
	setflag FLAG_COLLECTED_ALL_SILVER_SYMBOLS
	setflag FLAG_TEMP_4
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGoldSymbols::
	goto_if_unset FLAG_SYS_TOWER_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_DOME_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PALACE_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_ARENA_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_FACTORY_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PIKE_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PYRAMID_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	msgbox BattleFrontier_ScottsHouse_Text_YouveCollectedAllGoldSymbols, MSGBOX_DEFAULT
	giveitem ITEM_STARF_BERRY
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_BerryPocketFull
	setflag FLAG_COLLECTED_ALL_GOLD_SYMBOLS
	setflag FLAG_TEMP_4
	release
	end

BattleFrontier_ScottsHouse_EventScript_BerryPocketFull::
	msgbox BattleFrontier_ScottsHouse_Text_BerryPocketStuffed, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_GivenBerry::
	msgbox BattleFrontier_ScottsHouse_Text_SoGladIBroughtYouHere, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_RandomComment::
	random 3
	goto_if_eq VAR_RESULT, 1, BattleFrontier_ScottsHouse_EventScript_FrontierBrainComment
	goto_if_eq VAR_RESULT, 2, BattleFrontier_ScottsHouse_EventScript_ArtisanCaveComment
	msgbox BattleFrontier_ScottsHouse_Text_WhyIGoSeekingTrainers, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_FrontierBrainComment::
	msgbox BattleFrontier_ScottsHouse_Text_HaveYouMetFrontierBrain, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_ArtisanCaveComment::
	msgbox BattleFrontier_ScottsHouse_Text_MayFindWildMonsInFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveSilverShield::
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_50
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 50, BattleFrontier_ScottsHouse_EventScript_GiveSilverShield
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_OPEN
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 50, BattleFrontier_ScottsHouse_EventScript_GiveSilverShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_GiveSilverShield::
	msgbox BattleFrontier_ScottsHouse_Text_Beat50TrainersInARow, MSGBOX_DEFAULT
	givedecoration DECOR_SILVER_SHIELD
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_NoRoomForShield
	setflag FLAG_RECEIVED_SILVER_SHIELD
	setflag FLAG_TEMP_3
	goto BattleFrontier_ScottsHouse_EventScript_GivenShield
	end

BattleFrontier_ScottsHouse_EventScript_NoRoomForShield::
	msgbox BattleFrontier_ScottsHouse_Text_ComeBackForThisLater, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_GivenShield::
	msgbox BattleFrontier_ScottsHouse_Text_ExpectingToHearEvenGreaterThings, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveGoldShield::
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_50
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 100, BattleFrontier_ScottsHouse_EventScript_GiveGoldShield
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_OPEN
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 100, BattleFrontier_ScottsHouse_EventScript_GiveGoldShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_GiveGoldShield::
	msgbox BattleFrontier_ScottsHouse_Text_Beat100TrainersInARow, MSGBOX_DEFAULT
	givedecoration DECOR_GOLD_SHIELD
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_NoRoomForShield
	setflag FLAG_RECEIVED_GOLD_SHIELD
	setflag FLAG_TEMP_3
	goto BattleFrontier_ScottsHouse_EventScript_GivenShield
	end

BattleFrontier_ScottsHouse_EventScript_GivenBattlePoints::
	msgbox BattleFrontier_ScottsHouse_Text_ExpectingGreatThings, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_WelcomeToFrontier::
	msgbox BattleFrontier_ScottsHouse_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	closemessage
	delay 30
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayNorth
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwaySouth
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayEast
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayWest
	msgbox BattleFrontier_ScottsHouse_Text_HowMuchEffortItTookToMakeReal, MSGBOX_DEFAULT
	applywaitmovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_FacePlayer
	msgbox BattleFrontier_ScottsHouse_Text_HaveThisAsMementoOfOurPathsCrossing, MSGBOX_DEFAULT
	goto_if_eq VAR_SCOTT_STATE, 13, BattleFrontier_ScottsHouse_EventScript_Give4BattlePoints
	goto_if_ge VAR_SCOTT_STATE, 9, BattleFrontier_ScottsHouse_EventScript_Give3BattlePoints
	goto_if_ge VAR_SCOTT_STATE, 6, BattleFrontier_ScottsHouse_EventScript_Give2BattlePoints
	goto BattleFrontier_ScottsHouse_EventScript_Give1BattlePoint
	end

BattleFrontier_ScottsHouse_EventScript_Give4BattlePoints::
	buffernumberstring STR_VAR_1, 4
	setvar VAR_0x8004, 4
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give3BattlePoints::
	buffernumberstring STR_VAR_1, 3
	setvar VAR_0x8004, 3
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give2BattlePoints::
	buffernumberstring STR_VAR_1, 2
	setvar VAR_0x8004, 2
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give1BattlePoint::
	buffernumberstring STR_VAR_1, 1
	setvar VAR_0x8004, 1
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints::
	special GiveFrontierBattlePoints
	msgbox BattleFrontier_ScottsHouse_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	msgbox BattleFrontier_ScottsHouse_Text_ExplainBattlePoints, MSGBOX_DEFAULT
	setflag FLAG_SCOTT_GIVES_BATTLE_POINTS
	setflag FLAG_TEMP_2
	release
	end

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayNorth::
	applywaitmovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterUp
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwaySouth::
	applywaitmovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterDown
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayEast::
	applywaitmovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterRight
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayWest::
	applywaitmovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterLeft
	return

BattleFrontier_ScottsHouse_Text_WelcomeToBattleFrontier:
	.string "ANNOUNCER: Well, well, welcome!\n"
	.string "Heheh… Pardon the cramped quarters.\p"
	.string "Now then, {PLAYER}{KUN}, allow me to\n"
	.string "officially welcome you to the BATTLE\l"
	.string "FRONTIER!\p"
	.string "This is my dream come true.\n"
	.string "It took years and years, but I finally\l"
	.string "gave my dream a body and a ring.$"

BattleFrontier_ScottsHouse_Text_HowMuchEffortItTookToMakeReal:
	.string "Looking back, it was an awfully long\n"
	.string "road…\p"
	.string "I left home alone, hunting for\n"
	.string "strong fighters across the world.\p"
	.string "Nobody can imagine how much sweat\n"
	.string "and time it took to build all this.$"

BattleFrontier_ScottsHouse_Text_HaveThisAsMementoOfOurPathsCrossing:
	.string "But that's all in the past.\n"
	.string "No use dwelling on it!\p"
	.string "All I want is for you to fight to your\n"
	.string "heart's content here.\p"
	.string "{PLAYER}{KUN}, please take this as a\n"
	.string "memento of every time our paths\l"
	.string "crossed on our journeys.$"

BattleFrontier_ScottsHouse_Text_ObtainedXBattlePoints:
	.string "{PLAYER} received\n"
	.string "{STR_VAR_1} Battle Point(s).$"

BattleFrontier_ScottsHouse_Text_ExplainBattlePoints:
	.string "ANNOUNCER: You can check your Battle\n"
	.string "Points on your FRONTIER PASS.\p"
	.string "The more you win here at the BATTLE\n"
	.string "FRONTIER, the more Battle Points you\l"
	.string "will collect.\p"
	.string "Spend them however you like, such as\n"
	.string "trading them in for items.$"

BattleFrontier_ScottsHouse_Text_ExpectingGreatThings:
	.string "I'm counting on you to do great things!$"

BattleFrontier_ScottsHouse_Text_WhyIGoSeekingTrainers:
	.string "ANNOUNCER: Every fighter is different.\n"
	.string "Each one lives a life of their own.\p"
	.string "But once the bell rings and the\n"
	.string "battle begins, who they are no longer\l"
	.string "matters.\p"
	.string "They are all the same!\p"
	.string "I love the purity of battle, where\n"
	.string "everyone stands equal.\p"
	.string "That's why I travel everywhere, looking\n"
	.string "for fighters who take battle seriously,\l"
	.string "and I invite them here.$"

BattleFrontier_ScottsHouse_Text_HaveYouMetFrontierBrain:
	.string "ANNOUNCER: Have you met any of the\n"
	.string "FRONTIER BRAINS yet?\p"
	.string "Better still, have you won any symbols\n"
	.string "from them?\p"
	.string "I handpicked the FRONTIER BRAINS as\n"
	.string "the strongest fighters anywhere.\p"
	.string "But I'm sure that once they see your\n"
	.string "power, even they will be stunned!$"

BattleFrontier_ScottsHouse_Text_MayFindWildMonsInFrontier:
	.string "ANNOUNCER: Surely battling isn't all\n"
	.string "you train for?\p"
	.string "I seem to recall you were filling out\n"
	.string "a POKéDEX, too.\p"
	.string "Well, listen to this…\p"
	.string "Wild POKéMON might turn up somewhere\n"
	.string "in the BATTLE FRONTIER…\l"
	.string "Fufufu!$"

BattleFrontier_ScottsHouse_Text_YouveCollectedAllSilverSymbols:
	.string "ANNOUNCER: Are you enjoying yourself\n"
	.string "in the BATTLE FRONTIER?\p"
	.string "…Hold on a moment…\n"
	.string "Oh!\p"
	.string "Your FRONTIER PASS!\n"
	.string "You've collected the full set of\l"
	.string "silver symbols!\p"
	.string "That's more than impressive!\n"
	.string "You're every bit as strong as I\l"
	.string "suspected!\p"
	.string "I wouldn't normally do this, but this\n"
	.string "is a special occasion!\p"
	.string "Please take this.\n"
	.string "I'm sure you'll put it to good use.$"

BattleFrontier_ScottsHouse_Text_YouveCollectedAllGoldSymbols:
	.string "ANNOUNCER: I hope you're enjoying\n"
	.string "every corner of the BATTLE FRONTIER.\p"
	.string "…Hold on a moment…\n"
	.string "Oh, I can hardly believe my eyes!\p"
	.string "Your FRONTIER PASS!\n"
	.string "You've gathered the full set of\l"
	.string "gold symbols!\p"
	.string "That's beyond stupendous!\n"
	.string "You're incredible, just as I suspected!\p"
	.string "… … … … … …\p"
	.string "{PLAYER}, you simply must accept this!\p"
	.string "I think you'll understand the value\n"
	.string "of my gift!$"

BattleFrontier_ScottsHouse_Text_SoGladIBroughtYouHere:
	.string "I do pride myself on spotting a true\n"
	.string "fighter when I see one.\p"
	.string "I'm so glad I trusted my instincts and\n"
	.string "brought you here!$"

BattleFrontier_ScottsHouse_Text_BerryPocketStuffed:
	.string "Your BERRY POCKET looks to be\n"
	.string "stuffed full.$"

BattleFrontier_ScottsHouse_Text_Beat50TrainersInARow:
	.string "ANNOUNCER: Oh, word has reached me!\n"
	.string "You ruled the BATTLE TOWER!\l"
	.string "Over 50 TRAINERS defeated in a row?\p"
	.string "Magnificent!\n"
	.string "I insist you take this!$"

BattleFrontier_ScottsHouse_Text_Beat100TrainersInARow:
	.string "ANNOUNCER: Good heavens, word has\n"
	.string "reached me! You crushed the BATTLE\l"
	.string "TOWER! Over 100 TRAINERS beaten in a\l"
	.string "row?\p"
	.string "That is absurdly spectacular!\n"
	.string "You must have this!$"

BattleFrontier_ScottsHouse_Text_ExpectingToHearEvenGreaterThings:
	.string "Now I'll be listening for even\n"
	.string "greater tales of your battles!$"

BattleFrontier_ScottsHouse_Text_ComeBackForThisLater:
	.string "Oops, if your bag is too full, come\n"
	.string "back for this later.$"

