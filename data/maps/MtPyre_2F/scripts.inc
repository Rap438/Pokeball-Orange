MtPyre_2F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveHole_CheckFallDownHole
	map_script MAP_SCRIPT_ON_TRANSITION, CaveHole_FixCrackedGround
	map_script MAP_SCRIPT_ON_RESUME, MtPyre_2F_SetHoleWarp
	.byte 0

MtPyre_2F_SetHoleWarp:
	setstepcallback STEP_CB_CRACKED_FLOOR
	setholewarp MAP_MT_PYRE_1F
	end

MtPyre_2F_EventScript_Woman::
	msgbox MtPyre_2F_Text_MemoriesOfSkitty, MSGBOX_NPC
	end

MtPyre_2F_EventScript_PokefanM::
	msgbox MtPyre_2F_Text_TumbledFromFloorAbove, MSGBOX_NPC
	end

MtPyre_2F_EventScript_Mark::
	trainerbattle_single TRAINER_MARK, MtPyre_2F_Text_MarkIntro, MtPyre_2F_Text_MarkDefeat
	msgbox MtPyre_2F_Text_MarkPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Luke::
	trainerbattle_double TRAINER_DEZ_AND_LUKE, MtPyre_2F_Text_LukeIntro, MtPyre_2F_Text_LukeDefeat, MtPyre_2F_Text_LukeNotEnoughMons
	msgbox MtPyre_2F_Text_LukePostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Dez::
	trainerbattle_double TRAINER_DEZ_AND_LUKE, MtPyre_2F_Text_DezIntro, MtPyre_2F_Text_DezDefeat, MtPyre_2F_Text_DezNotEnoughMons
	msgbox MtPyre_2F_Text_DezPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Leah::
	trainerbattle_single TRAINER_LEAH, MtPyre_2F_Text_LeahIntro, MtPyre_2F_Text_LeahDefeat
	msgbox MtPyre_2F_Text_LeahPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Zander::
	trainerbattle_single TRAINER_ZANDER, MtPyre_2F_Text_ZanderIntro, MtPyre_2F_Text_ZanderDefeat
	msgbox MtPyre_2F_Text_ZanderPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_Text_MemoriesOfSkitty:
	.string "I keep thinking of my darling SKITTY…\n"
	.string "The tears just won't stop.$"

MtPyre_2F_Text_TumbledFromFloorAbove:
	.string "Ooch, ouch… There are holes in the floor\n"
	.string "here and there.\p"
	.string "I didn't notice, and I took a tumble\n"
	.string "from the floor above.$"

MtPyre_2F_Text_MarkIntro:
	.string "Hey! Are you hunting for POKéMON, too?\n"
	.string "You tailed me up here! That's rude!$"

MtPyre_2F_Text_MarkDefeat:
	.string "Yikes!\n"
	.string "Sorry, sorry, please forgive me!$"

MtPyre_2F_Text_MarkPostBattle:
	.string "Hardly anyone climbs this mountain, so I\n"
	.string "figured the POKéMON would be rare.$"

MtPyre_2F_Text_LukeIntro:
	.string "LUKE: We came up here on a dare.\p"
	.string "Heheh, if I show her how cool I am, she'll\n"
	.string "fall for me. I know it!\p"
	.string "I know! I'll flatten you with my power\n"
	.string "level and show her how cool I am!$"

MtPyre_2F_Text_LukeDefeat:
	.string "LUKE: Whoa, whoopsie!$"

MtPyre_2F_Text_LukePostBattle:
	.string "LUKE: We lost, but that's okay! I'm right\n"
	.string "here by your side.\p"
	.string "We'll finish this dare together!$"

MtPyre_2F_Text_LukeNotEnoughMons:
	.string "LUKE: Want to fight me? Come back with\n"
	.string "more POKéMON.\p"
	.string "One won't do. I can't show my girl how\n"
	.string "cool I am against someone who only\l"
	.string "brought one!$"

MtPyre_2F_Text_DezIntro:
	.string "DEZ: We came up here on a dare, my\n"
	.string "boyfriend and me.\p"
	.string "It's really scary, but I'm with my\n"
	.string "boyfriend, so it's okay.\p"
	.string "I know! I'll beat you so he looks extra\n"
	.string "cool!$"

MtPyre_2F_Text_DezDefeat:
	.string "DEZ: Waaaah! I'm scared!$"

MtPyre_2F_Text_DezPostBattle:
	.string "DEZ: We're so lovey-dovey, we don't even\n"
	.string "care if we lose!$"

MtPyre_2F_Text_DezNotEnoughMons:
	.string "DEZ: If you want to challenge us, bring\n"
	.string "at least two POKéMON.\p"
	.string "My boyfriend's strong. Just one\n"
	.string "POKéMON won't do at all.$"

MtPyre_2F_Text_LeahIntro:
	.string "A stranger… Depart this slope before\n"
	.string "something befalls you!$"

MtPyre_2F_Text_LeahDefeat:
	.string "Hmm…\n"
	.string "You are tough.$"

MtPyre_2F_Text_LeahPostBattle:
	.string "My family has been guarding this\n"
	.string "mountain since my great-grandmother's\l"
	.string "time…\p"
	.string "It is my duty to protect the spirits who\n"
	.string "rest here…$"

MtPyre_2F_Text_ZanderIntro:
	.string "Kyaaaah!\n"
	.string "I'm so scared!$"

MtPyre_2F_Text_ZanderDefeat:
	.string "Noooooo!\n"
	.string "I lost my nerve!$"

MtPyre_2F_Text_ZanderPostBattle:
	.string "I get freaked out every time I see\n"
	.string "anything move…\p"
	.string "I shouldn't have come up here to train…$"

