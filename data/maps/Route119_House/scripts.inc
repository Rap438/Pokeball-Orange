Route119_House_MapScripts::
	.byte 0

Route119_House_EventScript_Woman::
	msgbox Route119_House_Text_RumorAboutCaveOfOrigin, MSGBOX_NPC
	end

Route119_House_EventScript_Wingull::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_WINGULL, CRY_MODE_NORMAL
	msgbox Route119_House_Text_Wingull, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route119_House_Text_RumorAboutCaveOfOrigin:
	.string "I heard about a place called the CAVE\n"
	.string "OF ORIGIN.\p"
	.string "Rumor has it the spirits of POKéMON can\n"
	.string "be revived there, like a wish granted by\l"
	.string "SHENRON. Could something like that\l"
	.string "really happen?$"

Route119_House_Text_Wingull:
	.string "WINGULL: Pihyoh!$"

