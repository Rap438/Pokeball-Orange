RustboroCity_Flat1_2F_MapScripts::
	.byte 0

RustboroCity_Flat1_2F_EventScript_WaldasDad::
	lock
	faceplayer
	specialvar VAR_RESULT, TryBufferWaldaPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase

RustboroCity_Flat1_2F_EventScript_GivePhrase::
	special DoWaldaNamingScreen
	goto_if_eq VAR_0x8004, 1, RustboroCity_Flat1_2F_EventScript_CancelGivePhrase
	goto_if_eq VAR_0x8004, 2, RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase
	specialvar VAR_RESULT, TryGetWallpaperWithWaldaPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase::
	msgbox RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhIsThatRight, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhYouDontKnowAny, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry2, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_OhShesLaughing, MSGBOX_DEFAULT
	applywaitmovement LOCALID_WALDAS_DAD, Common_Movement_FacePlayer
	msgbox RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda::
	turnobject LOCALID_WALDAS_DAD, DIR_EAST
	return

RustboroCity_Flat1_2F_EventScript_WaldasMom::
	msgbox RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard, MSGBOX_NPC
	end

RustboroCity_Flat1_2F_EventScript_PokeDoll::
	msgbox RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll, MSGBOX_SIGN
	end

RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard:
	.string "Oh, it's so hard every day…\p"
	.string "What's hard? You need to ask?\p"
	.string "It's figuring out what to make for meals\n"
	.string "every single day.\p"
	.string "Honestly, it isn't easy. And these days\n"
	.string "everyone eats like a SAIYAN!$"

RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase:
	.string "Oh, hello! Welcome to the PEPPER\n"
	.string "household.\p"
	.string "I have a question for you. Have you\n"
	.string "ever baby-sat?\p"
	.string "You see, I'm a new father, so raising a\n"
	.string "child is all new to me.\p"
	.string "And I have a problem. My daughter WALDA\n"
	.string "doesn't laugh enough.\p"
	.string "I think she'd laugh for me if I said\n"
	.string "something funny.\p"
	.string "Do you know a funny word or phrase you\n"
	.string "could teach me?$"

RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase:
	.string "Lately I've been saying “{STR_VAR_1}” to\n"
	.string "amuse her.\p"
	.string "Do you know a better word or phrase\n"
	.string "that might work?$"

RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion:
	.string "Oh, wonderful!\n"
	.string "Let's hear your suggestion.$"

RustboroCity_Flat1_2F_Text_OhIsThatRight:
	.string "Oh, is that right?\p"
	.string "Well, if you come up with a good\n"
	.string "suggestion, I'm all ears.$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry2:
	.string "Ah, I see.\n"
	.string "Well, let's give it a try, shall we?$"

RustboroCity_Flat1_2F_Text_OhShesLaughing:
	.string "{STR_VAR_1}.\n"
	.string "{STR_VAR_1}.\p"
	.string "Oh, yes! She's laughing!\n"
	.string "I'm every bit as delighted as she is!$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry:
	.string "Ah, I see.\n"
	.string "Well, let's give it a try, shall we?$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll:
	.string "{STR_VAR_1}.\n"
	.string "{STR_VAR_1}.\p"
	.string "Hmmm… She's not smiling at all.\n"
	.string "Maybe WALDA is just a very serious\l"
	.string "child…$"

RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase:
	.string "Oh, so you don't know any good words.\n"
	.string "I'd better think of one myself, then.\p"
	.string "Hmm…\n"
	.string "How about “{STR_VAR_1}”? Let's see if that\l"
	.string "works.$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2:
	.string "{STR_VAR_1}.\n"
	.string "{STR_VAR_1}.\p"
	.string "Hmmm… She's not smiling at all.\n"
	.string "Maybe WALDA is just a very serious\l"
	.string "child…$"

RustboroCity_Flat1_2F_Text_OhYouDontKnowAny:
	.string "Oh, so you don't know any good words.\n"
	.string "I guess I'll keep amusing her with the\l"
	.string "phrase I used before.\p"
	.string "Anyway, if you think of a good\n"
	.string "suggestion, don't hesitate to tell me,\l"
	.string "okay?$"

RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper:
	.string "Thank you!\p"
	.string "Thanks to you, my darling WALDA laughed\n"
	.string "for me!\p"
	.string "I may not look like much, but I'm one of\n"
	.string "CAPSULE CORP.'s top researchers.\p"
	.string "So how about I do something in return?\p"
	.string "I know! I'll add some new wallpaper\n"
	.string "patterns for the BOXES in the PC\l"
	.string "POKéMON Storage System.\p"
	.string "In the wallpaper pattern menu, select\n"
	.string "“FRIENDS.”\p"
	.string "That will give you access to the new\n"
	.string "wallpaper patterns.$"

RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll:
	.string "It's a POKéMON plush DOLL!$"

