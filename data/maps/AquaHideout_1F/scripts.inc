AquaHideout_1F_MapScripts::
	.byte 0

@ The below two entrance guards give hints about what to do to progress the story
AquaHideout_1F_EventScript_HideoutEntranceGrunt1::
	lock
	faceplayer
	goto_if_set FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT, AquaHideout_1F_EventScript_SlateportHint1
	goto_if_set FLAG_RECEIVED_RED_OR_BLUE_ORB, AquaHideout_1F_EventScript_MagmaHideoutHint1
	msgbox AquaHideout_1F_Text_OurBossIsSnatchingSomething, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_MagmaHideoutHint1::
	msgbox AquaHideout_1F_Text_WhereMightMagmaHideoutBe, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_SlateportHint1::
	msgbox AquaHideout_1F_Text_BossWentToJackASubmarine, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_HideoutEntranceGrunt2::
	lock
	faceplayer
	goto_if_set FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT, AquaHideout_1F_EventScript_SlateportHint2
	goto_if_set FLAG_RECEIVED_RED_OR_BLUE_ORB, AquaHideout_1F_EventScript_MagmaHideoutHint2
	msgbox AquaHideout_1F_Text_BossIsOnRoute122, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_MagmaHideoutHint2::
	msgbox AquaHideout_1F_Text_TeamMagmaAtMtChimney, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_SlateportHint2::
	msgbox AquaHideout_1F_Text_BossIsInSlateportCity, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_1, AquaHideout_1F_Text_Grunt1Intro, AquaHideout_1F_Text_Grunt1Defeat, AquaHideout_1F_EventScript_Grunt1Defeated
	msgbox AquaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_1F_EventScript_Grunt1Defeated::
	msgbox AquaHideout_1F_Text_Grunt1PostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_Text_OurBossIsSnatchingSomething:
	.string "Hey! Halt! What are YOU doing in\n"
	.string "the RED RIBBON ARMY's SEA BASE?\p"
	.string "DR. GERO isn't here! He went off to\n"
	.string "snatch something important!\p"
	.string "… …\n"
	.string "Where did he go?\p"
	.string "Ha! You think I'd salute and then spill\n"
	.string "the plan? Not a chance!$"

AquaHideout_1F_Text_WhereMightMagmaHideoutBe:
	.string "What? What?\n"
	.string "Are you from the LAND squad?\p"
	.string "I hear the LAND squad is trying to wake\n"
	.string "some monster POKéMON at their own\l"
	.string "BASE.\p"
	.string "But where in the world is that BASE\n"
	.string "hiding?$"

AquaHideout_1F_Text_BossWentToJackASubmarine:
	.string "Hey! Halt! What are YOU doing in\n"
	.string "the RED RIBBON ARMY's SEA BASE?\p"
	.string "DR. GERO isn't here! He went off to\n"
	.string "hijack a submarine!\p"
	.string "… …\n"
	.string "Where did he go?\p"
	.string "Ha! You think I'd salute and then spill\n"
	.string "the plan? Not a chance!$"

AquaHideout_1F_Text_BossIsOnRoute122:
	.string "Hey! Halt! What are YOU doing in\n"
	.string "the RED RIBBON ARMY's SEA BASE?\p"
	.string "DR. GERO isn't here! He's headed for MT.\n"
	.string "PYRE on ROUTE 122!\p"
	.string "… …\n"
	.string "Why did he go?\p"
	.string "Ha! You think I'd salute and then spill\n"
	.string "the plan? Not a chance!$"

AquaHideout_1F_Text_TeamMagmaAtMtChimney:
	.string "What? What?\n"
	.string "Are you from the LAND squad?\p"
	.string "I hear the LAND squad is after a\n"
	.string "monster POKéMON up at MT. CHIMNEY.\p"
	.string "But what is that thing even like?$"

AquaHideout_1F_Text_BossIsInSlateportCity:
	.string "Hey! Halt! What are YOU doing in\n"
	.string "the RED RIBBON ARMY's SEA BASE?\p"
	.string "DR. GERO isn't here! He's on his way to\n"
	.string "SLATEPORT CITY!\p"
	.string "… …\n"
	.string "Why did he go?\p"
	.string "Ha! You think I'd salute and then spill\n"
	.string "the plan? Not a chance!$"

AquaHideout_1F_Text_Grunt1Intro:
	.string "Yikes! An intruder!\n"
	.string "Sound the alarm! Salute! Fight!$"

AquaHideout_1F_Text_Grunt1Defeat:
	.string "Gaaah…\n"
	.string "I got flattened!$"

AquaHideout_1F_Text_Grunt1PostBattle:
	.string "I took the fall for the ARMY, but I did\n"
	.string "my duty…\p"
	.string "Please don't tell KID BUU!$"

