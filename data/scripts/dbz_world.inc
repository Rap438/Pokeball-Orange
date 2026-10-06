@ PokeBall Orange: World Martial Arts Tournament (Slateport), Capsule Corp. Gravity Room (Rustboro),
@ Hyperbolic Time Chamber (Ever Grande, after the credits) and Shenron's power wishes.

@ ------------------------------------------------------------------ World Martial Arts Tournament
EventScript_DBZ_TourneyAnnouncer::
	lock
	faceplayer
	goto_if_unset FLAG_BADGE04_GET, EventScript_DBZ_TourneyNotYet
	goto_if_set FLAG_DBZ_TOURNAMENT, EventScript_DBZ_TourneyWelcomeBack
	msgbox Text_DBZ_TourneyWelcome, MSGBOX_DEFAULT
	goto EventScript_DBZ_TourneyAsk
EventScript_DBZ_TourneyWelcomeBack::
	msgbox Text_DBZ_TourneyWelcomeBack, MSGBOX_DEFAULT
EventScript_DBZ_TourneyAsk::
	msgbox Text_DBZ_TourneyEnter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DBZ_TourneyLater
	special HealPlayerParty
	special DBZ_HealGoku
	msgbox Text_DBZ_TourneyRules, MSGBOX_DEFAULT
	@ round 1: POKeMON
	msgbox Text_DBZ_TourneyRound1, MSGBOX_DEFAULT
	closemessage
	cleartrainerflag TRAINER_DBZ_JACKIE_CHUN
	trainerbattle_no_intro TRAINER_DBZ_JACKIE_CHUN, Text_DBZ_JackieDefeat
	special HealPlayerParty
	msgbox Text_DBZ_JackieAfter, MSGBOX_DEFAULT
	@ round 2: hand to hand
	msgbox Text_DBZ_TourneyRound2, MSGBOX_DEFAULT
	setvar VAR_0x800A, 1
	special DBZ_SetupTourneyFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_TourneyForfeit
	closemessage
	special DBZ_StartFightAndWait
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_TourneyForfeit
	waitstate
	lockall
	call EventScript_DBZ_FightResults
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_TourneyOut
	special DBZ_HealGoku
	special HealPlayerParty
	@ round 3: POKeMON
	msgbox Text_DBZ_TourneyRound3, MSGBOX_DEFAULT
	closemessage
	cleartrainerflag TRAINER_DBZ_MIGHTY_MASK
	trainerbattle_no_intro TRAINER_DBZ_MIGHTY_MASK, Text_DBZ_MaskDefeat
	special HealPlayerParty
	msgbox Text_DBZ_MaskAfter, MSGBOX_DEFAULT
	@ final: VEGETA, POKeMON then fists
	msgbox Text_DBZ_TourneyFinal, MSGBOX_DEFAULT
	closemessage
	cleartrainerflag TRAINER_DBZ_VEGETA_TOURNEY
	trainerbattle_no_intro TRAINER_DBZ_VEGETA_TOURNEY, Text_DBZ_TourneyVegetaDefeat
	special HealPlayerParty
	msgbox Text_DBZ_TourneyVegetaFists, MSGBOX_DEFAULT
	setvar VAR_0x800A, 3
	special DBZ_SetupTourneyFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_TourneyForfeit
	closemessage
	special DBZ_StartFightAndWait
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_TourneyForfeit
	waitstate
	lockall
	call EventScript_DBZ_FightResults
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_TourneyOut
	special DBZ_HealGoku
	special DBZ_QueueFightEvolveCheck
	playfanfare MUS_OBTAIN_BADGE
	message Text_DBZ_TourneyChampion
	waitfanfare
	waitmessage
	waitbuttonpress
	goto_if_set FLAG_DBZ_TOURNAMENT, EventScript_DBZ_TourneyRepeatPrize
	setflag FLAG_DBZ_TOURNAMENT
	msgbox Text_DBZ_TourneyPrize, MSGBOX_DEFAULT
	giveitem ITEM_RARE_CANDY, 3
	giveitem ITEM_PP_MAX
	addmoney 30000
	msgbox Text_DBZ_TourneyPrizeMoney, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_TourneyRepeatPrize::
	addmoney 5000
	msgbox Text_DBZ_TourneyRepeatMoney, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_TourneyOut::
	special DBZ_ReviveGokuWeak
	special DBZ_QueueFightEvolveCheck
	msgbox Text_DBZ_TourneyOut, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_TourneyForfeit::
	msgbox Text_DBZ_TourneyForfeit, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_TourneyNotYet::
	msgbox Text_DBZ_TourneyNotYet, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_TourneyLater::
	msgbox Text_DBZ_TourneyLater, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_MrSatan::
	lock
	faceplayer
	goto_if_set FLAG_DBZ_TOURNAMENT, EventScript_DBZ_MrSatanAfter
	msgbox Text_DBZ_MrSatan, MSGBOX_DEFAULT
	release
	end
EventScript_DBZ_MrSatanAfter::
	msgbox Text_DBZ_MrSatanAfter, MSGBOX_DEFAULT
	release
	end

@ ------------------------------------------------------------------ Capsule Corp. Gravity Room
EventScript_DBZ_GravityRoom::
	specialvar VAR_RESULT, DBZ_GravityStepsLeft
	goto_if_ne VAR_RESULT, 0, EventScript_DBZ_GravityActive
	goto_if_set FLAG_DBZ_GRAVITY_INTRO, EventScript_DBZ_GravityAsk
	setflag FLAG_DBZ_GRAVITY_INTRO
	msgbox Text_DBZ_GravityIntro, MSGBOX_DEFAULT
EventScript_DBZ_GravityAsk::
	msgbox Text_DBZ_GravityAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DBZ_GravityNo
	closemessage
	playse SE_M_EARTHQUAKE
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 0
	setvar VAR_0x8006, 12
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	special DBZ_StartGravityTraining
	msgbox Text_DBZ_GravityOn, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_GravityActive::
	buffernumberstring STR_VAR_1, VAR_RESULT
	msgbox Text_DBZ_GravityStillOn, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_GravityNo::
	msgbox Text_DBZ_GravityNo, MSGBOX_DEFAULT
	release
	end

@ ------------------------------------------------------------------ Hyperbolic Time Chamber
EventScript_DBZ_Dende::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, EventScript_DBZ_DendeNotYet
	msgbox Text_DBZ_DendeChamber, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DBZ_DendeNo
	special HealPlayerParty
	special DBZ_HealGoku
	msgbox Text_DBZ_ChamberEnter, MSGBOX_DEFAULT
	closemessage
	playse SE_M_TELEPORT
	fadescreen FADE_TO_WHITE
	fadescreen FADE_FROM_WHITE
	setvar VAR_TEMP_E, 0
EventScript_DBZ_ChamberWave::
	copyvar VAR_0x800A, VAR_TEMP_E
	copyvar VAR_TEMP_D, VAR_TEMP_E
	addvar VAR_TEMP_D, 1
	buffernumberstring STR_VAR_1, VAR_TEMP_D
	msgbox Text_DBZ_ChamberWave, MSGBOX_DEFAULT
	special DBZ_SetupChamberFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_ChamberStop
	closemessage
	special DBZ_StartFightAndWait
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_ChamberStop
	waitstate
	lockall
	call EventScript_DBZ_FightResults
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_ChamberLost
	addvar VAR_TEMP_E, 1
	goto_if_ge VAR_TEMP_E, 5, EventScript_DBZ_ChamberClear
	special DBZ_RestGoku
	msgbox Text_DBZ_ChamberRest, MSGBOX_DEFAULT
	goto EventScript_DBZ_ChamberWave

EventScript_DBZ_ChamberClear::
	special DBZ_QueueFightEvolveCheck
	playse SE_M_TELEPORT
	fadescreen FADE_TO_WHITE
	fadescreen FADE_FROM_WHITE
	special DBZ_HealGoku
	goto_if_set FLAG_DBZ_CHAMBER_CLEAR, EventScript_DBZ_ChamberClearAgain
	setflag FLAG_DBZ_CHAMBER_CLEAR
	msgbox Text_DBZ_ChamberClear, MSGBOX_DEFAULT
	giveitem ITEM_RARE_CANDY, 5
	release
	end
EventScript_DBZ_ChamberClearAgain::
	msgbox Text_DBZ_ChamberClearAgain, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_ChamberLost::
	special DBZ_ReviveGokuWeak
	special DBZ_QueueFightEvolveCheck
	playse SE_M_TELEPORT
	fadescreen FADE_TO_WHITE
	fadescreen FADE_FROM_WHITE
	msgbox Text_DBZ_ChamberLost, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_ChamberStop::
	msgbox Text_DBZ_ChamberStop, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_DendeNotYet::
	msgbox Text_DBZ_DendeNotYet, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_DendeNo::
	msgbox Text_DBZ_DendeNo, MSGBOX_DEFAULT
	release
	end

@ ------------------------------------------------------------------ Shenron: power wishes (VAR_0x8005 = 1..3)
EventScript_DBZ_PowerWish::
	msgbox Text_DBZ_ConfirmPowerWish, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DBZ_ChooseWish
	special DBZ_GrantPowerWish
	special DBZ_ConsumeDragonBalls
	msgbox Text_DBZ_PowerWishGranted, MSGBOX_DEFAULT
	closemessage
	playse SE_M_REFLECT
	fadescreenspeed FADE_TO_WHITE, 1
	fadescreenspeed FADE_FROM_WHITE, 1
	playfanfare MUS_LEVEL_UP
	call_if_eq VAR_0x8005, 1, EventScript_DBZ_WishGokuDone
	call_if_eq VAR_0x8005, 2, EventScript_DBZ_WishTeamDone
	call_if_eq VAR_0x8005, 3, EventScript_DBZ_WishZeniDone
	waitfanfare
	call_if_eq VAR_0x8005, 2, EventScript_DBZ_FightReports
	special DBZ_QueueFightEvolveCheck
	msgbox Text_DBZ_Farewell, MSGBOX_DEFAULT
	closemessage
	special DBZ_ShenronDepart
	waitstate
	msgbox Text_DBZ_BallsScatter, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_WishGokuDone::
	message Text_DBZ_WishGokuDone
	waitmessage
	waitbuttonpress
	return
EventScript_DBZ_WishTeamDone::
	message Text_DBZ_WishTeamDone
	waitmessage
	waitbuttonpress
	return
EventScript_DBZ_WishZeniDone::
	message Text_DBZ_WishZeniDone
	waitmessage
	waitbuttonpress
	return

@ ------------------------------------------------------------------ text
Text_DBZ_TourneyNotYet:
	.string "ANNOUNCER: The WORLD MARTIAL ARTS\n"
	.string "TOURNAMENT only takes fighters with\l"
	.string "four GYM BADGES or more.\p"
	.string "Come back when you've earned them,\n"
	.string "{PLAYER}!$"

Text_DBZ_TourneyWelcome:
	.string "ANNOUNCER: Ladies and gentlemen!\n"
	.string "Welcome to the WORLD MARTIAL ARTS\l"
	.string "TOURNAMENT!\p"
	.string "Hosted by the champion of the world,\n"
	.string "the one, the only… MR. SATAN!$"

Text_DBZ_TourneyWelcomeBack:
	.string "ANNOUNCER: Back for more, champ?\n"
	.string "The crowd loves you!$"

Text_DBZ_TourneyEnter:
	.string "Will you enter the tournament?$"

Text_DBZ_TourneyLater:
	.string "ANNOUNCER: The ring will be waiting!$"

Text_DBZ_TourneyRules:
	.string "ANNOUNCER: Four rounds! Some with\n"
	.string "POKéMON, some with your own fists!\p"
	.string "Your team is healed between rounds.\n"
	.string "Lose once and you're out!$"

Text_DBZ_TourneyRound1:
	.string "ANNOUNCER: Round one! A mysterious\n"
	.string "old master… JACKIE CHUN!$"

Text_DBZ_JackieDefeat:
	.string "Oh ho! You've grown, {PLAYER}…\n"
	.string "I mean, er… nice match, young man!$"

Text_DBZ_JackieAfter:
	.string "JACKIE CHUN: That old fellow who\n"
	.string "trains you must be very wise.\l"
	.string "And handsome. Very handsome.$"

Text_DBZ_TourneyRound2:
	.string "ANNOUNCER: Round two is\n"
	.string "hand-to-hand! No POKéMON!\p"
	.string "Facing {PLAYER}… the brute\n"
	.string "SPOPOVICH!$"

Text_DBZ_TourneyRound3:
	.string "ANNOUNCER: The semifinal! A masked\n"
	.string "hero who's suspiciously short…\l"
	.string "MIGHTY MASK!$"

Text_DBZ_MaskDefeat:
	.string "Aww, man! Goten, you're stepping on\n"
	.string "my head! Er… we mean… MIGHTY MASK\l"
	.string "retreats!$"

Text_DBZ_MaskAfter:
	.string "MIGHTY MASK: Don't tell our moms,\n"
	.string "okay?$"

Text_DBZ_TourneyFinal:
	.string "ANNOUNCER: The FINAL MATCH!\p"
	.string "The Prince of all Saiyans…\n"
	.string "VEGETA!\p"
	.string "VEGETA: Finally, KAKAROT. No more\n"
	.string "interruptions. Just you and me!$"

Text_DBZ_TourneyVegetaDefeat:
	.string "Tch… Your POKéMON are good.\n"
	.string "But that was only the warm-up!$"

Text_DBZ_TourneyVegetaFists:
	.string "VEGETA: Now we settle it the Saiyan\n"
	.string "way. Fists only, KAKAROT!$"

Text_DBZ_TourneyChampion:
	.string "ANNOUNCER: It's over! The winner of\n"
	.string "the WORLD MARTIAL ARTS TOURNAMENT\l"
	.string "is… {PLAYER}!$"

Text_DBZ_TourneyPrize:
	.string "MR. SATAN: Hahaha! Not bad, pal!\n"
	.string "Of course, I took it easy on…\l"
	.string "er, I mean… here's your prize!$"

Text_DBZ_TourneyPrizeMoney:
	.string "{PLAYER} also received ¥30,000 in\n"
	.string "prize money!$"

Text_DBZ_TourneyRepeatMoney:
	.string "{PLAYER} received ¥5,000 in prize\n"
	.string "money!$"

Text_DBZ_TourneyOut:
	.string "ANNOUNCER: And {PLAYER} is down!\n"
	.string "What a fight, folks!\p"
	.string "Train hard and come back stronger!$"

Text_DBZ_TourneyForfeit:
	.string "ANNOUNCER: {PLAYER} can't continue!\n"
	.string "That's a forfeit!$"

Text_DBZ_MrSatan:
	.string "MR. SATAN: Hahaha! I'm MR. SATAN,\n"
	.string "the strongest man in the world!\p"
	.string "Win this tournament and maybe…\n"
	.string "MAYBE… you can shake my hand!$"

Text_DBZ_MrSatanAfter:
	.string "MR. SATAN: Hey, champ! Keep it\n"
	.string "between us, but… how do you do\l"
	.string "that light-show thing?$"

Text_DBZ_GravityIntro:
	.string "Welcome to CAPSULE CORP.'s GRAVITY\n"
	.string "ROOM! DR. BRIEF built it for VEGETA.\p"
	.string "At ten times Earth's gravity, every\n"
	.string "fight trains you twice as hard!$"

Text_DBZ_GravityAsk:
	.string "Turn the gravity up to 10x?\n"
	.string "(Double fight EXP for 600 steps.)$"

Text_DBZ_GravityOn:
	.string "10x GRAVITY! {PLAYER}'s body feels\n"
	.string "like lead…\p"
	.string "Fights give double EXP while the\n"
	.string "training lasts!$"

Text_DBZ_GravityStillOn:
	.string "You're still feeling the gravity!\n"
	.string "{STR_VAR_1} steps of training left.$"

Text_DBZ_GravityNo:
	.string "Come back anytime. VEGETA practically\n"
	.string "lives in there.$"

Text_DBZ_DendeNotYet:
	.string "MR. POPO: Hello, {PLAYER}. DENDE is\n"
	.string "waiting for you up ahead.\p"
	.string "Become the CHAMPION, and MR. POPO\n"
	.string "opens the HYPERBOLIC TIME CHAMBER.$"

Text_DBZ_DendeChamber:
	.string "MR. POPO: The HYPERBOLIC TIME CHAMBER.\n"
	.string "A year inside is a day outside.\p"
	.string "Five waves of sparring partners,\n"
	.string "triple EXP. Will you go in?$"

Text_DBZ_DendeNo:
	.string "MR. POPO: The door is always open\n"
	.string "for you.$"

Text_DBZ_ChamberEnter:
	.string "MR. POPO: Good luck! Don't let the\n"
	.string "air get to you in there.$"

Text_DBZ_ChamberWave:
	.string "Wave {STR_VAR_1} of 5: a TIME SPIRIT\n"
	.string "steps out of the white!$"

Text_DBZ_ChamberRest:
	.string "{PLAYER} caught his breath…$"

Text_DBZ_ChamberClear:
	.string "MR. POPO: You came out in one piece!\n"
	.string "Here, a gift from the Lookout.$"

Text_DBZ_ChamberClearAgain:
	.string "MR. POPO: Another year of training in\n"
	.string "one day. Amazing!$"

Text_DBZ_ChamberLost:
	.string "DENDE: That's enough for now.\n"
	.string "Rest, and try again!$"

Text_DBZ_ChamberStop:
	.string "DENDE: You're too worn out to\n"
	.string "train right now.$"

Text_DBZ_ConfirmPowerWish:
	.string "SHENRON: Is that your wish?$"

Text_DBZ_PowerWishGranted:
	.string "SHENRON: Your wish has been\n"
	.string "granted!$"

Text_DBZ_WishGokuDone:
	.string "Power surges through {PLAYER}!\n"
	.string "He's now level {STR_VAR_1}!$"

Text_DBZ_WishTeamDone:
	.string "{PLAYER}'s POKéMON are bursting with\n"
	.string "new power!$"

Text_DBZ_WishZeniDone:
	.string "¥{STR_VAR_1} rained down from the\n"
	.string "sky!$"

@ PokeBall Orange: Pokemon living in town (tools/pbo/place_ambient.py). They face Goku and cry.
EventScript_DBZ_AmbientMon::
	lock
	faceplayer
	special DBZ_AmbientMonCry
	msgbox Text_DBZ_AmbientMon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Text_DBZ_AmbientMon:
	.string "{STR_VAR_1}: {STR_VAR_2}$"
