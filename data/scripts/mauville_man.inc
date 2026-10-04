MauvilleCity_PokemonCenter_1F_EventScript_MauvilleOldMan::
	special Script_GetCurrentMauvilleMan
	switch VAR_RESULT
	case MAUVILLE_MAN_BARD, MauvilleCity_PokemonCenter_1F_EventScript_Bard
	case MAUVILLE_MAN_HIPSTER, MauvilleCity_PokemonCenter_1F_EventScript_Hipster
	case MAUVILLE_MAN_TRADER, MauvilleCity_PokemonCenter_1F_EventScript_Trader
	case MAUVILLE_MAN_STORYTELLER, MauvilleCity_PokemonCenter_1F_EventScript_Storyteller
	case MAUVILLE_MAN_GIDDY, MauvilleCity_PokemonCenter_1F_EventScript_Giddy
	end

@ Bard
MauvilleCity_PokemonCenter_1F_EventScript_Bard::
	lock
	faceplayer
	msgbox MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToHearMySong, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_PlaySong
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineSong
	end

MauvilleCity_PokemonCenter_1F_EventScript_PlaySong::
	setvar VAR_0x8004, FALSE  @ Play his current song
	special PlayBardSong
	delay 60
	special HasBardSongBeenChanged
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_AskToWriteLyrics
	msgbox MauvilleCity_PokemonCenter_1F_Text_WishICouldPlaySongForOthers, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineSong::
	msgbox MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues1, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_AskToWriteLyrics::
	msgbox MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToWriteSomeLyrics, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_WriteLyrics
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineWritingLyrics
	end

MauvilleCity_PokemonCenter_1F_EventScript_WriteLyrics::
	setvar VAR_0x8004, EASY_CHAT_TYPE_BARD_SONG
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_DeclineWritingLyrics
	msgbox MauvilleCity_PokemonCenter_1F_Text_LetMeSingItForYou, MSGBOX_DEFAULT
	setvar VAR_0x8004, TRUE  @ Play the new song
	special PlayBardSong
	delay 60
	msgbox MauvilleCity_PokemonCenter_1F_Text_ThatHowYouWantedSongToGo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_WriteLyrics
	special SaveBardSongLyrics
	msgbox MauvilleCity_PokemonCenter_1F_Text_IllSingThisSongForAWhile, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineWritingLyrics::
	msgbox MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues2, MSGBOX_DEFAULT
	release
	end

@ Hipster
MauvilleCity_PokemonCenter_1F_EventScript_Hipster::
	lock
	faceplayer
	setflag FLAG_UNLOCKED_TRENDY_SAYINGS
	msgbox MauvilleCity_PokemonCenter_1F_Text_TeachWhatsHipAndHappening, MSGBOX_DEFAULT
	special HasHipsterTaughtWord
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_TryTeachWord
	msgbox MauvilleCity_PokemonCenter_1F_Text_IAlreadyTaughtYou, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_TryTeachWord::
	special HipsterTryTeachWord
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_TeachWord
	msgbox MauvilleCity_PokemonCenter_1F_Text_IveGotNothingNewToTeach, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_TeachWord::
	msgbox MauvilleCity_PokemonCenter_1F_Text_HaveYouHeardOfWord, MSGBOX_DEFAULT
	special SetHipsterTaughtWord
	release
	end

@ Trader
MauvilleCity_PokemonCenter_1F_Text_WantToTradeDecor:
	.string "Yo, I'm the TRADER.\n"
	.string "Want to swap decorations with me?$"

MauvilleCity_PokemonCenter_1F_Text_TraderFeelingTheBlues:
	.string "Oh…\n"
	.string "Now I'm feeling the blues…$"

MauvilleCity_PokemonCenter_1F_Text_WeveAlreadyTraded:
	.string "But we already swapped decorations,\n"
	.string "you and I.$"

MauvilleCity_PokemonCenter_1F_Text_PickADecorItem:
	.string "If you see a decoration of mine that\n"
	.string "you want, speak up.$"

MauvilleCity_PokemonCenter_1F_Text_YouDontWantAnything:
	.string "You don't want anything?\n"
	.string "I feel so unwanted…$"

MauvilleCity_PokemonCenter_1F_Text_OnceBelongedToPlayerDoYouWantIt:
	.string "That decoration once belonged\n"
	.string "to {STR_VAR_1}.\p"
	.string "Do you want it?$"

MauvilleCity_PokemonCenter_1F_Text_YouDontHaveAnyDecor:
	.string "Uh… Wait a second. You don't have a\n"
	.string "single decoration!$"

MauvilleCity_PokemonCenter_1F_Text_PickTheDecorToTrade:
	.string "Okay, pick the decoration you'll\n"
	.string "trade to me.$"

MauvilleCity_PokemonCenter_1F_Text_YouDontWantToTrade:
	.string "You won't trade with me?\n"
	.string "I feel so unwanted…$"

MauvilleCity_PokemonCenter_1F_Text_YouveNoRoomForThis:
	.string "You already hold all the {STR_VAR_2}S\n"
	.string "you can store. No room for this one.$"

MauvilleCity_PokemonCenter_1F_Text_SoWellTradeTheseDecor:
	.string "Okay, so we swap my {STR_VAR_3}\n"
	.string "for your {STR_VAR_2}?$"

MauvilleCity_PokemonCenter_1F_Text_ThatDecorIsInUse:
	.string "That decoration is in use right now.\n"
	.string "You can't trade it.$"

MauvilleCity_PokemonCenter_1F_Text_SendDecorToYourPC:
	.string "Then it's a deal!\n"
	.string "I'll send my decoration to your PC.$"

MauvilleCity_PokemonCenter_1F_Text_CantTradeThatOne:
	.string "Whoa, sorry! That one is really rare.\n"
	.string "I can't let it go!\p"
	.string "Can I interest you in something else?$"

MauvilleCity_PokemonCenter_1F_EventScript_Trader::
	lock
	faceplayer
	msgbox MauvilleCity_PokemonCenter_1F_Text_WantToTradeDecor, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineTrade
	special GetTraderTradedFlag
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_AlreadyTraded
	message MauvilleCity_PokemonCenter_1F_Text_PickADecorItem
	waitmessage
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineTrade::
	msgbox MauvilleCity_PokemonCenter_1F_Text_TraderFeelingTheBlues, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_AlreadyTraded::
	msgbox MauvilleCity_PokemonCenter_1F_Text_WeveAlreadyTraded, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive::
	special TraderMenuGetDecoration
	goto_if_eq VAR_0x8004, 0, MauvilleCity_PokemonCenter_1F_EventScript_CancelPickDecor
	goto_if_eq VAR_0x8004, 0xFFFF, MauvilleCity_PokemonCenter_1F_EventScript_InvalidDecor
	msgbox MauvilleCity_PokemonCenter_1F_Text_OnceBelongedToPlayerDoYouWantIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_PickDifferentDecor
	special DoesPlayerHaveNoDecorations
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_DontHaveAnyDecor
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive
	end

MauvilleCity_PokemonCenter_1F_EventScript_CancelPickDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDontWantAnything, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_InvalidDecor::
	message MauvilleCity_PokemonCenter_1F_Text_CantTradeThatOne
	waitmessage
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive
	end

MauvilleCity_PokemonCenter_1F_EventScript_PickDifferentDecor::
	message MauvilleCity_PokemonCenter_1F_Text_PickADecorItem
	waitmessage
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive
	end

MauvilleCity_PokemonCenter_1F_EventScript_DontHaveAnyDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDontHaveAnyDecor, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive::
	msgbox MauvilleCity_PokemonCenter_1F_Text_PickTheDecorToTrade, MSGBOX_DEFAULT
	special TraderShowDecorationMenu
	goto_if_eq VAR_0x8006, 0, MauvilleCity_PokemonCenter_1F_EventScript_CancelGiveDecor
	goto_if_eq VAR_0x8006, 0xFFFF, MauvilleCity_PokemonCenter_1F_EventScript_DecorInUse
	special IsDecorationCategoryFull
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_NoRoomForDecor
	msgbox MauvilleCity_PokemonCenter_1F_Text_SoWellTradeTheseDecor, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive
	special TraderDoDecorationTrade
	msgbox MauvilleCity_PokemonCenter_1F_Text_SendDecorToYourPC, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_CancelGiveDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDontWantToTrade, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DecorInUse::
	msgbox MauvilleCity_PokemonCenter_1F_Text_ThatDecorIsInUse, MSGBOX_DEFAULT
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive
	end

MauvilleCity_PokemonCenter_1F_EventScript_NoRoomForDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouveNoRoomForThis, MSGBOX_DEFAULT
	release
	end

@ Storyteller
MauvilleCity_PokemonCenter_1F_Text_WillYouHearMyTale:
	.string "I'm the STORYTELLER.\n"
	.string "I spin tales of legendary warriors\l"
	.string "and TRAINERS.\p"
	.string "Will you hear my tale?$"

MauvilleCity_PokemonCenter_1F_Text_StorytellerFeelingTheBlues:
	.string "Oh…\n"
	.string "Now I'm feeling the blues…$"

MauvilleCity_PokemonCenter_1F_Text_WhichTaleToTell:
	.string "I know these legends.\n"
	.string "Which tale shall I tell?$"

MauvilleCity_PokemonCenter_1F_Text_IKnowNoTales:
	.string "But I know of no legendary TRAINERS,\n"
	.string "and so I know no tales.\p"
	.string "Where does one find a TRAINER worthy\n"
	.string "of a legendary tale?$"

MauvilleCity_PokemonCenter_1F_Text_YouDidStatXTimes:
	.string "What's that?!\n"
	.string "You… You…\p"
	.string "{STR_VAR_2}\n"
	.string "{STR_VAR_1} time(s)?!\p"
	.string "Now that is magnificent!\n"
	.string "A new legend is born!$"

MauvilleCity_PokemonCenter_1F_Text_CouldThereBeOtherLegends:
	.string "It makes me wonder, could other\n"
	.string "TRAINERS with even greater power\l"
	.string "be waiting to be discovered?$"

MauvilleCity_PokemonCenter_1F_Text_HaveYouAnyLegendaryTales:
	.string "Are you a TRAINER?\p"
	.string "Then tell me, have you any deeds\n"
	.string "that are even remotely legendary?$"

@ Unused
MauvilleCity_PokemonCenter_1F_Text_HearAnotherLegendaryTale:
	.string "By the way… Would you care to hear\n"
	.string "another legendary tale?$"

MauvilleCity_PokemonCenter_1F_Text_NotWorthyOfLegend:
	.string "Hmm…\n"
	.string "I'm not satisfied…\p"
	.string "Bring me news worthy of being\n"
	.string "called a legend.$"

MauvilleCity_PokemonCenter_1F_Text_IWishMorePeopleWereInterested:
	.string "I wish more people cared to hear my\n"
	.string "epic tales of legendary\l"
	.string "TRAINERS.$"

MauvilleCity_PokemonCenter_1F_Text_SavedGameTitle::
	.string "The Save-Happy TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_SavedGameAction::
	.string "Saved the game$"

MauvilleCity_PokemonCenter_1F_Text_SavedGameStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER saved the game\n"
	.string "{STR_VAR_1} times!\p"
	.string "No one is more cautious than\n"
	.string "{STR_VAR_3}, that's for sure!$"

MauvilleCity_PokemonCenter_1F_Text_TrendsStartedTitle::
	.string "The Trendsetter TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_TrendsStartedAction::
	.string "Started trends$"

MauvilleCity_PokemonCenter_1F_Text_TrendsStartedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER started new trends\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} is setting the pace for all\n"
	.string "of the HOENN region!$"

MauvilleCity_PokemonCenter_1F_Text_BerriesPlantedTitle::
	.string "The BERRY-Planting TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_BerriesPlantedAction::
	.string "Planted BERRIES$"

MauvilleCity_PokemonCenter_1F_Text_BerriesPlantedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER planted BERRIES\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} is a legendary lover of\n"
	.string "BERRIES!$"

MauvilleCity_PokemonCenter_1F_Text_BikeTradesTitle::
	.string "The BIKE-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_BikeTradesAction::
	.string "Traded BIKES$"

MauvilleCity_PokemonCenter_1F_Text_BikeTradesStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER swapped BIKES\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must love BIKES as much\n"
	.string "as BULMA loves her inventions!$"

MauvilleCity_PokemonCenter_1F_Text_InterviewsTitle::
	.string "The Interviewed TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_InterviewsAction::
	.string "Got interviewed$"

MauvilleCity_PokemonCenter_1F_Text_InterviewsStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER got interviewed\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must be drawing the\n"
	.string "attention of the whole world!$"

MauvilleCity_PokemonCenter_1F_Text_TrainerBattlesTitle::
	.string "The Battle-Happy TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_TrainerBattlesAction::
	.string "Battled$"

MauvilleCity_PokemonCenter_1F_Text_TrainerBattlesStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER battled {STR_VAR_1} times!\p"
	.string "{STR_VAR_3} can never turn down a\n"
	.string "chance to fight, a true warrior!$"

MauvilleCity_PokemonCenter_1F_Text_PokemonCaughtTitle::
	.string "The POKéMON-Catching TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_PokemonCaughtAction::
	.string "Caught POKéMON$"

MauvilleCity_PokemonCenter_1F_Text_PokemonCaughtStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER caught\n"
	.string "{STR_VAR_1} POKéMON!\p"
	.string "{STR_VAR_3} is a legendary catcher of\n"
	.string "wild POKéMON!$"

MauvilleCity_PokemonCenter_1F_Text_FishingPokemonCaughtTitle::
	.string "The Fishing TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_FishingPokemonCaughtAction::
	.string "Caught POKéMON with a ROD$"

MauvilleCity_PokemonCenter_1F_Text_FishingPokemonCaughtStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER caught\n"
	.string "{STR_VAR_1} POKéMON while fishing!\p"
	.string "{STR_VAR_3} is a legendary master of\n"
	.string "the fishing rod!$"

MauvilleCity_PokemonCenter_1F_Text_EggsHatchedTitle::
	.string "The EGG-Warming TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_EggsHatchedAction::
	.string "Hatched EGGS$"

MauvilleCity_PokemonCenter_1F_Text_EggsHatchedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER hatched {STR_VAR_1} POKéMON\n"
	.string "from EGGS!\p"
	.string "{STR_VAR_3} is a legendary warmer\n"
	.string "of EGGS!$"

MauvilleCity_PokemonCenter_1F_Text_PokemonEvolvedTitle::
	.string "The Evolver TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_PokemonEvolvedAction::
	.string "Evolved POKéMON$"

MauvilleCity_PokemonCenter_1F_Text_PokemonEvolvedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER made {STR_VAR_1} POKéMON\n"
	.string "evolve!\p"
	.string "{STR_VAR_3} is the ultimate evolver\n"
	.string "of POKéMON!$"

MauvilleCity_PokemonCenter_1F_Text_UsedPokemonCenterTitle::
	.string "The POKéMON CENTER-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_UsedPokemonCenterAction::
	.string "Used POKéMON CENTERS$"

MauvilleCity_PokemonCenter_1F_Text_UsedPokemonCenterStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER healed POKéMON\n"
	.string "{STR_VAR_1} times at POKéMON CENTERS!\p"
	.string "No one loves POKéMON CENTERS more\n"
	.string "than {STR_VAR_3}, I'd wager!$"

MauvilleCity_PokemonCenter_1F_Text_RestedAtHomeTitle::
	.string "The Homebody TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_RestedAtHomeAction::
	.string "Rested POKéMON at home$"

MauvilleCity_PokemonCenter_1F_Text_RestedAtHomeStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER healed POKéMON\n"
	.string "{STR_VAR_1} times at home!\p"
	.string "No one is more of a homebody than\n"
	.string "{STR_VAR_3}, a dinner-at-home hero!$"

MauvilleCity_PokemonCenter_1F_Text_SafariGamesTitle::
	.string "The SAFARI-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_SafariGamesAction::
	.string "Entered the SAFARI ZONE$"

MauvilleCity_PokemonCenter_1F_Text_SafariGamesStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER went into the SAFARI ZONE\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3}'s wild side must come out\n"
	.string "in the SAFARI ZONE!$"

MauvilleCity_PokemonCenter_1F_Text_UsedCutTitle::
	.string "The CUT-Frenzy TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_UsedCutAction::
	.string "Used CUT$"

MauvilleCity_PokemonCenter_1F_Text_UsedCutStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER used CUT\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} just must love to CUT\n"
	.string "things down!$"

MauvilleCity_PokemonCenter_1F_Text_UsedRockSmashTitle::
	.string "The ROCK-SMASHING TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_UsedRockSmashAction::
	.string "Smashed rocks$"

MauvilleCity_PokemonCenter_1F_Text_UsedRockSmashStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER used ROCK SMASH\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} can't leave a single stone\n"
	.string "unsmashed!$"

MauvilleCity_PokemonCenter_1F_Text_MovedBasesTitle::
	.string "The Move-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_MovedBasesAction::
	.string "Moved the SECRET BASE$"

MauvilleCity_PokemonCenter_1F_Text_MovedBasesStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER moved the SECRET BASE\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} loves to move house, always\n"
	.string "looking for a better hideout!$"

MauvilleCity_PokemonCenter_1F_Text_UsedSplashTitle::
	.string "The SPLASH-Happy TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_UsedSplashAction::
	.string "Used SPLASH$"

MauvilleCity_PokemonCenter_1F_Text_UsedSplashStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER used SPLASH\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must love SPLASHING\n"
	.string "around!$"

MauvilleCity_PokemonCenter_1F_Text_UsedStruggleTitle::
	.string "The Tenacious TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_UsedStruggleAction::
	.string "Resorted to using STRUGGLE$"

MauvilleCity_PokemonCenter_1F_Text_UsedStruggleStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER had to rely on STRUGGLE\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} never gives in, no matter\n"
	.string "how dire things get!$"

MauvilleCity_PokemonCenter_1F_Text_SlotJackpotsTitle::
	.string "The SLOT Champ$"

MauvilleCity_PokemonCenter_1F_Text_SlotJackpotsAction::
	.string "Won the jackpot on the SLOTS$"

MauvilleCity_PokemonCenter_1F_Text_SlotJackpotsStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER won the jackpot on\n"
	.string "the SLOTS {STR_VAR_1} times.\p"
	.string "{STR_VAR_3} must have luck on their\n"
	.string "side at the SLOTS!$"

MauvilleCity_PokemonCenter_1F_Text_RouletteWinsTitle::
	.string "The ROULETTE Champ$"

MauvilleCity_PokemonCenter_1F_Text_RouletteWinsAction::
	.string "Had consecutive ROULETTE wins of$"

MauvilleCity_PokemonCenter_1F_Text_RouletteWinsStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER won at ROULETTE\n"
	.string "{STR_VAR_1} times in a row.\p"
	.string "{STR_VAR_3} had luck when the ball\n"
	.string "bounced in ROULETTE!$"

MauvilleCity_PokemonCenter_1F_Text_BattleTowerChallengesTitle::
	.string "The BATTLE TOWER Challenger$"

MauvilleCity_PokemonCenter_1F_Text_BattleTowerChallengesAction::
	.string "Took the BATTLE TOWER challenge$"

MauvilleCity_PokemonCenter_1F_Text_BattleTowerChallengesStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER took the BATTLE TOWER\n"
	.string "challenge {STR_VAR_1} times!\p"
	.string "{STR_VAR_3} aims for greatness in the\n"
	.string "BATTLE TOWER!$"

MauvilleCity_PokemonCenter_1F_Text_MadePokeblocksTitle::
	.string "The Blend-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_MadePokeblocksAction::
	.string "Made {POKEBLOCK}S$"

MauvilleCity_PokemonCenter_1F_Text_MadePokeblocksStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER made {POKEBLOCK}S\n"
	.string "{STR_VAR_1} times!\p"
	.string "No one works a BERRY BLENDER\n"
	.string "better than {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_EnteredContestsTitle::
	.string "The CONTEST-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_EnteredContestsAction::
	.string "Entered CONTESTS$"

MauvilleCity_PokemonCenter_1F_Text_EnteredContestsStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER entered CONTESTS\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must love showing off\n"
	.string "POKéMON to the crowd!$"

MauvilleCity_PokemonCenter_1F_Text_WonContestsTitle::
	.string "The CONTEST Master$"

MauvilleCity_PokemonCenter_1F_Text_WonContestsAction::
	.string "Won CONTESTS$"

MauvilleCity_PokemonCenter_1F_Text_WonContestsStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER won CONTESTS\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must be an incredible\n"
	.string "CONTEST master!$"

MauvilleCity_PokemonCenter_1F_Text_TimesShoppedTitle::
	.string "The Happy Shopper$"

MauvilleCity_PokemonCenter_1F_Text_TimesShoppedAction::
	.string "Shopped$"

MauvilleCity_PokemonCenter_1F_Text_TimesShoppedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER bought items in shops\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must be someone who was\n"
	.string "born to shop.$"

MauvilleCity_PokemonCenter_1F_Text_UsedItemFinderTitle::
	.string "The Item-Finding TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_UsedItemFinderAction::
	.string "Used an ITEMFINDER$"

MauvilleCity_PokemonCenter_1F_Text_UsedItemFinderStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER used an ITEMFINDER\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must love scouring the\n"
	.string "ground like a DRAGON RADAR hunter!$"

MauvilleCity_PokemonCenter_1F_Text_TimesRainedTitle::
	.string "The Rain-Soaked TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_TimesRainedAction::
	.string "Got rained on$"

MauvilleCity_PokemonCenter_1F_Text_TimesRainedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER got soaked by rain\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3}'s presence must even\n"
	.string "attract the rain clouds!$"

MauvilleCity_PokemonCenter_1F_Text_CheckedPokedexTitle::
	.string "The Avid POKéDEX Reader$"

MauvilleCity_PokemonCenter_1F_Text_CheckedPokedexAction::
	.string "Checked a POKéDEX$"

MauvilleCity_PokemonCenter_1F_Text_CheckedPokedexStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER checked a POKéDEX\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must love studying\n"
	.string "POKéMON in a POKéDEX!$"

MauvilleCity_PokemonCenter_1F_Text_ReceivedRibbonsTitle::
	.string "The RIBBON Collector$"

MauvilleCity_PokemonCenter_1F_Text_ReceivedRibbonsAction::
	.string "Received RIBBONS$"

MauvilleCity_PokemonCenter_1F_Text_ReceivedRibbonsStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER received RIBBONS\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must love to collect\n"
	.string "RIBBONS!$"

MauvilleCity_PokemonCenter_1F_Text_LedgesJumpedTitle::
	.string "The Ledge-Jumping TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_LedgesJumpedAction::
	.string "Jumped down ledges$"

MauvilleCity_PokemonCenter_1F_Text_LedgesJumpedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER jumped down ledges\n"
	.string "{STR_VAR_1} times!\p"
	.string "If there's a ledge to hop, then\n"
	.string "{STR_VAR_3} can't resist it!$"

MauvilleCity_PokemonCenter_1F_Text_TVWatchedTitle::
	.string "The Legendary TV Viewer$"

MauvilleCity_PokemonCenter_1F_Text_TVWatchedAction::
	.string "Watched TV$"

MauvilleCity_PokemonCenter_1F_Text_TVWatchedStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER watched TV\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must love watching TV!$"

MauvilleCity_PokemonCenter_1F_Text_CheckedClockTitle::
	.string "The Time-Conscious TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_CheckedClockAction::
	.string "Checked the time$"

MauvilleCity_PokemonCenter_1F_Text_CheckedClockStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER checked the time\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must be a punctual TRAINER\n"
	.string "who never loses track of time.$"

MauvilleCity_PokemonCenter_1F_Text_WonLotteryTitle::
	.string "The POKéMON LOTTERY Wizard$"

MauvilleCity_PokemonCenter_1F_Text_WonLotteryAction::
	.string "Won POKéMON LOTTERIES$"

MauvilleCity_PokemonCenter_1F_Text_WonLotteryStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER won POKéMON LOTTERIES\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must have a lot of luck\n"
	.string "on their side!$"

MauvilleCity_PokemonCenter_1F_Text_UsedDaycareTitle::
	.string "The DAY CARE-Using Trainer$"

MauvilleCity_PokemonCenter_1F_Text_UsedDaycareAction::
	.string "Left POKéMON at the DAY CARE$"

MauvilleCity_PokemonCenter_1F_Text_UsedDaycareStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER left POKéMON with the\n"
	.string "DAY CARE {STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must be a real go-getter\n"
	.string "who raises POKéMON hard!$"

MauvilleCity_PokemonCenter_1F_Text_RodeCableCarTitle::
	.string "The CABLE CAR-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_RodeCableCarAction::
	.string "Rode the CABLE CAR$"

MauvilleCity_PokemonCenter_1F_Text_RodeCableCarStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER rode the CABLE CAR\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must be a busy TRAINER,\n"
	.string "always going up and down!$"

MauvilleCity_PokemonCenter_1F_Text_HotSpringsTitle::
	.string "The Hot Spring-Loving TRAINER$"

MauvilleCity_PokemonCenter_1F_Text_HotSpringsAction::
	.string "Bathed in hot springs$"

MauvilleCity_PokemonCenter_1F_Text_HotSpringsStory::
	.string "Gather round for the tale of a\n"
	.string "TRAINER named {STR_VAR_3}.\p"
	.string "This TRAINER bathed in hot springs\n"
	.string "{STR_VAR_1} times!\p"
	.string "{STR_VAR_3} must have skin as smooth\n"
	.string "as a baby's!$"


MauvilleCity_PokemonCenter_1F_EventScript_Storyteller::
	lock
	faceplayer
	setvar VAR_0x8008, 0
	setvar VAR_0x8009, 0
	setvar VAR_0x800A, 0
	setvar VAR_0x800B, 0
	msgbox MauvilleCity_PokemonCenter_1F_Text_WillYouHearMyTale, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller
	specialvar VAR_RESULT, StorytellerGetFreeStorySlot
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_KnowNoTales
	message MauvilleCity_PokemonCenter_1F_Text_WhichTaleToTell
	waitmessage
	special StorytellerStoryListMenu
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_CancelStorySelection
	setvar VAR_0x8008, 1
	special Script_StorytellerDisplayStory
	waitmessage
	waitbuttonpress
	specialvar VAR_RESULT, StorytellerUpdateStat
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_AreThereOtherTales
	goto MauvilleCity_PokemonCenter_1F_EventScript_TellPlayersTale

MauvilleCity_PokemonCenter_1F_EventScript_CancelStorySelection::
	goto MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller
	end

MauvilleCity_PokemonCenter_1F_EventScript_AreThereOtherTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_CouldThereBeOtherLegends, MSGBOX_DEFAULT
	specialvar VAR_RESULT, HasStorytellerAlreadyRecorded
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_StorytellerEnd
	goto MauvilleCity_PokemonCenter_1F_EventScript_DoYouHaveAnyTales

MauvilleCity_PokemonCenter_1F_EventScript_KnowNoTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_IKnowNoTales, MSGBOX_DEFAULT
MauvilleCity_PokemonCenter_1F_EventScript_DoYouHaveAnyTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_HaveYouAnyLegendaryTales, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller
	specialvar VAR_RESULT, Script_StorytellerInitializeRandomStat
	goto_if_eq VAR_RESULT, 1, MauvilleCity_PokemonCenter_1F_EventScript_TellPlayersTale
	msgbox MauvilleCity_PokemonCenter_1F_Text_NotWorthyOfLegend, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_TellPlayersTale::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDidStatXTimes, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller::
	msgbox MauvilleCity_PokemonCenter_1F_Text_StorytellerFeelingTheBlues, MSGBOX_DEFAULT
	release
	end

@ Unused
MauvilleCity_PokemonCenter_1F_EventScript_WaitingForRecordMix::
	msgbox MauvilleCity_PokemonCenter_1F_Text_IWishMorePeopleWereInterested, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_StorytellerEnd::
	release
	end

@ Giddy
MauvilleCity_PokemonCenter_1F_Text_HearMyStory:
	.string "Yo, I'm GIDDY!\n"
	.string "I've got a story that'll blow your\l"
	.string "scouter right off your face!\p"
	.string "Would you like to hear my story?$"

MauvilleCity_PokemonCenter_1F_Text_GiddyFeelingTheBlues:
	.string "Oh…\n"
	.string "Now I'm feeling the blues…$"

MauvilleCity_PokemonCenter_1F_Text_AlsoIWasThinking:
	.string "Also, I was thinking…$"

MauvilleCity_PokemonCenter_1F_Text_WeShouldChatAgain:
	.string "That's about it, I think…\p"
	.string "We should chat again!\n"
	.string "Bye-bye, and keep training!$"

MauvilleCity_PokemonCenter_1F_EventScript_Giddy::
	lock
	faceplayer
	msgbox MauvilleCity_PokemonCenter_1F_Text_HearMyStory, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_TryTellTale
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineGiddy
	end

MauvilleCity_PokemonCenter_1F_EventScript_TryTellTale::
	special GiddyShouldTellAnotherTale
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_GiddyTellTale
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_ToldEnoughTales
	end

MauvilleCity_PokemonCenter_1F_EventScript_TryTellNewTale::
	special GiddyShouldTellAnotherTale
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_GiddyStartNewTale
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_ToldEnoughTales
	end

MauvilleCity_PokemonCenter_1F_EventScript_GiddyStartNewTale::
	msgbox MauvilleCity_PokemonCenter_1F_Text_AlsoIWasThinking, MSGBOX_DEFAULT
	goto MauvilleCity_PokemonCenter_1F_EventScript_GiddyTellTale
	end

@ Giddy will continue to tell stories regardless of whether yes or no is selected below.
@ Each story there is a 10% chance it will be his last. Otherwise he will stop at 10 stories.
MauvilleCity_PokemonCenter_1F_EventScript_GiddyTellTale::
	special GenerateGiddyLine
	special ShowFieldMessageStringVar4
	waitmessage
	yesnobox 20, 8
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_TryTellNewTale
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_TryTellNewTale
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineGiddy::
	msgbox MauvilleCity_PokemonCenter_1F_Text_GiddyFeelingTheBlues, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_ToldEnoughTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_WeShouldChatAgain, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToHearMySong:
	.string "Hi, I'm the BARD.\n"
	.string "I sing ballads of the mightiest\l"
	.string "warriors.\p"
	.string "Would you like to hear my song?$"

MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues1:
	.string "Oh…\n"
	.string "Now I'm feeling the blues…$"

MauvilleCity_PokemonCenter_1F_Text_WishICouldPlaySongForOthers:
	.string "Oh, what a moving song…\n"
	.string "I wish I could play it for everyone…$"

MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToWriteSomeLyrics:
	.string "So?\n"
	.string "How do you like my song?\p"
	.string "But I'm not too happy with\n"
	.string "the lyrics.\p"
	.string "How about writing some\n"
	.string "new lyrics for me?$"

MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues2:
	.string "Oh…\n"
	.string "Now I'm feeling the blues…$"

MauvilleCity_PokemonCenter_1F_Text_LetMeSingItForYou:
	.string "Thank you kindly!\n"
	.string "Let me sing it for you with all\l"
	.string "my heart and ki.$"

MauvilleCity_PokemonCenter_1F_Text_ThatHowYouWantedSongToGo:
	.string "Was that how you wanted your song\n"
	.string "to go?$"

MauvilleCity_PokemonCenter_1F_Text_IllSingThisSongForAWhile:
	.string "Okay! That's it, then.\n"
	.string "I'll sing this song for a while,\l"
	.string "loud enough for the whole city!$"

MauvilleCity_PokemonCenter_1F_Text_TeachWhatsHipAndHappening:
	.string "Hey, yo! They call me the HIPSTER.\n"
	.string "I'll teach you what's hip and\l"
	.string "happening.$"

MauvilleCity_PokemonCenter_1F_Text_IAlreadyTaughtYou:
	.string "But, hey, I already taught you\n"
	.string "what's hip and happening.\p"
	.string "I want to spread the good word to\n"
	.string "other folks.$"

MauvilleCity_PokemonCenter_1F_Text_IveGotNothingNewToTeach:
	.string "But, hey, you already know plenty\n"
	.string "about what's hip and happening.\p"
	.string "I've got nothing new to teach you!$"

MauvilleCity_PokemonCenter_1F_Text_HaveYouHeardOfWord:
	.string "Hey, have you heard about\n"
	.string "“{STR_VAR_1}”?\p"
	.string "What's it mean? Well…\n"
	.string "Ask CHI-CHI, okay?$"
