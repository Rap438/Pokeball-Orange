AquaHideout_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AquaHideout_B2F_OnTransition
	.byte 0

AquaHideout_B2F_OnTransition:
	call_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, AquaHideout_B2F_EventScript_PreventMattNoticing
	end

AquaHideout_B2F_EventScript_PreventMattNoticing::
	setvar VAR_TEMP_1, 1
	return

AquaHideout_B2F_EventScript_MattNoticePlayer::
	lockall
	setvar VAR_0x8008, LOCALID_AQUA_HIDEOUT_MATT
	playse SE_PIN
	applywaitmovement VAR_0x8008, Common_Movement_ExclamationMark
	applywaitmovement VAR_0x8008, Common_Movement_FacePlayer
	setvar VAR_TEMP_1, 1
	releaseall
	end

AquaHideout_B2F_EventScript_Matt::
	trainerbattle_single TRAINER_MATT, AquaHideout_B2F_Text_MattIntro, AquaHideout_B2F_Text_MattDefeat, AquaHideout_B2F_EventScript_SubmarineEscape
	msgbox AquaHideout_B2F_Text_MattPostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_B2F_EventScript_SubmarineEscape::
	setvar VAR_0x8008, LOCALID_AQUA_HIDEOUT_MATT
	setvar VAR_0x8009, LOCALID_AQUA_HIDEOUT_SUBMARINE
	applywaitmovement VAR_0x8008, Common_Movement_WalkInPlaceFasterLeft
	delay 20
	applywaitmovement VAR_0x8008, Common_Movement_FacePlayer
	msgbox AquaHideout_B2F_Text_OurBossGotThroughHisPreparations, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8008, Common_Movement_WalkInPlaceFasterLeft
	applymovement VAR_0x8009, AquaHideout_B2F_Movement_SumbarineDepartLeft
	waitmovement 0
	removeobject VAR_0x8009
	delay 20
	applywaitmovement VAR_0x8008, Common_Movement_FacePlayer
	msgbox AquaHideout_B2F_Text_MattPostBattle, MSGBOX_DEFAULT
	setflag FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE
	setflag FLAG_HIDE_LILYCOVE_CITY_AQUA_GRUNTS
	release
	end

AquaHideout_B2F_Movement_SumbarineDepartLeft:
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

@ Unused
AquaHideout_B2F_Movement_SumbarineDepartRight:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

AquaHideout_B2F_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_4, AquaHideout_B2F_Text_Grunt4Intro, AquaHideout_B2F_Text_Grunt4Defeat, AquaHideout_B2F_EventScript_Grunt4Defeated
	msgbox AquaHideout_B2F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_EventScript_Grunt4Defeated::
	msgbox AquaHideout_B2F_Text_Grunt4PostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_B2F_EventScript_Grunt6::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_6, AquaHideout_B2F_Text_Grunt6Intro, AquaHideout_B2F_Text_Grunt6Defeat
	msgbox AquaHideout_B2F_Text_Grunt6PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_EventScript_Grunt8::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_8, AquaHideout_B2F_Text_Grunt8Intro, AquaHideout_B2F_Text_Grunt8Defeat
	msgbox AquaHideout_B2F_Text_Grunt8PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_Text_MattIntro:
	.string "Hehehe…\p"
	.string "You got this far already? We\n"
	.string "underestimated you!\p"
	.string "But this is the end of the line!\p"
	.string "I'm COLONEL SILVER, a cut above the\n"
	.string "grunts you've flattened so far.\p"
	.string "I'm not stalling for time. I'm going to\n"
	.string "pulverize you!$"

AquaHideout_B2F_Text_MattDefeat:
	.string "Hehehe…\n"
	.string "So I lost, too…$"

AquaHideout_B2F_Text_OurBossGotThroughHisPreparations:
	.string "Hehehe!\p"
	.string "While I was toying with you, DR. GERO\n"
	.string "finished his preparations!$"

AquaHideout_B2F_Text_MattPostBattle:
	.string "Hehehe!\p"
	.string "DR. GERO has already set off in the\n"
	.string "stolen submarine, bound for some cave\l"
	.string "under the sea!\p"
	.string "If you're going to give chase, you'd\n"
	.string "better search the wide, deep sea\l"
	.string "beyond LILYCOVE.\p"
	.string "But will you ever find it? Hehehe!$"

AquaHideout_B2F_Text_Grunt4Intro:
	.string "Wahaha, I got sick of waiting around!\n"
	.string "You owe me a fight, too!$"

AquaHideout_B2F_Text_Grunt4Defeat:
	.string "Tired of waiting…\n"
	.string "Now I'm beaten and dizzy…$"

AquaHideout_B2F_Text_Grunt4PostBattle:
	.string "Colonel…\n"
	.string "Was that good enough?$"

AquaHideout_B2F_Text_Grunt6Intro:
	.string "The warp panels are the pride and joy of\n"
	.string "this BASE!\p"
	.string "You have no idea where you are, do you?\p"
	.string "Confuse the enemy, wear them out, then\n"
	.string "drop the hammer! That's our plan!$"

AquaHideout_B2F_Text_Grunt6Defeat:
	.string "What's with you?\n"
	.string "You're not even tired!$"

AquaHideout_B2F_Text_Grunt6PostBattle:
	.string "Come to think of it… I can't remember\n"
	.string "where I put the MASTER BALL.\p"
	.string "If I fail to guard it, KID BUU might turn\n"
	.string "me into candy…$"

AquaHideout_B2F_Text_Grunt8Intro:
	.string "The first thing they taught me when I\n"
	.string "joined the RED RIBBON ARMY was how\l"
	.string "these warp panels connect.$"

AquaHideout_B2F_Text_Grunt8Defeat:
	.string "I was too busy thinking about the warp\n"
	.string "panels…$"

AquaHideout_B2F_Text_Grunt8PostBattle:
	.string "I'd better study how to fight more\n"
	.string "effectively…$"
