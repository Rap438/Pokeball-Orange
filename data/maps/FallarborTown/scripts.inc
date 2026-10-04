FallarborTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FallarborTown_OnTransition
	.byte 0

FallarborTown_OnTransition:
	setflag FLAG_VISITED_FALLARBOR_TOWN
	setvar VAR_CONTEST_HALL_STATE, 0
	clearflag FLAG_CONTEST_SKETCH_CREATED
	end

FallarborTown_EventScript_ExpertM::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, FallarborTown_EventScript_ExpertMNormal
	msgbox FallarborTown_Text_ShadyCharactersCozmosHome, MSGBOX_DEFAULT
	release
	end

FallarborTown_EventScript_ExpertMNormal::
	msgbox FallarborTown_Text_RegionKnownForMeteors, MSGBOX_DEFAULT
	release
	end

FallarborTown_EventScript_Girl::
	msgbox FallarborTown_Text_MyPreciousAzurill, MSGBOX_NPC
	end

FallarborTown_EventScript_Gentleman::
	msgbox FallarborTown_Text_HaveYouChallengedFlannery, MSGBOX_NPC
	end

FallarborTown_EventScript_Azurill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZURILL, CRY_MODE_NORMAL
	msgbox FallarborTown_Text_Azurill, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FallarborTown_EventScript_BattleTentSign::
	msgbox FallarborTown_Text_BattleTentSign, MSGBOX_SIGN
	end

FallarborTown_EventScript_TownSign::
	msgbox FallarborTown_Text_TownSign, MSGBOX_SIGN
	end

FallarborTown_EventScript_MoveTutorSign::
	msgbox FallarborTown_Text_MoveTutorSign, MSGBOX_SIGN
	end

FallarborTown_Text_ShadyCharactersCozmosHome:
	.string "Something's off around here, and I\n"
	.string "don't like it!\p"
	.string "I keep seeing guys in red armbands\n"
	.string "marching in and out of PROF. COZMO's\l"
	.string "home. They salute each other like it's a\l"
	.string "parade…$"

FallarborTown_Text_RegionKnownForMeteors:
	.string "This region's been famous for meteors\n"
	.string "since the old days.\p"
	.string "They say METEOR FALLS was gouged out\n"
	.string "by a falling meteorite long ago.\p"
	.string "My grandpa swears it lit up the sky\n"
	.string "brighter than a KAMEHAMEHA.$"

FallarborTown_Text_MyPreciousAzurill:
	.string "Whoa, look, look!\n"
	.string "This is my precious AZURILL!\p"
	.string "It's smooth, squishy, and bounces like a\n"
	.string "rubber ball!$"

FallarborTown_Text_Azurill:
	.string "AZURILL: Roo-ree-looo!$"

FallarborTown_Text_HaveYouChallengedFlannery:
	.string "Have you challenged VIDEL, the LEADER\n"
	.string "of LAVARIDGE GYM yet?\p"
	.string "Her father is MR. SATAN, the loud and\n"
	.string "famous World Champion. Everybody knows\l"
	.string "his name.\p"
	.string "But that girl fights on her own\n"
	.string "strength. I wouldn't be surprised if she\l"
	.string "became a great TRAINER in her own\l"
	.string "right.$"

FallarborTown_Text_BattleTentSign:
	.string "BATTLE TENT FALLARBOR SITE “Bring\n"
	.string "Your Best Team and Your Best Spirit!”$"

FallarborTown_Text_TownSign:
	.string "FALLARBOR TOWN “A quiet farm town of\n"
	.string "small gardens and big appetites.”$"

FallarborTown_Text_MoveTutorSign:
	.string "MOVE TUTOR'S HOUSE\n"
	.string "“New techniques taught to POKéMON.”$"

