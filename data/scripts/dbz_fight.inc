@ PokeBall Orange: Goku's overworld fights

EventScript_DBZ_OfferTrainerFight::
	specialvar VAR_RESULT, DBZ_CanFightTrainer
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_Return
	msgbox Text_DBZ_OfferFight, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DBZ_Return
	msgbox Text_DBZ_FightAccepted, MSGBOX_DEFAULT
	closemessage
	special DBZ_SetupTrainerFight
	special DBZ_StartFight
	releaseall
	end

EventScript_DBZ_Return::
	return

EventScript_DBZ_GymFight::
	lockall
	specialvar VAR_0x8009, DBZ_GetNewFormHint
	call_if_eq VAR_0x8009, DBZ_FORM_SSJ, EventScript_DBZ_HintSSJ
	call_if_eq VAR_0x8009, DBZ_FORM_SSJ2, EventScript_DBZ_HintSSJ2
	call_if_eq VAR_0x8009, DBZ_FORM_SSJ3, EventScript_DBZ_HintSSJ3
	specialvar VAR_RESULT, DBZ_CanFightTrainer
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_FightDone
	msgbox Text_DBZ_GymFightIntro, MSGBOX_DEFAULT
	closemessage
	special DBZ_SetupTrainerFight
	special DBZ_StartFight
	releaseall
	end

EventScript_DBZ_HintSSJ::
	msgbox Text_DBZ_HintSSJ, MSGBOX_DEFAULT
	return

EventScript_DBZ_HintSSJ2::
	msgbox Text_DBZ_HintSSJ2, MSGBOX_DEFAULT
	return

EventScript_DBZ_HintSSJ3::
	msgbox Text_DBZ_HintSSJ3, MSGBOX_DEFAULT
	return

EventScript_DBZ_Ambush::
	lockall
	special DBZ_PrepareAmbush
	playse SE_PIN
	msgbox Text_DBZ_Ambush, MSGBOX_DEFAULT
	closemessage
	special DBZ_StartFight
	releaseall
	end

EventScript_DBZ_LevelUpNotice::
	lockall
	playfanfare MUS_LEVEL_UP
	message Text_DBZ_TrainingPaidOff
	waitfanfare
	waitmessage
	waitbuttonpress
	closemessage
	releaseall
	end

EventScript_DBZ_FightEnd::
	lockall
	goto_if_eq VAR_RESULT, DBZ_FIGHT_LOST, EventScript_DBZ_FightLost
	msgbox Text_DBZ_FightWon, MSGBOX_DEFAULT
	special DBZ_FightAwardExp
	copyvar VAR_0x8007, VAR_RESULT
	msgbox Text_DBZ_FightExp, MSGBOX_DEFAULT
	goto_if_eq VAR_0x8007, 0, EventScript_DBZ_FightReports
	playfanfare MUS_LEVEL_UP
	message Text_DBZ_GokuLevelUp
	waitfanfare
	waitmessage
	waitbuttonpress
EventScript_DBZ_FightReports::
	specialvar VAR_0x8008, DBZ_NextFightReport
	goto_if_eq VAR_0x8008, DBZ_REPORT_NONE, EventScript_DBZ_FightEvolve
	call_if_eq VAR_0x8008, DBZ_REPORT_LEVEL, EventScript_DBZ_ReportLevel
	call_if_eq VAR_0x8008, DBZ_REPORT_LEARNED, EventScript_DBZ_ReportLearned
	call_if_eq VAR_0x8008, DBZ_REPORT_NO_ROOM, EventScript_DBZ_ReportNoRoom
	goto EventScript_DBZ_FightReports

EventScript_DBZ_ReportLevel::
	playfanfare MUS_LEVEL_UP
	message Text_DBZ_MonGrew
	waitfanfare
	waitmessage
	waitbuttonpress
	return

EventScript_DBZ_ReportLearned::
	playfanfare MUS_LEVEL_UP
	message Text_DBZ_MonLearned
	waitfanfare
	waitmessage
	waitbuttonpress
	return

EventScript_DBZ_ReportNoRoom::
	msgbox Text_DBZ_MonNoRoom, MSGBOX_DEFAULT
	return

EventScript_DBZ_FightEvolve::
	closemessage
	specialvar VAR_RESULT, DBZ_TryEvolveAfterFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_FightDone
	fadescreen FADE_TO_BLACK
	special DBZ_StartFightEvolution
	waitstate
	goto EventScript_DBZ_FightEvolve

EventScript_DBZ_FightLost::
	msgbox Text_DBZ_FightLost, MSGBOX_DEFAULT
EventScript_DBZ_FightDone::
	closemessage
	releaseall
	end

Text_DBZ_OfferFight:
	.string "Your POKéMON are tough…\n"
	.string "But what about you, GOKU?\p"
	.string "Want to go a round,\n"
	.string "hand to hand?$"

Text_DBZ_FightAccepted:
	.string "Alright! No holding back!$"

Text_DBZ_GymFightIntro:
	.string "…Not bad at all!\p"
	.string "But a true champion can fight\n"
	.string "on his own two feet, too.\p"
	.string "Show me what YOU'VE got, GOKU!$"

Text_DBZ_Ambush:
	.string "A {STR_VAR_1} soldier jumped out!\n"
	.string "Get ready, GOKU!$"

Text_DBZ_TrainingPaidOff:
	.string "Training with the team paid off!\n"
	.string "GOKU's power level rose to {STR_VAR_1}!$"

Text_DBZ_FightWon:
	.string "GOKU beat {STR_VAR_1}!$"

Text_DBZ_FightExp:
	.string "GOKU gained {STR_VAR_1} EXP.!\n"
	.string "The team gained {STR_VAR_2} EXP. each!$"

Text_DBZ_GokuLevelUp:
	.string "GOKU's power level rose to {STR_VAR_3}!$"

Text_DBZ_MonGrew:
	.string "{STR_VAR_1} grew to Lv. {STR_VAR_2}!$"

Text_DBZ_MonLearned:
	.string "{STR_VAR_1} learned {STR_VAR_2}!$"

Text_DBZ_MonNoRoom:
	.string "{STR_VAR_1} wanted to learn {STR_VAR_2},\n"
	.string "but it already knows four moves.\p"
	.string "The MOVE TUTOR can teach it later.$"

Text_DBZ_FightLost:
	.string "GOKU got knocked down by {STR_VAR_1}…\p"
	.string "He needs more training!$"

Text_DBZ_HintSSJ:
	.string "The BADGE's power surges through\n"
	.string "GOKU… Something is awakening!\p"
	.string "Press R to go SUPER SAIYAN!\n"
	.string "SSJ + KAMEHAMEHA can cut trees.$"

Text_DBZ_HintSSJ2:
	.string "GOKU's ki is crackling with\n"
	.string "lightning!\p"
	.string "SUPER SAIYAN 2 unlocked!\n"
	.string "Its KAMEHAMEHA smashes rocks.$"

Text_DBZ_HintSSJ3:
	.string "GOKU's hair grows long and his\n"
	.string "power shakes the ground!\p"
	.string "SUPER SAIYAN 3 unlocked!\n"
	.string "Now GOKU can push big boulders.$"
