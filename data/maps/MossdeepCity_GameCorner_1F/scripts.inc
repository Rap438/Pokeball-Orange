MossdeepCity_GameCorner_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MossdeepCity_GameCorner_1F_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, MossdeepCity_GameCorner_1F_OnWarp
	map_script MAP_SCRIPT_ON_LOAD, CableClub_OnLoad
	.byte 0

MossdeepCity_GameCorner_1F_OnWarp:
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, CableClub_EventScript_CheckTurnAttendant
	.2byte 0

MossdeepCity_GameCorner_1F_OnFrame:
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, CableClub_EventScript_ExitMinigameRoom
	.2byte 0

@ Script is redundant, the label in the goto also does lock and faceplayer
MossdeepCity_GameCorner_1F_EventScript_InfoMan::
	lock
	faceplayer
	goto MossdeepCity_GameCorner_1F_EventScript_InfoMan2
	release
	end

@ Script is redundant, the label in the goto also does lock and faceplayer
MossdeepCity_GameCorner_1F_EventScript_OldMan::
	lock
	faceplayer
	goto MossdeepCity_GameCorner_1F_EventScript_OldMan2
	release
	end


@ Everything below is leftover from RS
@ The house that was here in RS was moved to Sootopolis and replaced by the Game Corner
@ The text below is unused and duplicated in its replacement in Sootopolis City
@ And the BG Door event was moved inaccessibly into a wall

RS_MysteryEventsHouse_EventScript_Door::
	msgbox RS_MysteryEventsHouse_Text_DoorIsLocked, MSGBOX_SIGN
	end

RS_MysteryEventsHouse_Text_OldManGreeting:
	.string "When I was young, I roamed the world as\n"
	.string "a POKéMON TRAINER and fought in the\l"
	.string "WORLD MARTIAL ARTS TOURNAMENT.\p"
	.string "Now that I'm an old buzzard, my only joy\n"
	.string "is watching young TRAINERS go at it.$"

RS_MysteryEventsHouse_Text_DoorIsLocked:
	.string "The door won't budge. It's locked.$"

RS_MysteryEventsHouse_Text_ChallengeVisitingTrainer:
	.string "A TRAINER named {STR_VAR_1} is visiting my\n"
	.string "home.\p"
	.string "Would you like to challenge {STR_VAR_1}?$"

RS_MysteryEventsHouse_Text_YouWontBattle:
	.string "You won't fight? What a letdown. I was\n"
	.string "hoping to see a battle…$"

RS_MysteryEventsHouse_Text_KeepItToA3On3:
	.string "Oh, wonderful!\p"
	.string "But my house isn't very sturdy.\p"
	.string "Could I ask you to keep it to a 3-on-3\n"
	.string "match? And no giant ki blasts, please.$"

RS_MysteryEventsHouse_Text_SaveYourProgress:
	.string "Before you two fight, you should save\n"
	.string "your progress.$"

RS_MysteryEventsHouse_Text_HopeToSeeAGoodMatch:
	.string "I hope to see a great match!$"

RS_MysteryEventsHouse_Text_BattleTie:
	.string "So it ended in a standoff.\p"
	.string "A brilliant match where neither side\n"
	.string "gave an inch!$"

RS_MysteryEventsHouse_Text_BattleWon:
	.string "Magnificent!\p"
	.string "Why, it was like watching myself in my\n"
	.string "youth again!$"

RS_MysteryEventsHouse_Text_BattleLost:
	.string "Ah, tough luck!\p"
	.string "But it was a fine match. Keep training\n"
	.string "and you'll win next time.$"
