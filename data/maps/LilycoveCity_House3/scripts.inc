LilycoveCity_House3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LilycoveCity_House3_OnTransition
	.byte 0

LilycoveCity_House3_OnTransition:
	random 4
	copyvar VAR_TEMP_1, VAR_RESULT
	end

LilycoveCity_House3_EventScript_PokefanF::
	lock
	faceplayer
	msgbox LilycoveCity_House3_Text_LearnFromMasterOfPokeblocks, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LilycoveCity_House3_EventScript_DeclinePokeblockLearn
	msgbox LilycoveCity_House3_Text_ExplainPokeblocks, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_House3_EventScript_DeclinePokeblockLearn::
	msgbox LilycoveCity_House3_Text_OhAreYouSure, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_House3_EventScript_Man::
	lock
	faceplayer
	msgbox LilycoveCity_House3_Text_HappyToHaveQuadruplets, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	release
	end

LilycoveCity_House3_EventScript_GameBoyKid1::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid2::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid3::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid4::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_WereDoingMultiBattle::
	msgbox LilycoveCity_House3_Text_GoingToWinMultiBattles, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereMixingRecords::
	msgbox LilycoveCity_House3_Text_LikeMixingAtRecordCorner, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereBlendingBerries::
	msgbox LilycoveCity_House3_Text_MakePokeblocksWithBerryBlender, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereDoingContest::
	msgbox LilycoveCity_House3_Text_GoingToEnterContest, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_Text_LearnFromMasterOfPokeblocks:
	.string "Oh, my, my! Are you traveling alone?\n"
	.string "And so young! Good for you!\p"
	.string "I'm sure my kids could learn a thing or\n"
	.string "two from you!\p"
	.string "Me? I'm a master of {POKEBLOCK}S.\p"
	.string "If I get serious for even a moment, why,\n"
	.string "I can whip up fantastic {POKEBLOCK}S.\p"
	.string "Would you like to learn from me, a master\n"
	.string "of {POKEBLOCK}S?$"

LilycoveCity_House3_Text_OhAreYouSure:
	.string "Oh? Are you sure?\p"
	.string "You shouldn't always try to do\n"
	.string "everything by yourself, dear!$"

LilycoveCity_House3_Text_ExplainPokeblocks:
	.string "Oh, good! You're a wise soul!\n"
	.string "This is a bit long, so listen up!\p"
	.string "Are you ready?\p"
	.string "If you look at {POKEBLOCK}S, you'll see\n"
	.string "they're rated on how they feel.\p"
	.string "The lower the feel rating, the better.\n"
	.string "Don't forget that!\p"
	.string "A good {POKEBLOCK} has a low feel rating\n"
	.string "and a high level.\p"
	.string "A POKéMON can eat more good {POKEBLOCK}S\n"
	.string "than ordinary ones, too.\p"
	.string "And this part is important.\p"
	.string "If you want to make smooth {POKEBLOCK}S,\n"
	.string "use different kinds of BERRIES.\p"
	.string "Don't be stingy! The kinds of BERRIES\n"
	.string "you use change how smooth the\l"
	.string "{POKEBLOCK}S turn out.\p"
	.string "And one more thing.\p"
	.string "The more people blending BERRIES, the\n"
	.string "smoother the {POKEBLOCK}S.\p"
	.string "That's why you should talk to other\n"
	.string "people and make {POKEBLOCK}S together.\p"
	.string "That's about all you need to know to\n"
	.string "make good {POKEBLOCK}S.\p"
	.string "If everyone had POKéMON they love,\n"
	.string "smooth {POKEBLOCK}S, and a loving family,\l"
	.string "the world would be a happier place.\p"
	.string "Don't give up, dear!$"

LilycoveCity_House3_Text_HappyToHaveQuadruplets:
	.string "When my wife gave birth to quadruplets,\n"
	.string "I was shocked, believe me.\p"
	.string "But now, watching them play together\n"
	.string "makes me happy.$"

LilycoveCity_House3_Text_GoingToWinMultiBattles:
	.string "We're having MULTI BATTLES, and I know\n"
	.string "I'm going to win.$"

LilycoveCity_House3_Text_LikeMixingAtRecordCorner:
	.string "We like mixing things up at the RECORD\n"
	.string "CORNER.\p"
	.string "But what actually gets mixed?$"

LilycoveCity_House3_Text_MakePokeblocksWithBerryBlender:
	.string "We're going to make super {POKEBLOCK}S with\n"
	.string "a BERRY BLENDER!$"

LilycoveCity_House3_Text_GoingToEnterContest:
	.string "I want to show off how tough my\n"
	.string "POKéMON is, so we're entering a CONTEST\l"
	.string "together.$"

