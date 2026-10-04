DewfordTown_Hall_MapScripts::
	.byte 0

DewfordTown_Hall_EventScript_Girl::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	special IsTrendyPhraseBoring
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_Hall_EventScript_GirlBoredOfTrend
	msgbox DewfordTown_Hall_Text_CantImagineLifeWithoutTrend, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_GirlBoredOfTrend::
	msgbox DewfordTown_Hall_Text_GettingBoredOfTrend, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_Woman::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_TeachingMonAboutTrend, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_Man::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	special BufferDeepLinkPhrase
	msgbox DewfordTown_Hall_Text_DeepLinkBetweenXAndY, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, DewfordTown_Hall_EventScript_ConfirmTrendLink
	goto_if_eq VAR_RESULT, NO, DewfordTown_Hall_EventScript_RejectTrendLink
	end

DewfordTown_Hall_EventScript_ConfirmTrendLink::
	msgbox DewfordTown_Hall_Text_MyHunchWasRight, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_RejectTrendLink::
	msgbox DewfordTown_Hall_Text_NotEasyToKeepUp, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_ExpertM::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_TVShowAboutTrend, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_DEWFORD_HALL_EXPERT_M, Common_Movement_WalkInPlaceFasterUp
	release
	end

DewfordTown_Hall_EventScript_Twin::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_IsTrendMorePopularAcrossSea, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_DEWFORD_HALL_TWIN, Common_Movement_WalkInPlaceFasterUp
	release
	end

DewfordTown_Hall_EventScript_LittleBoy::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_CollectTrendMerchandise, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_Bookshelf::
	lockall
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_BooksAboutTrend, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_Painting::
	lockall
	call Common_EventScript_BufferTrendyPhrase
	special GetDewfordHallPaintingNameIndex
	switch VAR_RESULT
	case 0, DewfordTown_Hall_EventScript_ScreamTitle
	case 4, DewfordTown_Hall_EventScript_ScreamTitle
	case 1, DewfordTown_Hall_EventScript_SmileTitle
	case 5, DewfordTown_Hall_EventScript_ScreamTitle
	case 2, DewfordTown_Hall_EventScript_LastTitle
	case 6, DewfordTown_Hall_EventScript_LastTitle
	case 3, DewfordTown_Hall_EventScript_BirthTitle
	case 7, DewfordTown_Hall_EventScript_LastTitle
	end

DewfordTown_Hall_EventScript_ScreamTitle::
	msgbox DewfordTown_Hall_Text_TrendsScream, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_SmileTitle::
	msgbox DewfordTown_Hall_Text_TrendsSmile, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_LastTitle::
	msgbox DewfordTown_Hall_Text_LastTrend, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_BirthTitle::
	msgbox DewfordTown_Hall_Text_BirthOfTrend, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_SchoolKidM::
	lockall
	setvar VAR_0x8008, 0
	goto DewfordTown_Hall_EventScript_DoTrendDebate
	end

DewfordTown_Hall_EventScript_PsychicM::
	lockall
	setvar VAR_0x8008, 1
	goto DewfordTown_Hall_EventScript_DoTrendDebate
	end

DewfordTown_Hall_EventScript_DoTrendDebate::
	call Common_EventScript_BufferTrendyPhrase
	special GetDewfordHallPaintingNameIndex
	switch VAR_RESULT
	case 0, DewfordTown_Hall_EventScript_TrendDebate1
	case 1, DewfordTown_Hall_EventScript_TrendDebate1
	case 2, DewfordTown_Hall_EventScript_TrendDebate2
	case 3, DewfordTown_Hall_EventScript_TrendDebate2
	case 4, DewfordTown_Hall_EventScript_TrendDebate3
	case 5, DewfordTown_Hall_EventScript_TrendDebate3
	case 6, DewfordTown_Hall_EventScript_TrendDebate4
	case 7, DewfordTown_Hall_EventScript_TrendDebate5
	end

DewfordTown_Hall_EventScript_TrendDebate1::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_SawTrendCoolestThing, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_AllegedTrendNotAuthentic, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate2::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_ComposedTrendThemeSong, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_WorkOnYourSinging, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate3::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_OrganizeTrendParty, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_BrilliantIndeed, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate4::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_TrendHasBecomePartOfLife, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_TrendWeighsHeavilyOnMind, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate5::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_IfWeTeamUpWellBeInvincible, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_WellBeTrendDuo, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_DontMovePlayer1::
	return

DewfordTown_Hall_EventScript_DebateReact1::
	applywaitmovement LOCALID_DEWFORD_HALL_PSYCHIC_M, DewfordTown_Hall_Movement_PsychicWalkInPlaceLeft
	goto_if_eq VAR_0x8008, 0, DewfordTown_Hall_EventScript_PlayerReactWest
	goto_if_eq VAR_0x8008, 1, DewfordTown_Hall_EventScript_DontMovePlayer2
	end

DewfordTown_Hall_EventScript_PlayerReactWest::
	goto_if_eq VAR_FACING, DIR_EAST, DewfordTown_Hall_EventScript_DontMovePlayer1
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	return

DewfordTown_Hall_EventScript_DontMovePlayer2::
	return

DewfordTown_Hall_EventScript_DebateReact2::
	applywaitmovement LOCALID_DEWFORD_HALL_SCHOOL_KID_M, DewfordTown_Hall_Movement_SchoolKidWalkInPlaceRight
	goto_if_eq VAR_0x8008, 0, DewfordTown_Hall_EventScript_PlayerReactNorthSouth
	goto_if_eq VAR_0x8008, 1, DewfordTown_Hall_EventScript_PlayerReactEast
	end

DewfordTown_Hall_EventScript_PlayerReactNorthSouth::
	call_if_eq VAR_FACING, DIR_NORTH, DewfordTown_Hall_EventScript_PlayerWalkInPlaceUp
	call_if_eq VAR_FACING, DIR_SOUTH, DewfordTown_Hall_EventScript_PlayerWalkInPlaceDown
	return

DewfordTown_Hall_EventScript_PlayerWalkInPlaceUp::
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	return

DewfordTown_Hall_EventScript_PlayerWalkInPlaceDown::
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	return

DewfordTown_Hall_EventScript_PlayerReactEast::
	goto_if_eq VAR_FACING, DIR_WEST, DewfordTown_Hall_EventScript_DontMovePlayer1
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	return

DewfordTown_Hall_Movement_PsychicWalkInPlaceLeft:
	walk_in_place_left
	step_end

DewfordTown_Hall_Movement_SchoolKidWalkInPlaceRight:
	walk_in_place_right
	step_end

DewfordTown_Hall_EventScript_SludgeBombMan::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	goto_if_set FLAG_RECEIVED_TM_SLUDGE_BOMB, DewfordTown_Hall_EventScript_ReceivedSludgeBomb
	msgbox DewfordTown_Hall_Text_GiveYouSludgeBomb, MSGBOX_DEFAULT
	giveitem ITEM_TM_SLUDGE_BOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_SLUDGE_BOMB
	release
	end

DewfordTown_Hall_EventScript_ReceivedSludgeBomb::
	msgbox DewfordTown_Hall_Text_LoveSludgeBombButTrendInToo, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_Text_CantImagineLifeWithoutTrend:
	.string "What's all the rage? Why, it has to\n"
	.string "be “{STR_VAR_1}”!\p"
	.string "I can't picture a single day without\n"
	.string "“{STR_VAR_1}”!$"

DewfordTown_Hall_Text_GettingBoredOfTrend:
	.string "What's all the rage? Why, it has to\n"
	.string "be “{STR_VAR_1}”!\p"
	.string "But I'm getting bored of it.\p"
	.string "Time to hunt for the next big thing,\n"
	.string "like a fighter chasing a stronger foe.$"

DewfordTown_Hall_Text_TeachingMonAboutTrend:
	.string "I'm teaching my POKéMON about\n"
	.string "“{STR_VAR_1},”\l"
	.string "but it isn't going well.\p"
	.string "Maybe it's too much for them.$"

DewfordTown_Hall_Text_DeepLinkBetweenXAndY:
	.string "I'm studying up on “{STR_VAR_1}”\n"
	.string "right now.\p"
	.string "Is it true there's a deep link between\n"
	.string "“{STR_VAR_1}” and “{STR_VAR_2}”?$"

DewfordTown_Hall_Text_MyHunchWasRight:
	.string "Oh! So my hunch was right!\p"
	.string "I'm one step closer to being the best\n"
	.string "informed fighter on the island!$"

DewfordTown_Hall_Text_NotEasyToKeepUp:
	.string "What?! Is that so?!\p"
	.string "It's not easy for an old timer like me\n"
	.string "to keep up! My power level isn't what\l"
	.string "it used to be!$"

DewfordTown_Hall_Text_TVShowAboutTrend:
	.string "This whole business about\n"
	.string "“{STR_VAR_1}”…\p"
	.string "I think I saw it on a TV show. Didn't I?$"

DewfordTown_Hall_Text_IsTrendMorePopularAcrossSea:
	.string "Over the sea…\p"
	.string "Is “{STR_VAR_1}”\n"
	.string "even bigger over there?$"

DewfordTown_Hall_Text_CollectTrendMerchandise:
	.string "I collect official “{STR_VAR_1}”\n"
	.string "merchandise.\p"
	.string "I've got “{STR_VAR_1}” DOLLS…\p"
	.string "And “{STR_VAR_1}” training gear…\p"
	.string "And licensed “{STR_VAR_1}”\n"
	.string "picture books.\p"
	.string "Heh heh, it's all mine!\n"
	.string "I'm not sharing a thing with you!$"

DewfordTown_Hall_Text_BooksAboutTrend:
	.string "IDENTIFYING GOOD\n"
	.string "“{STR_VAR_1}” & BAD “{STR_VAR_1}”…\p"
	.string "THE LINK BETWEEN\n"
	.string "“{STR_VAR_1}” AND POKéMON…\p"
	.string "USEFUL “{STR_VAR_1}” FOR\n"
	.string "MARTIAL ARTISTS…\p"
	.string "The shelf is jammed with books about\n"
	.string "“{STR_VAR_1}.”$"

DewfordTown_Hall_Text_TrendsScream:
	.string "The title reads “{STR_VAR_1}'S\n"
	.string "SCREAM.”$"

DewfordTown_Hall_Text_TrendsSmile:
	.string "The title reads “{STR_VAR_1}'S\n"
	.string "SMILE.”$"

DewfordTown_Hall_Text_LastTrend:
	.string "The title reads “THE LAST\n"
	.string "{STR_VAR_1}.”$"

DewfordTown_Hall_Text_BirthOfTrend:
	.string "The title reads “THE BIRTH OF\n"
	.string "{STR_VAR_1}.”$"

DewfordTown_Hall_Text_SawTrendCoolestThing:
	.string "I saw “{STR_VAR_1}” in person!\p"
	.string "It was incredible! My power level shot\n"
	.string "through the roof just watching!\p"
	.string "It was the real deal, oh yeah!$"

DewfordTown_Hall_Text_AllegedTrendNotAuthentic:
	.string "Oh, no, no, no.\p"
	.string "That so-called\n"
	.string "“{STR_VAR_1}”\l"
	.string "you saw wasn't the real thing.\p"
	.string "The authentic article is sharper, yet\n"
	.string "somehow more mellow!\p"
	.string "Ah, never mind. It's amazing anyway!$"

DewfordTown_Hall_Text_ComposedTrendThemeSong:
	.string "Hey, listen, I wrote a theme song\n"
	.string "for “{STR_VAR_1}.”\p"
	.string "“{STR_VAR_1}” is all the rage\n"
	.string "right now.\p"
	.string "Okay, here goes!\n"
	.string "… …\p"
	.string "Oh!\n"
	.string "{STR_VAR_1}!\p"
	.string "Marvelous\n"
	.string "{STR_VAR_1}!$"

DewfordTown_Hall_Text_WorkOnYourSinging:
	.string "… …\p"
	.string "Friend, you'd best train your singing\n"
	.string "voice before you go meddling with\l"
	.string "“{STR_VAR_1}.”$"

DewfordTown_Hall_Text_OrganizeTrendParty:
	.string "Anyway, like I said before, we should\n"
	.string "gather the whole island and throw a\l"
	.string "“{STR_VAR_1}” tournament.$"

DewfordTown_Hall_Text_BrilliantIndeed:
	.string "Splendid idea!\p"
	.string "It'll settle once and for all who is\n"
	.string "the best at “{STR_VAR_1}.”\l"
	.string "Brilliant!\p"
	.string "Starting today, our lives revolve\n"
	.string "around “{STR_VAR_1}”!$"

DewfordTown_Hall_Text_TrendHasBecomePartOfLife:
	.string "But then I got to thinking…\p"
	.string "Don't you agree “{STR_VAR_1}” has\n"
	.string "grown from a passing trend into part\l"
	.string "of our daily lives?$"

DewfordTown_Hall_Text_TrendWeighsHeavilyOnMind:
	.string "Pardon? That much?\p"
	.string "Still, it's true that “{STR_VAR_1}”\n"
	.string "weighs on your mind, awake or asleep.\p"
	.string "No doubt about it, you're the\n"
	.string "“{STR_VAR_1}”\l"
	.string "MASTER, old sport!$"

DewfordTown_Hall_Text_IfWeTeamUpWellBeInvincible:
	.string "If you and I team up as a duo, we'll\n"
	.string "be unbeatable when it comes to\l"
	.string "anything “{STR_VAR_1}”!$"

DewfordTown_Hall_Text_WellBeTrendDuo:
	.string "Spot on, my friend!\p"
	.string "We shall be the\n"
	.string "“{STR_VAR_1}” DUO!\p"
	.string "Isn't that a splendid image?\n"
	.string "Hahahah!$"

DewfordTown_Hall_Text_GiveYouSludgeBomb:
	.string "In my book, SLUDGE BOMB is the\n"
	.string "hottest move there is.\p"
	.string "Huh? You don't know about SLUDGE\n"
	.string "BOMB? A fighter like you?\p"
	.string "That's just sad.\n"
	.string "Here, I'll give you one.$"

DewfordTown_Hall_Text_LoveSludgeBombButTrendInToo:
	.string "I love SLUDGE BOMB.\p"
	.string "But POKéMON with the move\n"
	.string "“{STR_VAR_1}”\l"
	.string "are all the rage, too.$"

