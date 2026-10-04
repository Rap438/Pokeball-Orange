BattleFrontier_OutsideWest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideWest_OnTransition
	.byte 0

BattleFrontier_OutsideWest_OnTransition:
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0
	setflag FLAG_HIDE_BATTLE_TOWER_REPORTER
	end

BattleFrontier_OutsideWest_EventScript_FerryAttendant::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_MayISeeYourTicket, MSGBOX_DEFAULT
	checkitem ITEM_SS_TICKET
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_OutsideWest_EventScript_NoSSTicket
	message BattleFrontier_OutsideWest_Text_WhereWouldYouLikeToGo
	waitmessage
	goto BattleFrontier_OutsideWest_EventScript_ChooseFerryDestination
	end

BattleFrontier_OutsideWest_EventScript_ChooseFerryDestination::
	multichoicedefault 18, 6, MULTI_SSTIDAL_BATTLE_FRONTIER, 2, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_OutsideWest_EventScript_FerryToSlateport
	case 1, BattleFrontier_OutsideWest_EventScript_FerryToLilycove
	case 2, BattleFrontier_OutsideWest_EventScript_CancelFerrySelect
	case MULTI_B_PRESSED, BattleFrontier_OutsideWest_EventScript_CancelFerrySelect
	end

BattleFrontier_OutsideWest_EventScript_NoSSTicket::
	msgbox BattleFrontier_OutsideWest_Text_MustHaveTicketToBoard, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_FerryToSlateport::
	msgbox BattleFrontier_OutsideWest_Text_SlateportItIs, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_OutsideWest_EventScript_ChooseNewFerryDestination
	msgbox BattleFrontier_OutsideWest_Text_PleaseBoardFerry, MSGBOX_DEFAULT
	call BattleFrontier_OutsideWest_EventScript_BoardFerry
	warp MAP_SLATEPORT_CITY_HARBOR, 8, 11
	waitstate
	release
	end

BattleFrontier_OutsideWest_EventScript_FerryToLilycove::
	msgbox BattleFrontier_OutsideWest_Text_LilycoveItIs, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_OutsideWest_EventScript_ChooseNewFerryDestination
	msgbox BattleFrontier_OutsideWest_Text_PleaseBoardFerry, MSGBOX_DEFAULT
	call BattleFrontier_OutsideWest_EventScript_BoardFerry
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end

BattleFrontier_OutsideWest_EventScript_ChooseNewFerryDestination::
	message BattleFrontier_OutsideWest_Text_ThenWhereWouldYouLikeToGo
	waitmessage
	goto BattleFrontier_OutsideWest_EventScript_ChooseFerryDestination
	end

BattleFrontier_OutsideWest_EventScript_BoardFerry::
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	delay 30
	hideobjectat LOCALID_FRONTIER_FERRY_ATTENDANT, MAP_BATTLE_FRONTIER_OUTSIDE_WEST
	setvar VAR_0x8004, LOCALID_FRONTIER_SS_TIDAL
	call Common_EventScript_FerryDepartIsland
	return

BattleFrontier_OutsideWest_EventScript_CancelFerrySelect::
	msgbox BattleFrontier_OutsideWest_Text_SailWithUsAnotherTime, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_BattleDomeSign::
	msgbox BattleFrontier_OutsideWest_Text_BattleDomeSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideWest_EventScript_BattleFactorySign::
	msgbox BattleFrontier_OutsideWest_Text_BattleFactorySign, MSGBOX_SIGN
	end

BattleFrontier_OutsideWest_EventScript_BattlePikeSign::
	msgbox BattleFrontier_OutsideWest_Text_BattlePikeSign, MSGBOX_SIGN
	end

@ Unused. Of note, Battle Tower is in the East section in the final release
BattleFrontier_OutsideWest_EventScript_UnusedNPC1::
	msgbox BattleFrontier_OutsideWest_Text_ThisIsBattleTower, MSGBOX_NPC
	end

@ Unused
BattleFrontier_OutsideWest_EventScript_UnusedNPC2::
	msgbox BattleFrontier_OutsideWest_Text_CantFindBattleTower, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Boy1::
	msgbox BattleFrontier_OutsideWest_Text_BestOutOfAllMyFriends, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Fisherman2::
	lock
	faceplayer
	message BattleFrontier_OutsideWest_Text_GotSeasickOnWayHere
	waitmessage
	applywaitmovement LOCALID_FRONTIER_FISHERMAN_2, Common_Movement_FaceAwayPlayer
	waitbuttonpress
	release
	end

BattleFrontier_OutsideWest_EventScript_Man1::
	msgbox BattleFrontier_OutsideWest_Text_OnlyToughTrainersBroughtHere, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Maniac1::
	lock
	goto BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk
	end

BattleFrontier_OutsideWest_EventScript_Maniac2::
	lock
	goto BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk
	end

BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk::
	applywaitmovement LOCALID_FRONTIER_MANIAC_1, Common_Movement_WalkInPlaceFasterRight
	msgbox BattleFrontier_OutsideWest_Text_SureWeCanChallengeWithNoMons, MSGBOX_DEFAULT
	applywaitmovement LOCALID_FRONTIER_MANIAC_2, Common_Movement_WalkInPlaceFasterLeft
	msgbox BattleFrontier_OutsideWest_Text_BigGuySaidIllLendYouMons, MSGBOX_DEFAULT
	closemessage
	delay 25
	applymovement LOCALID_FRONTIER_MANIAC_1, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_FRONTIER_MANIAC_2, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	release
	end

BattleFrontier_OutsideWest_EventScript_Camper::
	lock
	faceplayer
	delay 20
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_OutsideWest_EventScript_CamperAlreadyFacingFactory
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	msgbox BattleFrontier_OutsideWest_Text_WhosRaisingThoseRentalMons, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_CamperFaceFactory::
	applywaitmovement LOCALID_FRONTIER_CAMPER, Common_Movement_WalkInPlaceFasterUp
	return

BattleFrontier_OutsideWest_EventScript_CamperAlreadyFacingFactory::
	return

BattleFrontier_OutsideWest_EventScript_Girl::
	lock
	faceplayer
	message BattleFrontier_OutsideWest_Text_ScaredOfPikeBecauseSeviper
	waitmessage
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_OutsideWest_EventScript_GirlShudderNorth
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_OutsideWest_EventScript_GirlShudderSouth
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_OutsideWest_EventScript_GirlShudderWest
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_OutsideWest_EventScript_GirlShudderEast
	waitbuttonpress
	release
	end

BattleFrontier_OutsideWest_EventScript_GirlShudderNorth::
	applywaitmovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderNorth
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderSouth::
	applywaitmovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderSouth
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderWest::
	applywaitmovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderWest
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderEast::
	applywaitmovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderEast
	return

BattleFrontier_OutsideWest_Movement_GirlShudderNorth:
	walk_in_place_faster_down
	walk_in_place_faster_down
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderSouth:
	walk_in_place_faster_up
	walk_in_place_faster_up
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderWest:
	walk_in_place_faster_right
	walk_in_place_faster_right
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderEast:
	walk_in_place_faster_left
	walk_in_place_faster_left
	step_end

BattleFrontier_OutsideWest_EventScript_Woman2::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_LetsPlayRockPaperScissors, MSGBOX_DEFAULT
	random 2
	goto_if_eq VAR_RESULT, 1, BattleFrontier_OutsideWest_EventScript_WomanWonRockPaperScissors
	goto BattleFrontier_OutsideWest_EventScript_WomanLostRockPaperScissors
	end

BattleFrontier_OutsideWest_EventScript_WomanWonRockPaperScissors::
	msgbox BattleFrontier_OutsideWest_Text_WonIllTakePikeChallenge, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_WomanLostRockPaperScissors::
	msgbox BattleFrontier_OutsideWest_Text_LostIllPutOffPikeChallenge, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_Fisherman1::
	msgbox BattleFrontier_OutsideWest_Text_ChooseFishingOverBattling, MSGBOX_NPC
	end

@ Unused
BattleFrontier_OutsideWest_EventScript_UnusedNPC3::
	msgbox BattleFrontier_OutsideWest_Text_DomeIsHereGrandpa, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Gentleman::
	msgbox BattleFrontier_OutsideWest_Text_YoureOffToChallengeDome, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Lass::
	msgbox BattleFrontier_OutsideWest_Text_KeepBattlingUntilIGetSymbol, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_ExpertM::
	msgbox BattleFrontier_OutsideWest_Text_WontLetGentlemenBeatMe, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Man2::
	msgbox BattleFrontier_OutsideWest_Text_NothingHereNotLongAgo, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Woman1::
	msgbox BattleFrontier_OutsideWest_Text_FinallyArrivedAtFrontier, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_FatMan1::
	msgbox BattleFrontier_OutsideWest_Text_SquareFilledWithToughPeople, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_FatMan2::
	msgbox BattleFrontier_OutsideWest_Text_MetOlderGirlAtPike, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Woman3::
	lock
	msgbox BattleFrontier_OutsideWest_Text_LastTimeOurEyesMet, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_Boy2::
	lock
	msgbox BattleFrontier_OutsideWest_Text_DomeAceLookedBecauseOfMyCheering, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_OldMan::
	msgbox BattleFrontier_OutsideWest_Text_DomeAceIsMine, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Man4::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_FansOverThereUsedToBeTrainers, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_FRONTIER_MAN_4, Common_Movement_FaceOriginalDirection
	release
	end

BattleFrontier_OutsideWest_EventScript_PokefanF::
	msgbox BattleFrontier_OutsideWest_Text_MonWithLongTailInFrontier, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_Text_BattleDomeSign:
	.string "BATTLE DOME\n"
	.string "Your road to becoming the invincible\l"
	.string "superstar!$"

BattleFrontier_OutsideWest_Text_BattleFactorySign:
	.string "BATTLE FACTORY\n"
	.string "Track down the toughest POKéMON!$"

BattleFrontier_OutsideWest_Text_BattlePikeSign:
	.string "BATTLE PIKE\n"
	.string "Pick one of three paths!$"

BattleFrontier_OutsideWest_Text_ThisIsBattleTower:
	.string "This is the BATTLE TOWER.\p"
	.string "TRAINERS bring their best POKéMON to\n"
	.string "fight in knockout matches.$"

BattleFrontier_OutsideWest_Text_MayISeeYourTicket:
	.string "Hello there! Boarding the ferry?\n"
	.string "I'll need to see your TICKET.$"

BattleFrontier_OutsideWest_Text_MustHaveTicketToBoard:
	.string "{PLAYER} doesn't have the TICKET…\p"
	.string "I'm terribly sorry.\p"
	.string "You can't board the ferry without\n"
	.string "a TICKET.$"

BattleFrontier_OutsideWest_Text_WhereWouldYouLikeToGo:
	.string "{PLAYER} held up the TICKET.\p"
	.string "Excellent! That's all we need!\p"
	.string "So, where will it be?$"

BattleFrontier_OutsideWest_Text_SlateportItIs:
	.string "SLATEPORT CITY, then! Off we go!$"

BattleFrontier_OutsideWest_Text_LilycoveItIs:
	.string "LILYCOVE CITY, then! Off we go!$"

BattleFrontier_OutsideWest_Text_SailWithUsAnotherTime:
	.string "Do sail with us again sometime!$"

BattleFrontier_OutsideWest_Text_PleaseBoardFerry:
	.string "Please step aboard the ferry and\n"
	.string "wait for it to set sail.$"

BattleFrontier_OutsideWest_Text_ThenWhereWouldYouLikeToGo:
	.string "Then where shall we take you?$"

BattleFrontier_OutsideWest_Text_BestOutOfAllMyFriends:
	.string "I'm the strongest in my whole group\n"
	.string "of friends. But here…\l"
	.string "I'm nothing special!$"

BattleFrontier_OutsideWest_Text_CantFindBattleTower:
	.string "The BATTLE TOWER is where I'm headed,\n"
	.string "but even with a map of the BATTLE\l"
	.string "FRONTIER I'm lost.\p"
	.string "This place is enormous!$"

BattleFrontier_OutsideWest_Text_GotSeasickOnWayHere:
	.string "I wanted to jump into a challenge the\n"
	.string "second I arrived.\p"
	.string "But I got seasick on the way over…\n"
	.string "Urrrrp…$"

BattleFrontier_OutsideWest_Text_OnlyToughTrainersBroughtHere:
	.string "Not just anybody gets to come here,\n"
	.string "you know.\p"
	.string "Only fighters who've been recognized\n"
	.string "as truly strong are invited.\p"
	.string "That's why so many TRAINERS have never\n"
	.string "even heard of the BATTLE FRONTIER.$"

BattleFrontier_OutsideWest_Text_SureWeCanChallengeWithNoMons:
	.string "Hey, bro…\p"
	.string "Do you really think they'll let us\n"
	.string "compete with no POKéMON at all?$"

BattleFrontier_OutsideWest_Text_BigGuySaidIllLendYouMons:
	.string "Uh…\n"
	.string "I think it'll be fine.\p"
	.string "Probably…\p"
	.string "Don't you remember that huge scary\n"
	.string "guy? He said, “I'll lend you POKéMON!”$"

BattleFrontier_OutsideWest_Text_WhosRaisingThoseRentalMons:
	.string "That's the BATTLE FACTORY, where you\n"
	.string "can borrow strong POKéMON.\p"
	.string "I always wonder who trains all\n"
	.string "those rentals…$"

BattleFrontier_OutsideWest_Text_ScaredOfPikeBecauseSeviper:
	.string "The BATTLE PIKE scares me because of\n"
	.string "SEVIPER…\p"
	.string "B-but I traveled so far, so I'll push\n"
	.string "through and win it all!\l"
	.string "…Gulp…$"

BattleFrontier_OutsideWest_Text_LetsPlayRockPaperScissors:
	.string "Let's decide it by rock, paper,\n"
	.string "scissors! Ready?\p"
	.string "One, two, three!\p"
	.string "… … … … … …$"

BattleFrontier_OutsideWest_Text_WonIllTakePikeChallenge:
	.string "Woo-hoo! I win!\n"
	.string "BATTLE PIKE challenge, here I come!$"

BattleFrontier_OutsideWest_Text_LostIllPutOffPikeChallenge:
	.string "Aw, no…\n"
	.string "I lost.\p"
	.string "Looks like my luck ran out today.\n"
	.string "The BATTLE PIKE challenge will have\l"
	.string "to wait until tomorrow.$"

BattleFrontier_OutsideWest_Text_ChooseFishingOverBattling:
	.string "I think I'm the only one here who'd\n"
	.string "choose fishing over fighting.\p"
	.string "Huh? You can't catch anything here?\n"
	.string "That's a letdown…$"

BattleFrontier_OutsideWest_Text_KeepBattlingUntilIGetSymbol:
	.string "Today I'm not stopping, not for\n"
	.string "anything, until I win a Symbol.$"

BattleFrontier_OutsideWest_Text_YoureOffToChallengeDome:
	.string "Oh? You're off to challenge the\n"
	.string "BATTLE DOME?\p"
	.string "Best of luck to you.\n"
	.string "Let's both fight our way up and meet\l"
	.string "in the finals.$"

BattleFrontier_OutsideWest_Text_DomeIsHereGrandpa:
	.string "Grandpa, this way!\n"
	.string "There's the BATTLE DOME!\l"
	.string "Show them what you've got, Grandpa!$"

BattleFrontier_OutsideWest_Text_WontLetGentlemenBeatMe:
	.string "So this is the BATTLE DOME?\n"
	.string "No GENTLEMAN is going to beat me!\p"
	.string "Now, where's the way in?$"

BattleFrontier_OutsideWest_Text_NothingHereNotLongAgo:
	.string "Not long ago, there was nothing here\n"
	.string "at all.\p"
	.string "And now look at it! Incredible!\n"
	.string "I'm bringing my mom to see it.$"

BattleFrontier_OutsideWest_Text_FinallyArrivedAtFrontier:
	.string "I've finally made it to the BATTLE\n"
	.string "FRONTIER!\p"
	.string "I'm sure to turn heads with my looks\n"
	.string "and my power!$"

BattleFrontier_OutsideWest_Text_SquareFilledWithToughPeople:
	.string "Munch, munch…\p"
	.string "This square looks packed with strong\n"
	.string "people.\p"
	.string "Munch, munch…$"

BattleFrontier_OutsideWest_Text_MetOlderGirlAtPike:
	.string "Crunch, munch…\p"
	.string "A while back, I ran into an older girl\n"
	.string "at the BATTLE PIKE.\p"
	.string "Her eyes were scary in a really\n"
	.string "ornery way.\p"
	.string "Crunch, munch…\p"
	.string "She went ballistic: “Will you quit\n"
	.string "eating!” Then she pounded me.\p"
	.string "It was a hair-raising experience,\n"
	.string "let me tell you.\p"
	.string "Crunch, munch…$"

BattleFrontier_OutsideWest_Text_LastTimeOurEyesMet:
	.string "Huh? Listen to you!\n"
	.string "That's nothing!\p"
	.string "When I last cheered for him, our\n"
	.string "eyes locked and sparks flew!$"

BattleFrontier_OutsideWest_Text_DomeAceLookedBecauseOfMyCheering:
	.string "Whaaaaaat?!\n"
	.string "Pfft!\p"
	.string "That was only because you happened\n"
	.string "to be sitting next to me!\p"
	.string "The DOME ACE glanced over because my\n"
	.string "cheering was the loudest!$"

BattleFrontier_OutsideWest_Text_DomeAceIsMine:
	.string "Oh, pipe down!\n"
	.string "Stop making such a racket!\p"
	.string "Fair-weather fans like you should go\n"
	.string "cheer for some GYM LEADER instead!\p"
	.string "The DOME ACE belongs to me!\n"
	.string "He's the only idol I need!$"

BattleFrontier_OutsideWest_Text_FansOverThereUsedToBeTrainers:
	.string "Those people squabbling over there…\p"
	.string "Hard to believe, but they were once\n"
	.string "TRAINERS, and good ones, too.\p"
	.string "They used to be incredibly strong, and\n"
	.string "famous besides.\p"
	.string "But they were crushed by a certain\n"
	.string "TRAINER at a tournament.\p"
	.string "They were beaten so thoroughly that\n"
	.string "they fell head over heels for him.\p"
	.string "Now they're just his fans. Every day\n"
	.string "they go cheer for their idol at the\l"
	.string "BATTLE DOME.$"

BattleFrontier_OutsideWest_Text_MonWithLongTailInFrontier:
	.string "Rumor has it somebody saw a POKéMON\n"
	.string "with a strangely long tail in the\l"
	.string "BATTLE FRONTIER.\p"
	.string "It must be so cute, I just know it…\p"
	.string "Challenges can wait. I have to find\n"
	.string "that darling creature!$"

