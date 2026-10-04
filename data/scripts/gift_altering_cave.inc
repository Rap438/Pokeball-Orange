MysteryGiftScript_AlteringCave::
	setvaddress MysteryGiftScript_AlteringCave
	addvar VAR_ALTERING_CAVE_WILD_SET, 1
	vgoto_if_ne VAR_ALTERING_CAVE_WILD_SET, (NUM_ALTERING_CAVE_TABLES + 1), MysteryGiftScript_AlteringCave_
	setvar VAR_ALTERING_CAVE_WILD_SET, 0
MysteryGiftScript_AlteringCave_:
	lock
	faceplayer
	vmessage sText_MysteryGiftAlteringCave
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftAlteringCave:
	.string "Thanks for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "Word around the wireless lines is\n"
	.string "that rare POKéMON have been\l"
	.string "spotted, with strange power levels.\p"
	.string "The readings all point to the\n"
	.string "ALTERING CAVE on ROUTE 103.\p"
	.string "A fighter with a curious nose like\n"
	.string "yours should go check it out!$"
