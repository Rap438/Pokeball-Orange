EverGrandeCity_ChampionsRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_ChampionsRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, EverGrandeCity_ChampionsRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, EverGrandeCity_ChampionsRoom_OnFrame
	.byte 0

EverGrandeCity_ChampionsRoom_OnTransition:
	call Common_EventScript_SetupRivalGfxId
	end

EverGrandeCity_ChampionsRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_ChampionsRoom_EventScript_PlayerTurnNorth
	.2byte 0

EverGrandeCity_ChampionsRoom_EventScript_PlayerTurnNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

EverGrandeCity_ChampionsRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_ChampionsRoom_EventScript_EnterRoom
	.2byte 0

EverGrandeCity_ChampionsRoom_EventScript_EnterRoom::
	lockall
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkUp4
	delay 40
	applywaitmovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerApproachWallace
	setvar VAR_TEMP_1, 1
	goto EverGrandeCity_ChampionsRoom_EventScript_Wallace
	releaseall
	end

EverGrandeCity_ChampionsRoom_Movement_PlayerApproachWallace:
	walk_up
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_EventScript_Wallace::
	playbgm MUS_ENCOUNTER_CHAMPION, FALSE
	msgbox EverGrandeCity_ChampionsRoom_Text_IntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLACE, EverGrandeCity_ChampionsRoom_Text_Defeat
	goto EverGrandeCity_ChampionsRoom_EventScript_Defeated
	end

EverGrandeCity_ChampionsRoom_EventScript_Defeated::
	playse SE_DOOR
	setmetatile 6, 1, METATILE_EliteFour_OpenDoorChampion_Frame, FALSE
	setmetatile 6, 2, METATILE_EliteFour_OpenDoorChampion_Opening, FALSE
	special DrawWholeMapView
	msgbox EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	closemessage
	playse SE_DOOR
	checkplayergender
	call_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_PlayMayMusic
	call_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_PlayBrendanMusic
	addobject LOCALID_CHAMPIONS_ROOM_RIVAL
	call EverGrandeCity_ChampionsRoom_EventScript_RivalApproachPlayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_MayAdvice
	goto_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_BrendanAdvice
	end

EverGrandeCity_ChampionsRoom_EventScript_PlayMayMusic::
	playbgm MUS_ENCOUNTER_MAY, FALSE
	return

EverGrandeCity_ChampionsRoom_EventScript_PlayBrendanMusic::
	playbgm MUS_ENCOUNTER_BRENDAN, FALSE
	return

EverGrandeCity_ChampionsRoom_EventScript_MayAdvice::
	msgbox EverGrandeCity_ChampionsRoom_Text_MayAdvice, MSGBOX_DEFAULT
	delay 40
	playse SE_PIN
	applywaitmovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_Delay48
	call EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth
	msgbox EverGrandeCity_ChampionsRoom_Text_MayItsAlreadyOver, MSGBOX_DEFAULT
	goto EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF
	end

EverGrandeCity_ChampionsRoom_EventScript_BrendanAdvice::
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanAdvice, MSGBOX_DEFAULT
	delay 40
	playse SE_PIN
	applywaitmovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_Delay48
	call EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanYouveWon, MSGBOX_DEFAULT
	goto EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF
	end

EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF::
	closemessage
	addobject LOCALID_CHAMPIONS_ROOM_BIRCH
	applywaitmovement LOCALID_CHAMPIONS_ROOM_BIRCH, EverGrandeCity_ChampionsRoom_Movement_BirchArrives
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	msgbox EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex, MSGBOX_DEFAULT
	call ProfBirch_EventScript_RatePokedex
	msgbox EverGrandeCity_ChampionsRoom_Text_BirchCongratulations, MSGBOX_DEFAULT
	applywaitmovement LOCALID_CHAMPIONS_ROOM_WALLACE, Common_Movement_WalkInPlaceFasterUp
	delay 20
	applywaitmovement LOCALID_CHAMPIONS_ROOM_WALLACE, Common_Movement_WalkInPlaceFasterDown
	msgbox EverGrandeCity_ChampionsRoom_Text_WallaceComeWithMe, MSGBOX_DEFAULT
	closemessage
	delay 30
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExitStart
	applymovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExitStart
	applymovement LOCALID_CHAMPIONS_ROOM_BIRCH, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalFollows
	waitmovement 0
	delay 20
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	msgbox EverGrandeCity_ChampionsRoom_Text_WallaceWaitOutside, MSGBOX_DEFAULT
	checkplayergender
	call_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_MayCongratulations
	call_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_BrendanCongratulations
	closemessage
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExit
	applymovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExit
	waitmovement 0
	setflag FLAG_HIDE_PETALBURG_GYM_GREETER
	warp MAP_EVER_GRANDE_CITY_HALL_OF_FAME, 7, 16
	waitstate
	releaseall
	end

EverGrandeCity_ChampionsRoom_EventScript_MayCongratulations::
	msgbox EverGrandeCity_ChampionsRoom_Text_MayCongratulations, MSGBOX_DEFAULT
	return

EverGrandeCity_ChampionsRoom_EventScript_BrendanCongratulations::
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanCongratulations, MSGBOX_DEFAULT
	return

EverGrandeCity_ChampionsRoom_EventScript_RivalApproachPlayer::
	applywaitmovement LOCALID_CHAMPIONS_ROOM_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalApproachPlayer
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	return

EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth::
	applywaitmovement LOCALID_CHAMPIONS_ROOM_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalLookBackAndForth
	return

EverGrandeCity_ChampionsRoom_Movement_PlayerExitStart:
	walk_up
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_Movement_PlayerExit:
	walk_up
	delay_16
	walk_up
	delay_8
	set_invisible
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalApproachPlayer:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_in_place_faster_right
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalLookBackAndForth:
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_up
	delay_8
	walk_in_place_faster_right
	delay_16
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalFollows:
	walk_right
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_Movement_WallaceExitStart:
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

EverGrandeCity_ChampionsRoom_Movement_WallaceExit:
	walk_up
	delay_8
	set_invisible
	step_end

EverGrandeCity_ChampionsRoom_Movement_BirchArrives:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_in_place_faster_left
	step_end

EverGrandeCity_ChampionsRoom_Text_IntroSpeech:
	.string "MR. SATAN: Wahahaha! Welcome, {PLAYER}{KUN}!\p"
	.string "I am MR. SATAN, the World Champion, and\n"
	.string "the CHAMPION of the POKéMON LEAGUE!\p"
	.string "That incident in SOOTOPOLIS CITY… I\n"
	.string "heard you ended that crisis all by\l"
	.string "yourself!\p"
	.string "Ahem! Well, that's what I would have\n"
	.string "done. But the truth is you didn't do it\l"
	.string "alone.\p"
	.string "You pulled through by fighting as one\n"
	.string "with your POKéMON.\p"
	.string "We TRAINERS raise POKéMON with items\n"
	.string "and new techniques, but we learn plenty\l"
	.string "from them, too.\p"
	.string "And this is where your power gets put\n"
	.string "to the test against everyone who has\l"
	.string "ever come before you.\p"
	.string "Now!\p"
	.string "Who in HOENN can fight most brilliantly\n"
	.string "side by side with their POKéMON?\p"
	.string "Show me right here and right now, with\n"
	.string "everything you've got!$"

EverGrandeCity_ChampionsRoom_Text_Defeat:
	.string "The World Champion… falls?!\p"
	.string "That was incredible! Not that I was\n"
	.string "worried, mind you. I may have gone easy\l"
	.string "on you a little.\p"
	.string "…No. I won't lie to a fighter like you.\n"
	.string "You were magnificent!\p"
	.string "Kudos to you, {PLAYER}{KUN}!\p"
	.string "You are a true warrior and a true\n"
	.string "POKéMON TRAINER!$"

EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech:
	.string "MR. SATAN: The POKéMON you brought into\n"
	.string "battle…\p"
	.string "One moment they flowed like water, the\n"
	.string "next they struck like lightning.\p"
	.string "The way you led them, and fought right\n"
	.string "beside them…\p"
	.string "Even I, the World Champion, was dazzled\n"
	.string "by that performance!\p"
	.string "You now stand on the summit of the\n"
	.string "POKéMON LEAGUE!\p"
	.string "And so I hereby proclaim you the new\n"
	.string "HOENN region…$"

EverGrandeCity_ChampionsRoom_Text_MayAdvice:
	.string "VEGETA: Kakarot{KUN}!\p"
	.string "Before you take on the CHAMPION, listen\n"
	.string "up. I've studied that loudmouth's\l"
	.string "fighting style…$"

EverGrandeCity_ChampionsRoom_Text_MayItsAlreadyOver:
	.string "VEGETA: Huh?\n"
	.string "What?! What is this?!\p"
	.string "Kakarot{KUN}…\n"
	.string "Don't tell me…\l"
	.string "It's already over?!$"

EverGrandeCity_ChampionsRoom_Text_BrendanAdvice:
	.string "VEGETA: Kakarot!\p"
	.string "Before you challenge the CHAMPION, how\n"
	.string "about a little advice from the prince of\l"
	.string "all Saiyans?…$"

EverGrandeCity_ChampionsRoom_Text_BrendanYouveWon:
	.string "VEGETA: Kakarot…\p"
	.string "Are you telling me…\n"
	.string "You already beat the CHAMPION?!$"

EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex:
	.string "MASTER ROSHI: See?\n"
	.string "What did I tell you, {RIVAL}?\p"
	.string "Didn't I say you needn't worry about\n"
	.string "{PLAYER}{KUN}? Hohoho!\p"
	.string "… … … … … … … …\n"
	.string "{PLAYER}{KUN}, you've finally done it.\p"
	.string "When I heard you beat your old friend\n"
	.string "KRILLIN at the PETALBURG GYM, I\l"
	.string "thought you might have a chance…\p"
	.string "But to think you've actually become the\n"
	.string "CHAMPION!\p"
	.string "Ah, yes!\p"
	.string "How is your POKéDEX coming along?\n"
	.string "Here, let me take a look.$"

EverGrandeCity_ChampionsRoom_Text_BirchCongratulations:
	.string "MASTER ROSHI: Anyway, congratulations!\p"
	.string "Now go on, hold your head high and step\n"
	.string "into the final room!$"

EverGrandeCity_ChampionsRoom_Text_WallaceComeWithMe:
	.string "MR. SATAN: {PLAYER}{KUN}…\p"
	.string "No, no, let me say that properly.\p"
	.string "Behold, the new CHAMPION!\p"
	.string "Come with me, champ!$"

EverGrandeCity_ChampionsRoom_Text_WallaceWaitOutside:
	.string "MR. SATAN: I'm sorry, but…\p"
	.string "From here on, only those who have\n"
	.string "become CHAMPIONS may enter.\p"
	.string "League rules! Not even the World\n"
	.string "Champion's best friends can sneak in.\p"
	.string "You'll have to wait out here with MASTER\n"
	.string "ROSHI.$"

EverGrandeCity_ChampionsRoom_Text_MayCongratulations:
	.string "VEGETA: Grrr…\p"
	.string "… … … … … … … …\n"
	.string "… … … … … … … …\p"
	.string "Tch. Fine. Rules are rules.\p"
	.string "Kakarot{KUN}! You're the strongest in HOENN\n"
	.string "now. Enjoy it while it lasts, because I'm\l"
	.string "coming for that title next!\p"
	.string "…Congratulations. Don't let it go to\n"
	.string "your head.$"

EverGrandeCity_ChampionsRoom_Text_BrendanCongratulations:
	.string "VEGETA: What?! … … … … …\n"
	.string "… … … … … … … …\p"
	.string "Hmph. If that's the rule, there's\n"
	.string "nothing I can do about it.\p"
	.string "Kakarot, good work! Congratulations!\n"
	.string "Now I have something to surpass.$"

