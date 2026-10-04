MeteorFalls_StevensCave_MapScripts::
	.byte 0

MeteorFalls_StevensCave_EventScript_Steven::
	lock
	goto_if_set FLAG_DEFEATED_METEOR_FALLS_STEVEN, MeteorFalls_StevensCave_EventScript_Defeated
	waitse
	playse SE_PIN
	applywaitmovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_Delay48
	applywaitmovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_FacePlayer
	msgbox MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_STEVEN, MeteorFalls_StevensCave_Text_StevenDefeat
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_METEOR_FALLS_STEVEN
	release
	end

MeteorFalls_StevensCave_EventScript_Defeated::
	applywaitmovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_FacePlayer
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	release
	end

MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst:
	.string "SUPREME KAI: Well now, {PLAYER}{KUN}.\n"
	.string "I'm impressed you found me here.\p"
	.string "Perhaps you think I'm nothing more than\n"
	.string "a collector of rocks?\p"
	.string "No, that can't be right.\p"
	.string "We fought side by side in SOOTOPOLIS.\p"
	.string "So you should have a very good idea of\n"
	.string "how strong I am.\p"
	.string "All right, {PLAYER}{KUN}. If you've come to\n"
	.string "challenge me in earnest, expect the\l"
	.string "worst!$"

MeteorFalls_StevensCave_Text_StevenDefeat:
	.string "You… I never imagined you'd grow this\n"
	.string "strong…$"

MeteorFalls_StevensCave_Text_MyPredictionCameTrue:
	.string "SUPREME KAI: Ever since our paths first\n"
	.string "crossed in GRANITE CAVE in DEWFORD,\l"
	.string "I've had a feeling.\p"
	.string "I thought you would one day become the\n"
	.string "CHAMPION.\p"
	.string "My predictions usually come true.\p"
	.string "And where will you go from here?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Ha ha, even I can't see that far.$"

