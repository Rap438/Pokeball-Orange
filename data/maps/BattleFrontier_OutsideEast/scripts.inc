BattleFrontier_OutsideEast_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_OutsideEast_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideEast_OnTransition
	.byte 0

BattleFrontier_OutsideEast_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

BattleFrontier_OutsideEast_OnTransition:
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0
	setflag FLAG_HIDE_BATTLE_TOWER_REPORTER
	call_if_unset FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_ShowSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_ShowSudowoodo::
	clearflag FLAG_HIDE_BATTLE_FRONTIER_SUDOWOODO
	return

BattleFrontier_OutsideEast_EventScript_BattleTowerSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleTowerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePalaceSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePalaceSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattleArenaSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleArenaSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePyramidSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePyramidSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_NinjaBoy::
	msgbox BattleFrontier_OutsideEast_Text_BattleTowerFeelsSpecial, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man1::
	msgbox BattleFrontier_OutsideEast_Text_ConquerLeagueAndFrontier, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Hiker::
	msgbox BattleFrontier_OutsideEast_Text_PyramidTooHarsh, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_HexManiac::
	msgbox BattleFrontier_OutsideEast_Text_ThriveInDarkness, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_BlackBelt1::
	msgbox BattleFrontier_OutsideEast_Text_PutTogetherUltimateTeam, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Cook::
	lock
	msgbox BattleFrontier_OutsideEast_Text_BelieveInYouBuddy, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant, MSGBOX_DEFAULT
	waitmoncry
	release
	end

BattleFrontier_OutsideEast_EventScript_RichBoy::
	msgbox BattleFrontier_OutsideEast_Text_PeopleCallMeBusybody, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_ExpertF::
	msgbox BattleFrontier_OutsideEast_Text_OnceBeatGymLeader, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_TriathleteF::
	msgbox BattleFrontier_OutsideEast_Text_FastOnBikeAndBattles, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Twin::
	msgbox BattleFrontier_OutsideEast_Text_BetterThanDaddyAtPokemon, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man2::
	lock
	msgbox BattleFrontier_OutsideEast_Text_GoRackUpSomeWinsForDaddy, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_TriathleteM::
	msgbox BattleFrontier_OutsideEast_Text_DidScottBringYouHere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Sudowoodo::
	lock
	faceplayer
	waitse
	playse SE_SUDOWOODO_SHAKE
	applywaitmovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	release
	end

BattleFrontier_OutsideEast_EventScript_WaterSudowoodo::
	lock
	faceplayer
	special DoWateringBerryTreeAnim
	waitse
	playse SE_SUDOWOODO_SHAKE
	applywaitmovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	msgbox gText_Sudowoodo_Attacked, MSGBOX_DEFAULT
	closemessage
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_FRONTIER_SUDOWOODO
	setwildbattle SPECIES_SUDOWOODO, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	setflag FLAG_DEFEATED_SUDOWOODO
	release
	end

BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo::
	setflag FLAG_DEFEATED_SUDOWOODO
	goto Common_EventScript_RemoveStaticPokemon
	end

BattleFrontier_OutsideEast_Movement_SudowoodoShake:
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	delay_8
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	step_end

BattleFrontier_OutsideEast_EventScript_Maniac1::
	msgbox BattleFrontier_OutsideEast_Text_HeardPrettyGirlAtBattleArena, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Girl::
	msgbox BattleFrontier_OutsideEast_Text_SometimesImportantOldManInThere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_PsychicM::
	msgbox BattleFrontier_OutsideEast_Text_LegendOfBattlePyramid, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man3::
	msgbox BattleFrontier_OutsideEast_Text_GotWipedOut, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Woman1::
	msgbox BattleFrontier_OutsideEast_Text_ToughTrainerInBattleTower, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Woman2::
	msgbox BattleFrontier_OutsideEast_Text_EnoughBattlePointsForDoll, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_BlackBelt2::
	msgbox BattleFrontier_OutsideEast_Text_LikeToHaveNameRecordedHere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Maniac2::
	lock
	msgbox BattleFrontier_OutsideEast_Text_CanDoAnythingWithYou, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Woman3::
	lock
	msgbox BattleFrontier_OutsideEast_Text_PowerOfOurLoveWillOvercome, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_RankingHallSign::
	msgbox BattleFrontier_OutsideEast_Text_RankingHallSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_ExchangeCornerSign::
	msgbox BattleFrontier_OutsideEast_Text_ExchangeCornerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_Gentleman::
	msgbox BattleFrontier_OutsideEast_Text_FrontierNotExclusivelyForToughTrainers, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_OldWoman::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone
	msgbox BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone::
	msgbox BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Camper::
	msgbox BattleFrontier_OutsideEast_Text_StickyMonWithLongTail, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_Text_BattleTowerSign:
	.string "BATTLE TOWER\n"
	.string "Stack up the wins and be crowned\l"
	.string "the strongest fighter!$"

BattleFrontier_OutsideEast_Text_BattlePalaceSign:
	.string "BATTLE PALACE\n"
	.string "Watch your POKéMON fight on their own!$"

BattleFrontier_OutsideEast_Text_BattleArenaSign:
	.string "BATTLE ARENA\n"
	.string "Where the toughest teams clash!$"

BattleFrontier_OutsideEast_Text_BattlePyramidSign:
	.string "BATTLE PYRAMID\n"
	.string "Press onward through the Battle Quest!$"

BattleFrontier_OutsideEast_Text_RankingHallSign:
	.string "RANKING HALL\n"
	.string "Records of the BATTLE FRONTIER's\l"
	.string "greatest warriors!$"

BattleFrontier_OutsideEast_Text_ExchangeCornerSign:
	.string "EXCHANGE SERVICE CORNER\n"
	.string "Trade Battle Points for prizes here!$"

BattleFrontier_OutsideEast_Text_BattleTowerFeelsSpecial:
	.string "Whoa!\n"
	.string "It's huge!\p"
	.string "I can sense a strange ki coming from\n"
	.string "the BATTLE TOWER. It feels different\l"
	.string "from the other buildings.$"

BattleFrontier_OutsideEast_Text_ConquerLeagueAndFrontier:
	.string "My dream is to be champion of both the\n"
	.string "BATTLE FRONTIER and the POKéMON\l"
	.string "LEAGUE, just like Mr. Satan!\p"
	.string "And once that dream comes true…\p"
	.string "I'm finally going to ask MIMI next\n"
	.string "door to be my friend!$"

BattleFrontier_OutsideEast_Text_PyramidTooHarsh:
	.string "The BATTLE PYRAMID is brutal!\n"
	.string "I just can't reach the top!\p"
	.string "I'm out of ideas, so maybe I'll try\n"
	.string "climbing the outside…$"

BattleFrontier_OutsideEast_Text_ThriveInDarkness:
	.string "I thrive in the dark…\n"
	.string "Yes… What is worthy of me?\l"
	.string "Nothing but the BATTLE PYRAMID…\p"
	.string "Dare you wander through the darkness\n"
	.string "with me, in utter and total despair?$"

BattleFrontier_OutsideEast_Text_PutTogetherUltimateTeam:
	.string "I trained for a week straight without\n"
	.string "sleep, and now I've built the ultimate\l"
	.string "team!\p"
	.string "My power level is off the charts.\n"
	.string "We're storming the BATTLE ARENA!$"

BattleFrontier_OutsideEast_Text_DidScottBringYouHere:
	.string "Did ANNOUNCER drag you here, too?\n"
	.string "What's with that guy?\p"
	.string "He really does seem to know how to\n"
	.string "spot talented fighters, but…\p"
	.string "I've never once seen him fight in\n"
	.string "person.$"

BattleFrontier_OutsideEast_Text_BelieveInYouBuddy:
	.string "ZIGG!\n"
	.string "I believe in you, little buddy!\p"
	.string "If you win, I'll make your favorite\n"
	.string "treats, any kind you like!$"

BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant:
	.string "ZIGZAGOON stares off into space…$"

BattleFrontier_OutsideEast_Text_PeopleCallMeBusybody:
	.string "They call me a meddler, but I can't\n"
	.string "help it.\p"
	.string "Your collar's crooked!\n"
	.string "Oh no, litter on the ground!\l"
	.string "Oh dear, it's nearly supper time!\p"
	.string "I don't think I can just sit back\n"
	.string "and watch at the BATTLE PALACE…$"

BattleFrontier_OutsideEast_Text_OnceBeatGymLeader:
	.string "I may not look like much now, but\n"
	.string "I once beat a GYM LEADER.\p"
	.string "Who knows, maybe it's time for one\n"
	.string "more shot at glory.$"

BattleFrontier_OutsideEast_Text_FastOnBikeAndBattles:
	.string "I'm fast on my BIKE, and just as fast\n"
	.string "in battle. My ki moves like lightning.\p"
	.string "I win matches in the blink of an eye!$"

BattleFrontier_OutsideEast_Text_BetterThanDaddyAtPokemon:
	.string "I'm already stronger than my daddy\n"
	.string "when it comes to POKéMON.$"

BattleFrontier_OutsideEast_Text_GoRackUpSomeWinsForDaddy:
	.string "Go get 'em, my little sweetie!\n"
	.string "Win big for your daddy!\p"
	.string "You get your fighting spirit from your\n"
	.string "mother, so you're sure to crush them!$"

BattleFrontier_OutsideEast_Text_HeardPrettyGirlAtBattleArena:
	.string "It wasn't easy getting here, but I'd\n"
	.string "heard about a knockout pretty girl at\l"
	.string "the BATTLE ARENA.\p"
	.string "Instead of her, guess what I found?\p"
	.string "A whole mob of sweaty, smelly,\n"
	.string "primitive martial artists!$"

BattleFrontier_OutsideEast_Text_SometimesImportantOldManInThere:
	.string "Now and then I spot a very dignified\n"
	.string "old gentleman heading in there.\p"
	.string "His POKéMON are incredible, but\n"
	.string "he seems awfully kind.$"

BattleFrontier_OutsideEast_Text_LegendOfBattlePyramid:
	.string "Have you heard the legend of the\n"
	.string "BATTLE PYRAMID?\p"
	.string "When a confident fighter reaches for\n"
	.string "the golden pinnacle, a man among men\l"
	.string "will appear.\p"
	.string "Don't know that legend?\n"
	.string "Of course not!\l"
	.string "I just made it up!\p"
	.string "What does it mean?\n"
	.string "That, my friend, I can't say!$"

BattleFrontier_OutsideEast_Text_GotWipedOut:
	.string "Man! Oh man!\n"
	.string "I'd never been beaten in my life!\p"
	.string "Then I got crushed without a shred\n"
	.string "of mercy!\p"
	.string "And they told me I've got no talent?!\n"
	.string "Man!$"

BattleFrontier_OutsideEast_Text_ToughTrainerInBattleTower:
	.string "This guy ran into a horribly strong\n"
	.string "opponent during his BATTLE TOWER\l"
	.string "challenge.\p"
	.string "Look at him now…\p"
	.string "Be careful if you ever decide to take\n"
	.string "on the BATTLE TOWER.$"

BattleFrontier_OutsideEast_Text_EnoughBattlePointsForDoll:
	.string "Yes!\n"
	.string "My Battle Points have finally piled up!\p"
	.string "Time to trade them for another\n"
	.string "giant plush DOLL!$"

BattleFrontier_OutsideEast_Text_LikeToHaveNameRecordedHere:
	.string "Oh, so this is the RANKING HALL!\p"
	.string "One day my name will be engraved\n"
	.string "here as a legend for all time.$"

BattleFrontier_OutsideEast_Text_CanDoAnythingWithYou:
	.string "As long as you're by my side,\n"
	.string "I feel like I could take on anything.$"

BattleFrontier_OutsideEast_Text_PowerOfOurLoveWillOvercome:
	.string "Ooh, darling, you're so wonderful!\p"
	.string "Ooh, I can't wait any longer!\p"
	.string "Let's go to a MULTI BATTLE ROOM\n"
	.string "right this second!\p"
	.string "If we team up in the BATTLE SALON,\n"
	.string "the power of our love will overcome\l"
	.string "everyone we meet.\p"
	.string "Why, darling, everyone will topple\n"
	.string "before us like dominoes!$"

BattleFrontier_OutsideEast_Text_FrontierNotExclusivelyForToughTrainers:
	.string "The BATTLE FRONTIER isn't only for\n"
	.string "strong TRAINERS.\p"
	.string "It seems many people with special\n"
	.string "abilities have been invited.\p"
	.string "That man ANNOUNCER…\p"
	.string "Maybe he built this place for some\n"
	.string "bigger purpose…$"

BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle:
	.string "Pardon me, young one.\n"
	.string "Are your eyes still sharp?\p"
	.string "There's a peculiar tree up ahead,\n"
	.string "and my old eyes swear it wiggles.$"

BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere:
	.string "Pardon me, young one.\n"
	.string "How is your memory?\p"
	.string "I have a feeling there once was an\n"
	.string "odd tree up ahead.$"

BattleFrontier_OutsideEast_Text_StickyMonWithLongTail:
	.string "I…\n"
	.string "I saw something!\p"
	.string "A gooey POKéMON with a long tail\n"
	.string "was ahead of me!\p"
	.string "It ducked behind a boulder and\n"
	.string "stared at me the whole time!$"
