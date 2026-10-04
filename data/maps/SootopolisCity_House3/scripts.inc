SootopolisCity_House3_MapScripts::
	.byte 0

SootopolisCity_House3_EventScript_Woman::
	lock
	faceplayer
	msgbox SootopolisCity_House3_Text_JuanHasManyFansDoYou, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SootopolisCity_House3_EventScript_HaveFans
	msgbox SootopolisCity_House3_Text_LonesomeTryWorkingHarder, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House3_EventScript_HaveFans::
	msgbox SootopolisCity_House3_Text_YouMustBePrettyStrong, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House3_EventScript_Girl::
	msgbox SootopolisCity_House3_Text_TrainerFanClubWasWild, MSGBOX_NPC
	end

SootopolisCity_House3_Text_JuanHasManyFansDoYou:
	.string "You're a POKéMON TRAINER, aren't you?\p"
	.string "SOOTOPOLIS's PICCOLO has many fans.\n"
	.string "Even more than his student MR. SATAN,\l"
	.string "can you believe it?\p"
	.string "Do you have any?$"

SootopolisCity_House3_Text_YouMustBePrettyStrong:
	.string "Oh, then you must be pretty strong.$"

SootopolisCity_House3_Text_LonesomeTryWorkingHarder:
	.string "Oh, dear…\p"
	.string "That's a little lonesome.\p"
	.string "Try working a little harder to build a\n"
	.string "fan following.$"

SootopolisCity_House3_Text_TrainerFanClubWasWild:
	.string "Dedicated fans come from even outside\n"
	.string "HOENN.\p"
	.string "It was really wild when I visited the\n"
	.string "TRAINER FAN CLUB in LILYCOVE.$"

