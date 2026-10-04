MagmaHideout_4F_MapScripts::
	.byte 0

MagmaHideout_4F_EventScript_Maxie::
	lockall
	playbgm MUS_ENCOUNTER_MAGMA, FALSE
	msgbox MagmaHideout_4F_Text_MaxieAwakenGroudon, MSGBOX_DEFAULT
	closemessage
	delay 20
	setvar VAR_RESULT, 1
	playse SE_M_DETECT
	dofieldeffectsparkle 18, 42, 0
	waitfieldeffect FLDEFF_SPARKLE
	setvar VAR_RESULT, 1
	playfanfare MUS_AWAKEN_LEGEND
	playse SE_ORB
	special DoOrbEffect
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	delay 150
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GROUDON_SLEEPING
	addobject LOCALID_MAGMA_HIDEOUT_4F_GROUDON
	waitstate
	delay 60
	applywaitmovement LOCALID_MAGMA_HIDEOUT_4F_GROUDON, MagmaHideout_4F_Movement_GroudonApproach
	special FadeOutOrbEffect
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	applywaitmovement LOCALID_MAGMA_HIDEOUT_4F_GROUDON, MagmaHideout_4F_Movement_GroudonExit
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GROUDON
	delay 4
	setvar VAR_0x8004, 2  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	delay 30
	applywaitmovement LOCALID_MAGMA_HIDEOUT_4F_MAXIE, MagmaHideout_4F_Movement_MaxieLookAround
	msgbox MagmaHideout_4F_Text_MaxieGroudonWhatsWrong, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	delay 30
	applywaitmovement LOCALID_MAGMA_HIDEOUT_4F_MAXIE, Common_Movement_FacePlayer
	msgbox MagmaHideout_4F_Text_MaxieOhItWasYou, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_MAXIE_MAGMA_HIDEOUT, MagmaHideout_4F_Text_MaxieDefeat
	msgbox MagmaHideout_4F_Text_MaxieImGoingAfterGroudon, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_SLATEPORT_CITY_CAPTAIN_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_GABBY_AND_TY
	setvar VAR_SLATEPORT_CITY_STATE, 1
	setflag FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT
	setvar VAR_SLATEPORT_HARBOR_STATE, 1
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_MAGMA_HIDEOUT_4F_MAXIE
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GRUNT_1
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GRUNT_2
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GRUNT_3
	removeobject LOCALID_MAGMA_HIDEOUT_4F_TABITHA
	setflag FLAG_HIDE_MAGMA_HIDEOUT_GRUNTS
	fadescreen FADE_FROM_BLACK
	releaseall
	end

MagmaHideout_4F_Movement_GroudonApproach:
	delay_16
	delay_16
	walk_slow_down
	delay_16
	delay_16
	delay_16
	walk_slow_down
	delay_16
	delay_16
	delay_16
	step_end

MagmaHideout_4F_Movement_GroudonExit:
	slide_up
	slide_up
	step_end

MagmaHideout_4F_Movement_MaxieLookAround:
	face_left
	delay_16
	face_right
	delay_16
	face_left
	delay_16
	face_right
	delay_16
	face_up
	delay_16
	delay_16
	step_end

MagmaHideout_4F_EventScript_Grunt11::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_11, MagmaHideout_4F_Text_Grunt11Intro, MagmaHideout_4F_Text_Grunt11Defeat
	msgbox MagmaHideout_4F_Text_Grunt11PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Grunt12::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_12, MagmaHideout_4F_Text_Grunt12Intro, MagmaHideout_4F_Text_Grunt12Defeat
	msgbox MagmaHideout_4F_Text_Grunt12PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Grunt13::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_13, MagmaHideout_4F_Text_Grunt13Intro, MagmaHideout_4F_Text_Grunt13Defeat
	msgbox MagmaHideout_4F_Text_Grunt13PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Tabitha::
	trainerbattle_single TRAINER_TABITHA_MAGMA_HIDEOUT, MagmaHideout_4F_Text_TabithaIntro, MagmaHideout_4F_Text_TabithaDefeat
	msgbox MagmaHideout_4F_Text_TabithaPostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_Text_Grunt11Intro:
	.string "I want to see GROUDON, too, but they\n"
	.string "won't even let me see its tail…\p"
	.string "It's so frustrating.\p"
	.string "Oh, no! I blabbed about GROUDON!$"

MagmaHideout_4F_Text_Grunt11Defeat:
	.string "I guess you can't win without a calm\n"
	.string "mind…$"

MagmaHideout_4F_Text_Grunt11PostBattle:
	.string "I wonder if GROUDON even has a tail?$"

MagmaHideout_4F_Text_Grunt12Intro:
	.string "Fuhahaha! Soon! Very soon!\p"
	.string "Our grand objective will be achieved!$"

MagmaHideout_4F_Text_Grunt12Defeat:
	.string "Grrr… I came so far, and now this?$"

MagmaHideout_4F_Text_Grunt12PostBattle:
	.string "KID BUU, sir! An intruder is headed your\n"
	.string "way!$"

MagmaHideout_4F_Text_Grunt13Intro:
	.string "You're not through yet! You won't get\n"
	.string "past me that easily!$"

MagmaHideout_4F_Text_Grunt13Defeat:
	.string "Was I really that easy to knock down?$"

MagmaHideout_4F_Text_Grunt13PostBattle:
	.string "C-come on, one more match…$"

MagmaHideout_4F_Text_TabithaIntro:
	.string "Hehehe! You made it this far, so I'll tell\n"
	.string "you!\p"
	.string "That's right! GROUDON is sleeping just\n"
	.string "up ahead!\p"
	.string "KID BUU went to see it only seconds\n"
	.string "ago!\p"
	.string "It's going to wake up any moment now!\n"
	.string "Hehe! Hehehe!$"

MagmaHideout_4F_Text_TabithaDefeat:
	.string "Beaten again… Hehe…$"

MagmaHideout_4F_Text_TabithaPostBattle:
	.string "…While you wasted time with me, KID BUU\n"
	.string "must have already awakened GROUDON…$"

MagmaHideout_4F_Text_MaxieAwakenGroudon:
	.string "KID BUU: Hee hee hee! GROUDON!\p"
	.string "Wake up, wake up! Nothing could rouse\n"
	.string "you from your nap in the magma…\p"
	.string "You wanted this BLUE ORB, didn't you?\p"
	.string "I brought it to you! Let it shine and\n"
	.string "wake you up!\p"
	.string "Now show me! Show me how much you can\n"
	.string "break!$"

MagmaHideout_4F_Text_MaxieGroudonWhatsWrong:
	.string "KID BUU: GROUDON! Hey! What's wrong?\p"
	.string "Wasn't the BLUE ORB the key?\p"
	.string "GROUDON! Where did you go…\p"
	.string "…It ran away! Hee hee! It really ran\n"
	.string "away!$"

MagmaHideout_4F_Text_MaxieOhItWasYou:
	.string "KID BUU: Oh? It was you?\p"
	.string "I saw you poking around here and there\n"
	.string "where you weren't invited…\p"
	.string "I get it now! You must have pulled some\n"
	.string "sneaky trick on my toy!$"

MagmaHideout_4F_Text_MaxieDefeat:
	.string "Huh? How come you're so good with\n"
	.string "POKéMON?$"

MagmaHideout_4F_Text_MaxieImGoingAfterGroudon:
	.string "KID BUU: You think GROUDON ran for a\n"
	.string "reason…\p"
	.string "That's what you're trying to say, isn't\n"
	.string "it?\p"
	.string "… … … … … …\p"
	.string "Hee hee! You think I didn't know that?\p"
	.string "Now that GROUDON's gone, I don't need\n"
	.string "this boring old volcano.\p"
	.string "I'm going to go chase GROUDON, so this\n"
	.string "is where we say bye-bye!$"
