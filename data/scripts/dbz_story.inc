@ PokeBall Orange: boss fights (General Blue, Kid Buu, Dr. Gero) and the Sootopolis finale (Potara fusion)

@ VAR_0x800A = boss kind. Keeps VAR_0x8004 for the map script that called it.
EventScript_DBZ_BossFight::
	copyvar VAR_0x800B, VAR_0x8004
	call_if_eq VAR_0x800A, DBZ_ENEMY_GENERAL_BLUE, EventScript_DBZ_BlueIntro
	call_if_eq VAR_0x800A, DBZ_ENEMY_KID_BUU, EventScript_DBZ_BuuIntro
	call_if_eq VAR_0x800A, DBZ_ENEMY_DR_GERO, EventScript_DBZ_GeroIntro
	copyvar VAR_TEMP_F, VAR_0x800A
	special DBZ_SetupBossFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_BossTooHurt
	closemessage
	special DBZ_StartFightAndWait
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_BossTooHurt
	waitstate
	lockall
	call EventScript_DBZ_RestorePositions
	call EventScript_DBZ_FightResults
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_BossLost
	call_if_eq VAR_TEMP_F, DBZ_ENEMY_GENERAL_BLUE, EventScript_DBZ_BlueBeaten
	call_if_eq VAR_TEMP_F, DBZ_ENEMY_KID_BUU, EventScript_DBZ_BuuBeaten
	call_if_eq VAR_TEMP_F, DBZ_ENEMY_DR_GERO, EventScript_DBZ_GeroBeaten
	special DBZ_QueueFightEvolveCheck
	goto EventScript_DBZ_BossDone

EventScript_DBZ_BossLost::
	special DBZ_ReviveGokuWeak
	call_if_eq VAR_TEMP_F, DBZ_ENEMY_GENERAL_BLUE, EventScript_DBZ_BlueWins
	call_if_eq VAR_TEMP_F, DBZ_ENEMY_KID_BUU, EventScript_DBZ_BuuWins
	call_if_eq VAR_TEMP_F, DBZ_ENEMY_DR_GERO, EventScript_DBZ_GeroWins
	special DBZ_QueueFightEvolveCheck
	goto EventScript_DBZ_BossDone

EventScript_DBZ_BossTooHurt::
	msgbox Text_DBZ_BossTooHurt, MSGBOX_DEFAULT
EventScript_DBZ_BossDone::
	closemessage
	copyvar VAR_0x8004, VAR_0x800B
	return

EventScript_DBZ_BlueIntro::
	msgbox Text_DBZ_BlueIntro, MSGBOX_DEFAULT
	return
EventScript_DBZ_BlueBeaten::
	setflag FLAG_DBZ_BOSS_BLUE
	msgbox Text_DBZ_BlueBeaten, MSGBOX_DEFAULT
	return
EventScript_DBZ_BlueWins::
	msgbox Text_DBZ_BlueWins, MSGBOX_DEFAULT
	return

EventScript_DBZ_BuuIntro::
	msgbox Text_DBZ_BuuIntro, MSGBOX_DEFAULT
	return
EventScript_DBZ_BuuBeaten::
	msgbox Text_DBZ_BuuBeaten, MSGBOX_DEFAULT
	return
EventScript_DBZ_BuuWins::
	msgbox Text_DBZ_BuuWins, MSGBOX_DEFAULT
	return

EventScript_DBZ_GeroIntro::
	msgbox Text_DBZ_GeroIntro, MSGBOX_DEFAULT
	return
EventScript_DBZ_GeroBeaten::
	msgbox Text_DBZ_GeroBeaten, MSGBOX_DEFAULT
	return
EventScript_DBZ_GeroWins::
	msgbox Text_DBZ_GeroWins, MSGBOX_DEFAULT
	return

@ Mt. Chimney: General Blue after his POKeMON battle (and on rematch until he's beaten)
EventScript_DBZ_BlueBoss::
	goto_if_set FLAG_DBZ_BOSS_BLUE, EventScript_DBZ_Return
	setvar VAR_0x800A, DBZ_ENEMY_GENERAL_BLUE
	call EventScript_DBZ_BossFight
	return

@ Magma Hideout: Kid Buu, once, right after the POKeMON battle
EventScript_DBZ_BuuBoss::
	goto_if_set FLAG_DBZ_BOSS_BUU, EventScript_DBZ_Return
	setflag FLAG_DBZ_BOSS_BUU
	setvar VAR_0x800A, DBZ_ENEMY_KID_BUU
	call EventScript_DBZ_BossFight
	return

@ Seafloor Cavern: Dr. Gero, once
EventScript_DBZ_GeroBoss::
	goto_if_set FLAG_DBZ_BOSS_GERO, EventScript_DBZ_Return
	setflag FLAG_DBZ_BOSS_GERO
	setvar VAR_0x800A, DBZ_ENEMY_DR_GERO
	call EventScript_DBZ_BossFight
	return

@ ------------------------------------------------------------------ finale: Sootopolis, after Rayquaza
EventScript_DBZ_Finale::
	goto_if_set FLAG_DBZ_FINALE, EventScript_DBZ_Return
	copyvar VAR_0x800B, VAR_0x8004
	msgbox Text_DBZ_FinaleBuu1, MSGBOX_DEFAULT
	special DBZ_HealGoku
	@ round one: Kid Buu just keeps regenerating
	setvar VAR_0x8008, DBZ_FIGHTF_SCRIPTED
	special DBZ_SetNextFightFlags
	setvar VAR_0x800A, DBZ_ENEMY_KID_BUU
	special DBZ_SetupBossFight
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_FinaleCantFight
	closemessage
	special DBZ_StartFightAndWait
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_FinaleCantFight
	waitstate
	lockall
	call EventScript_DBZ_RestorePositions
	msgbox Text_DBZ_FinaleGokuWorn, MSGBOX_DEFAULT
	closemessage
	@ Vegeta arrives
	playse SE_M_TELEPORT
	fadescreenspeed FADE_TO_WHITE, 0
	special DBZ_SpawnVegetaAlly
	fadescreenspeed FADE_FROM_WHITE, 0
	msgbox Text_DBZ_FinaleVegeta1, MSGBOX_DEFAULT
	msgbox Text_DBZ_FinaleGoku1, MSGBOX_DEFAULT
	msgbox Text_DBZ_FinaleVegeta2, MSGBOX_DEFAULT
	closemessage
	@ Potara
	playse SE_M_REFLECT
	delay 20
	playse SE_M_SKY_UPPERCUT
	fadescreenspeed FADE_TO_WHITE, 2
	special DBZ_StartFusion
	delay 30
	playfanfare MUS_OBTAIN_TMHM
	fadescreenspeed FADE_FROM_WHITE, 2
	special DBZ_RefreshPlayerGfx
	msgbox Text_DBZ_FinaleFused, MSGBOX_DEFAULT
	waitfanfare
	msgbox Text_DBZ_FinaleVegito, MSGBOX_DEFAULT
	msgbox Text_DBZ_FinaleBuu2, MSGBOX_DEFAULT
	@ round two: VEGITO
	setvar VAR_0x8008, DBZ_FIGHTF_FUSION
	special DBZ_SetNextFightFlags
	setvar VAR_0x800A, DBZ_ENEMY_KID_BUU
	special DBZ_SetupBossFight
	closemessage
	special DBZ_StartFightAndWait
	goto_if_eq VAR_RESULT, FALSE, EventScript_DBZ_FinaleDefuse
	waitstate
	lockall
	call EventScript_DBZ_RestorePositions
	call EventScript_DBZ_FightResults
	goto_if_eq VAR_0x8007, DBZ_FIGHT_LOST, EventScript_DBZ_FinaleLost
	setflag FLAG_DBZ_FINALE
	msgbox Text_DBZ_FinaleWon, MSGBOX_DEFAULT
	call EventScript_DBZ_FinaleDefuseFx
	msgbox Text_DBZ_FinaleDefused, MSGBOX_DEFAULT
	special DBZ_HealGoku
	special DBZ_QueueFightEvolveCheck
	goto EventScript_DBZ_FinaleSkip

EventScript_DBZ_FinaleLost::
	call EventScript_DBZ_FinaleDefuseFx
	msgbox Text_DBZ_FinaleLost, MSGBOX_DEFAULT
	special DBZ_HealGoku
	goto EventScript_DBZ_FinaleSkip

@ no room to fight (or not on foot): the story goes on without the fight
EventScript_DBZ_FinaleCantFight::
	setflag FLAG_DBZ_FINALE
	goto EventScript_DBZ_FinaleSkip

EventScript_DBZ_FinaleDefuse::
	call EventScript_DBZ_FinaleDefuseFx
EventScript_DBZ_FinaleSkip::
	setvar VAR_0x8008, 0
	special DBZ_SetNextFightFlags
	closemessage
	copyvar VAR_0x8004, VAR_0x800B
	return

@ keeps VAR_RESULT (the fight result)
EventScript_DBZ_RestorePositions::
	copyvar VAR_0x8007, VAR_RESULT
	fadescreen FADE_TO_BLACK
	special DBZ_RestoreFightPositions
	fadescreen FADE_FROM_BLACK
	copyvar VAR_RESULT, VAR_0x8007
	return

EventScript_DBZ_FinaleDefuseFx::
	closemessage
	playse SE_M_TELEPORT
	fadescreenspeed FADE_TO_WHITE, 1
	special DBZ_EndFusion
	fadescreenspeed FADE_FROM_WHITE, 1
	special DBZ_RefreshPlayerGfx
	return

Text_DBZ_BossTooHurt:
	.string "{PLAYER} is too worn out to fight\n"
	.string "right now…$"

Text_DBZ_BlueIntro:
	.string "GENERAL BLUE: Hmph. Beating my\n"
	.string "POKéMON proves nothing, boy.\p"
	.string "Let me show you why the RED RIBBON\n"
	.string "ARMY made me a GENERAL!\p"
	.string "Look into my eyes… You can't move,\n"
	.string "can you? Heh heh heh!$"

Text_DBZ_BlueBeaten:
	.string "GENERAL BLUE: Impossible…\n"
	.string "My paralysis stare…\p"
	.string "Kid Buu will… hear about this…$"

Text_DBZ_BlueWins:
	.string "GENERAL BLUE: Ha! Know your place,\n"
	.string "monkey boy. Come back when you've\l"
	.string "grown a spine!$"

Text_DBZ_BuuIntro:
	.string "KID BUU: Hee hee hee!\n"
	.string "Funny little monkey man!\p"
	.string "POKéMON fight was boring.\n"
	.string "YOU play with Buu now!$"

Text_DBZ_BuuBeaten:
	.string "KID BUU: Grrr… Buu not done!\n"
	.string "Buu go wake big red thing!\p"
	.string "Then Buu squish monkey man!\n"
	.string "Hee hee hee!$"

Text_DBZ_BuuWins:
	.string "KID BUU: Hee hee! Monkey man broke!\n"
	.string "Buu go play with big red thing now!$"

Text_DBZ_GeroIntro:
	.string "DR. GERO: Your ki readings are\n"
	.string "remarkable, boy. Truly.\p"
	.string "My machines will drink every last\n"
	.string "drop of it. Attack me with energy\l"
	.string "and you only make me stronger!$"

Text_DBZ_GeroBeaten:
	.string "DR. GERO: My calculations…\n"
	.string "They were flawless…\p"
	.string "No matter. KYOGRE wakes either way!$"

Text_DBZ_GeroWins:
	.string "DR. GERO: As predicted.\n"
	.string "Pure data beats raw instinct.$"

Text_DBZ_FinaleBuu1:
	.string "KID BUU: Big things went back to\n"
	.string "sleep? BORING!\p"
	.string "Buu blow up whole town instead!\n"
	.string "Monkey man first! Hee hee hee!$"

Text_DBZ_FinaleGokuWorn:
	.string "{PLAYER}: Huff… huff…\p"
	.string "Every time I hit him, he just\n"
	.string "puts himself back together…\p"
	.string "I can't finish him by myself!$"

Text_DBZ_FinaleVegeta1:
	.string "VEGETA: Pathetic, KAKAROT.\n"
	.string "Rolling around in the dirt again?$"

Text_DBZ_FinaleGoku1:
	.string "{PLAYER}: VEGETA! You came!\p"
	.string "Hey… are those the earrings the\n"
	.string "SUPREME KAI was talking about?$"

Text_DBZ_FinaleVegeta2:
	.string "VEGETA: The Potara. Put one on and\n"
	.string "we fuse. Forever.\p"
	.string "…Don't look so happy about it!\n"
	.string "Just this once, KAKAROT!$"

Text_DBZ_FinaleFused:
	.string "{PLAYER} and VEGETA fused into\n"
	.string "VEGITO!$"

Text_DBZ_FinaleVegito:
	.string "VEGITO: Alright, Buu.\n"
	.string "Let's see what you've really got.$"

Text_DBZ_FinaleBuu2:
	.string "KID BUU: …Who are YOU?!\n"
	.string "Buu not like you!$"

Text_DBZ_FinaleWon:
	.string "KID BUU: Buu… so tired…\n"
	.string "Buu go sleep… for long time…\p"
	.string "VEGITO: Sweet dreams. Try not to\n"
	.string "wake up grumpy.$"

Text_DBZ_FinaleDefused:
	.string "The Potara's power faded…\p"
	.string "VEGETA: Hmph. That never happened.\n"
	.string "Tell anyone and you're dead,\l"
	.string "KAKAROT.$"

Text_DBZ_FinaleLost:
	.string "The fusion came apart!\p"
	.string "KID BUU: Hee hee! Two monkeys,\n"
	.string "still lose! Come back, play again!$"
