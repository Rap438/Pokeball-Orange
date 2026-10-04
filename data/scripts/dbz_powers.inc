@ PokeBall Orange: Goku's overworld powers

EventScript_DBZ_PowerUp::
	lockall
	specialvar VAR_RESULT, DBZ_GetNextForm
	goto_if_eq VAR_RESULT, 0xFF, EventScript_DBZ_CantTransform
	goto_if_eq VAR_RESULT, 0, EventScript_DBZ_PowerDown
	special DBZ_StartAura
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	setvar VAR_0x8006, 12
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	special DBZ_ApplyNextForm
	waitstate
	specialvar VAR_RESULT, DBZ_ShouldShowFormIntro
	goto_if_eq VAR_RESULT, 1, EventScript_DBZ_IntroSSJ
	goto_if_eq VAR_RESULT, 2, EventScript_DBZ_IntroSSJ2
	goto_if_eq VAR_RESULT, 3, EventScript_DBZ_IntroSSJ3
	releaseall
	end

EventScript_DBZ_PowerDown::
	special DBZ_ApplyNextForm
	waitstate
	releaseall
	end

EventScript_DBZ_CantTransform::
	msgbox Text_DBZ_CantTransform, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_IntroSSJ::
	msgbox Text_DBZ_IntroSSJ, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_IntroSSJ2::
	msgbox Text_DBZ_IntroSSJ2, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_IntroSSJ3::
	msgbox Text_DBZ_IntroSSJ3, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_Fire::
	lockall
	special DBZ_FireBlast
	waitstate
	goto_if_eq VAR_RESULT, DBZ_HIT_TREE, EventScript_DBZ_BlastHitTree
	goto_if_eq VAR_RESULT, DBZ_HIT_ROCK, EventScript_DBZ_BlastHitRock
	releaseall
	end

EventScript_DBZ_BlastHitTree::
	goto_if_ne VAR_0x8004, 1, EventScript_DBZ_ReleaseEnd
	specialvar VAR_RESULT, DBZ_GetFormSpecial
	goto_if_lt VAR_RESULT, 1, EventScript_DBZ_TreeTooTough
	goto EventScript_CutTreeDown

EventScript_DBZ_BlastHitRock::
	specialvar VAR_RESULT, DBZ_GetFormSpecial
	goto_if_lt VAR_RESULT, 2, EventScript_DBZ_RockTooTough
	goto EventScript_SmashRock

EventScript_DBZ_TreeTooTough::
	msgbox Text_DBZ_TreeTooTough, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_RockTooTough::
	msgbox Text_DBZ_RockTooTough, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_ReleaseEnd::
	releaseall
	end

@ A-button on a cuttable tree while Super Saiyan
EventScript_DBZ_KameTree::
	setvar VAR_0x8004, 1
	special DBZ_FireBlast
	waitstate
	goto EventScript_CutTreeDown

@ A-button on a rock while Super Saiyan 2
EventScript_DBZ_PunchRock::
	setvar VAR_0x8004, 0
	special DBZ_FireBlast
	waitstate
	goto EventScript_SmashRock

EventScript_DBZ_BoulderSSJ3::
	msgbox Text_DBZ_BoulderSSJ3, MSGBOX_DEFAULT
	releaseall
	end

Text_DBZ_CantTransform:
	.string "{PLAYER} focused his energy…\p"
	.string "But he can't go SUPER SAIYAN yet.\n"
	.string "Win a GYM BADGE to unlock his power!$"

Text_DBZ_IntroSSJ:
	.string "{PLAYER} went SUPER SAIYAN!\p"
	.string "Hold L, then let go to fire a\n"
	.string "KAMEHAMEHA. It can blast through\l"
	.string "small trees!\p"
	.string "Tap L for a quick KI BLAST.\n"
	.string "Press R to power up or power down.$"

Text_DBZ_IntroSSJ2:
	.string "{PLAYER} went SUPER SAIYAN 2!\p"
	.string "Lightning crackles around him…\n"
	.string "He can smash cracked rocks now!$"

Text_DBZ_IntroSSJ3:
	.string "{PLAYER} went SUPER SAIYAN 3!\p"
	.string "His power is overwhelming!\n"
	.string "He can push huge boulders around!$"

Text_DBZ_TreeTooTough:
	.string "The tree shrugged off the blast!\n"
	.string "{PLAYER} needs to be a SUPER SAIYAN.$"

Text_DBZ_RockTooTough:
	.string "The blast bounced off the rock!\n"
	.string "{PLAYER} needs SUPER SAIYAN 2 power.$"

Text_DBZ_BoulderSSJ3:
	.string "{PLAYER}'s SUPER SAIYAN 3 power\n"
	.string "makes it possible to push boulders.$"

EventScript_DBZ_Debug::
	lockall
	msgbox Text_DBZ_DebugFight, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Fight
	msgbox Text_DBZ_DebugTime, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Time
	msgbox Text_DBZ_DebugWild, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Wild
	msgbox Text_DBZ_DebugTrainer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Trainer
	msgbox Text_DBZ_Debug, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Badges
	msgbox Text_DBZ_DebugWarp116, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Warp116
	msgbox Text_DBZ_DebugWarp114, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Warp114
	msgbox Text_DBZ_DebugBalls, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_Balls
	msgbox Text_DBZ_DebugWarpBall, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, EventScript_DBZ_Debug_WarpBall
	releaseall
	end

EventScript_DBZ_Debug_Balls::
	special DBZ_Debug_GiveSixBalls
	msgbox Text_DBZ_DebugDone, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_Debug_WarpBall::
	closemessage
	special DBZ_Debug_WarpToDragonBall
	waitstate
	releaseall
	end

EventScript_DBZ_Debug_Warp116::
	warp MAP_ROUTE116, 24, 10
	waitstate
	releaseall
	end

EventScript_DBZ_Debug_Warp114::
	warp MAP_ROUTE114, 12, 44
	waitstate
	releaseall
	end

EventScript_DBZ_Debug_Badges::
	setflag FLAG_BADGE01_GET
	setflag FLAG_BADGE02_GET
	setflag FLAG_BADGE03_GET
	setflag FLAG_BADGE04_GET
	setflag FLAG_BADGE05_GET
	setflag FLAG_BADGE06_GET
	setflag FLAG_BADGE07_GET
	setflag FLAG_BADGE08_GET
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_SYS_POKEDEX_GET
	givemon SPECIES_MUDKIP, 50
	givemon SPECIES_SWELLOW, 50
	setvar VAR_0x8004, 40
	special DBZ_Debug_SetGokuLevel
	closemessage
	msgbox Text_DBZ_DebugDone, MSGBOX_DEFAULT
	releaseall
	end

Text_DBZ_Debug:
	.string "DEBUG: give all badges + team?$"

Text_DBZ_DebugWarp116:
	.string "DEBUG: warp to ROUTE 116 (trees)?$"

Text_DBZ_DebugWarp114:
	.string "DEBUG: warp to ROUTE 114 (rocks)?$"

Text_DBZ_DebugDone:
	.string "DEBUG: done!$"

Text_DBZ_DebugBalls:
	.string "DEBUG: 6 DRAGON BALLS + RADAR\n+ NIMBUS?$"

Text_DBZ_DebugWarpBall:
	.string "DEBUG: warp to a DRAGON BALL?$"

EventScript_DBZ_Debug_Fight::
	setvar VAR_0x8004, 25
	special DBZ_Debug_SetGokuLevel
	closemessage
	releaseall
	goto EventScript_DBZ_Ambush

Text_DBZ_DebugFight:
	.string "DEBUG: fight a soldier now?$"

EventScript_DBZ_Debug_Time::
	addvar VAR_DBZ_TOD_OVERRIDE, 1
	call_if_gt VAR_DBZ_TOD_OVERRIDE, 4, EventScript_DBZ_Debug_TimeWrap
	buffernumberstring STR_VAR_1, VAR_DBZ_TOD_OVERRIDE
	msgbox Text_DBZ_DebugTimeSet, MSGBOX_DEFAULT
	closemessage
	warpsilent MAP_MAUVILLE_CITY, 22, 10
	waitstate
	releaseall
	end

EventScript_DBZ_Debug_TimeWrap::
	setvar VAR_DBZ_TOD_OVERRIDE, 0
	return

Text_DBZ_DebugTime:
	.string "DEBUG: change time of day?$"

Text_DBZ_DebugTimeSet:
	.string "Time mode {STR_VAR_1} (0 clock 1 day\n2 morning 3 evening 4 night)$"

EventScript_DBZ_Debug_Wild::
	closemessage
	setwildbattle SPECIES_ZIGZAGOON, 12
	dowildbattle
	releaseall
	end

Text_DBZ_DebugWild:
	.string "DEBUG: wild battle here?$"

EventScript_DBZ_Debug_Trainer::
	closemessage
	cleartrainerflag TRAINER_ROXANNE_1
	trainerbattle_no_intro TRAINER_ROXANNE_1, Text_DBZ_DebugDone
	releaseall
	end

Text_DBZ_DebugTrainer:
	.string "DEBUG: trainer battle (YAMCHA)?$"
