FortreeCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_Gym_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, FortreeCity_Gym_OnWarp
	.byte 0

FortreeCity_Gym_OnTransition:
	special RotatingGate_InitPuzzle
	setflag FLAG_TEMP_HIDE_FOLLOWER @ Hide follower bc of rotating gates
	end

FortreeCity_Gym_OnWarp:
	map_script_2 VAR_TEMP_0, VAR_TEMP_0, FortreeCity_Gym_EventScript_InitRotatingGates
	.2byte 0

@ NOTE: This rotating gate puzzle makes use of VAR_TEMP_0 - VAR_TEMP_3
FortreeCity_Gym_EventScript_InitRotatingGates::
	special RotatingGate_InitPuzzleAndGraphics
	end

FortreeCity_Gym_EventScript_Winona::
	trainerbattle_single TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaIntro, FortreeCity_Gym_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, FortreeCity_Gym_EventScript_WinonaRematch
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, FortreeCity_Gym_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeated::
	message FortreeCity_Gym_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FORTREE_GYM
	setflag FLAG_BADGE06_GET
	special DBZ_QueueGymFight
	setvar VAR_0x8008, 6
	call Common_EventScript_SetGymTrainers
	call FortreeCity_Gym_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox FortreeCity_Gym_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce2::
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce::
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	return

FortreeCity_Gym_EventScript_WinonaRematch::
	trainerbattle_rematch_double TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaPreRematch, FortreeCity_Gym_Text_WinonaRematchDefeat, FortreeCity_Gym_Text_WinonaRematchNeedTwoMons
	msgbox FortreeCity_Gym_Text_WinonaPostRematch, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Jared::
	trainerbattle_single TRAINER_JARED, FortreeCity_Gym_Text_JaredIntro, FortreeCity_Gym_Text_JaredDefeat
	msgbox FortreeCity_Gym_Text_JaredPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Edwardo::
	trainerbattle_single TRAINER_EDWARDO, FortreeCity_Gym_Text_EdwardoIntro, FortreeCity_Gym_Text_EdwardoDefeat
	msgbox FortreeCity_Gym_Text_EdwardoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Flint::
	trainerbattle_single TRAINER_FLINT, FortreeCity_Gym_Text_FlintIntro, FortreeCity_Gym_Text_FlintDefeat
	msgbox FortreeCity_Gym_Text_FlintPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Ashley::
	trainerbattle_single TRAINER_ASHLEY, FortreeCity_Gym_Text_AshleyIntro, FortreeCity_Gym_Text_AshleyDefeat
	msgbox FortreeCity_Gym_Text_AshleyPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Humberto::
	trainerbattle_single TRAINER_HUMBERTO, FortreeCity_Gym_Text_HumbertoIntro, FortreeCity_Gym_Text_HumbertoDefeat
	msgbox FortreeCity_Gym_Text_HumbertoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Darius::
	trainerbattle_single TRAINER_DARIUS, FortreeCity_Gym_Text_DariusIntro, FortreeCity_Gym_Text_DariusDefeat
	msgbox FortreeCity_Gym_Text_DariusPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_FORTREE_GYM, FortreeCity_Gym_EventScript_GymGuidePostVictory
	msgbox FortreeCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_GymGuidePostVictory::
	msgbox FortreeCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_GymStatueCertified::
	msgbox FortreeCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_EventScript_GymStatue::
	msgbox FortreeCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_Text_GymGuideAdvice:
	.string "Yo, {PLAYER}! Your power level keeps on\n"
	.string "rising, CHAMPION-bound!\p"
	.string "FORTREE GYM LEADER GREAT SAIYAMAN is a\n"
	.string "master of FLYING-type POKéMON.\p"
	.string "He's waiting at the back of this GYM,\n"
	.string "behind the rotating doors.\p"
	.string "He's looking for new challengers with\n"
	.string "the nerve to take wing!\p"
	.string "Okay, go for it!$"

FortreeCity_Gym_Text_GymGuidePostVictory:
	.string "You did it, {PLAYER}!\n"
	.string "You took off like a rocket!$"

FortreeCity_Gym_Text_JaredIntro:
	.string "Strike a pose! BIRD POKéMON fight with\n"
	.string "style, and so do I!$"

FortreeCity_Gym_Text_JaredDefeat:
	.string "Whoa…\n"
	.string "Your power level is high…$"

FortreeCity_Gym_Text_JaredPostBattle:
	.string "A fighter needs a quick head to keep up\n"
	.string "when a battle takes a sudden turn.\p"
	.string "Are you sharp enough to reach our\n"
	.string "LEADER?$"

FortreeCity_Gym_Text_EdwardoIntro:
	.string "Everyone who loves BIRD POKéMON\n"
	.string "dreams of joining this GYM.\p"
	.string "As a student of the GREAT SAIYAMAN, I'm\n"
	.string "not allowed to lose!$"

FortreeCity_Gym_Text_EdwardoDefeat:
	.string "That was more power than I could\n"
	.string "handle…$"

FortreeCity_Gym_Text_EdwardoPostBattle:
	.string "The world is huge, and it's full of\n"
	.string "fighters stronger than me.\p"
	.string "I'll keep training until I'm even\n"
	.string "stronger!$"

FortreeCity_Gym_Text_FlintIntro:
	.string "No need to bother the GREAT SAIYAMAN\n"
	.string "with the likes of you!\p"
	.string "I'm plenty strong enough to handle you\n"
	.string "myself!$"

FortreeCity_Gym_Text_FlintDefeat:
	.string "GREAT SAIYAMAN, I…\n"
	.string "I let you down!$"

FortreeCity_Gym_Text_FlintPostBattle:
	.string "The GREAT SAIYAMAN is so cool, and so\n"
	.string "strong! He's the ultimate hero and\l"
	.string "LEADER!\p"
	.string "…I want a cape just like his.$"

FortreeCity_Gym_Text_AshleyIntro:
	.string "The GREAT SAIYAMAN trained me himself!\n"
	.string "You won't beat me easily!$"

FortreeCity_Gym_Text_AshleyDefeat:
	.string "Beaten… by someone stronger…$"

FortreeCity_Gym_Text_AshleyPostBattle:
	.string "Thanks to the GREAT SAIYAMAN, the\n"
	.string "people of FORTREE sleep soundly at\l"
	.string "night.$"

FortreeCity_Gym_Text_HumbertoIntro:
	.string "When the GREAT SAIYAMAN goes into\n"
	.string "battle, his visor gleams with the light\l"
	.string "of justice…\p"
	.string "I won't let you see that glorious sight\n"
	.string "until you get past me!$"

FortreeCity_Gym_Text_HumbertoDefeat:
	.string "Urgh!\n"
	.string "I couldn't hold you back.$"

FortreeCity_Gym_Text_HumbertoPostBattle:
	.string "Heads up! If you stare too long at the\n"
	.string "GREAT SAIYAMAN's poses, you'll lose\l"
	.string "focus and come crashing down in a heap!$"

FortreeCity_Gym_Text_DariusIntro:
	.string "Listen up! There are all kinds of\n"
	.string "FLYING-type POKéMON out there.\p"
	.string "You already know that, right? Right?!$"

FortreeCity_Gym_Text_DariusDefeat:
	.string "Hey, you really do know your stuff!$"

FortreeCity_Gym_Text_DariusPostBattle:
	.string "Fine, you beat me. But watch out! The\n"
	.string "GREAT SAIYAMAN's POKéMON mean\l"
	.string "business.$"

FortreeCity_Gym_Text_WinonaIntro:
	.string "Behold! I am the GREAT SAIYAMAN!\p"
	.string "Defender of justice and LEADER of the\n"
	.string "FORTREE POKéMON GYM!\p"
	.string "BIRD POKéMON and I have become one,\n"
	.string "and together we have soared the skies!\p"
	.string "No battle is too grueling for justice.\n"
	.string "Now witness our graceful, heroic\l"
	.string "choreography!\p"
	.string "SAIYAMAN POSE!$"

FortreeCity_Gym_Text_WinonaDefeat:
	.string "Incredible! Justice itself has been\n"
	.string "beaten…\p"
	.string "Never have I met a TRAINER with such\n"
	.string "power and grace.\p"
	.string "In honor of your strength, I present\n"
	.string "you with this GYM BADGE!$"

FortreeCity_Gym_Text_ReceivedFeatherBadge:
	.string "{PLAYER} received the FEATHER BADGE\n"
	.string "from the GREAT SAIYAMAN.$"

FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis:
	.string "The FEATHER BADGE means all POKéMON up\n"
	.string "to LV 70, even ones you got in trades,\l"
	.string "will obey your every command!\p"
	.string "It also lets you use the HM move FLY\n"
	.string "outside of battle.\p"
	.string "And take this, too. It's a gift from the\n"
	.string "hero himself!$"

FortreeCity_Gym_Text_ExplainAerialAce:
	.string "TM40 contains AERIAL ACE!\p"
	.string "It moves with the speed of justice. No\n"
	.string "POKéMON should be able to avoid it!\p"
	.string "… … … … … …$"

FortreeCity_Gym_Text_RegisteredWinona:
	.string "Registered GYM LEADER GREAT SAIYAMAN\n"
	.string "in the POKéNAV.$"

FortreeCity_Gym_Text_WinonaPostBattle:
	.string "I may have lost, but justice never loses\n"
	.string "heart!\p"
	.string "I will keep fighting alongside BIRD\n"
	.string "POKéMON!$"

FortreeCity_Gym_Text_GymStatue:
	.string "FORTREE CITY POKéMON GYM$"

FortreeCity_Gym_Text_GymStatueCertified:
	.string "FORTREE CITY POKéMON GYM\p"
	.string "GREAT SAIYAMAN'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

FortreeCity_Gym_Text_WinonaPreRematch:
	.string "GREAT SAIYAMAN: Gravity pulls on all of\n"
	.string "us, hero or not.\p"
	.string "But through training, we can rise above\n"
	.string "it!\p"
	.string "You've reached the very top as the\n"
	.string "reigning CHAMPION.\p"
	.string "Now let me see your power at full flight!$"

FortreeCity_Gym_Text_WinonaRematchDefeat:
	.string "I couldn't reach your height again…$"

FortreeCity_Gym_Text_WinonaPostRematch:
	.string "GREAT SAIYAMAN: I lost, but my wings are\n"
	.string "not broken. Justice never stays down!\p"
	.string "I'll rise and soar again, and again, and\n"
	.string "again!\p"
	.string "I'm absolutely sure of it!$"

FortreeCity_Gym_Text_WinonaRematchNeedTwoMons:
	.string "GREAT SAIYAMAN: Gravity pulls on all of\n"
	.string "us, hero or not.\p"
	.string "But through training, we can rise above\n"
	.string "it!\p"
	.string "You've reached the very top as the\n"
	.string "reigning CHAMPION.\p"
	.string "Now let me see your power at full flight!\p"
	.string "Oh, no… Only one POKéMON with you?\p"
	.string "Please come back with two POKéMON at\n"
	.string "the very least. Even a hero needs a\l"
	.string "partner!$"
