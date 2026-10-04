@ All the LOCALID constants used in this file are essentially redundant.
@ They're all used to have the NPC the player talked to turn back so they're facing
@ their original direction (because they're watching/participating in the contests).
@ They could all be substituted for VAR_LAST_TALKED.
LilycoveCity_ContestHall_MapScripts::
	.byte 0

LilycoveCity_ContestHall_EventScript_Boy1::
	msgbox LilycoveCity_ContestHall_Text_TodayWonSmartnessContest, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Boy2::
	msgbox LilycoveCity_ContestHall_Text_EnteredBunchOfContests, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Girl::
	msgbox LilycoveCity_ContestHall_Text_ManWhoWonEarlierHadPokeblocks, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Sailor::
	msgbox LilycoveCity_ContestHall_Text_IsntThisPlaceHumongous, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_SmartContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_GiveItBestSmartAppeal, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_MC, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AreYouEnjoyingThisContest, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_JUDGE, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EnteredWrongContest, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_RaisedMonToBeSmart, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_IfMonPullsSmartMoveNext, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_DontAppreciateCuteLeechLife, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience1::
	msgbox LilycoveCity_ContestHall_Text_YoureBeautifulGrandpa, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AllSeemToUseDifferentMoves, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PokemonSmarterThanTrainers, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_StillLoveSmartnessContests, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_SMART_AUDIENCE_4, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AreYouEnteringBeautyContest, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_MC, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EveryPokemonPristineBeauty, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_JUDGE, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EyesWillBeGluedToMyBeauty, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_OverdidGrooming, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_JudgeWontSeeAuroraBeam, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PokemonLooksLikeYoungerMe, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_WinBeautyContestMakesMeHappy, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_AUDIENCE_1, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_GanderAtAllThosePrettyPokemon, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_CantWinOnBeautyAlone, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_BEAUTY_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_InTheMiddleOfContest, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_MC, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_SuchCharmingCuteAppeals, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_JUDGE, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_MyAzurillWasDistracted, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_NeverWonBattleButContest, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PetalDanceIsMarvel, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_MyMonAppealSoMuchCuter, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience1::
	lockall
	applywaitmovement LOCALID_CUTE_AUDIENCE_1, Common_Movement_FacePlayer
	msgbox LilycoveCity_ContestHall_Text_MyChildIsInContest, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_AUDIENCE_1, Common_Movement_FaceOriginalDirection
	delay 25
	msgbox LilycoveCity_ContestHall_Text_ComeOnDear, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_WantCuteMonOfMyOwn, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_ThatGirlThereIsCutest, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_CUTE_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyStageSign::
	msgbox LilycoveCity_ContestHall_Text_BeautyContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_CuteStageSign::
	msgbox LilycoveCity_ContestHall_Text_CuteContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_SmartStageSign::
	msgbox LilycoveCity_ContestHall_Text_SmartContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_Text_TodayWonSmartnessContest:
	.string "This POKéMON has won a BEAUTY CONTEST\n"
	.string "here before.\p"
	.string "And guess what? Today I made it win a\n"
	.string "SMARTNESS CONTEST!\p"
	.string "My own talent scares me…$"

LilycoveCity_ContestHall_Text_EnteredBunchOfContests:
	.string "I've entered a bunch of CONTESTS to\n"
	.string "learn how they work.\p"
	.string "In a COOLNESS CONTEST, cool moves\n"
	.string "score well as appeals.\p"
	.string "Smart moves and cute moves don't go\n"
	.string "over nearly as well, for instance.$"

LilycoveCity_ContestHall_Text_ManWhoWonEarlierHadPokeblocks:
	.string "The young man who won earlier had a\n"
	.string "huge stash of different {POKEBLOCK}S.\p"
	.string "Could you win if you had that many of\n"
	.string "them?$"

LilycoveCity_ContestHall_Text_IsntThisPlaceHumongous:
	.string "Whoa, this place is huge!\p"
	.string "The tension in the air… It's nothing\n"
	.string "like a CONTEST hall out in the sticks.$"

LilycoveCity_ContestHall_Text_GiveItBestSmartAppeal:
	.string "MC: All right, SMART POKéMON and their\n"
	.string "TRAINERS, are you ready?!\p"
	.string "Give it everything you've got!\n"
	.string "Let's appeal!$"

LilycoveCity_ContestHall_Text_AreYouEnjoyingThisContest:
	.string "JUDGE: Are you enjoying this CONTEST?\p"
	.string "Come back with three friends and all of\n"
	.string "you can enter a CONTEST together!$"

LilycoveCity_ContestHall_Text_EnteredWrongContest:
	.string "Aaaargh…\n"
	.string "I entered the wrong CONTEST.\p"
	.string "I put this tough POKéMON in the\n"
	.string "SMARTNESS CONTEST…\p"
	.string "Come on, wow them with a smart-looking\n"
	.string "ROCK SMASH!$"

LilycoveCity_ContestHall_Text_RaisedMonToBeSmart:
	.string "I've spent many a year, and every scrap\n"
	.string "of my wisdom, raising this POKéMON to be\l"
	.string "smart.\p"
	.string "No young whippersnapper's POKéMON will\n"
	.string "beat it easily.\p"
	.string "My dear wife, are you watching this?$"

LilycoveCity_ContestHall_Text_IfMonPullsSmartMoveNext:
	.string "There it is!\p"
	.string "If my POKéMON pulls off a smart move\n"
	.string "next, the crowd's excitement will peak!$"

LilycoveCity_ContestHall_Text_DontAppreciateCuteLeechLife:
	.string "Oh, no, no!\p"
	.string "My darling ZUBAT's LEECH LIFE is so\n"
	.string "cute that I kept using it again and\l"
	.string "again!\p"
	.string "But these cold-hearted people don't\n"
	.string "appreciate it at all!$"

LilycoveCity_ContestHall_Text_YoureBeautifulGrandpa:
	.string "Kyaaah! You're so beautiful, Grandpa!$"

LilycoveCity_ContestHall_Text_AllSeemToUseDifferentMoves:
	.string "Even when TRAINERS enter the same kind\n"
	.string "of POKéMON, they all use different\l"
	.string "moves for their appeals.\p"
	.string "It's just like how every fighter has a\n"
	.string "different style.$"

LilycoveCity_ContestHall_Text_PokemonSmarterThanTrainers:
	.string "If you ask me, the POKéMON look way\n"
	.string "smarter than their TRAINERS.$"

LilycoveCity_ContestHall_Text_StillLoveSmartnessContests:
	.string "When all is said and done, I still love\n"
	.string "SMARTNESS CONTESTS.\p"
	.string "That intellectual green color…\n"
	.string "It's so… so… cool.$"

LilycoveCity_ContestHall_Text_AreYouEnteringBeautyContest:
	.string "MC: Are you entering the BEAUTY\n"
	.string "CONTEST, too? Good luck!$"

LilycoveCity_ContestHall_Text_EveryPokemonPristineBeauty:
	.string "JUDGE: Ah, every POKéMON here is a model\n"
	.string "of flawless beauty!\p"
	.string "Why, I nearly forgot to score them!$"

LilycoveCity_ContestHall_Text_EyesWillBeGluedToMyBeauty:
	.string "A POKéMON this beautiful…\n"
	.string "There's no other like it.\p"
	.string "Every eye in the hall will be glued to my\n"
	.string "beauty.$"

LilycoveCity_ContestHall_Text_OverdidGrooming:
	.string "Waaaah!\p"
	.string "I brushed and groomed my POKéMON so\n"
	.string "carefully for this CONTEST…\p"
	.string "But I overdid it…\n"
	.string "Its coat turned all scraggly…\p"
	.string "What do I do?$"

LilycoveCity_ContestHall_Text_JudgeWontSeeAuroraBeam:
	.string "This AURORA BEAM is so dazzling that\n"
	.string "the JUDGE won't be able to see it.\p"
	.string "Uh, wait a second…\n"
	.string "Then it's pointless!$"

LilycoveCity_ContestHall_Text_PokemonLooksLikeYoungerMe:
	.string "This pretty POKéMON looks just like me\n"
	.string "when I was young.\p"
	.string "The moment I said that, my husband spat\n"
	.string "out his coffee.\p"
	.string "Did I say something funny?$"

LilycoveCity_ContestHall_Text_WinBeautyContestMakesMeHappy:
	.string "You know, when I win a BEAUTY CONTEST\n"
	.string "it makes me happy. Happier than usual,\l"
	.string "anyway.$"

LilycoveCity_ContestHall_Text_GanderAtAllThosePrettyPokemon:
	.string "Wahahaha!\p"
	.string "Look at all those pretty POKéMON!\n"
	.string "I just love this kind of glitz!$"

LilycoveCity_ContestHall_Text_CantWinOnBeautyAlone:
	.string "You can't win on beauty alone.\p"
	.string "You have to groom your POKéMON until\n"
	.string "it's nice and glossy, like mine.$"

LilycoveCity_ContestHall_Text_InTheMiddleOfContest:
	.string "MC: Uh-oh! Hello there!\n"
	.string "We're in the middle of a CONTEST!\p"
	.string "Please sign up at our registration\n"
	.string "counter and come out, okay?$"

LilycoveCity_ContestHall_Text_SuchCharmingCuteAppeals:
	.string "JUDGE: Oh, such charming, cute appeals!\p"
	.string "Oh, my goodness! What a perfectly\n"
	.string "adorable WATER SPORT appeal!$"

LilycoveCity_ContestHall_Text_MyAzurillWasDistracted:
	.string "Oh, no… My sweet AZURILL got\n"
	.string "distracted by another POKéMON.$"

LilycoveCity_ContestHall_Text_NeverWonBattleButContest:
	.string "My POKéMON has never won a battle, but\n"
	.string "put it in a CONTEST and watch out!$"

LilycoveCity_ContestHall_Text_PetalDanceIsMarvel:
	.string "My POKéMON's PETAL DANCE is a marvel of\n"
	.string "elegance.\p"
	.string "I won't let anyone disturb its\n"
	.string "performance.$"

LilycoveCity_ContestHall_Text_MyMonAppealSoMuchCuter:
	.string "Everyone's POKéMON is very cute.\p"
	.string "But if my POKéMON made a cute appeal…\p"
	.string "I'm sure it would be so much cuter than\n"
	.string "the rest.$"

LilycoveCity_ContestHall_Text_MyChildIsInContest:
	.string "My child is competing in this CONTEST.$"

LilycoveCity_ContestHall_Text_ComeOnDear:
	.string "Come on, dear. You can do it!\n"
	.string "Your POKéMON is the best!$"

LilycoveCity_ContestHall_Text_ThatGirlThereIsCutest:
	.string "I think that girl over there is the\n"
	.string "cutest of the bunch.\p"
	.string "What's that? They're judging the\n"
	.string "POKéMON on looks?$"

LilycoveCity_ContestHall_Text_WantCuteMonOfMyOwn:
	.string "Ohh, seeing all these cute POKéMON\n"
	.string "makes me want one of my own!\p"
	.string "I'm going out to catch some!$"

LilycoveCity_ContestHall_Text_BeautyContestStage:
	.string "BEAUTY CONTEST STAGE\n"
	.string "BE ENCHANTED BY BEAUTIFUL POKéMON!$"

LilycoveCity_ContestHall_Text_CuteContestStage:
	.string "CUTENESS CONTEST STAGE\n"
	.string "BE CHARMED BY CUTE POKéMON!$"

LilycoveCity_ContestHall_Text_SmartContestStage:
	.string "SMARTNESS CONTEST STAGE\n"
	.string "BE AMAZED BY SMART POKéMON!$"

