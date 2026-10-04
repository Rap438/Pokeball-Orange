BattleFrontier_Lounge3_MapScripts::
	.byte 0

	.set BET_AMOUNT_5,  5
	.set BET_AMOUNT_10, 10
	.set BET_AMOUNT_15, 15

BattleFrontier_Lounge3_EventScript_Gambler::
	lock
	faceplayer
	goto_if_set FLAG_MET_BATTLE_FRONTIER_GAMBLER, BattleFrontier_Lounge3_EventScript_AlreadyMetGambler
	call BattleFrontier_Lounge3_EventScript_CountSilverSymbols
	goto_if_le VAR_0x8004, 2, BattleFrontier_Lounge3_EventScript_NotEnoughSilverSymbols
	setflag FLAG_MET_BATTLE_FRONTIER_GAMBLER
	msgbox BattleFrontier_Lounge3_Text_YouLookToughExplainGambling, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_AskToEnterChallenge
	end

BattleFrontier_Lounge3_EventScript_AskToEnterChallenge::
	special ShowFrontierGamblerLookingMessage
	waitmessage
	waitbuttonpress
	msgbox BattleFrontier_Lounge3_Text_HowAboutEnteringEventForMe, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge3_EventScript_DeclineChallenge
	msgbox BattleFrontier_Lounge3_Text_SpotMeSomeBattlePoints, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge3_EventScript_DeclineChallenge
	message BattleFrontier_Lounge3_Text_HowMuchCanYouSpot
	waitmessage
	special ShowBattlePointsWindow
	goto BattleFrontier_Lounge3_EventScript_ChooseBetAmount
	end

BattleFrontier_Lounge3_EventScript_ChooseBetAmount::
	multichoice 20, 4, MULTI_FRONTIER_GAMBLER_BET, FALSE
	copyvar VAR_FRONTIER_GAMBLER_AMOUNT_BET, VAR_RESULT
	switch VAR_RESULT
	case FRONTIER_GAMBLER_BET_5, BattleFrontier_Lounge3_EventScript_Bet5
	case FRONTIER_GAMBLER_BET_10, BattleFrontier_Lounge3_EventScript_Bet10
	case FRONTIER_GAMBLER_BET_15, BattleFrontier_Lounge3_EventScript_Bet15
	case FRONTIER_GAMBLER_BET_CANCEL, BattleFrontier_Lounge3_EventScript_CancelBet
	case MULTI_B_PRESSED, BattleFrontier_Lounge3_EventScript_CancelBet
	end

BattleFrontier_Lounge3_EventScript_Bet5::
	setvar VAR_0x8008, BET_AMOUNT_5
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_Bet10::
	setvar VAR_0x8008, BET_AMOUNT_10
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_Bet15::
	setvar VAR_0x8008, BET_AMOUNT_15
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_TryPlaceBet::
	specialvar VAR_TEMP_1, GetFrontierBattlePoints
	goto_if_ge VAR_TEMP_1, VAR_0x8008, BattleFrontier_Lounge3_EventScript_PlaceBet
	msgbox BattleFrontier_Lounge3_Text_YouDontHaveEnoughPoints, MSGBOX_DEFAULT
	message BattleFrontier_Lounge3_Text_HowMuchCanYouSpot
	waitmessage
	goto BattleFrontier_Lounge3_EventScript_ChooseBetAmount
	end

BattleFrontier_Lounge3_EventScript_PlaceBet::
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_Lounge3_Text_ThanksOffYouGo, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FinishBet
	end

BattleFrontier_Lounge3_EventScript_FinishBet::
	special ShowFrontierGamblerGoMessage
	waitmessage
	waitbuttonpress
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_Lounge3_EventScript_CountSilverSymbols::
	setvar VAR_0x8004, 0
	call_if_set FLAG_SYS_TOWER_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_DOME_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PALACE_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_ARENA_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_FACTORY_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PIKE_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PYRAMID_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	return

BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount::
	addvar VAR_0x8004, 1
	return

BattleFrontier_Lounge3_EventScript_NotEnoughSilverSymbols::
	msgbox BattleFrontier_Lounge3_Text_CantYouSeeWereBusyHere, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_AlreadyMetGambler::
	msgbox BattleFrontier_Lounge3_Text_Oh, MSGBOX_DEFAULT
	goto_if_ge VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET, BattleFrontier_Lounge3_EventScript_CheckBetResults
	goto BattleFrontier_Lounge3_EventScript_AskToEnterChallenge
	end

BattleFrontier_Lounge3_EventScript_CheckBetResults::
	goto_if_eq VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET, BattleFrontier_Lounge3_EventScript_ChallengeNotAttempted
	goto_if_eq VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WON, BattleFrontier_Lounge3_EventScript_WonChallenge
	goto BattleFrontier_Lounge3_EventScript_LostChallenge
	end

BattleFrontier_Lounge3_EventScript_WonChallenge::
	msgbox BattleFrontier_Lounge3_Text_HelloChampHeresYourPoints, MSGBOX_DEFAULT
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_5, BattleFrontier_Lounge3_EventScript_RewardBet5
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_10, BattleFrontier_Lounge3_EventScript_RewardBet10
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_15, BattleFrontier_Lounge3_EventScript_RewardBet15
	msgbox BattleFrontier_Lounge3_Text_ObtainedBattlePoints, MSGBOX_GETPOINTS
	special GiveFrontierBattlePoints
	msgbox BattleFrontier_Lounge3_Text_ThinkOfMeForAnotherChallenge, MSGBOX_DEFAULT
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WAITING
	release
	end

BattleFrontier_Lounge3_EventScript_LostChallenge::
	msgbox BattleFrontier_Lounge3_Text_NiceTryCantReturnPoints, MSGBOX_DEFAULT
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WAITING
	release
	end

BattleFrontier_Lounge3_EventScript_RewardBet5::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_5 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_5 * 2)
	return

BattleFrontier_Lounge3_EventScript_RewardBet10::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_10 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_10 * 2)
	return

BattleFrontier_Lounge3_EventScript_RewardBet15::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_15 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_15 * 2)
	return

BattleFrontier_Lounge3_EventScript_ChallengeNotAttempted::
	special ShowFrontierGamblerGoMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge3_EventScript_DeclineChallenge::
	msgbox BattleFrontier_Lounge3_Text_NotInterested, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge3_EventScript_CancelBet::
	special CloseBattlePointsWindow
	goto BattleFrontier_Lounge3_EventScript_DeclineChallenge
	end

BattleFrontier_Lounge3_EventScript_Man::
	msgbox BattleFrontier_Lounge3_Text_ShouldBeTakingChallenges, MSGBOX_NPC
	end

BattleFrontier_Lounge3_EventScript_Woman::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_BackedWrongTrainer, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_PokefanF::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_KnowWinnerWhenISeeOne, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_FatMan::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_TrainerGoodButRattled, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_FaceOriginalDirection::
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	release
	end

BattleFrontier_Lounge3_Text_CantYouSeeWereBusyHere::
	.string "…What do you want?\p"
	.string "Can't you see we're in the middle\n"
	.string "of something? Can't it wait?$"

BattleFrontier_Lounge3_Text_YouLookToughExplainGambling::
	.string "…Huh?\n"
	.string "You look like a tough fighter.\p"
	.string "Heheh…\n"
	.string "Listen, I have a proposition.\p"
	.string "We've got a little circle here, and\n"
	.string "we play a little game with whatever\l"
	.string "goes on in the BATTLE FRONTIER.\p"
	.string "The rules are dead simple.\p"
	.string "First, we pick one of the facilities\n"
	.string "in the BATTLE FRONTIER.\p"
	.string "Then we each back a different TRAINER\n"
	.string "who's taking that facility's challenge,\l"
	.string "and we bet our Battle Points.\p"
	.string "Whoever backed the TRAINER with the\n"
	.string "best record scoops up the whole\l"
	.string "pot of Battle Points.\p"
	.string "Simple, huh?\n"
	.string "So, anyway…$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerSingle::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be challenging the BATTLE TOWER's\l"
	.string "SINGLE BATTLE ROOMS.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerDouble::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be challenging the BATTLE TOWER's\l"
	.string "DOUBLE BATTLE ROOMS.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerMulti::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be challenging the BATTLE TOWER's\l"
	.string "MULTI BATTLE ROOMS.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattleDomeSingle::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be entering the BATTLE DOME's SINGLE\l"
	.string "BATTLE Tourney.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattleDomeDouble::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be entering the BATTLE DOME's DOUBLE\l"
	.string "BATTLE Tourney.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattleFactorySingle::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be entering the BATTLE FACTORY's\l"
	.string "Battle Swap Single Tourney.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattleFactoryDouble::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be entering the BATTLE FACTORY's\l"
	.string "Battle Swap Double Tourney.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePalaceSingle::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be challenging the BATTLE PALACE's\l"
	.string "SINGLE BATTLE HALLS.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePalaceDouble::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be challenging the BATTLE PALACE's\l"
	.string "DOUBLE BATTLE HALLS.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattleArena::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be entering the BATTLE ARENA's Set KO\l"
	.string "Tourney.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePike::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be entering the BATTLE PIKE's Battle\l"
	.string "Choice.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePyramid::
	.string "I'm looking for a TRAINER who's going to\n"
	.string "be entering the BATTLE PYRAMID's\l"
	.string "Battle Quest.\p"
	.string "But so far, I haven't seen a fighter\n"
	.string "with that winner's spark.$"

BattleFrontier_Lounge3_Text_HowAboutEnteringEventForMe::
	.string "I'll make sure you benefit, too.\n"
	.string "So what do you say?\l"
	.string "Enter that event for me?$"

BattleFrontier_Lounge3_Text_SpotMeSomeBattlePoints::
	.string "Perfect, that's perfect.\n"
	.string "So, uh… could you front me some of\l"
	.string "your Battle Points?\p"
	.string "Trust me, you'll get your thanks\n"
	.string "afterward.$"

BattleFrontier_Lounge3_Text_HowMuchCanYouSpot::
	.string "Excellent, excellent!\n"
	.string "Now, how much can you front me?$"

BattleFrontier_Lounge3_Text_YouDontHaveEnoughPoints::
	.string "Oh, no, no, no!\n"
	.string "You don't have enough Battle Points!\p"
	.string "Don't play games with me and waste\n"
	.string "everyone's time!$"

BattleFrontier_Lounge3_Text_ThanksOffYouGo::
	.string "Heheh! Much obliged!\n"
	.string "Now off you go!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerSingle::
	.string "Now get yourself to the BATTLE TOWER's\n"
	.string "SINGLE BATTLE ROOMS, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerDouble::
	.string "Now get yourself to the BATTLE TOWER's\n"
	.string "DOUBLE BATTLE ROOMS, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerMulti::
	.string "Now get yourself to the BATTLE TOWER's\n"
	.string "MULTI BATTLE ROOMS, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattleDomeSingle::
	.string "Now get yourself to the BATTLE DOME's\n"
	.string "SINGLE BATTLE Tourney, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattleDomeDouble::
	.string "Now get yourself to the BATTLE DOME's\n"
	.string "DOUBLE BATTLE Tourney, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattleFactorySingle::
	.string "Now get yourself to the BATTLE\n"
	.string "FACTORY's Battle Swap Single Tourney,\l"
	.string "fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattleFactoryDouble::
	.string "Now get yourself to the BATTLE\n"
	.string "FACTORY's Battle Swap Double Tourney,\l"
	.string "fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattlePalaceSingle::
	.string "Now get yourself to the BATTLE\n"
	.string "PALACE's SINGLE BATTLE HALLS, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattlePalaceDouble::
	.string "Now get yourself to the BATTLE\n"
	.string "PALACE's DOUBLE BATTLE HALLS, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattleArena::
	.string "Now get yourself to the BATTLE ARENA's\n"
	.string "Set KO Tourney, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattlePike::
	.string "Now get yourself to the BATTLE PIKE's\n"
	.string "Battle Choice, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_GetToBattlePyramid::
	.string "Now get yourself to the BATTLE\n"
	.string "PYRAMID's Battle Quest, fast!\p"
	.string "This one has to be a win!\n"
	.string "Don't choke!\l"
	.string "Both our futures ride on you!$"

BattleFrontier_Lounge3_Text_NiceTryCantReturnPoints::
	.string "Oh, it's you…\n"
	.string "Nice try…\p"
	.string "Sorry to break it to you, but I can't\n"
	.string "give your Battle Points back…\p"
	.string "Let's call it fuel to train harder\n"
	.string "next time!$"

BattleFrontier_Lounge3_Text_HelloChampHeresYourPoints::
	.string "Oh, yes!\n"
	.string "Well, if it isn't the champ!\p"
	.string "I knew you had it in you!\n"
	.string "I never doubted you!\l"
	.string "We both come out ahead!\p"
	.string "Here's your stake back, and a little\n"
	.string "bonus from yours truly, of course!$"

BattleFrontier_Lounge3_Text_ObtainedBattlePoints::
	.string "{PLAYER} received\n"
	.string "{STR_VAR_1} Battle Points.$"

BattleFrontier_Lounge3_Text_ThinkOfMeForAnotherChallenge::
	.string "If you're ever up for another round,\n"
	.string "remember my offer!$"

BattleFrontier_Lounge3_Text_NotInterested::
	.string "Not interested?! Don't be so scared\n"
	.string "to take a gamble!$"

BattleFrontier_Lounge3_Text_Oh::
	.string "Ah…$"

BattleFrontier_Lounge3_Text_BackedWrongTrainer::
	.string "I backed the wrong fighter again!\p"
	.string "Maybe I should just battle honestly\n"
	.string "like everybody else…$"

BattleFrontier_Lounge3_Text_TrainerGoodButRattled::
	.string "That fighter…\p"
	.string "He's strong, but he rattles too\n"
	.string "easily to last in the BATTLE DOME…$"

BattleFrontier_Lounge3_Text_KnowWinnerWhenISeeOne::
	.string "Giggle!\n"
	.string "I can spot a winner a mile away!$"

BattleFrontier_Lounge3_Text_ShouldBeTakingChallenges::
	.string "Those fighters…\n"
	.string "What are they up to?\l"
	.string "They ought to be out taking challenges.$"
