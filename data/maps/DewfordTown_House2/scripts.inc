DewfordTown_House2_MapScripts::
	.byte 0

DewfordTown_House2_EventScript_Man::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SILK_SCARF, DewfordTown_House2_EventScript_ExplainSilkScarf
	msgbox DewfordTown_House2_Text_WantYouToHaveSilkScarf, MSGBOX_DEFAULT
	giveitem ITEM_SILK_SCARF
	goto_if_eq VAR_RESULT, FALSE, DewfordTown_House2_EventScript_NoRoomForScarf
	setflag FLAG_RECEIVED_SILK_SCARF
	release
	end

DewfordTown_House2_EventScript_NoRoomForScarf::
	msgbox DewfordTown_House2_Text_NoRoom, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_ExplainSilkScarf::
	msgbox DewfordTown_House2_Text_ExplainSilkScarf, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_Boy::
	msgbox DewfordTown_House2_Text_BrawlySoCool, MSGBOX_NPC
	end

DewfordTown_House2_Text_WantYouToHaveSilkScarf:
	.string "Behold!\p"
	.string "This is a SILK SCARF, the cutting edge\n"
	.string "of island fashion, yeah!\p"
	.string "Oh, I can see your eyes sparkling!\n"
	.string "You appreciate my dazzling style!\p"
	.string "You're a delight!\n"
	.string "Here, I want you to have it!$"

DewfordTown_House2_Text_NoRoom:
	.string "Oh, you've got no room?\p"
	.string "Listen up, this SCARF is a must-have!\n"
	.string "I'd sell everything I own to get one!$"

DewfordTown_House2_Text_ExplainSilkScarf:
	.string "The SILK SCARF boosts the power of\n"
	.string "NORMAL-type moves.\p"
	.string "It's a marvelous SCARF that goes with\n"
	.string "almost any POKéMON!$"

DewfordTown_House2_Text_BrawlySoCool:
	.string "Wow, you crossed the whole sea just to\n"
	.string "visit DEWFORD?\p"
	.string "Did you come because you heard about\n"
	.string "TIEN?\p"
	.string "He's so cool, three eyes and all…\n"
	.string "Everyone looks up to him.$"

