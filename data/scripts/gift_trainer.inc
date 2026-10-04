MysteryGiftScript_VisitingTrainer::
	setvaddress MysteryGiftScript_VisitingTrainer
	special ValidateEReaderTrainer
	vgoto_if_eq VAR_RESULT, 0, MysteryGiftScript_VisitingTrainerArrived
	lock
	faceplayer
	vmessage sText_MysteryGiftVisitingTrainerInstructions
	waitmessage
	waitbuttonpress
	release
	end

MysteryGiftScript_VisitingTrainerArrived:
	lock
	faceplayer
	vmessage sText_MysteryGiftVisitingTrainerArrived
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftVisitingTrainerInstructions:
	.string "Thanks for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "With this WONDER CARD in hand, you\n"
	.string "can join a survey at a\l"
	.string "POKéMON MART.\p"
	.string "Those surveys can call fighters\n"
	.string "out to SOOTOPOLIS CITY.\p"
	.string "…Here's a secret password to write\n"
	.string "in on a survey:\p"
	.string "“GIVE ME\n"
	.string "AWESOME TRAINER”\p"
	.string "Write that in and send it to the\n"
	.string "WIRELESS COMMUNICATION SYSTEM.$"

sText_MysteryGiftVisitingTrainerArrived:
	.string "Thanks for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "A TRAINER has shown up in\n"
	.string "SOOTOPOLIS CITY looking for you.\p"
	.string "I hope you enjoy the fight with\n"
	.string "your visiting challenger.\p"
	.string "You can call out more TRAINERS\n"
	.string "by entering other passwords.\p"
	.string "Keep an ear out for other\n"
	.string "passwords that might work.$"
