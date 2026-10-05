LavaridgeTown_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavaridgeTown_Gym_1F_OnTransition
	.byte 0

LavaridgeTown_Gym_1F_OnTransition:
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	call LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers
	end

@ Unclear where/if these temp vars are getting checked
LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars::
	setvar VAR_TEMP_B, 0
	setvar VAR_TEMP_C, 0
	setvar VAR_TEMP_D, 0
	setvar VAR_TEMP_E, 0
	setvar VAR_TEMP_F, 0
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar
	setvar VAR_TEMP_B, 1
LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar
	setvar VAR_TEMP_C, 1
LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar
	setvar VAR_TEMP_D, 1
LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars
	setvar VAR_TEMP_E, 1
LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars::
	return

LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers::
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald
	setobjectmovementtype LOCALID_COLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle
	setobjectmovementtype LOCALID_GERALD, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle
	setobjectmovementtype LOCALID_AXLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers
	setobjectmovementtype LOCALID_DANIELLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers::
	return

LavaridgeTown_Gym_1F_EventScript_Flannery::
	trainerbattle_single TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryIntro, LavaridgeTown_Gym_1F_Text_FlanneryDefeat, LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, LavaridgeTown_Gym_1F_EventScript_FlanneryRematch
	goto_if_unset FLAG_RECEIVED_TM_OVERHEAT, LavaridgeTown_Gym_1F_EventScript_GiveOverheat2
	msgbox LavaridgeTown_Gym_1F_Text_FlanneryPostBattle, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated::
	message LavaridgeTown_Gym_1F_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LavaridgeTown_Gym_1F_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_WHITEOUT_TO_LAVARIDGE
	setflag FLAG_DEFEATED_LAVARIDGE_GYM
	setflag FLAG_BADGE04_GET
	special DBZ_QueueGymFight
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 4
	call Common_EventScript_SetGymTrainers
	setflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	setvar VAR_LAVARIDGE_TOWN_STATE, 1
	call LavaridgeTown_Gym_1F_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LavaridgeTown_Gym_1F_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LavaridgeTown_Gym_1F_EventScript_GiveOverheat2::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	release
	end

LavaridgeTown_Gym_1F_EventScript_GiveOverheat::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	return

LavaridgeTown_Gym_1F_EventScript_FlanneryRematch::
	trainerbattle_rematch_double TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryPreRematch, LavaridgeTown_Gym_1F_Text_FlanneryRematchDefeat, LavaridgeTown_Gym_1F_Text_FlanneryRematchNeedTwoMons
	msgbox LavaridgeTown_Gym_1F_Text_FlanneryPostRematch, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Cole::
	trainerbattle_lavaridge LOCALID_COLE, TRAINER_COLE, LavaridgeTown_Gym_1F_Text_ColeIntro, LavaridgeTown_Gym_1F_Text_ColeDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	end

LavaridgeTown_Gym_EventScript_CheckTrainerScript::
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	release
	special ShouldTryGetTrainerScript
	goto_if_eq VAR_RESULT, 1, EventScript_GotoTrainerScript
	end

LavaridgeTown_Gym_1F_EventScript_Axle::
	trainerbattle_lavaridge LOCALID_AXLE, TRAINER_AXLE, LavaridgeTown_Gym_1F_Text_AxleIntro, LavaridgeTown_Gym_1F_Text_AxleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_AxlePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Keegan::
	trainerbattle_lavaridge LOCALID_KEEGAN, TRAINER_KEEGAN, LavaridgeTown_Gym_B1F_Text_KeeganIntro, LavaridgeTown_Gym_B1F_Text_KeeganDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_KeeganPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Danielle::
	trainerbattle_lavaridge LOCALID_DANIELLE, TRAINER_DANIELLE, LavaridgeTown_Gym_1F_Text_DanielleIntro, LavaridgeTown_Gym_1F_Text_DanielleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_DaniellePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Gerald::
	trainerbattle_lavaridge LOCALID_GERALD, TRAINER_GERALD, LavaridgeTown_Gym_1F_Text_GeraldIntro, LavaridgeTown_Gym_1F_Text_GeraldDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_GeraldPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jace::
	trainerbattle_lavaridge LOCALID_JACE, TRAINER_JACE, LavaridgeTown_Gym_B1F_Text_JaceIntro, LavaridgeTown_Gym_B1F_Text_JaceDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_JacePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jeff::
	trainerbattle_lavaridge LOCALID_JEFF, TRAINER_JEFF, LavaridgeTown_Gym_B1F_Text_JeffIntro, LavaridgeTown_Gym_B1F_Text_JeffDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_JeffPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Eli::
	trainerbattle_lavaridge LOCALID_ELI, TRAINER_ELI, LavaridgeTown_Gym_B1F_Text_EliIntro, LavaridgeTown_Gym_B1F_Text_EliDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_EliPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory
	msgbox LavaridgeTown_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox LavaridgeTown_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_GymStatueCertified::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_EventScript_GymStatue::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_Text_GymGuideAdvice:
	.string "Hey, how's it going, CHAMPION-bound\n"
	.string "{PLAYER}?\p"
	.string "LAVARIDGE's GYM LEADER, VIDEL, is MR.\n"
	.string "SATAN's daughter, and she uses\l"
	.string "FIRE-type POKéMON.\p"
	.string "Her passion burns hotter than a\n"
	.string "volcano.\p"
	.string "Don't get too close or you'll get\n"
	.string "burned! Hose her down with WATER-type\l"
	.string "moves and then go for it!$"

LavaridgeTown_Gym_1F_Text_GymGuidePostVictory:
	.string "Whoa! That was a scorching-hot battle!\n"
	.string "My ki is still buzzing!$"

LavaridgeTown_Gym_1F_Text_ColeIntro:
	.string "Owowowowow! Hot, hot, hot! This gym is a\n"
	.string "sauna!$"

LavaridgeTown_Gym_1F_Text_ColeDefeat:
	.string "I've got sweat in my eyes, I can't see…$"

LavaridgeTown_Gym_1F_Text_ColePostBattle:
	.string "Getting buried in hot sand is great for\n"
	.string "circulation.\p"
	.string "It's wonderful for aching joints after\n"
	.string "a hard day of training.$"

LavaridgeTown_Gym_1F_Text_AxleIntro:
	.string "I'm trying to relieve my stress. Don't\n"
	.string "come along and stress me out!$"

LavaridgeTown_Gym_1F_Text_AxleDefeat:
	.string "I hope VIDEL roasts you good!$"

LavaridgeTown_Gym_1F_Text_AxlePostBattle:
	.string "Haaah… Whew…\p"
	.string "If you stay buried in that hot sand too\n"
	.string "long, it wears you out…$"

LavaridgeTown_Gym_B1F_Text_KeeganIntro:
	.string "You must be getting tired by now. You'd\n"
	.string "love to rest in the hot sand, wouldn't\l"
	.string "you?\p"
	.string "But remember, a strong will is one of the\n"
	.string "most important abilities a TRAINER can\l"
	.string "have.$"

LavaridgeTown_Gym_B1F_Text_KeeganDefeat:
	.string "Play with fire, and you get burned…$"

LavaridgeTown_Gym_B1F_Text_KeeganPostBattle:
	.string "Your skill is real… but our LEADER VIDEL\n"
	.string "is strong.\p"
	.string "If you let your guard down, she'll burn\n"
	.string "you badly.$"

LavaridgeTown_Gym_1F_Text_GeraldIntro:
	.string "Can your POKéMON withstand 392-degree\n"
	.string "heat?$"

LavaridgeTown_Gym_1F_Text_GeraldDefeat:
	.string "It wasn't hot enough to burn you…$"

LavaridgeTown_Gym_1F_Text_GeraldPostBattle:
	.string "Magma burns at about 392 degrees.\p"
	.string "You beat me, so your POKéMON should\n"
	.string "survive in magma just fine.$"

LavaridgeTown_Gym_1F_Text_DanielleIntro:
	.string "Um… okay. I'll battle you.$"

LavaridgeTown_Gym_1F_Text_DanielleDefeat:
	.string "Oh, you're way too strong.$"

LavaridgeTown_Gym_1F_Text_DaniellePostBattle:
	.string "Someday I'm going to be a pretty and\n"
	.string "strong TRAINER, just like VIDEL.$"

LavaridgeTown_Gym_B1F_Text_JaceIntro:
	.string "Come on, get fired up! Let's go before\n"
	.string "my fighting spirit cools!$"

LavaridgeTown_Gym_B1F_Text_JaceDefeat:
	.string "It's so hot in here, yet my heart is\n"
	.string "frozen…$"

LavaridgeTown_Gym_B1F_Text_JacePostBattle:
	.string "The fighting spirit burning inside you\n"
	.string "might be enough to stand up to our\l"
	.string "LEADER.$"

LavaridgeTown_Gym_B1F_Text_JeffIntro:
	.string "See how the flames blaze wildly? They're\n"
	.string "flaring because I'm about to win!$"

LavaridgeTown_Gym_B1F_Text_JeffDefeat:
	.string "Something went wrong.$"

LavaridgeTown_Gym_B1F_Text_JeffPostBattle:
	.string "So what? I say, so what! I can walk on\n"
	.string "hot coals barefoot!\p"
	.string "…Don't you dare try it!$"

LavaridgeTown_Gym_B1F_Text_EliIntro:
	.string "I love mountains, but I especially love\n"
	.string "volcanoes.$"

LavaridgeTown_Gym_B1F_Text_EliDefeat:
	.string "Seems to me I lost without ever being in\n"
	.string "control.$"

LavaridgeTown_Gym_B1F_Text_EliPostBattle:
	.string "I train here because I'm a fan of\n"
	.string "VIDEL's power.\p"
	.string "Hehehehe.$"

LavaridgeTown_Gym_1F_Text_FlanneryIntro:
	.string "Welcome… no, wait.\p"
	.string "Hmph! So you made it here, challenger!\p"
	.string "I've been entrusted with the… no, wait.\p"
	.string "I'm VIDEL, and I'm the GYM LEADER here!\p"
	.string "Uh… Don't underestimate me just\n"
	.string "because I've only been LEADER a short\l"
	.string "time!\p"
	.string "My father may be MR. SATAN, the World\n"
	.string "Champion, but I don't need his name to\l"
	.string "win!\p"
	.string "With the martial arts I've honed on my\n"
	.string "own, I'll show you the red-hot moves of\l"
	.string "this land!$"

LavaridgeTown_Gym_1F_Text_FlanneryDefeat:
	.string "Oh… I guess I was trying too hard…\p"
	.string "I've only just become a GYM LEADER.\p"
	.string "I tried so hard to be the World\n"
	.string "Champion's daughter that I forgot to\l"
	.string "be myself.\p"
	.string "I have to fight my own way. If I don't,\n"
	.string "my POKéMON get confused.\p"
	.string "Thanks for teaching me that. For that,\n"
	.string "you deserve this.$"

LavaridgeTown_Gym_1F_Text_ReceivedHeatBadge:
	.string "{PLAYER} received the HEAT BADGE from\n"
	.string "VIDEL.$"

LavaridgeTown_Gym_1F_Text_ExplainHeatBadgeTakeThis:
	.string "With the HEAT BADGE, all POKéMON up to\n"
	.string "Level 50, even ones you got in trades,\l"
	.string "will obey you completely.\p"
	.string "It also lets POKéMON use the HM move\n"
	.string "STRENGTH outside of battle.\p"
	.string "And here's a token of my thanks. Don't\n"
	.string "be shy, take it!$"

LavaridgeTown_Gym_1F_Text_ExplainOverheat:
	.string "That TM50 contains OVERHEAT.\p"
	.string "It's an all-out blast that does serious\n"
	.string "damage to the opponent.\p"
	.string "But it sharply cuts the SP. ATK of the\n"
	.string "POKéMON using it, like a fighter who\l"
	.string "spent all his ki at once. It may not suit\l"
	.string "a long battle.$"

LavaridgeTown_Gym_1F_Text_RegisteredFlannery:
	.string "Registered GYM LEADER VIDEL in the\n"
	.string "POKéNAV.$"

LavaridgeTown_Gym_1F_Text_FlanneryPostBattle:
	.string "Your power reminds me of someone…\p"
	.string "Oh! I know! You fight like KRILLIN, the\n"
	.string "GYM LEADER of PETALBURG. Did you two\l"
	.string "train together?$"

LavaridgeTown_Gym_1F_Text_GymStatue:
	.string "LAVARIDGE TOWN POKéMON GYM$"

LavaridgeTown_Gym_1F_Text_GymStatueCertified:
	.string "LAVARIDGE TOWN POKéMON GYM\p"
	.string "VIDEL'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

LavaridgeTown_Gym_1F_Text_FlanneryPreRematch:
	.string "VIDEL: A loss won't put out my fire.\p"
	.string "I love POKéMON. I love to battle. And… I\n"
	.string "love this GYM!\p"
	.string "Let's trade more red-hot moves in\n"
	.string "another battle!$"

LavaridgeTown_Gym_1F_Text_FlanneryRematchDefeat:
	.string "Whew! I'm about to erupt!$"

LavaridgeTown_Gym_1F_Text_FlanneryPostRematch:
	.string "VIDEL: I lost, but I'm completely\n"
	.string "satisfied.\p"
	.string "It's not often I get a battle this\n"
	.string "heated.\p"
	.string "Let's do it again sometime!$"

LavaridgeTown_Gym_1F_Text_FlanneryRematchNeedTwoMons:
	.string "VIDEL: A loss won't put out my fire.\p"
	.string "I love POKéMON. I love to battle. And… I\n"
	.string "love this GYM!\p"
	.string "Let's trade more red-hot moves in\n"
	.string "another battle!\p"
	.string "Oh, wait. Do you only have one POKéMON\n"
	.string "that can battle?\p"
	.string "I hate to say this, but when you come\n"
	.string "see me, bring at least two POKéMON.$"
