DewfordTown_House1_MapScripts::
	.byte 0

DewfordTown_House1_EventScript_Man::
	msgbox DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Woman::
	msgbox DewfordTown_House1_Text_LifeGoesSlowlyOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox DewfordTown_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland:
	.string "There's a lot to be said for a small\n"
	.string "island like this, living in peace with\l"
	.string "POKéMON and family.$"

DewfordTown_House1_Text_LifeGoesSlowlyOnIsland:
	.string "I left the big port of SLATEPORT CITY\n"
	.string "when I married my husband here.\p"
	.string "Life moves slowly on this little island.\n"
	.string "But with the beautiful sea all around,\l"
	.string "that's a kind of happiness, too.$"

DewfordTown_House1_Text_Zigzagoon:
	.string "ZIGZAGOON: Gugu, guguuh!$"

