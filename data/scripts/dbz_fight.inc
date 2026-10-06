@ PokeBall Orange: Goku's overworld fights

EventScript_DBZ_OfferTrainerFight::
	specialvar VAR_RESULT, DBZ_TrainerIsPendingBoss
	goto_if_eq VAR_RESULT, TRUE, EventScript_DBZ_Return
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
	call EventScript_DBZ_TechniqueHints
	specialvar VAR_RESULT, DBZ_GokuCanFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_TooHurtForGym
	specialvar VAR_RESULT, DBZ_CanFightTrainer
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_FightDone
	msgbox Text_DBZ_GymFightIntro, MSGBOX_DEFAULT
	closemessage
	special DBZ_SetupTrainerFight
	special DBZ_StartFight
	releaseall
	end

EventScript_DBZ_TooHurtForGym::
	msgbox Text_DBZ_TooHurtForGym, MSGBOX_DEFAULT
	goto EventScript_DBZ_FightDone

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
	call EventScript_DBZ_FightResults
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_KnockedOut
	goto EventScript_DBZ_FightEvolve

@ shows the outcome of a fight (VAR_RESULT = DBZ_FIGHT_WON / LOST), EXP and level reports; VAR_0x8007 = result
EventScript_DBZ_FightResults::
	copyvar VAR_0x8007, VAR_RESULT
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_FightLostMsg
	msgbox Text_DBZ_FightWon, MSGBOX_DEFAULT
	special DBZ_FightAwardExp
	copyvar VAR_0x800A, VAR_RESULT
	msgbox Text_DBZ_FightExp, MSGBOX_DEFAULT
	goto_if_eq VAR_0x800A, 0, EventScript_DBZ_FightReports
	playfanfare MUS_LEVEL_UP
	message Text_DBZ_GokuLevelUp
	waitfanfare
	waitmessage
	waitbuttonpress
EventScript_DBZ_FightReports::
	specialvar VAR_0x8008, DBZ_NextFightReport
	goto_if_eq VAR_0x8008, DBZ_REPORT_NONE, EventScript_DBZ_Return
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

EventScript_DBZ_FightLostMsg::
	msgbox Text_DBZ_FightLost, MSGBOX_DEFAULT
	return

EventScript_DBZ_KnockedOut::
	msgbox Text_DBZ_BlackedOut, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	special DBZ_WarpToLastHeal
	waitstate
	end

EventScript_DBZ_WokeUp::
	lockall
	msgbox Text_DBZ_WokeUp, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_EvolveCheck::
	lockall
	goto EventScript_DBZ_FightEvolve

EventScript_DBZ_FightDone::
	closemessage
	releaseall
	end

@ Rival VEGETA: called from the rival battle scripts right after the POKEMON battle.
EventScript_DBZ_VegetaFight::
	special DBZ_SetupVegetaFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_VegetaNoFight
	copyvar VAR_0x800B, VAR_0x8007
	call_if_lt VAR_0x800B, 2, EventScript_DBZ_VegetaIntro1
	call_if_eq VAR_0x800B, 2, EventScript_DBZ_VegetaIntro2
	call_if_eq VAR_0x800B, 3, EventScript_DBZ_VegetaIntro2
	call_if_ge VAR_0x800B, 4, EventScript_DBZ_VegetaIntro3
	closemessage
	special DBZ_StartFightAndWait
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_Return
	waitstate
	lockall
	call EventScript_DBZ_FightResults
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_VegetaWon
	msgbox Text_DBZ_VegetaLost, MSGBOX_DEFAULT
	special DBZ_QueueFightEvolveCheck
	return

EventScript_DBZ_VegetaWon::
	special DBZ_ReviveGokuWeak
	msgbox Text_DBZ_VegetaWins, MSGBOX_DEFAULT
	special DBZ_QueueFightEvolveCheck
	return

EventScript_DBZ_VegetaNoFight::
	msgbox Text_DBZ_VegetaNoFight, MSGBOX_DEFAULT
	return

EventScript_DBZ_VegetaIntro1::
	msgbox Text_DBZ_VegetaIntro1, MSGBOX_DEFAULT
	return

EventScript_DBZ_VegetaIntro2::
	msgbox Text_DBZ_VegetaIntro2, MSGBOX_DEFAULT
	return

EventScript_DBZ_VegetaIntro3::
	msgbox Text_DBZ_VegetaIntro3, MSGBOX_DEFAULT
	return

Text_DBZ_OfferFight:
	.string "Your POKéMON are tough…\n"
	.string "But what about you, {PLAYER}?\p"
	.string "Want to go a round,\n"
	.string "hand to hand?$"

Text_DBZ_FightAccepted:
	.string "Alright! No holding back!$"

Text_DBZ_GymFightIntro:
	.string "…Not bad at all!\p"
	.string "But a true champion can fight\n"
	.string "on his own two feet, too.\p"
	.string "Show me what YOU'VE got, {PLAYER}!$"

Text_DBZ_Ambush:
	.string "A RED RIBBON ARMY soldier jumped\n"
	.string "out! Get ready, {PLAYER}!$"

Text_DBZ_TrainingPaidOff:
	.string "Training with the team paid off!\n"
	.string "{PLAYER}'s power level rose to {STR_VAR_1}!$"

Text_DBZ_FightWon:
	.string "{PLAYER} beat {STR_VAR_1}!$"

Text_DBZ_FightExp:
	.string "{PLAYER} gained {STR_VAR_1} EXP.!\n"
	.string "The team gained {STR_VAR_2} EXP. each!$"

Text_DBZ_GokuLevelUp:
	.string "{PLAYER}'s power level rose to {STR_VAR_3}!$"

Text_DBZ_MonGrew:
	.string "{STR_VAR_1} grew to Lv. {STR_VAR_2}!$"

Text_DBZ_MonLearned:
	.string "{STR_VAR_1} learned {STR_VAR_2}!$"

Text_DBZ_MonNoRoom:
	.string "{STR_VAR_1} wanted to learn {STR_VAR_2},\n"
	.string "but it already knows four moves.\p"
	.string "The MOVE TUTOR can teach it later.$"

Text_DBZ_FightLost:
	.string "{PLAYER} got knocked down by {STR_VAR_1}…$"

Text_DBZ_BlackedOut:
	.string "Everything went dark…$"

Text_DBZ_WokeUp:
	.string "{PLAYER} came to after a while.\p"
	.string "He's still sore… Some medicine, a rest\n"
	.string "at home or a POKéMON CENTER will\l"
	.string "patch him up.$"

Text_DBZ_TooHurtForGym:
	.string "…But {PLAYER} is too beat up to fight.\n"
	.string "He needs to heal up first.$"

Text_DBZ_VegetaIntro1:
	.string "VEGETA: Hmph. Your POKéMON got lucky,\n"
	.string "KAKAROT.\p"
	.string "But a POKéMON battle proves nothing.\n"
	.string "Put up your fists!$"

Text_DBZ_VegetaIntro2:
	.string "VEGETA: Don't look so smug.\n"
	.string "I've been training every day since\l"
	.string "our last fight.\p"
	.string "Saiyan against Saiyan. Now!$"

Text_DBZ_VegetaIntro3:
	.string "VEGETA: You think you're the only one\n"
	.string "who can go Super Saiyan?\p"
	.string "The Prince of all Saiyans bows to\n"
	.string "no one! Fight me, KAKAROT!$"

Text_DBZ_VegetaLost:
	.string "VEGETA: Grr… This isn't over.\n"
	.string "Next time I'll surpass you!$"

Text_DBZ_VegetaWins:
	.string "VEGETA: Ha! Remember this feeling,\n"
	.string "KAKAROT. I'm still number one!\p"
	.string "Go rest up. I want you at full\n"
	.string "power when I beat you again.$"

Text_DBZ_VegetaNoFight:
	.string "VEGETA: Tch. You're in no shape to\n"
	.string "fight. I'll crush you another day.$"

Text_DBZ_HintSSJ:
	.string "The BADGE's power surges through\n"
	.string "{PLAYER}… Something is awakening!\p"
	.string "Press R to go SUPER SAIYAN!\n"
	.string "SSJ + KAMEHAMEHA can cut trees.$"

Text_DBZ_HintSSJ2:
	.string "{PLAYER}'s ki is crackling with\n"
	.string "lightning!\p"
	.string "SUPER SAIYAN 2 unlocked!\n"
	.string "Its KAMEHAMEHA smashes rocks.$"

Text_DBZ_HintSSJ3:
	.string "{PLAYER}'s hair grows long and his\n"
	.string "power shakes the ground!\p"
	.string "SUPER SAIYAN 3 unlocked!\n"
	.string "Now {PLAYER} can push big boulders.$"
