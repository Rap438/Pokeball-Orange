DewfordTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DewfordTown_OnTransition
	.byte 0

DewfordTown_OnTransition:
	setflag FLAG_VISITED_DEWFORD_TOWN
	end

DewfordTown_EventScript_Briney::
	lock
	faceplayer
	goto_if_unset FLAG_DELIVERED_STEVEN_LETTER, DewfordTown_EventScript_ReturnToPetalburgPrompt
	message DewfordTown_Text_WhereAreWeBound
	waitmessage
	multichoicedefault 21, 6, MULTI_BRINEY_ON_DEWFORD, 2, FALSE
	switch VAR_RESULT
	case 0, DewfordTown_EventScript_ChoosePetalburg
	case 1, DewfordTown_EventScript_ChooseSlateport
	case 2, DewfordTown_EventScript_CancelSailSelect
	case MULTI_B_PRESSED, DewfordTown_EventScript_CancelSailSelect
	end

DewfordTown_EventScript_ChoosePetalburg::
	msgbox DewfordTown_Text_PetalburgWereSettingSail, MSGBOX_DEFAULT
	closemessage
	goto DewfordTown_EventScript_SailToPetalburg
	release
	end

DewfordTown_EventScript_ChooseSlateport::
	msgbox DewfordTown_Text_SlateportWereSettingSail, MSGBOX_DEFAULT
	closemessage
	goto DewfordTown_EventScript_SailToSlateport
	release
	end

DewfordTown_EventScript_CancelSailSelect::
	msgbox DewfordTown_Text_JustTellMeWhenYouNeedToSetSail, MSGBOX_DEFAULT
	closemessage
	release
	end

DewfordTown_EventScript_ReturnToPetalburgPrompt::
	msgbox DewfordTown_Text_SetSailBackToPetalburg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, DewfordTown_EventScript_SailBackToPetalburg
	msgbox DewfordTown_Text_GoDeliverIllBeWaiting, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_SailBackToPetalburg::
	msgbox DewfordTown_Text_PetalburgWereSettingSail2, MSGBOX_DEFAULT
	closemessage
	goto DewfordTown_EventScript_SailToPetalburg
	end

DewfordTown_EventScript_Woman::
	msgbox DewfordTown_Text_TinyIslandCommunity, MSGBOX_NPC
	end

DewfordTown_EventScript_TownSign::
	msgbox DewfordTown_Text_TownSign, MSGBOX_SIGN
	end

DewfordTown_EventScript_GymSign::
	msgbox DewfordTown_Text_GymSign, MSGBOX_SIGN
	end

DewfordTown_EventScript_HallSign::
	msgbox DewfordTown_Text_HallSign, MSGBOX_SIGN
	end

DewfordTown_EventScript_OldRodFisherman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_OLD_ROD, DewfordTown_EventScript_HowsFishing
	msgbox DewfordTown_Text_GettingItchToFish, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, DewfordTown_EventScript_GiveOldRod
	goto_if_eq VAR_RESULT, NO, DewfordTown_EventScript_NotGettingItchToFish
	end

DewfordTown_EventScript_GiveOldRod::
	msgbox DewfordTown_Text_GiveYouOneOfMyRods, MSGBOX_DEFAULT
	giveitem ITEM_OLD_ROD
	setflag FLAG_RECEIVED_OLD_ROD
	msgbox DewfordTown_Text_ThrowInFishingAdvice, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_NotGettingItchToFish::
	msgbox DewfordTown_Text_ThatsTooBadThen, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_HowsFishing::
	message DewfordTown_Text_HowsYourFishing
	waitmessage
	multichoice 20, 8, MULTI_HOWS_FISHING, TRUE
	goto_if_eq VAR_RESULT, 0, DewfordTown_EventScript_FishingExcellent
	goto_if_eq VAR_RESULT, 1, DewfordTown_EventScript_FishingNotSoGood
	end

DewfordTown_EventScript_FishingExcellent::
	msgbox DewfordTown_Text_GreatHaulInSomeBigOnes, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_FishingNotSoGood::
	msgbox DewfordTown_Text_FishingAdvice, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_SailToPetalburg::
	call EventScript_BackupMrBrineyLocation
	setobjectsubpriority LOCALID_DEWFORD_BRINEY, MAP_DEWFORD_TOWN, 0
	setobjectsubpriority LOCALID_PLAYER, MAP_DEWFORD_TOWN, 0
	applywaitmovement LOCALID_DEWFORD_BRINEY, DewfordTown_Movement_BrineyBoardBoat
	removeobject LOCALID_DEWFORD_BRINEY
	applywaitmovement LOCALID_PLAYER, DewfordTown_Movement_PlayerBoardBoat
	hideobjectat LOCALID_PLAYER, MAP_DEWFORD_TOWN
	call Common_EventScript_PlayBrineysBoatMusic
	applymovement LOCALID_DEWFORD_BOAT, DewfordTown_Movement_SailToPetalburg
	applymovement LOCALID_PLAYER, DewfordTown_Movement_SailToPetalburg
	waitmovement 0
	showobjectat LOCALID_PLAYER, MAP_ROUTE104
	call Common_EventScript_StopBrineysBoatMusic
	applywaitmovement LOCALID_PLAYER, DewfordTown_Movement_ExitBoatPetalburg
	showobjectat LOCALID_PLAYER, MAP_ROUTE104
	clearflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	clearflag FLAG_HIDE_ROUTE_104_MR_BRINEY_BOAT
	setflag FLAG_HIDE_MR_BRINEY_BOAT_DEWFORD_TOWN
	hideobjectat LOCALID_DEWFORD_BOAT, MAP_DEWFORD_TOWN
	setvar VAR_BOARD_BRINEY_BOAT_STATE, 2
	resetobjectsubpriority LOCALID_PLAYER, MAP_DEWFORD_TOWN
	warp MAP_ROUTE104_MR_BRINEYS_HOUSE, 5, 4
	copyvar VAR_BRINEY_LOCATION, VAR_0x8008
	waitstate
	release
	end

DewfordTown_EventScript_SailToSlateport::
	call EventScript_BackupMrBrineyLocation
	setobjectsubpriority LOCALID_DEWFORD_BRINEY, MAP_DEWFORD_TOWN, 0
	setobjectsubpriority LOCALID_PLAYER, MAP_DEWFORD_TOWN, 1
	applywaitmovement LOCALID_DEWFORD_BRINEY, DewfordTown_Movement_BrineyBoardBoat
	removeobject LOCALID_DEWFORD_BRINEY
	applywaitmovement LOCALID_PLAYER, DewfordTown_Movement_PlayerBoardBoat
	hideobjectat LOCALID_PLAYER, MAP_DEWFORD_TOWN
	call Common_EventScript_PlayBrineysBoatMusic
	applymovement LOCALID_DEWFORD_BOAT, DewfordTown_Movement_SailToSlateport
	applymovement LOCALID_PLAYER, DewfordTown_Movement_SailToSlateport
	waitmovement 0
	call Common_EventScript_StopBrineysBoatMusic
	showobjectat LOCALID_PLAYER, MAP_ROUTE109
	applywaitmovement LOCALID_PLAYER, DewfordTown_Movement_ExitBoatSlateport
	setobjectxyperm LOCALID_ROUTE109_BRINEY, 21, 26
	addobject LOCALID_ROUTE109_BRINEY
	setobjectsubpriority LOCALID_ROUTE109_BRINEY, MAP_ROUTE109, 0
	applywaitmovement LOCALID_ROUTE109_BRINEY, DewfordTown_Movement_BrineyExitBoat
	clearflag FLAG_HIDE_ROUTE_109_MR_BRINEY
	addobject LOCALID_ROUTE109_BOAT
	clearflag FLAG_HIDE_ROUTE_109_MR_BRINEY_BOAT
	setflag FLAG_HIDE_MR_BRINEY_BOAT_DEWFORD_TOWN
	hideobjectat LOCALID_DEWFORD_BOAT, MAP_DEWFORD_TOWN
	call_if_unset FLAG_DELIVERED_DEVON_GOODS, DewfordTown_EventScript_LandedSlateportDeliverGoods
	call_if_set FLAG_DELIVERED_DEVON_GOODS, DewfordTown_EventScript_LandedSlateport
	closemessage
	copyvar VAR_BRINEY_LOCATION, VAR_0x8008
	resetobjectsubpriority LOCALID_PLAYER, MAP_DEWFORD_TOWN
	resetobjectsubpriority LOCALID_ROUTE109_BRINEY, MAP_ROUTE109
	copyobjectxytoperm LOCALID_ROUTE109_BRINEY
	release
	end

DewfordTown_EventScript_LandedSlateportDeliverGoods::
	msgbox DewfordTown_Text_BrineyLandedInSlateportDeliverGoods, MSGBOX_DEFAULT
	return

DewfordTown_EventScript_LandedSlateport::
	msgbox DewfordTown_Text_BrineyLandedInSlateport, MSGBOX_DEFAULT
	return

DewfordTown_Movement_SailToPetalburg:
	walk_up
	walk_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_left
	walk_fast_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

DewfordTown_Movement_SailToSlateport:
	walk_right
	walk_fast_right
	walk_fast_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

DewfordTown_Movement_PlayerBoardBoat:
	walk_right
	walk_up
	step_end

DewfordTown_Movement_ExitBoatPetalburg:
	walk_up
	walk_up
	walk_up
	step_end

DewfordTown_Movement_ExitBoatSlateport:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

DewfordTown_Movement_BrineyBoardBoat:
	walk_up
	step_end

DewfordTown_Movement_BrineyExitBoat:
	walk_up
	walk_up
	step_end

DewfordTown_EventScript_TrendyPhraseBoy::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Text_XIsTheBiggestHappeningThingRight, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, DewfordTown_EventScript_ConfirmTrendyPhrase
	goto_if_eq VAR_RESULT, NO, DewfordTown_EventScript_RejectTrendyPhrase
	end

DewfordTown_EventScript_ConfirmTrendyPhrase::
	msgbox DewfordTown_Text_YeahDefinitionOfInRightNow, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_RejectTrendyPhrase::
	msgbox DewfordTown_Text_TellMeWhatsNewAndIn, MSGBOX_DEFAULT
	setvar VAR_0x8004, EASY_CHAT_TYPE_TRENDY_PHRASE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_EventScript_GiveNewTrendyPhrase
	goto_if_eq VAR_RESULT, FALSE, DewfordTown_EventScript_CancelNewTrendyPhrase
	end

DewfordTown_EventScript_GiveNewTrendyPhrase::
	incrementgamestat GAME_STAT_STARTED_TRENDS
	goto_if_eq VAR_0x8004, FALSE, DewfordTown_EventScript_PhraseNotTrendyEnough
	msgbox DewfordTown_Text_OfCourseIKnowAboutThat, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_CancelNewTrendyPhrase::
	msgbox DewfordTown_Text_HearOfAnyTrendsComeShareWithMe, MSGBOX_DEFAULT
	release
	end

DewfordTown_EventScript_PhraseNotTrendyEnough::
	msgbox DewfordTown_Text_XHuhIThinkYIsCool, MSGBOX_DEFAULT
	release
	end

DewfordTown_Text_TinyIslandCommunity:
	.string "DEWFORD is a tiny island, so word\n"
	.string "travels fast.\p"
	.string "If one fighter picks up a new trick\n"
	.string "here, the whole island is copying it by\l"
	.string "sundown.$"

DewfordTown_Text_TownSign:
	.string "DEWFORD TOWN\n"
	.string "“Where the tide trains the fighter.”$"

DewfordTown_Text_GymSign:
	.string "DEWFORD TOWN POKéMON GYM\n"
	.string "LEADER: TIEN\l"
	.string "“Strike with all three eyes open!”$"

DewfordTown_Text_HallSign:
	.string "DEWFORD HALL\n"
	.string "“Swap news, rumors and ki tips!”$"

Route104_Text_LandedInDewfordDeliverLetter:
	.string "MR. BRINEY: Ahoy!\n"
	.string "We've made landfall in DEWFORD.\p"
	.string "I take it you're off to hand DR. BRIEF's\n"
	.string "LETTER to, what was his name now…\l"
	.string "SUPREME KAI!$"

DewfordTown_Text_SetSailBackToPetalburg:
	.string "MR. BRINEY: Did you hand over the\n"
	.string "LETTER yet?\p"
	.string "Or did you want me to sail you back to\n"
	.string "PETALBURG?$"

DewfordTown_Text_PetalburgWereSettingSail2:
	.string "MR. BRINEY: PETALBURG, then!\p"
	.string "Anchors aweigh!\n"
	.string "PEEKO, we're shoving off, my darling!$"

DewfordTown_Text_GoDeliverIllBeWaiting:
	.string "MR. BRINEY: Then go and deliver that\n"
	.string "LETTER. I'll be right here waiting.$"

DewfordTown_Text_BrineyLandedInDewford:
	.string "MR. BRINEY: Ahoy!\n"
	.string "We've made landfall in DEWFORD!\p"
	.string "Just say the word whenever you want\n"
	.string "to set sail again!$"

DewfordTown_Text_WhereAreWeBound:
	.string "MR. BRINEY: Ahoy!\n"
	.string "For you, I'll sail out any time at all!\p"
	.string "Now then, my friend, where are we bound?$"

DewfordTown_Text_PetalburgWereSettingSail:
	.string "MR. BRINEY: PETALBURG, is it?\p"
	.string "Anchors aweigh!\n"
	.string "PEEKO, we're shoving off, my darling!$"

DewfordTown_Text_SlateportWereSettingSail:
	.string "MR. BRINEY: SLATEPORT, is it?\p"
	.string "Anchors aweigh!\n"
	.string "PEEKO, we're shoving off, my darling!$"

DewfordTown_Text_JustTellMeWhenYouNeedToSetSail:
	.string "MR. BRINEY: Whenever you need to sail\n"
	.string "again, just give me a holler!$"

DewfordTown_Text_GettingItchToFish:
	.string "This is a famous fishing spot, and a\n"
	.string "fresh-caught fish makes a fine meal.\p"
	.string "Are you getting the itch to cast a line?$"

DewfordTown_Text_GiveYouOneOfMyRods:
	.string "Ha! I knew you had the spirit!\p"
	.string "Here, I'll give you one of my fishing\n"
	.string "RODS.$"

DewfordTown_Text_ThrowInFishingAdvice:
	.string "And as a bonus, a little fishing advice!\p"
	.string "First, face the water and use the ROD.\p"
	.string "Clear your mind, like in meditation.\n"
	.string "When something bites, pull on the ROD.\p"
	.string "Small fry come in right away, but with\n"
	.string "the big ones you have to time your\l"
	.string "pulls to haul them in.$"

DewfordTown_Text_ThatsTooBadThen:
	.string "Oh, not interested?\n"
	.string "Well, that's a shame.$"

DewfordTown_Text_HowsYourFishing:
	.string "Yo!\n"
	.string "How's the fishing going?$"

DewfordTown_Text_GreatHaulInSomeBigOnes:
	.string "Is that so? Excellent!\n"
	.string "Go haul in some monsters!$"

DewfordTown_Text_FishingAdvice:
	.string "Hey, don't be down on yourself!\n"
	.string "I'll give you a little advice.\p"
	.string "First, face the water and use the ROD.\p"
	.string "Clear your mind, like in meditation.\n"
	.string "When something bites, pull on the ROD.\p"
	.string "Small fry come in right away, but with\n"
	.string "the big ones you have to time your\l"
	.string "pulls to haul them in.$"

DewfordTown_Text_XIsTheBiggestHappeningThingRight:
	.string "I live for whatever's hot and trending.\n"
	.string "I'm always on the lookout.\p"
	.string "Hey, have you heard about this new\n"
	.string "“{STR_VAR_1}”?\p"
	.string "That's right!\n"
	.string "Of course you have!\p"
	.string "I mean, wow,\n"
	.string "“{STR_VAR_1}”…\l"
	.string "It's the hottest thing going!\p"
	.string "No matter where you come from,\n"
	.string "“{STR_VAR_1}”\l"
	.string "is the next big thing, right?$"

DewfordTown_Text_TellMeWhatsNewAndIn:
	.string "Huh?\n"
	.string "It's not the hot new thing?\p"
	.string "Well then, tell me!\n"
	.string "What's new and what's “in” where you\l"
	.string "come from?$"

DewfordTown_Text_OfCourseIKnowAboutThat:
	.string "Huh?\n"
	.string "“{STR_VAR_2}”?\p"
	.string "… …\p"
	.string "…Uh… Yeah! Of course!\n"
	.string "I knew that! Knew it all along!\p"
	.string "I know all about it!\n"
	.string "“{STR_VAR_2},” right?\p"
	.string "Yep, that's the one!\n"
	.string "Isn't “{STR_VAR_2}”\l"
	.string "the coolest thing ever?\p"
	.string "It's the hottest of the hot.\n"
	.string "You think I wouldn't know?\p"
	.string "“{STR_VAR_1}”…\n"
	.string "That's so five minutes ago.\p"
	.string "Now “{STR_VAR_2}” is\n"
	.string "what's really in tune with the times!$"

DewfordTown_Text_XHuhIThinkYIsCool:
	.string "Hmm…\n"
	.string "“{STR_VAR_2},” huh?\p"
	.string "Personally, though, I still think\n"
	.string "“{STR_VAR_1}”\l"
	.string "is the real deal.$"

DewfordTown_Text_HearOfAnyTrendsComeShareWithMe:
	.string "If you hear about any hot new trends,\n"
	.string "come tell me about them, okay?$"

DewfordTown_Text_YeahDefinitionOfInRightNow:
	.string "Yeah, you said it!\p"
	.string "“{STR_VAR_1}” is exactly\n"
	.string "what's “in” right now.$"
