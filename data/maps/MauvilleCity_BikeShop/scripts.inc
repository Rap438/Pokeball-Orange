MauvilleCity_BikeShop_MapScripts::
	.byte 0

MauvilleCity_BikeShop_EventScript_Rydel::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_BIKE, MauvilleCity_BikeShop_EventScript_AskSwitchBikes
	goto_if_set FLAG_DECLINED_BIKE, MauvilleCity_BikeShop_EventScript_SkipGreeting
	msgbox MauvilleCity_BikeShop_Text_RydelGreeting, MSGBOX_DEFAULT
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_SkipGreeting::
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_ChooseBike::
	message MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne
	waitmessage
	multichoice 21, 8, MULTI_BIKE, TRUE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_GetMachBike
	case 1, MauvilleCity_BikeShop_EventScript_GetAcroBike
	end

MauvilleCity_BikeShop_EventScript_NotFar::
	setflag FLAG_DECLINED_BIKE
	msgbox MauvilleCity_BikeShop_Text_GuessYouDontNeedBike, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_YesFar::
	setflag FLAG_RECEIVED_BIKE
	goto MauvilleCity_BikeShop_EventScript_ChooseBike
	end

MauvilleCity_BikeShop_EventScript_GetMachBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseMachBike, MSGBOX_DEFAULT
	giveitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_GetAcroBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseAcroBike, MSGBOX_DEFAULT
	giveitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes, MSGBOX_DEFAULT
	special SwapRegisteredBike
	release
	end

MauvilleCity_BikeShop_EventScript_AskSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_WantToSwitchBikes, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_SwitchBikes
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_KeepBike
	end

@ If the player does not have a bike on them Rydel assumes its stored in the PC
MauvilleCity_BikeShop_EventScript_SwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_IllSwitchBikes, MSGBOX_DEFAULT
	checkitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchAcroForMach
	checkitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchMachForAcro
	msgbox MauvilleCity_BikeShop_Text_OhYourBikeIsInPC, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_KeepBike::
	msgbox MauvilleCity_BikeShop_Text_HappyYouLikeIt, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_SwitchAcroForMach::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedAcroForMach, MSGBOX_DEFAULT
	removeitem ITEM_ACRO_BIKE
	giveitem ITEM_MACH_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_SwitchMachForAcro::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedMachForAcro, MSGBOX_DEFAULT
	removeitem ITEM_MACH_BIKE
	giveitem ITEM_ACRO_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_Assistant::
	msgbox MauvilleCity_BikeShop_Text_HandbooksAreInBack, MSGBOX_NPC
	end

MauvilleCity_BikeShop_EventScript_MachBikeHandbook::
	message MauvilleCity_BikeShop_Text_MachHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage::
	multichoice 0, 0, MULTI_MACH_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_HowToRide
	case 1, MauvilleCity_BikeShop_EventScript_HowToTurn
	case 2, MauvilleCity_BikeShop_EventScript_SandySlopes
	case 3, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	end

MauvilleCity_BikeShop_EventScript_HowToRide::
	message MauvilleCity_BikeShop_Text_HowToRideMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_HowToTurn::
	message MauvilleCity_BikeShop_Text_HowToTurnMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_SandySlopes::
	message MauvilleCity_BikeShop_Text_SandySlopes
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitMachHandbook::
	release
	end

MauvilleCity_BikeShop_EventScript_AcroBikeHandbook::
	message MauvilleCity_BikeShop_Text_AcroHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage::
	multichoice 0, 0, MULTI_ACRO_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_Wheelies
	case 1, MauvilleCity_BikeShop_EventScript_BunnyHops
	case 2, MauvilleCity_BikeShop_EventScript_Jumps
	case 3, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	end

MauvilleCity_BikeShop_EventScript_Wheelies::
	message MauvilleCity_BikeShop_Text_Wheelies
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_BunnyHops::
	message MauvilleCity_BikeShop_Text_BunnyHops
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_Jumps::
	message MauvilleCity_BikeShop_Text_Jumps
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitAcroHandbook::
	release
	end

MauvilleCity_BikeShop_Text_RydelGreeting:
	.string "Well, well, what have we here?\n"
	.string "A customer brimming with energy!\p"
	.string "Me? Call me RYDEL.\n"
	.string "I own this cycle shop.$"

MauvilleCity_BikeShop_Text_DidYouComeFromFarAway:
	.string "RYDEL: Those RUNNING SHOES of yours…\n"
	.string "They're awfully filthy.\p"
	.string "Did you come from far away?$"

MauvilleCity_BikeShop_Text_GuessYouDontNeedBike:
	.string "RYDEL: Is that right?\p"
	.string "Then I guess you've got no need for any\n"
	.string "of my BIKES.$"

MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne:
	.string "RYDEL: Hm, hm… … … … …\p"
	.string "You're saying you came all the way from\n"
	.string "LITTLEROOT?\p"
	.string "My goodness! That's ridiculously far!\p"
	.string "If you had one of my BIKES, you could go\n"
	.string "anywhere easily with the wind in your\l"
	.string "face!\p"
	.string "I'll tell you what! I'm giving you a BIKE!\p"
	.string "Oh, wait a second!\p"
	.string "I forgot to mention there are two kinds\n"
	.string "of BIKES!\p"
	.string "They're the CAPSULE BIKE and the HOVER\n"
	.string "BIKE!\p"
	.string "The CAPSULE BIKE is built for speed.\n"
	.string "Nothing beats the rush of wind against\l"
	.string "your body!\p"
	.string "The HOVER BIKE is for riders who like\n"
	.string "technical tricks!\p"
	.string "I'm a real softie, so you can have\n"
	.string "whichever one you like!\p"
	.string "Which one will you choose?$"

MauvilleCity_BikeShop_Text_ChoseMachBike:
	.string "{PLAYER} picked the CAPSULE BIKE.$"

MauvilleCity_BikeShop_Text_ChoseAcroBike:
	.string "{PLAYER} picked the HOVER BIKE.$"

MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes:
	.string "RYDEL: If you ever feel like switching\n"
	.string "BIKES, come see me!$"

MauvilleCity_BikeShop_Text_WantToSwitchBikes:
	.string "RYDEL: Oh? Are you thinking about\n"
	.string "switching BIKES?$"

MauvilleCity_BikeShop_Text_IllSwitchBikes:
	.string "RYDEL: No problem at all!\n"
	.string "I'll swap your BIKE right now!$"

MauvilleCity_BikeShop_Text_ExchangedMachForAcro:
	.string "{PLAYER} traded the CAPSULE BIKE\n"
	.string "for a HOVER BIKE.$"

MauvilleCity_BikeShop_Text_ExchangedAcroForMach:
	.string "{PLAYER} traded the HOVER BIKE\n"
	.string "for a CAPSULE BIKE.$"

MauvilleCity_BikeShop_Text_HappyYouLikeIt:
	.string "RYDEL: Good, good!\n"
	.string "I'm glad you like it!$"

MauvilleCity_BikeShop_Text_OhYourBikeIsInPC:
	.string "Oh? What happened to the BIKE I gave\n"
	.string "you?\p"
	.string "Ah, I get it. You stored it in your PC.\p"
	.string "Well, take it out of PC storage and I'll\n"
	.string "be happy to swap it for you!\p"
	.string "May the wind always be at your back on\n"
	.string "your adventure!$"

MauvilleCity_BikeShop_Text_HandbooksAreInBack:
	.string "I'm learning about BIKES while I work\n"
	.string "here.\p"
	.string "If you need advice on how to ride yours,\n"
	.string "there are a couple of handbooks in the\l"
	.string "back.$"

MauvilleCity_BikeShop_Text_MachHandbookWhichPage:
	.string "It's a handbook on the CAPSULE BIKE.\p"
	.string "Which page do you want to read?$"

MauvilleCity_BikeShop_Text_HowToRideMachBike:
	.string "The BIKE moves in whichever direction\n"
	.string "you press on the + Control Pad.\p"
	.string "It picks up speed once it gets rolling.\p"
	.string "To stop, let go of the + Control Pad. The\n"
	.string "BIKE will slow to a halt.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_HowToTurnMachBike:
	.string "A CAPSULE BIKE is fast, but it can't\n"
	.string "stop quickly.\p"
	.string "Corners get a little tricky.\p"
	.string "Let go of the + Control Pad a bit before\n"
	.string "the corner and slow down.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_SandySlopes:
	.string "There are small sandy slopes all over\n"
	.string "the HOENN region.\p"
	.string "The loose, crumbly sand makes them\n"
	.string "impossible to climb normally.\p"
	.string "But with a CAPSULE BIKE, you can zip\n"
	.string "right up a sandy slope.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_AcroHandbookWhichPage:
	.string "It's a handbook on the HOVER BIKE.\p"
	.string "Which page do you want to read?$"

MauvilleCity_BikeShop_Text_Wheelies:
	.string "Press the B Button while riding and the\n"
	.string "front wheel pops up.\p"
	.string "You can keep moving with the front\n"
	.string "wheel up by using the + Control Pad.\p"
	.string "This technique is called a wheelie.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_BunnyHops:
	.string "Hold the B Button down and your BIKE\n"
	.string "hops on the spot.\p"
	.string "This technique is called a bunny hop.\p"
	.string "You can ride while hopping, too.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_Jumps:
	.string "Press the B Button and the + Control\n"
	.string "Pad at the same time to jump.\p"
	.string "Press the + Control Pad sideways to\n"
	.string "jump sideways.\p"
	.string "Press it backwards to make the BIKE\n"
	.string "turn around mid-jump.\p"
	.string "Want to read a different page?$"

