SlateportCity_Harbor_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SlateportCity_Harbor_OnTransition
	.byte 0

SlateportCity_Harbor_OnTransition:
	setescapewarp MAP_SLATEPORT_CITY, 28, 13
	setvar VAR_TEMP_1, 0
	call_if_eq VAR_SLATEPORT_HARBOR_STATE, 1, SlateportCity_Harbor_EventScript_ReadyAquaEscapeScene
	call_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_Harbor_EventScript_ShowSSTidal
	end

SlateportCity_Harbor_EventScript_ShowSSTidal::
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_SS_TIDAL
	return

SlateportCity_Harbor_EventScript_ReadyAquaEscapeScene::
	savebgm MUS_ENCOUNTER_AQUA
	setobjectxyperm LOCALID_SLATEPORT_HARBOR_CAPT_STERN, 12, 13
	setobjectmovementtype LOCALID_SLATEPORT_HARBOR_CAPT_STERN, MOVEMENT_TYPE_FACE_LEFT
	setflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_PATRONS
	return

SlateportCity_Harbor_EventScript_AquaEscapeTrigger0::
	lockall
	setvar VAR_0x8008, 0
	goto SlateportCity_Harbor_EventScript_AquaEscapeScene
	end

SlateportCity_Harbor_EventScript_AquaEscapeTrigger1::
	lockall
	setvar VAR_0x8008, 1
	goto SlateportCity_Harbor_EventScript_AquaEscapeScene
	end

SlateportCity_Harbor_EventScript_AquaEscapeTrigger2::
	lockall
	setvar VAR_0x8008, 2
	goto SlateportCity_Harbor_EventScript_AquaEscapeScene
	end

SlateportCity_Harbor_EventScript_AquaEscapeTrigger3::
	lockall
	setvar VAR_0x8008, 3
	applywaitmovement LOCALID_PLAYER, SlateportCity_Harbor_Movement_PlayerWalkUp
	goto SlateportCity_Harbor_EventScript_AquaEscapeScene
	end

SlateportCity_Harbor_EventScript_AquaEscapeScene::
	applywaitmovement LOCALID_SLATEPORT_HARBOR_ARCHIE, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_SLATEPORT_HARBOR_GRUNT, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_Harbor_Text_ArchieYouAgainHideoutInLilycove, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_HARBOR_GRUNT, SlateportCity_Harbor_Movement_AquaBoardSub
	applymovement LOCALID_SLATEPORT_HARBOR_ARCHIE, SlateportCity_Harbor_Movement_AquaBoardSub
	applymovement LOCALID_SLATEPORT_HARBOR_SUBMARINE, SlateportCity_Harbor_Movement_SubmarineExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_HARBOR_GRUNT
	removeobject LOCALID_SLATEPORT_HARBOR_ARCHIE
	removeobject LOCALID_SLATEPORT_HARBOR_SUBMARINE
	setvar VAR_SLATEPORT_HARBOR_STATE, 2
	setflag FLAG_MET_TEAM_AQUA_HARBOR
	setflag FLAG_HIDE_LILYCOVE_MOTEL_SCOTT
	call_if_eq VAR_0x8008, 0, SlateportCity_Harbor_EventScript_SternApproachPlayer0
	call_if_eq VAR_0x8008, 1, SlateportCity_Harbor_EventScript_SternApproachPlayer1
	call_if_eq VAR_0x8008, 2, SlateportCity_Harbor_EventScript_SternApproachPlayer
	call_if_eq VAR_0x8008, 3, SlateportCity_Harbor_EventScript_SternApproachPlayer
	msgbox SlateportCity_Harbor_Text_CaptSternWhyStealMySubmarine, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HIDE_AQUA_HIDEOUT_1F_GRUNT_1_BLOCKING_ENTRANCE
	setflag FLAG_HIDE_AQUA_HIDEOUT_1F_GRUNT_2_BLOCKING_ENTRANCE
	copyobjectxytoperm LOCALID_SLATEPORT_HARBOR_CAPT_STERN
	setobjectmovementtype LOCALID_SLATEPORT_HARBOR_CAPT_STERN, MOVEMENT_TYPE_FACE_RIGHT
	releaseall
	end

SlateportCity_Harbor_EventScript_SternApproachPlayer0::
	applywaitmovement LOCALID_SLATEPORT_HARBOR_CAPT_STERN, SlateportCity_Harbor_Movement_SternApproachPlayer0
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	return

SlateportCity_Harbor_EventScript_SternApproachPlayer1::
	applywaitmovement LOCALID_SLATEPORT_HARBOR_CAPT_STERN, SlateportCity_Harbor_Movement_SternApproachPlayer1
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	return

SlateportCity_Harbor_EventScript_SternApproachPlayer::
	applywaitmovement LOCALID_SLATEPORT_HARBOR_CAPT_STERN, SlateportCity_Harbor_Movement_SternApproachPlayer
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	return

SlateportCity_Harbor_Movement_AquaBoardSub:
	delay_16
	delay_16
	jump_up
	set_invisible
	step_end

@ Unused, Archie instead shares above identical movement script with Grunt
SlateportCity_Harbor_Movement_ArchieBoardSub:
	delay_16
	delay_16
	jump_up
	set_invisible
	step_end

SlateportCity_Harbor_Movement_SubmarineExit:
	delay_16
	delay_16
	delay_16
	delay_16
	walk_right
	walk_right
	walk_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

SlateportCity_Harbor_Movement_SternApproachPlayer0:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

SlateportCity_Harbor_Movement_SternApproachPlayer1:
	walk_left
	walk_left
	walk_up
	walk_left
	step_end

SlateportCity_Harbor_Movement_SternApproachPlayer:
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Harbor_Movement_PlayerWalkUp:
	walk_up
	step_end

SlateportCity_Harbor_EventScript_FerryAttendant::
	lock
	faceplayer
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_Harbor_EventScript_AskForTicket
	msgbox SlateportCity_Harbor_Text_FerryServiceUnavailable, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_AskForTicket::
	msgbox SlateportCity_Harbor_Text_MayISeeYourTicket, MSGBOX_DEFAULT
	message SlateportCity_Harbor_Text_FlashedTicketWhereTo
	waitmessage
	goto SlateportCity_Harbor_EventScript_ChooseDestination
	end

SlateportCity_Harbor_EventScript_ChooseDestination::
	goto_if_set FLAG_MET_SCOTT_ON_SS_TIDAL, SlateportCity_Harbor_EventScript_ChooseDestinationWithBattleFrontier
	multichoicedefault 18, 8, MULTI_SSTIDAL_SLATEPORT_NO_BF, 2, FALSE
	switch VAR_RESULT
	case 0, SlateportCity_Harbor_EventScript_Lilycove
	case 1, SlateportCity_Harbor_EventScript_CancelDestinationSelect
	case MULTI_B_PRESSED, SlateportCity_Harbor_EventScript_CancelDestinationSelect
	end

SlateportCity_Harbor_EventScript_ChooseDestinationWithBattleFrontier::
	multichoicedefault 17, 6, MULTI_SSTIDAL_SLATEPORT_WITH_BF, 2, FALSE
	switch VAR_RESULT
	case 0, SlateportCity_Harbor_EventScript_Lilycove
	case 1, SlateportCity_Harbor_EventScript_BattleFrontier
	case 2, SlateportCity_Harbor_EventScript_CancelDestinationSelect
	case MULTI_B_PRESSED, SlateportCity_Harbor_EventScript_CancelDestinationSelect
	end

@ Unused. Should be impossible for player to reach Ferry without having received SS Tidal ticket
SlateportCity_Harbor_EventScript_NoTicket::
	msgbox SlateportCity_Harbor_Text_YouMustHaveTicket, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_Lilycove::
	msgbox SlateportCity_Harbor_Text_LilycoveItIs, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_Harbor_EventScript_ChooseNewDestination
	setvar VAR_SS_TIDAL_STATE, SS_TIDAL_BOARD_SLATEPORT
	call SlateportCity_Harbor_EventScript_BoardFerry
	warp MAP_SS_TIDAL_CORRIDOR, 1, 10
	waitstate
	release
	end

SlateportCity_Harbor_EventScript_BattleFrontier::
	msgbox SlateportCity_Harbor_Text_BattleFrontierItIs, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_Harbor_EventScript_ChooseNewDestination
	call SlateportCity_Harbor_EventScript_BoardFerry
	warp MAP_BATTLE_FRONTIER_OUTSIDE_WEST, 19, 67
	waitstate
	release
	end

SlateportCity_Harbor_EventScript_ChooseNewDestination::
	message SlateportCity_Harbor_Text_WhereWouldYouLikeToGo
	waitmessage
	goto SlateportCity_Harbor_EventScript_ChooseDestination
	end

SlateportCity_Harbor_EventScript_BoardFerry::
	msgbox SlateportCity_Harbor_Text_PleaseBoardFerry, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterUp
	delay 30
	hideobjectat VAR_LAST_TALKED, MAP_SLATEPORT_CITY_HARBOR
	call_if_eq VAR_FACING, DIR_NORTH, SlateportCity_Harbor_EventScript_BoardFerryNorth
	call_if_eq VAR_FACING, DIR_EAST, SlateportCity_Harbor_EventScript_BoardFerryEast
	delay 30
	hideplayer
	setvar VAR_0x8004, LOCALID_SLATEPORT_HARBOR_SS_TIDAL
	call Common_EventScript_FerryDepart
	return

SlateportCity_Harbor_EventScript_CancelDestinationSelect::
	msgbox SlateportCity_Harbor_Text_SailAnotherTime, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_BoardFerryEast::
	applywaitmovement LOCALID_PLAYER, SlateportCity_Harbor_Movement_BoardFerryEast
	return

SlateportCity_Harbor_EventScript_BoardFerryNorth::
	applywaitmovement LOCALID_PLAYER, SlateportCity_Harbor_Movement_BoardFerryNorth
	return

SlateportCity_Harbor_Movement_BoardFerryEast:
	walk_right
	walk_in_place_faster_up
	step_end

SlateportCity_Harbor_Movement_BoardFerryNorth:
	walk_up
	step_end

SlateportCity_Harbor_EventScript_Sailor::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, SlateportCity_Harbor_EventScript_SailorNoAbnormalWeather
	setvar VAR_0x8004, 0
	call_if_set FLAG_DEFEATED_KYOGRE, SlateportCity_Harbor_EventScript_CountDefeatedLegendary
	call_if_set FLAG_DEFEATED_GROUDON, SlateportCity_Harbor_EventScript_CountDefeatedLegendary
	goto_if_eq VAR_0x8004, 2, SlateportCity_Harbor_EventScript_SailorNoAbnormalWeather  @ Defeated both
	msgbox SlateportCity_Harbor_Text_AbnormalWeather, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_SailorNoAbnormalWeather::
	msgbox SlateportCity_Harbor_Text_LoveToGoDeepUnderwaterSomeday, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_CountDefeatedLegendary::
	addvar VAR_0x8004, 1
	return

SlateportCity_Harbor_EventScript_FatMan::
	msgbox SlateportCity_Harbor_Text_SubTooSmallForMe, MSGBOX_NPC
	end

SlateportCity_Harbor_EventScript_CaptStern::
	lock
	faceplayer
	goto_if_set FLAG_BADGE07_GET, SlateportCity_Harbor_EventScript_CaptSternFerryOrScannerComment
	goto_if_set FLAG_EVIL_TEAM_ESCAPED_STERN_SPOKE, SlateportCity_Harbor_EventScript_NeedDive
	goto_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, SlateportCity_Harbor_EventScript_TeamAquaLeftNeedDive
	goto_if_eq VAR_SLATEPORT_HARBOR_STATE, 2, SlateportCity_Harbor_EventScript_WhyStealSubmarine
	msgbox SlateportCity_Harbor_Text_SameThugsTriedToRobAtMuseum, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_Harbor_EventScript_WhyStealSubmarine::
	msgbox SlateportCity_Harbor_Text_CaptSternWhyStealMySubmarine, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_TeamAquaLeftNeedDive::
	setflag FLAG_EVIL_TEAM_ESCAPED_STERN_SPOKE
	msgbox SlateportCity_Harbor_Text_TeamAquaLeftNeedDive, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_NeedDive::
	msgbox SlateportCity_Harbor_Text_NeedDiveToCatchSub, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_CaptSternFerryOrScannerComment::
	goto_if_eq VAR_TEMP_1, 1, SlateportCity_Harbor_EventScript_TradedScanner
	checkitem ITEM_SCANNER
	goto_if_eq VAR_RESULT, TRUE, SlateportCity_Harbor_EventScript_AskToTradeScanner
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_Harbor_EventScript_FerryFinished
	msgbox SlateportCity_Harbor_Text_WontBeLongBeforeWeFinishFerry, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_FerryFinished::
	msgbox SlateportCity_Harbor_Text_FinishedMakingFerry, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_AskToTradeScanner::
	message SlateportCity_Harbor_Text_WouldYouTradeScanner
	waitmessage
	goto SlateportCity_Harbor_EventScript_ChooseScannerTrade
	end

SlateportCity_Harbor_EventScript_ChooseScannerTrade::
	multichoice 0, 0, MULTI_STERN_DEEPSEA, FALSE
	switch VAR_RESULT
	case 0, SlateportCity_Harbor_EventScript_DeepSeaTooth
	case 1, SlateportCity_Harbor_EventScript_DeepSeaScale
	case 2, SlateportCity_Harbor_EventScript_DeclineTrade
	case MULTI_B_PRESSED, SlateportCity_Harbor_EventScript_DeclineTrade
	end

SlateportCity_Harbor_EventScript_DeepSeaTooth::
	msgbox SlateportCity_Harbor_Text_TradeForDeepSeaTooth, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_Harbor_EventScript_ChooseDifferentTrade
	giveitem ITEM_DEEP_SEA_TOOTH
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	removeitem ITEM_SCANNER
	msgbox SlateportCity_Harbor_Text_HandedScannerToStern, MSGBOX_DEFAULT
	setflag FLAG_EXCHANGED_SCANNER
	goto SlateportCity_Harbor_EventScript_TradedScanner
	end

SlateportCity_Harbor_EventScript_DeepSeaScale::
	msgbox SlateportCity_Harbor_Text_TradeForDeepSeaScale, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_Harbor_EventScript_ChooseDifferentTrade
	giveitem ITEM_DEEP_SEA_SCALE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	removeitem ITEM_SCANNER
	msgbox SlateportCity_Harbor_Text_HandedScannerToStern, MSGBOX_DEFAULT
	setflag FLAG_EXCHANGED_SCANNER
	goto SlateportCity_Harbor_EventScript_TradedScanner
	end

SlateportCity_Harbor_EventScript_DeclineTrade::
	msgbox SlateportCity_Harbor_Text_IfYouWantToTradeLetMeKnow, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_EventScript_ChooseDifferentTrade::
	message SlateportCity_Harbor_Text_WhichOneDoYouWant
	waitmessage
	goto SlateportCity_Harbor_EventScript_ChooseScannerTrade
	end

SlateportCity_Harbor_EventScript_TradedScanner::
	setvar VAR_TEMP_1, 1
	msgbox SlateportCity_Harbor_Text_ThisWillHelpResearch, MSGBOX_DEFAULT
	release
	end

SlateportCity_Harbor_Text_FerryServiceUnavailable:
	.string "Pardon me?\n"
	.string "You're looking for a ship?\p"
	.string "I'm sorry, but the ferry isn't running at\n"
	.string "the moment…$"

SlateportCity_Harbor_Text_MayISeeYourTicket:
	.string "Hello, are you here to board the ferry?\n"
	.string "May I see your TICKET?$"

SlateportCity_Harbor_Text_YouMustHaveTicket:
	.string "{PLAYER} doesn't have the TICKET…\p"
	.string "I'm terribly sorry, but you need a\n"
	.string "TICKET to board the ferry.$"

SlateportCity_Harbor_Text_FlashedTicketWhereTo:
	.string "{PLAYER} showed the TICKET.\p"
	.string "Perfect! That's all you need!\p"
	.string "Now, where would you like to go?$"

SlateportCity_Harbor_Text_SailAnotherTime:
	.string "Please sail with us another time!$"

SlateportCity_Harbor_Text_LilycoveItIs:
	.string "LILYCOVE CITY, then! Smooth seas\n"
	.string "ahead!$"

SlateportCity_Harbor_Text_BattleFrontierItIs:
	.string "The BATTLE FRONTIER, then! Good luck\n"
	.string "out there!$"

SlateportCity_Harbor_Text_PleaseBoardFerry:
	.string "Please board the ferry and wait for\n"
	.string "departure.$"

SlateportCity_Harbor_Text_WhereWouldYouLikeToGo:
	.string "Then where would you like to go\n"
	.string "instead?$"

SlateportCity_Harbor_Text_LoveToGoDeepUnderwaterSomeday:
	.string "A journey to the bottom of the sea…\n"
	.string "What would that even be like?\p"
	.string "I'd love to dive that deep someday.$"

SlateportCity_Harbor_Text_AbnormalWeather:
	.string "A ship's crew has to know the weather\n"
	.string "to sail safely!\p"
	.string "Speaking of which, a fellow from the\n"
	.string "WEATHER INSTITUTE told me something\l"
	.string "strange.\p"
	.string "He says abnormal weather is being\n"
	.string "reported all over the region!\p"
	.string "You should stop by the WEATHER\n"
	.string "INSTITUTE and ask around!$"

SlateportCity_Harbor_Text_SubTooSmallForMe:
	.string "I wanted to go with CAPT. STERN on his\n"
	.string "ocean floor expedition.\p"
	.string "But the sub's too small for me. I'm built\n"
	.string "big from all the eating.\p"
	.string "If I squeezed in, there'd be no room\n"
	.string "left for the CAPTAIN…$"

SlateportCity_Harbor_Text_SameThugsTriedToRobAtMuseum:
	.string "CAPT. STERN: Those thugs…\p"
	.string "They're the same bunch who tried to\n"
	.string "snatch the CC GOODS at the MUSEUM.$"

SlateportCity_Harbor_Text_ArchieYouAgainHideoutInLilycove:
	.string "DR. GERO: Oh? You again, boy? Hmph.\p"
	.string "{PLAYER}, wasn't it? A child playing at\n"
	.string "hero.\p"
	.string "I'll grant you this much: you're\n"
	.string "tenacious, to chase us all the way here.\p"
	.string "But it's too late. No one can stop the\n"
	.string "SEA squad now!\p"
	.string "Or will you follow us back to our SEA\n"
	.string "BASE in LILYCOVE CITY?\p"
	.string "Kyahahaha…$"

SlateportCity_Harbor_Text_CaptSternWhyStealMySubmarine:
	.string "CAPT. STERN: Why…\p"
	.string "Why would the RED RIBBON ARMY steal my\n"
	.string "SUBMARINE EXPLORER 1?\p"
	.string "Surely they can't be after the POKéMON\n"
	.string "sleeping at the bottom of the sea…\p"
	.string "But even if I gave chase, I'd be no\n"
	.string "match for them…$"

SlateportCity_Harbor_Text_TeamAquaLeftNeedDive:
	.string "CAPT. STERN: Oh, {PLAYER}{KUN}…\p"
	.string "So the RED RIBBON ARMY got away before\n"
	.string "you could stop them…\p"
	.string "No, no, don't blame yourself. This isn't\n"
	.string "your fault.\p"
	.string "Catching a submarine underwater…\n"
	.string "That's impossible for most people.\p"
	.string "You would need a POKéMON that knows\n"
	.string "how to DIVE…\p"
	.string "Perhaps if you went to MOSSDEEP CITY…\p"
	.string "Plenty of divers live out there.\n"
	.string "Someone might teach you…$"

SlateportCity_Harbor_Text_NeedDiveToCatchSub:
	.string "CAPT. STERN: Catching a submarine\n"
	.string "underwater… That's impossible.\p"
	.string "You would need a POKéMON that knows\n"
	.string "how to DIVE…\p"
	.string "Perhaps if you went to MOSSDEEP CITY…\p"
	.string "Plenty of divers live out there.\n"
	.string "Someone might teach you…$"

SlateportCity_Harbor_Text_WontBeLongBeforeWeFinishFerry:
	.string "CAPT. STERN: Oh, yes. MR. BRINEY dropped\n"
	.string "by our SHIPYARD.\p"
	.string "It won't be long before we finish\n"
	.string "building the ferry!$"

SlateportCity_Harbor_Text_FinishedMakingFerry:
	.string "CAPT. STERN: {PLAYER}{KUN}!\p"
	.string "We finally finished the ferry S.S. TIDAL!\p"
	.string "We couldn't have done it without your\n"
	.string "friend MR. BRINEY.\p"
	.string "Please, take a short cruise on her!$"

SlateportCity_Harbor_Text_WouldYouTradeScanner:
	.string "CAPT. STERN: Oh? {PLAYER}{KUN}, that item\n"
	.string "you're carrying…\p"
	.string "That's a SCOUTER! It would be a huge\n"
	.string "help on our expeditions.\p"
	.string "{PLAYER}{KUN}, would you trade your SCOUTER\n"
	.string "for something?\p"
	.string "How about a DEEPSEATOOTH or a\n"
	.string "DEEPSEASCALE from my collection?$"

SlateportCity_Harbor_Text_IfYouWantToTradeLetMeKnow:
	.string "CAPT. STERN: Are you sure? It's no use\n"
	.string "to you, {PLAYER}{KUN}…\p"
	.string "Very well. If you change your mind about\n"
	.string "trading your SCOUTER, just let me know.$"

SlateportCity_Harbor_Text_TradeForDeepSeaTooth:
	.string "CAPT. STERN: So you'll trade it for my\n"
	.string "DEEPSEATOOTH?$"

SlateportCity_Harbor_Text_TradeForDeepSeaScale:
	.string "CAPT. STERN: So you'll trade it for my\n"
	.string "DEEPSEASCALE?$"

SlateportCity_Harbor_Text_WhichOneDoYouWant:
	.string "CAPT. STERN: Which one would you like?$"

SlateportCity_Harbor_Text_HandedScannerToStern:
	.string "{PLAYER} handed the SCOUTER over to\n"
	.string "CAPT. STERN.$"

SlateportCity_Harbor_Text_ThisWillHelpResearch:
	.string "CAPT. STERN: Thank you, {PLAYER}{KUN}! This\n"
	.string "will help our research a great deal!$"
