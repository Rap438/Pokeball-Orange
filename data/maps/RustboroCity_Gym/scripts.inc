RustboroCity_Gym_MapScripts::
	.byte 0

RustboroCity_Gym_EventScript_Roxanne::
	trainerbattle_single TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneIntro, RustboroCity_Gym_Text_RoxanneDefeat, RustboroCity_Gym_EventScript_RoxanneDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Gym_EventScript_RoxanneRematch
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, RustboroCity_Gym_EventScript_GiveRockTomb
	msgbox RustboroCity_Gym_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneDefeated::
	message RustboroCity_Gym_Text_ReceivedStoneBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox RustboroCity_Gym_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_RUSTBORO_GYM
	setflag FLAG_BADGE01_GET
	special DBZ_QueueGymFight
	setvar VAR_RUSTBORO_CITY_STATE, 1
	addvar VAR_PETALBURG_GYM_STATE, 1
	setvar VAR_0x8008, 1
	call Common_EventScript_SetGymTrainers
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto RustboroCity_Gym_EventScript_GiveRockTomb
	end

RustboroCity_Gym_EventScript_GiveRockTomb::
	giveitem ITEM_TM_ROCK_TOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_ROCK_TOMB
	msgbox RustboroCity_Gym_Text_ExplainRockTomb, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneRematch::
	trainerbattle_rematch_double TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxannePreRematch, RustboroCity_Gym_Text_RoxanneRematchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox RustboroCity_Gym_Text_RoxannePostRematch, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Josh::
	trainerbattle_single TRAINER_JOSH, RustboroCity_Gym_Text_JoshIntro, RustboroCity_Gym_Text_JoshDefeat
	msgbox RustboroCity_Gym_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Tommy::
	trainerbattle_single TRAINER_TOMMY, RustboroCity_Gym_Text_TommyIntro, RustboroCity_Gym_Text_TommyDefeat
	msgbox RustboroCity_Gym_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Marc::
	trainerbattle_single TRAINER_MARC, RustboroCity_Gym_Text_MarcIntro, RustboroCity_Gym_Text_MarcDefeat
	msgbox RustboroCity_Gym_Text_MarcPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_RUSTBORO_GYM, RustboroCity_Gym_EventScript_GymGuidePostVictory
	msgbox RustboroCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_GymGuidePostVictory::
	msgbox RustboroCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_GymStatueCertified::
	msgbox RustboroCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_GymStatue::
	msgbox RustboroCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_RegisterRoxanne::
	lockall
	pokenavcall RustboroCity_Gym_Text_RoxanneRegisterCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Gym_Text_RegisteredRoxanne, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_MATCH_CALL
	clearflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	releaseall
	end

RustboroCity_Gym_Text_GymGuideAdvice:
	.string "Yo, how's it going?\p"
	.string "Listen, my friend! Would you like to\n"
	.string "become the CHAMPION?\p"
	.string "I'm no TRAINER, not me, but I've watched\n"
	.string "enough tournaments to give you winning\l"
	.string "advice.\p"
	.string "That's settled, then! We'll aim for the\n"
	.string "POKéMON CHAMPIONSHIP together!\p"
	.string "It's your job as a TRAINER to collect\n"
	.string "GYM BADGES, am I right?\p"
	.string "But GYM LEADERS aren't pushovers!\p"
	.string "And that's where I come in! I'm here to\n"
	.string "provide expert advice!\p"
	.string "YAMCHA, the GYM LEADER, used to be a\n"
	.string "desert bandit, believe it or not. Now he\l"
	.string "uses ROCK-type POKéMON.\p"
	.string "The ROCK type is very durable, but it\n"
	.string "can't stand WATER-type and GRASS-type\l"
	.string "moves.\p"
	.string "Come see me afterwards if you beat the\n"
	.string "GYM LEADER.\p"
	.string "Well, go for it!$"

RustboroCity_Gym_Text_GymGuidePostVictory:
	.string "Whoa! What a breathtaking victory! My\n"
	.string "cheering must've worked!\p"
	.string "Great! Check your TRAINER CARD.\p"
	.string "The STONE BADGE you earned should be\n"
	.string "properly recorded on it.\p"
	.string "In other words…\p"
	.string "You've climbed the first step on the\n"
	.string "stairs to the CHAMPIONSHIP!\p"
	.string "That's got to feel awesome!$"

RustboroCity_Gym_Text_JoshIntro:
	.string "Don't take us GYM TRAINERS lightly!\n"
	.string "I'll show you why we're better!$"

RustboroCity_Gym_Text_JoshDefeat:
	.string "You were too strong for me…$"

RustboroCity_Gym_Text_JoshPostBattle:
	.string "You haven't seen anything yet of the\n"
	.string "ROCK type's terrifying power!$"

RustboroCity_Gym_Text_TommyIntro:
	.string "If you can't beat me, you don't stand a\n"
	.string "chance against YAMCHA!$"

RustboroCity_Gym_Text_TommyDefeat:
	.string "Wow! You've got serious potential!$"

RustboroCity_Gym_Text_TommyPostBattle:
	.string "YAMCHA is several times stronger than\n"
	.string "me.\p"
	.string "You'd better be on your guard!$"

RustboroCity_Gym_Text_MarcIntro:
	.string "We might be in the middle of town, but so\n"
	.string "what?\p"
	.string "My ROCK POKéMON need room to rampage!$"

RustboroCity_Gym_Text_MarcDefeat:
	.string "Oh, man oh man!\n"
	.string "Our challenger is one feisty customer!$"

RustboroCity_Gym_Text_MarcPostBattle:
	.string "I have to hand it to our LEADER.\p"
	.string "Turning his life around and choosing\n"
	.string "the ROCK type took real brains and\l"
	.string "guts.$"

RustboroCity_Gym_Text_RoxanneIntro:
	.string "Yo! I'm YAMCHA, the RUSTBORO POKéMON\n"
	.string "GYM LEADER.\p"
	.string "I took this job so I could put\n"
	.string "everything I learned at the POKéMON\l"
	.string "TRAINER'S SCHOOL to use in battle.\p"
	.string "So show me how you fight, and with which\n"
	.string "POKéMON! Fair warning, I'm not all that\l"
	.string "lucky, but I don't plan on losing today.$"

RustboroCity_Gym_Text_RoxanneDefeat:
	.string "So…\n"
	.string "I lost…\p"
	.string "Figures. Bad luck follows me everywhere,\n"
	.string "but that was all you.\p"
	.string "It seems I still have much to learn…\p"
	.string "I understand.\p"
	.string "The POKéMON LEAGUE's rules say\n"
	.string "TRAINERS are to be given this if they\l"
	.string "defeat a GYM LEADER.\p"
	.string "Please accept the official POKéMON\n"
	.string "LEAGUE STONE BADGE.$"

RustboroCity_Gym_Text_ReceivedStoneBadge:
	.string "{PLAYER} received the STONE BADGE\n"
	.string "from YAMCHA.$"

RustboroCity_Gym_Text_StoneBadgeInfoTakeThis:
	.string "The STONE BADGE boosts the ATTACK\n"
	.string "power of your POKéMON.\p"
	.string "It also lets them use the HM move CUT\n"
	.string "outside of battle.\p"
	.string "Take this with you, too.$"

RustboroCity_Gym_Text_ExplainRockTomb:
	.string "That TECHNICAL MACHINE, TM39, contains\n"
	.string "ROCK TOMB.\p"
	.string "It not only does damage by dropping\n"
	.string "rocks, it also lowers SPEED.\p"
	.string "If you use a TM, it instantly teaches\n"
	.string "the move to a POKéMON.\p"
	.string "Remember, a TM can be used only once, so\n"
	.string "think before you use it.$"

RustboroCity_Gym_Text_RoxannePostBattle:
	.string "You're strong enough to challenge the\n"
	.string "other GYM LEADERS.\p"
	.string "By battling many TRAINERS, you'll learn\n"
	.string "many things.$"

RustboroCity_Gym_Text_GymStatue:
	.string "RUSTBORO CITY POKéMON GYM$"

RustboroCity_Gym_Text_GymStatueCertified:
	.string "RUSTBORO CITY POKéMON GYM\p"
	.string "YAMCHA'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

RustboroCity_Gym_Text_RoxanneRegisterCall:
	.string "… … … … … …\n"
	.string "… … … … … Beep!\p"
	.string "YAMCHA: Oh, hey, {PLAYER}? It's YAMCHA,\n"
	.string "from RUSTBORO CITY.\p"
	.string "I heard from TIEN that you're climbing\n"
	.string "the ranks of TRAINERS at an incredible\l"
	.string "clip.\p"
	.string "Heh, I'd rather not think that I can't\n"
	.string "beat you.\p"
	.string "I promise I'll whip my GYM into shape so\n"
	.string "you can challenge us again.\p"
	.string "I hope you're looking forward to it as\n"
	.string "much as I am!\p"
	.string "… … … … … …\n"
	.string "… … … … … Click!$"

RustboroCity_Gym_Text_RegisteredRoxanne:
	.string "Registered GYM LEADER YAMCHA\n"
	.string "in the POKéNAV.$"

RustboroCity_Gym_Text_RoxannePreRematch:
	.string "YAMCHA: Great to see you again! I'm\n"
	.string "YAMCHA, the GYM LEADER here.\p"
	.string "I'm sure we've both fought plenty of\n"
	.string "battles since we last met.\p"
	.string "I'd like to see how much stronger we've\n"
	.string "gotten. Let's go!$"

RustboroCity_Gym_Text_RoxanneRematchDefeat:
	.string "Grr…\n"
	.string "I lost again…$"

RustboroCity_Gym_Text_RoxannePostRematch:
	.string "YAMCHA: I still have a lot to learn about\n"
	.string "battling POKéMON.\p"
	.string "Knowing that makes me love battling\n"
	.string "even more!$"

RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons:
	.string "YAMCHA: Great to see you again! I'm\n"
	.string "YAMCHA, the GYM LEADER here.\p"
	.string "I'm sure we've both fought plenty of\n"
	.string "battles since we last met.\p"
	.string "I'd like to see how much stronger we've\n"
	.string "gotten…\p"
	.string "Oh, no…\n"
	.string "You only have one POKéMON with you?\p"
	.string "Please come back with at least one more\n"
	.string "POKéMON.$"
