PacifidlogTown_House1_MapScripts::
	.byte 0

PacifidlogTown_House1_EventScript_Man::
	msgbox PacifidlogTown_House1_Text_RegiStory, MSGBOX_NPC
	end

PacifidlogTown_House1_EventScript_Woman::
	msgbox PacifidlogTown_House1_Text_SixDotsOpenThreeDoors, MSGBOX_NPC
	end

PacifidlogTown_House1_Text_RegiStory:
	.string "Here in HOENN, there are three POKéMON\n"
	.string "that stand for the power of rock, ice,\l"
	.string "and steel.\p"
	.string "Each of them is said to hide deep inside\n"
	.string "a cave.\p"
	.string "That's the tale an old hermit told me\n"
	.string "when I was just a kid.$"

PacifidlogTown_House1_Text_SixDotsOpenThreeDoors:
	.string "“Six dots open three doors.”\p"
	.string "Grandpa used to say that, but he never\n"
	.string "told me what it meant.$"

