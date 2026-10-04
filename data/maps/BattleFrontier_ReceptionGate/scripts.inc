BattleFrontier_ReceptionGate_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_ReceptionGate_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_ReceptionGate_OnTransition
	.byte 0

BattleFrontier_ReceptionGate_OnTransition:
	setflag FLAG_LANDMARK_BATTLE_FRONTIER
	end

BattleFrontier_ReceptionGate_OnFrame:
	map_script_2 VAR_HAS_ENTERED_BATTLE_FRONTIER, 0, BattleFrontier_ReceptionGate_EventScript_FirstTimeEntering
	.2byte 0

BattleFrontier_ReceptionGate_EventScript_FirstTimeEntering::
	lockall
	setvar VAR_HAS_ENTERED_BATTLE_FRONTIER, 1
	playse SE_PIN
	applywaitmovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_Delay48
	msgbox BattleFrontier_ReceptionGate_Text_FirstTimeHereThisWay, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_PLAYER, BattleFrontier_ReceptionGate_Movement_PlayerApproachCounter
	goto BattleFrontier_ReceptionGate_EventScript_ScottScene
	end

BattleFrontier_ReceptionGate_EventScript_ScottScene::
	msgbox BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_IssueFrontierPass, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass
	waitfanfare
	waitmessage
	msgbox BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass, MSGBOX_DEFAULT
	setflag FLAG_SYS_FRONTIER_PASS
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_IfItIsntPlayerYouCame, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_ExclamationMark
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, Common_Movement_ExclamationMark
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_OhMrScottGoodDay, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, BattleFrontier_ReceptionGate_Movement_GreeterFaceScott
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, BattleFrontier_ReceptionGate_Movement_FacilityGuideFaceScott
	applymovement LOCALID_PLAYER, BattleFrontier_ReceptionGate_Movement_PlayerFaceScott
	applymovement LOCALID_FRONTIER_RECEPTION_SCOTT, BattleFrontier_ReceptionGate_Movement_ScottEnter
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_ScottGreatToSeeYouHere, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_FRONTIER_RECEPTION_SCOTT, BattleFrontier_ReceptionGate_Movement_ScottExit
	removeobject LOCALID_FRONTIER_RECEPTION_SCOTT
	releaseall
	end

BattleFrontier_ReceptionGate_Movement_PlayerApproachCounter:
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

BattleFrontier_ReceptionGate_Movement_PlayerFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

@ Unused
BattleFrontier_ReceptionGate_Movement_WalkDown:
	walk_down
	walk_down
	step_end

BattleFrontier_ReceptionGate_Movement_ScottEnter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	step_end

BattleFrontier_ReceptionGate_Movement_ScottExit:
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleFrontier_ReceptionGate_Movement_GreeterFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

BattleFrontier_ReceptionGate_Movement_FacilityGuideFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

BattleFrontier_ReceptionGate_EventScript_Greeter::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_FacilityGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToFacilities, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhich2
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BF_RECEPTIONIST
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_BattleTower
	case 1, BattleFrontier_ReceptionGate_EventScript_BattleDome
	case 2, BattleFrontier_ReceptionGate_EventScript_BattlePalace
	case 3, BattleFrontier_ReceptionGate_EventScript_BattleArena
	case 4, BattleFrontier_ReceptionGate_EventScript_BattleFactory
	case 5, BattleFrontier_ReceptionGate_EventScript_BattlePike
	case 6, BattleFrontier_ReceptionGate_EventScript_BattlePyramid
	case 7, BattleFrontier_ReceptionGate_EventScript_RankingHall
	case 8, BattleFrontier_ReceptionGate_EventScript_ExchangeCorner
	case 9, BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide
	end

BattleFrontier_ReceptionGate_EventScript_BattleTower::
	msgbox BattleFrontier_ReceptionGate_Text_BattleTowerInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleDome::
	msgbox BattleFrontier_ReceptionGate_Text_BattleDomeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePalace::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePalaceInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleArena::
	msgbox BattleFrontier_ReceptionGate_Text_BattleArenaInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleFactory::
	msgbox BattleFrontier_ReceptionGate_Text_BattleFactoryInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePike::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePikeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePyramid::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePyramidInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_RankingHall::
	msgbox BattleFrontier_ReceptionGate_Text_RankingHallInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExchangeCorner::
	msgbox BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_RulesGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToRules, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhat
	waitmessage
	multichoice 15, 0, MULTI_FRONTIER_RULES, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_LevelMode
	case 1, BattleFrontier_ReceptionGate_EventScript_Level50
	case 2, BattleFrontier_ReceptionGate_EventScript_OpenLevel
	case 3, BattleFrontier_ReceptionGate_EventScript_MonEntry
	case 4, BattleFrontier_ReceptionGate_EventScript_HoldItems
	case 5, BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide
	end

BattleFrontier_ReceptionGate_EventScript_LevelMode::
	msgbox BattleFrontier_ReceptionGate_Text_LevelModeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_Level50::
	msgbox BattleFrontier_ReceptionGate_Text_Level50Info, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_OpenLevel::
	msgbox BattleFrontier_ReceptionGate_Text_OpenLevelInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_MonEntry::
	msgbox BattleFrontier_ReceptionGate_Text_MonEntryInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_HoldItems::
	msgbox BattleFrontier_ReceptionGate_Text_HoldItemsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_FrontierPassGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhich1
	waitmessage
	multichoice 16, 4, MULTI_FRONTIER_PASS_INFO, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_Symbols
	case 1, BattleFrontier_ReceptionGate_EventScript_RecordBattle
	case 2, BattleFrontier_ReceptionGate_EventScript_BattlePoints
	case 3, BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide
	end

BattleFrontier_ReceptionGate_EventScript_Symbols::
	msgbox BattleFrontier_ReceptionGate_Text_SymbolsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_RecordBattle::
	msgbox BattleFrontier_ReceptionGate_Text_RecordedBattleInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePoints::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePointsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_Text_FirstTimeHereThisWay:
	.string "Welcome! Is this your first time?\n"
	.string "Right this way, please!$"

BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier:
	.string "Step into the ultimate tournament!\n"
	.string "Welcome to the BATTLE FRONTIER, where\l"
	.string "POKéMON battling hits its peak!$"

BattleFrontier_ReceptionGate_Text_IssueFrontierPass:
	.string "Every first-time contender receives\n"
	.string "a FRONTIER PASS.\p"
	.string "It's your entry ticket to every\n"
	.string "facility in the BATTLE FRONTIER.\p"
	.string "Here you are!$"

BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass:
	.string "{PLAYER} received\n"
	.string "the FRONTIER PASS.$"

BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass:
	.string "{PLAYER} tucked the TRAINER CARD\n"
	.string "inside the FRONTIER PASS.$"

BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier:
	.string "May you enjoy every battle the\n"
	.string "BATTLE FRONTIER has in store for you!$"

BattleFrontier_ReceptionGate_Text_IfItIsntPlayerYouCame:
	.string "???: Well, well, if it isn't {PLAYER}{KUN}!\n"
	.string "So you really came all this way!$"

BattleFrontier_ReceptionGate_Text_OhMrScottGoodDay:
	.string "GUIDE: Oh! M-MR. ANNOUNCER, sir!\n"
	.string "What an honor to see you, sir!$"

BattleFrontier_ReceptionGate_Text_ScottGreatToSeeYouHere:
	.string "ANNOUNCER: Ladies and gentlemen, what\n"
	.string "a thrill to see you here, truly!\p"
	.string "Take your time and look around every\n"
	.string "corner of the grounds.\p"
	.string "And, naturally, I hope you'll savor\n"
	.string "the pure essence of battle.\p"
	.string "My own quarters are in this building,\n"
	.string "so come by whenever you like.$"

BattleFrontier_ReceptionGate_Text_YourGuideToFacilities:
	.string "I'm here to guide you through the\n"
	.string "facilities of the BATTLE FRONTIER.$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich2:
	.string "Which one shall I explain?$"

BattleFrontier_ReceptionGate_Text_BattleTowerInfo:
	.string "That towering giant is the symbol of\n"
	.string "the BATTLE FRONTIER.\p"
	.string "Inside you'll find four kinds of BATTLE\n"
	.string "ROOMS: SINGLE, DOUBLE, MULTI, and\l"
	.string "LINK MULTI BATTLES.$"

BattleFrontier_ReceptionGate_Text_BattleDomeInfo:
	.string "The huge egg-shaped building is the\n"
	.string "BATTLE DOME.\p"
	.string "Its events are Battle Tourneys, run\n"
	.string "like a real tournament bracket.\p"
	.string "There are two courses: one for SINGLE\n"
	.string "BATTLES and one for DOUBLE BATTLES.$"

BattleFrontier_ReceptionGate_Text_BattlePalaceInfo:
	.string "The red building on the right side of\n"
	.string "the BATTLE FRONTIER is the BATTLE\l"
	.string "PALACE.\p"
	.string "It offers two kinds of BATTLE HALLS:\n"
	.string "SINGLE and DOUBLE BATTLES.$"

BattleFrontier_ReceptionGate_Text_BattleArenaInfo:
	.string "The dojo-style building toward the\n"
	.string "center-right of the BATTLE FRONTIER\l"
	.string "is the BATTLE ARENA.\p"
	.string "Its event is the Set KO Tourney.$"

BattleFrontier_ReceptionGate_Text_BattleFactoryInfo:
	.string "The big building nearest to us is the\n"
	.string "BATTLE FACTORY.\p"
	.string "Its event is the Battle Swap.\p"
	.string "There are two Battle Swap courses:\n"
	.string "SINGLE and DOUBLE BATTLES.$"

BattleFrontier_ReceptionGate_Text_BattlePikeInfo:
	.string "The building shaped like a POKéMON at\n"
	.string "the center-left of the BATTLE\l"
	.string "FRONTIER is the BATTLE PIKE.\p"
	.string "Its event is the Battle Choice.$"

BattleFrontier_ReceptionGate_Text_BattlePyramidInfo:
	.string "The colossal pyramid is the BATTLE\n"
	.string "PYRAMID.\p"
	.string "Its event is the Battle Quest.$"

BattleFrontier_ReceptionGate_Text_RankingHallInfo:
	.string "The RANKING HALL is close to the\n"
	.string "BATTLE TOWER.\p"
	.string "It displays the astonishing records\n"
	.string "set by the TRAINERS who conquered the\l"
	.string "many challenges of the BATTLE\l"
	.string "FRONTIER.$"

BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo:
	.string "The EXCHANGE SERVICE CORNER also\n"
	.string "stands near the BATTLE TOWER.\p"
	.string "There, the Battle Points you've won in\n"
	.string "the BATTLE FRONTIER turn into\l"
	.string "fabulous prizes.$"

BattleFrontier_ReceptionGate_Text_YourGuideToRules:
	.string "I'll walk you through the basic rules\n"
	.string "that apply to every challenge at the\l"
	.string "BATTLE FRONTIER's facilities.$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhat:
	.string "Which rule shall I explain?$"

BattleFrontier_ReceptionGate_Text_LevelModeInfo:
	.string "Every challenge at the BATTLE FRONTIER\n"
	.string "comes with two courses: Level 50\l"
	.string "and Open Level.$"

BattleFrontier_ReceptionGate_Text_Level50Info:
	.string "In the Level 50 course, only POKéMON of\n"
	.string "Level 50 or lower may enter.\p"
	.string "Be warned, though: not one of your\n"
	.string "opponents will field a POKéMON below\l"
	.string "Level 50.\p"
	.string "This is the starter course for battles\n"
	.string "at the BATTLE FRONTIER.\p"
	.string "We recommend beginning here.$"

BattleFrontier_ReceptionGate_Text_OpenLevelInfo:
	.string "In the Open Level course, there is no\n"
	.string "cap on the levels of your POKéMON.\p"
	.string "Your opponents' levels are adjusted to\n"
	.string "match your team.\p"
	.string "Even so, none of them will field a\n"
	.string "POKéMON below Level 60.$"

BattleFrontier_ReceptionGate_Text_MonEntryInfo:
	.string "Almost any POKéMON may enter the\n"
	.string "challenges at every facility.\p"
	.string "EGGS and certain special POKéMON are\n"
	.string "banned, however.\p"
	.string "How many POKéMON you need depends on\n"
	.string "the facility.\p"
	.string "At all of them, you may not enter two\n"
	.string "or more of the same kind of POKéMON.$"

BattleFrontier_ReceptionGate_Text_HoldItemsInfo:
	.string "When you enter a challenge at a BATTLE\n"
	.string "FRONTIER facility, no two of your\l"
	.string "POKéMON may hold the same kind of item.\p"
	.string "Please check that each POKéMON you\n"
	.string "enter holds a different item.$"

BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass:
	.string "I will be your guide to the FRONTIER\n"
	.string "PASS.$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich1:
	.string "Which part shall I explain?$"

BattleFrontier_ReceptionGate_Text_SymbolsInfo:
	.string "The BATTLE FRONTIER has seven\n"
	.string "facilities.\p"
	.string "Fighters who prove themselves may be\n"
	.string "awarded a symbol by each facility.\p"
	.string "To win a symbol, you may have to beat\n"
	.string "opponents in a long streak, and may\l"
	.string "have to win events again and again.\p"
	.string "Symbols are never easy to win.\n"
	.string "Best of luck out there!$"

BattleFrontier_ReceptionGate_Text_RecordedBattleInfo:
	.string "Your FRONTIER PASS can store one\n"
	.string "battle record.\p"
	.string "You can save a battle you fought\n"
	.string "against a friend or friends.\p"
	.string "You can also save one fought in the\n"
	.string "BATTLE FRONTIER, but not matches from\l"
	.string "the BATTLE PIKE or BATTLE PYRAMID.\p"
	.string "The game asks whether to record your\n"
	.string "match right after it ends.$"

BattleFrontier_ReceptionGate_Text_BattlePointsInfo:
	.string "Battle Points are prize money for\n"
	.string "TRAINERS who fought outstandingly in\l"
	.string "the BATTLE FRONTIER.\p"
	.string "Trade them in for prizes at the\n"
	.string "EXCHANGE SERVICE CORNER.$"

