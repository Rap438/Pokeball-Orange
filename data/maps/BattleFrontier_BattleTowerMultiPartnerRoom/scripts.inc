BattleFrontier_BattleTowerMultiPartnerRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_BattleTowerMultiPartnerRoom_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattleTowerMultiPartnerRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleFrontier_BattleTowerMultiPartnerRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattleTowerMultiPartnerRoom_OnFrame
	.byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_OnResume:
	pyramid_resetparty
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnTransition:
	goto_if_set FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ChosePartner
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_1
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_2
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_3
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_4
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_5
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_6
	setflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_ALT_1
	setflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_ALT_2
	tower_loadpartners
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ChosePartner::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_2, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_4, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_5, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_6, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_7, OBJ_EVENT_GFX_BOY_1
	setvar VAR_TEMP_1, 1
	setvar VAR_TEMP_3, 1
	setobjectxyperm LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, 10, 2
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnWarp:
	map_script_2 VAR_TEMP_3, 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TurnPlayerNorth
	.2byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterRoom
	map_script_2 VAR_TEMP_3, 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ExitRoom
	.2byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ExitRoom::
	lockall
	setvar VAR_TEMP_3, 0
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterElevator
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterRoom::
	lockall
	applywaitmovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterRoom
	applywaitmovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantBlockExit
	copyobjectxytoperm LOCALID_TOWER_PARTNER_ROOM_ATTENDANT
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner, MSGBOX_DEFAULT
	special HealPlayerParty
	setvar VAR_TEMP_1, 1
	releaseall
	end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterRoom:
	walk_down
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantBlockExit:
	walk_left
	walk_in_place_faster_down
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Attendant::
	lock
	faceplayer
	message BattleFrontier_BattleTowerMultiPartnerRoom_Text_QuitLookingForPartner
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_QuitChallenge
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner2, MSGBOX_DEFAULT
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_QuitChallenge::
	frontier_set FRONTIER_DATA_RECORD_DISABLED, TRUE
	goto BattleFrontier_BattleTower_EventScript_WarpToLobbyLost
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterElevator::
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_ThankYouForChoosingPartner, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, Common_Movement_WalkInPlaceFasterUp
	opendoor 10, 1
	waitdooranim
	call BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevator
	closedoor 10, 1
	waitdooranim
	warp MAP_BATTLE_FRONTIER_BATTLE_TOWER_ELEVATOR, 1, 6
	waitstate
	releaseall
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevator::
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevator
	waitmovement 0
	return

@ Unused
BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevatorEast:
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorEast
	waitmovement 0
	return

@ Unused
BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevatorWest:
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorWest
	waitmovement 0
	return

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevator:
	walk_up
	walk_up
	set_invisible
	step_end

@ Functionally unused
BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorEast:
	walk_right
	walk_up
	set_invisible
	step_end

@ Functionally unused
BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorWest:
	walk_left
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator:
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner1::
	lock
	faceplayer
	setvar VAR_TEMP_2, 0
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner::
	goto_if_set FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	tower_dopartnermsg PARTNER_MSGID_INTRO
	waitmessage
	waitbuttonpress
	tower_dopartnermsg PARTNER_MSGID_MON1
	waitmessage
	waitbuttonpress
	tower_dopartnermsg PARTNER_MSGID_MON2_ASK
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	switch VAR_RESULT
	case 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	case MULTI_B_PRESSED, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	tower_dopartnermsg PARTNER_MSGID_ACCEPT
	waitmessage
	waitbuttonpress
	closemessage
	call_if_ne VAR_FACING, DIR_SOUTH, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExit
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExitSouth
	removeobject VAR_LAST_TALKED
	setflag FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER
	warpsilent MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_PARTNER_ROOM, 10, 3
	waitstate
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner::
	tower_dopartnermsg PARTNER_MSGID_REJECT
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExit::
	applywaitmovement VAR_LAST_TALKED, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExit
	return

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExitSouth::
	applywaitmovement VAR_LAST_TALKED, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExitSouth
	return

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExit:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExitSouth:
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner2::
	lock
	faceplayer
	setvar VAR_TEMP_2, 1
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner3::
	lock
	faceplayer
	setvar VAR_TEMP_2, 2
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner4::
	lock
	faceplayer
	setvar VAR_TEMP_2, 3
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner5::
	lock
	faceplayer
	setvar VAR_TEMP_2, 4
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner6::
	lock
	faceplayer
	setvar VAR_TEMP_2, 5
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner7::
	lock
	faceplayer
	setvar VAR_TEMP_2, 6
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner8::
	lock
	faceplayer
	setvar VAR_TEMP_2, 7
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner:
	.string "Pick a partner from the fighters\n"
	.string "waiting in this room.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_QuitLookingForPartner:
	.string "{PLAYER}, your tag team still has no\n"
	.string "partner.\p"
	.string "Do you want to give up the search\n"
	.string "and go back to the reception counter?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner2:
	.string "Then go on, pick a partner from the\n"
	.string "fighters waiting in this room.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ThankYouForChoosingPartner::
	.string "A fine choice.\p"
	.string "Now I'll lead you both to your\n"
	.string "MULTI BATTLE ROOM.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Intro::
	.string "I'm the no. {STR_VAR_2} disciple of {STR_VAR_1}.\n"
	.string "Call me {STR_VAR_3}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon1::
	.string "Master {STR_VAR_1} advised me, so I\n"
	.string "brought one {STR_VAR_3} with {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Please, let me fight at your side\n"
	.string "in a tag team.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Accept::
	.string "Thank you!\n"
	.string "I'm registering us this second.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Reject::
	.string "I was dying to fight beside you,\n"
	.string "{PLAYER}…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Intro::
	.string "I study under {STR_VAR_1} as the no.\n"
	.string "{STR_VAR_2} disciple. My name is {STR_VAR_3}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon1::
	.string "Master {STR_VAR_1} coached me, and I\n"
	.string "picked one {STR_VAR_3} with {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Please, let's fight as a tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Accept::
	.string "Wonderful!\n"
	.string "Registration will take me no time!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Reject::
	.string "{PLAYER}, I'd hoped we could\n"
	.string "be partners…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Intro::
	.string "Um, I'm {STR_VAR_3}. I'm the no. {STR_VAR_2}\n"
	.string "disciple of {STR_VAR_1}.\p"
	.string "Sniffle…\p"
	.string "Sorry! The pressure's so heavy\n"
	.string "that I'm tearing up…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon1::
	.string "Master {STR_VAR_3} drilled me, and now I\n"
	.string "have one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Sniffle…\n"
	.string "Please, please be my partner!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Accept::
	.string "Y-you mean it? You'll team up?\n"
	.string "Awesome! Incredible! Awoooh!\p"
	.string "Oh… sorry…\n"
	.string "I'm so happy that I'm crying…\p"
	.string "I'll register us right away.\n"
	.string "Please stay right there!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Reject::
	.string "Oh, b-but…\n"
	.string "Boo-hoo… Waaaaah!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Intro::
	.string "Hey, hey! I'm {STR_VAR_3}!\n"
	.string "The no. {STR_VAR_2} disciple of {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon1::
	.string "Master {STR_VAR_3} helped me pick my crew.\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\n"
	.string "Pretty awesome, right?\p"
	.string "So come on!\n"
	.string "Be my tag-team partner!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Accept::
	.string "Woo-hoo! Yes!\n"
	.string "Give me a sec to register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Reject::
	.string "Aww, how come?\n"
	.string "I really wanted to team up, {PLAYER}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Intro::
	.string "I am {STR_VAR_3}, the no. {STR_VAR_2} disciple\n"
	.string "of the renowned {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon1::
	.string "I asked {STR_VAR_3} for guidance.\n"
	.string "One {STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon2Ask::
	.string "and one {STR_VAR_2} with {STR_VAR_1}\n"
	.string "make up my pair.\p"
	.string "Will you honor me by joining forces\n"
	.string "as a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Accept::
	.string "Thank you!\n"
	.string "Registering us right now!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Reject::
	.string "{PLAYER}, I'd so wanted to team\n"
	.string "up with you…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Intro::
	.string "Um… It means a lot that you stopped\n"
	.string "to talk to someone like me.\p"
	.string "I'm {STR_VAR_3}.\n"
	.string "The no. {STR_VAR_2} disciple of {STR_VAR_1}…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon1::
	.string "{STR_VAR_3} gave me so much good advice.\n"
	.string "I'm truly thankful.\p"
	.string "My team is one {STR_VAR_2}\n"
	.string "with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\n"
	.string "I know it's a lot to ask…\p"
	.string "You'll probably say no,\n"
	.string "but could I be your partner?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Accept::
	.string "Really?! I can't believe it!\n"
	.string "You're actually letting me join you!\l"
	.string "I… I won't disappoint you!\p"
	.string "Um… If it's truly all right, I'll go\n"
	.string "register us immediately!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Reject::
	.string "Oh…\n"
	.string "I guess I'm just not strong enough…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Intro::
	.string "Hi, I'm {STR_VAR_3}.\n"
	.string "Disciple no. {STR_VAR_2} under {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon1::
	.string "{STR_VAR_3} said this lineup would work:\l"
	.string "one {STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon2Ask::
	.string "and one {STR_VAR_2} that has mastered\n"
	.string "{STR_VAR_1}.\p"
	.string "Pretty good, right?\n"
	.string "So, want me on your team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Accept::
	.string "Fine, I'll join you!\n"
	.string "Try not to drag me down, okay?\l"
	.string "I'm off to register.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Reject::
	.string "Huh? You said no?\n"
	.string "You can't recognize talent!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Intro::
	.string "I'm {STR_VAR_3}, a TRIATHLETE.\n"
	.string "My days are packed with running,\l"
	.string "training, and rapping.\p"
	.string "Oh, and I'm also the no. {STR_VAR_2}\n"
	.string "disciple of {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon1::
	.string "{STR_VAR_3} drew up my lineup.\n"
	.string "One {STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon2Ask::
	.string "and one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2}.\p"
	.string "I put in the work raising them, even\n"
	.string "with my crazy schedule.\p"
	.string "Let's form a tag team\n"
	.string "and see what happens!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Accept::
	.string "Now that's the attitude!\n"
	.string "Hold tight while I go register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Reject::
	.string "I carved time out of my schedule\n"
	.string "for this! Come on, give me a chance!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Intro::
	.string "Hi, hi!\n"
	.string "How are you doing?\p"
	.string "I'm {STR_VAR_3}, disciple no. {STR_VAR_2}\n"
	.string "of {STR_VAR_1}. Great to meet you!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon1::
	.string "Hey, hey! You've got to hear about\n"
	.string "the POKéMON {STR_VAR_3} picked for me.\l"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\n"
	.string "Naturally I trained them to perfection!\p"
	.string "So, shall we team up?\n"
	.string "It'll be a great combo, I promise!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Accept::
	.string "Yay, this is gonna be a blast!\n"
	.string "I'm off to register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Reject::
	.string "Aw, that's mean!\n"
	.string "I was personally recommended, you know.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Intro::
	.string "Allow me to introduce myself.\n"
	.string "My name is {STR_VAR_3}.\p"
	.string "I have the honor of serving as the\n"
	.string "no. {STR_VAR_2} disciple of {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon1::
	.string "On the wise counsel of {STR_VAR_3},\n"
	.string "I trained one {STR_VAR_2} with {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon2Ask::
	.string "and one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "I humbly ask you to join me in a\n"
	.string "tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Accept::
	.string "Oh… I'm overjoyed!\n"
	.string "I swear I will give it my all!\p"
	.string "I shall register us immediately!\n"
	.string "Please wait a moment!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Reject::
	.string "I was so hoping to stand beside you,\n"
	.string "{PLAYER}…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Intro::
	.string "Eek! You talked to me!\n"
	.string "I… I'm so happy!\p"
	.string "I'm {STR_VAR_3}! The no. {STR_VAR_2} disciple\n"
	.string "of {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon1::
	.string "Following {STR_VAR_3}'s teachings, I\n"
	.string "trained one {STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon2Ask::
	.string "and one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2}.\p"
	.string "Please, will you make my dream come\n"
	.string "true? I want to be your tag partner!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Accept::
	.string "Eek! I… my knees are shaking!\n"
	.string "Thank you, thank you!\l"
	.string "I'll register us this instant!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Reject::
	.string "Waaah! Have you no mercy?\n"
	.string "But somehow that makes you cooler…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Intro::
	.string "Yeehaw! Disciple no. {STR_VAR_2}\n"
	.string "of {STR_VAR_1}, that's me!\p"
	.string "{STR_VAR_3} is the name, howdy, howdy!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon1::
	.string "My master {STR_VAR_3} suggested\n"
	.string "one {STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon2Ask::
	.string "and one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2}.\p"
	.string "Pretty sweet, huh?\n"
	.string "So you'll be my partner, right?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Accept::
	.string "Yes, excellent!\n"
	.string "Registration, coming right up!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Reject::
	.string "Gwaaah!\n"
	.string "You drive a hard bargain, {PLAYER}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Intro::
	.string "Hey! Emergency! Today's the last day\n"
	.string "the BATTLE TOWER is open!\p"
	.string "… … … … … …\n"
	.string "Ha, just kidding!\p"
	.string "Everyone calls me {STR_VAR_3}, so I do too!\n"
	.string "I'm {STR_VAR_1}'s 1,000th disciple!\l"
	.string "Well, okay, I'm no. {STR_VAR_2}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon1::
	.string "{STR_VAR_3} gave me a few hints.\n"
	.string "{STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon2Ask::
	.string "{STR_VAR_2} with {STR_VAR_1}.\n"
	.string "I've got ten of each!\l"
	.string "Well, one of each, really!\p"
	.string "So what do you say?\n"
	.string "Wanna try some tag battles?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Accept::
	.string "Hooray!\n"
	.string "Here, have a POKéMON as my thanks!\p"
	.string "Just kidding! But I really will go\n"
	.string "register us, promise!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Reject::
	.string "Ouch, that's cold! I'll have to flatten\n"
	.string "the BATTLE TOWER with a ki blast!\p"
	.string "Just kidding! I won't!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Intro::
	.string "Yo, yo, I'm rockin' and a-rollin'!\n"
	.string "POKéMON, I be controllin'!\p"
	.string "I'm {STR_VAR_3}, the rappin' SAILOR.\n"
	.string "Don't confuse me with some wailer!\p"
	.string "Disciple no. {STR_VAR_2} of {STR_VAR_1}.\n"
	.string "Me, you shouldn't be quick to dismiss!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon1::
	.string "{STR_VAR_3}'s wisdom I did heed,\n"
	.string "built a team for the win, guaranteed!\l"
	.string "One {STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon2Ask::
	.string "and one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2}!\p"
	.string "Let's mark this meeting with flair,\n"
	.string "win as a tag team, a victorious pair!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Accept::
	.string "Yo, yo, I dig your groove!\n"
	.string "To the counter, I'll move!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Reject::
	.string "Whoa, hey, {PLAYER}, that's frosty!\n"
	.string "Why so mean to me, bossy?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Intro::
	.string "Yippee-yahoo!\n"
	.string "Oh, don't run! I'm only having fun!\p"
	.string "Howdy! I'm {STR_VAR_3}!\n"
	.string "Disciple no. {STR_VAR_2} of {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon1::
	.string "My master {STR_VAR_3} showed me the way.\n"
	.string "So one {STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon2Ask::
	.string "and one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2} are my pride.\p"
	.string "So, what's the word?\n"
	.string "Come on, make my day sing!\p"
	.string "Please, I'm begging you!\n"
	.string "Let me join in, don't leave me blue!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Accept::
	.string "Yeahah! Lady Luck's on my side!\n"
	.string "I'm so happy, I could fly!\p"
	.string "Before we storm the field,\n"
	.string "I'll go register, signed and sealed!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Reject::
	.string "You're saying no?\n"
	.string "{PLAYER}, now my smile's\l"
	.string "turned upside-down!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Intro::
	.string "Cough!\p"
	.string "Oh, sorry, I've caught a cold.\n"
	.string "My POKéMON are doing great, though.\p"
	.string "I'm {STR_VAR_3}, disciple no. {STR_VAR_2}\n"
	.string "of {STR_VAR_1}. Cough!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon1::
	.string "I took the lessons of {STR_VAR_3} to heart\n"
	.string "and built a team of\l"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "I trained them every day, rain or\n"
	.string "shine, wind or storm.\p"
	.string "That's probably how I caught\n"
	.string "this cold.\p"
	.string "That's the kind of fighter I am.\n"
	.string "Want to be my tag partner?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Accept::
	.string "Thank you, I really appreciate it!\n"
	.string "Cough, cough!\l"
	.string "Wait here while I go register us.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Reject::
	.string "Oh, you won't? {PLAYER}, I just know we'd\n"
	.string "be a great pair.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Intro::
	.string "Oh, hello!\n"
	.string "I'm so nervous right now.\p"
	.string "I'm {STR_VAR_3}.\n"
	.string "The no. {STR_VAR_2} disciple of {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon1::
	.string "{STR_VAR_3} promised me this team is\n"
	.string "solid-- one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "I'm really embarrassed to ask,\n"
	.string "but will you let me join you?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Accept::
	.string "Oh, gosh, thank you!\n"
	.string "I'm blushing, but I'll try my best!\p"
	.string "I'll go register us now.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Reject::
	.string "Oh, please don't say no!\n"
	.string "I'm embarrassed enough already…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Intro::
	.string "I am {STR_VAR_3}, and I speak the truth. I\n"
	.string "am the no. {STR_VAR_2} disciple of {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon1::
	.string "{STR_VAR_3} suggested my team.\p"
	.string "Because the advice rang true,\n"
	.string "I decided to bring\l"
	.string "my {STR_VAR_1}-using {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon2Ask::
	.string "my {STR_VAR_1}-using {STR_VAR_2}.\p"
	.string "If it pleases you, I humbly ask to be\n"
	.string "your partner on a tag team.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Accept::
	.string "You will truly accept me?\n"
	.string "It would have been dreadful otherwise.\p"
	.string "But now that you've agreed, all is well.\n"
	.string "I shall register the two of us.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Reject::
	.string "Hm? On what grounds do you refuse?\n"
	.string "Is this some kind of joke?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassIntro::
	.string "Hi! LASS {STR_VAR_1} here, ready to fight!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon1::
	.string "Here's my team: one {STR_VAR_2}\n"
	.string "with {STR_VAR_1} and one$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon2Ask::
	.string "{STR_VAR_2} with {STR_VAR_1}.\n"
	.string "That's all I brought.\p"
	.string "So, will you be my partner?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassAccept::
	.string "Yay, thank you!\n"
	.string "I'm off to register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassReject::
	.string "Huh? You don't want me?\n"
	.string "You're going to regret it!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterIntro::
	.string "Hey there!\p"
	.string "YOUNGSTER {STR_VAR_1} reporting in!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon1::
	.string "Wanna see my team?\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Say you'll fight beside me as my\n"
	.string "tag-team partner!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterAccept::
	.string "Yay!\n"
	.string "I'll go register us, okay?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterReject::
	.string "Aww! Our POKéMON together would be\n"
	.string "unbeatable, like a fusion!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerIntro::
	.string "Yahoo!\n"
	.string "HIKER {STR_VAR_1}, fresh off the trail!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon1::
	.string "Know what I carried all this way?\n"
	.string "My {STR_VAR_1}-using {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon2Ask::
	.string "my {STR_VAR_1}-using {STR_VAR_2}!\p"
	.string "Pretty solid lineup, eh?\n"
	.string "What do you say to a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerAccept::
	.string "Yahoo!\n"
	.string "Let me go register us, then.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerReject::
	.string "I'd have loved to fight shoulder to\n"
	.string "shoulder with you.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyIntro::
	.string "Hello, darling!\n"
	.string "The name's {STR_VAR_1}, a proud BEAUTY!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon1::
	.string "Care to guess what I've been training?\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Well, what do you think?\n"
	.string "Shall we be a glamorous tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyAccept::
	.string "Marvelous!\n"
	.string "I'll register us right this second!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyReject::
	.string "How disappointing!\p"
	.string "The two of us together would have\n"
	.string "been unbeatable!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanIntro::
	.string "Yo!\p"
	.string "You know me?\n"
	.string "FISHERMAN {STR_VAR_1}, that's who!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon1::
	.string "I've reeled in a real catch of a team:\n"
	.string "one {STR_VAR_1}-using {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon2Ask::
	.string "one {STR_VAR_1}-using {STR_VAR_2}.\p"
	.string "So what do you say?\n"
	.string "Will you fight at my side?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanAccept::
	.string "Good, good!\n"
	.string "Leave everything to me!\p"
	.string "I'll register us right now.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanReject::
	.string "And we were such a perfect match…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyIntro::
	.string "Delighted to make your acquaintance.\n"
	.string "I am {STR_VAR_1}, a LADY.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon1::
	.string "I travel with a team of\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "I trust I meet with your approval,\n"
	.string "for you are who I wish to partner with.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyAccept::
	.string "I thank you most sincerely.\n"
	.string "I shall attend to the registration.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyReject::
	.string "I am certain you will regret not\n"
	.string "having me as your partner.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFIntro::
	.string "TRIATHLETE {STR_VAR_1}, at your service!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon1::
	.string "Here's my lineup…\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\n"
	.string "That's my pair!\p"
	.string "So what do you say?\n"
	.string "Will you team up with me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFAccept::
	.string "Gee, thanks!\n"
	.string "I'll sign us up at the counter.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFReject::
	.string "Together we would have been\n"
	.string "really tough!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherIntro::
	.string "Hiya!\n"
	.string "BUG CATCHER {STR_VAR_1}, that's me!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon1::
	.string "Look what I caught!\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Hey, listen!\n"
	.string "Wanna be a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherAccept::
	.string "Gotcha!\p"
	.string "I'll go handle the paperwork\n"
	.string "at the counter.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherReject::
	.string "Aww, but my POKéMON are the best!\n"
	.string "Don't come crying to me later!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMIntro::
	.string "Good day!\n"
	.string "SCHOOL KID {STR_VAR_1}, top of the class!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon1::
	.string "My research subjects are one\n"
	.string "{STR_VAR_2} that uses {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon2Ask::
	.string "and one {STR_VAR_2} that uses\n"
	.string "{STR_VAR_1}.\p"
	.string "Quite decent, wouldn't you say?\n"
	.string "Would you like to be a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMAccept::
	.string "Thank you very much!\n"
	.string "I'll go finish the registration.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMReject::
	.string "That's a shame…\p"
	.string "I'd hoped to learn from you\n"
	.string "as your partner…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyIntro::
	.string "Yo! Bow down to greatness!\n"
	.string "RICH BOY {STR_VAR_1} has arrived!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon1::
	.string "Guess what money can buy!\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "I'm graciously offering you a spot\n"
	.string "on my tag team.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyAccept::
	.string "Smart choice!\p"
	.string "I'll have the registration done\n"
	.string "in no time!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyReject::
	.string "You'd refuse me, of all people?\n"
	.string "You'll regret that, I guarantee it!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltIntro::
	.string "Hiyah!\n"
	.string "BLACK BELT {STR_VAR_1}, master of the fist!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon1::
	.string "At my side stand one {STR_VAR_2}\n"
	.string "wielding {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon2Ask::
	.string "and one {STR_VAR_2} wielding\n"
	.string "{STR_VAR_1}.\p"
	.string "I beg you, grant my request!\n"
	.string "Let me be your tag partner!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltAccept::
	.string "Hiyah!\n"
	.string "I shall register us at once!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltReject::
	.string "I see… May our paths cross again\n"
	.string "and give me another chance…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFIntro::
	.string "Hi, there!\n"
	.string "{STR_VAR_1} the TUBER, floating by!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon1::
	.string "Take a look at my team.\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\n"
	.string "Could I please join your tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFAccept::
	.string "Thank you!\n"
	.string "I'll go register us right now!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFReject::
	.string "If we'd teamed up, we'd have been\n"
	.string "super strong!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacIntro::
	.string "Greetings…\n"
	.string "The spirits call me HEX MANIAC {STR_VAR_1}…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon1::
	.string "Bound to me are one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2} and one$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon2Ask::
	.string "{STR_VAR_1}-using {STR_VAR_2}…\p"
	.string "I implore you…\n"
	.string "Become my tag team…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacAccept::
	.string "I thank you…\n"
	.string "I shall seal our registration…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacReject::
	.string "I yearned so deeply to join you…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMIntro::
	.string "Pleased to meet you! {STR_VAR_1} here,\n"
	.string "POKéMON BREEDER by trade!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon1::
	.string "I'm raising a pair of beauties!\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "What do you say?\n"
	.string "Want to make a tag team with me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMAccept::
	.string "Much obliged!\p"
	.string "I'll handle the registration\n"
	.string "stuff, so stay right there!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMReject::
	.string "I was really looking forward to\n"
	.string "fighting at your side…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFIntro::
	.string "Well, hello! TRIATHLETE {STR_VAR_1}, always\n"
	.string "on the move!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon1::
	.string "Wanna know who runs with me?\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Well?\n"
	.string "Want to run a tag team with me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFAccept::
	.string "Nice going!\p"
	.string "I'll sprint to the counter and\n"
	.string "register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFReject::
	.string "You and I would have been on top.\n"
	.string "What a shame…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMIntro::
	.string "Hey, there! I'm {STR_VAR_1}, a TRIATHLETE\n"
	.string "who never stops moving!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon1::
	.string "I do my road work with a hardy team--\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Not bad at all, right?\n"
	.string "Let's be a tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMAccept::
	.string "Right on!\n"
	.string "I'll register us in a flash!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMReject::
	.string "I wanted so badly to fight as your\n"
	.string "tag-team partner…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlIntro::
	.string "BATTLE GIRL {STR_VAR_1}, and I'm itching to\n"
	.string "fight!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon1::
	.string "I've been training hard with one\n"
	.string "{STR_VAR_1}-using {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon2Ask::
	.string "one {STR_VAR_1}-using {STR_VAR_2}!\p"
	.string "Like that combo?\n"
	.string "Want to join forces with me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlAccept::
	.string "Great, thanks!\p"
	.string "I'll finish the registration\n"
	.string "right away!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlReject::
	.string "I thought with you, we could be\n"
	.string "the ultimate tag team…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMIntro::
	.string "TRIATHLETE {STR_VAR_1}, ready to roll!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon1::
	.string "I've been training alongside one\n"
	.string "{STR_VAR_2} that has mastered {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon2Ask::
	.string "and one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "We could be a tag team.\n"
	.string "Wouldn't that be awesome?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMAccept::
	.string "Thanks!\p"
	.string "I'll register us, and I mean\n"
	.string "right now!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMReject::
	.string "Aw, that's a bummer. We could have\n"
	.string "been the toughest team around!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMIntro::
	.string "Who, me?\n"
	.string "TUBER {STR_VAR_1}, that's me!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon1::
	.string "Wanna see my team?\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "So?\n"
	.string "You're teaming up with me, right?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMAccept::
	.string "Okay!\p"
	.string "I'll go register us!\n"
	.string "Let's be awesome together!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMReject::
	.string "My POKéMON are strong, for sure…\n"
	.string "Shame you won't team up.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristIntro::
	.string "Yay-hey! The crowd calls me\n"
	.string "GUITARIST {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon1::
	.string "Meet my backup band!\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Yay-hey! Wild, huh?\n"
	.string "Let's play a duet as a tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristAccept::
	.string "Yay-hey! Rock on!\n"
	.string "I'll go take care of the paperwork!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristReject::
	.string "My POKéMON shred harder than my guitar!\n"
	.string "You'll be sorry, I tell you!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanIntro::
	.string "It is a pleasure to meet you.\n"
	.string "I am {STR_VAR_1}, a GENTLEMAN.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon1::
	.string "I am accompanied by my faithful\n"
	.string "{STR_VAR_1}-using {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon2Ask::
	.string "one {STR_VAR_1}-using {STR_VAR_2}.\p"
	.string "May I propose that we join together\n"
	.string "in a tag-team partnership?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanAccept::
	.string "Ah, I am grateful for your trust.\n"
	.string "I shall see to the registration.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanReject::
	.string "That is most regrettable…\p"
	.string "Perhaps fate will bring us together\n"
	.string "another day…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMIntro::
	.string "Hello there. {STR_VAR_1}, devoted\n"
	.string "POKéFAN.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon1::
	.string "Right now I'm traveling with one\n"
	.string "{STR_VAR_2} that knows the move {STR_VAR_1}\l"
	.string "and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Like them?\n"
	.string "Then be my tag partner!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMAccept::
	.string "Thank you so much!\n"
	.string "I'll go take care of the registration!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMReject::
	.string "My POKéMON are first class…\n"
	.string "Shame you can't tell.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMIntro::
	.string "Hm! {STR_VAR_1} is my name,\n"
	.string "and I am a true EXPERT!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon1::
	.string "The POKéMON I have forged are\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Will you fight beside me as a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMAccept::
	.string "Hm!\n"
	.string "I shall register us at once!\l"
	.string "Let us both give our all!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMReject::
	.string "I can only hope your decision\n"
	.string "proves wise…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFIntro::
	.string "Well, hello.\n"
	.string "I'm {STR_VAR_1}, a veteran EXPERT.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon1::
	.string "I've trained my POKéMON to the bone.\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}--\n"
	.string "that's my pair.\p"
	.string "Won't you team up with me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFAccept::
	.string "Good, good.\n"
	.string "I'll see to the registration shortly.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFReject::
	.string "Perhaps we'll team up when we meet\n"
	.string "again.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerIntro::
	.string "I'm DRAGON TAMER {STR_VAR_1}!\n"
	.string "Dragons listen when I speak!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon1::
	.string "My dragon-grade training produced\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "What do you say?\n"
	.string "Be my partner?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerAccept::
	.string "Excellent, I won't let you down!\n"
	.string "I'll go register us right now!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerReject::
	.string "You won't find a tougher partner\n"
	.string "anywhere, mark my words!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperIntro::
	.string "BIRD KEEPER {STR_VAR_1}, soaring in!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon1::
	.string "Which POKéMON did I bring along?\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Don't you think we'd fly well\n"
	.string "together as a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperAccept::
	.string "Great, thanks!\n"
	.string "I'll go take care of the registration!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperReject::
	.string "My POKéMON and I are strong.\n"
	.string "You just let a good one go.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyIntro::
	.string "NINJA BOY {STR_VAR_1}, appearing from\n"
	.string "the shadows!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon1::
	.string "My ninja team: one\n"
	.string "{STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Let's join forces as a tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyAccept::
	.string "Yay!\n"
	.string "I'll slip away to register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyReject::
	.string "You'll regret leaving my tough\n"
	.string "POKéMON out of your plans!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyIntro::
	.string "Why, hello!\n"
	.string "PARASOL LADY {STR_VAR_1}, at your service!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon1::
	.string "Keeping me company today are my\n"
	.string "{STR_VAR_2} that uses {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon2Ask::
	.string "one {STR_VAR_2} that uses\n"
	.string "{STR_VAR_1}.\p"
	.string "Aren't they lovely?\n"
	.string "Would you care to join us as a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyAccept::
	.string "Many thanks!\p"
	.string "I'll go register us at the counter.\n"
	.string "Let's not let each other down!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyReject::
	.string "My POKéMON are remarkably strong.\n"
	.string "What a pity…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacIntro::
	.string "Hello.\n"
	.string "{STR_VAR_1}, BUG MANIAC, at your service!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon1::
	.string "I tracked down my own POKéMON, yes.\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}--\n"
	.string "these are my discoveries.\p"
	.string "Would you care to form\n"
	.string "a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacAccept::
	.string "Very well!\n"
	.string "Understood!\p"
	.string "The registration will be quick!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacReject::
	.string "With the POKéMON I found, we'd never\n"
	.string "have lost…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorIntro::
	.string "Ahoy, matey!\n"
	.string "SAILOR {STR_VAR_1} here!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon1::
	.string "Feast your eyes on my pride and joy!\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "You wouldn't turn down a sailor,\n"
	.string "would you? Let's team up!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorAccept::
	.string "Knew you had it in you!\n"
	.string "I'll go register us now.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorReject::
	.string "We'd have sailed right through\n"
	.string "the competition! Bad luck!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorIntro::
	.string "Hello, I'm {STR_VAR_1}, a COLLECTOR\n"
	.string "of rare and wonderful things.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon1::
	.string "The gems of my collection are\n"
	.string "my {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Pretty fine, huh?\n"
	.string "We ought to team up.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorAccept::
	.string "Oh, excellent!\p"
	.string "Let's not waste any time.\n"
	.string "I'll go register the two of us.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorReject::
	.string "Well, how upsetting.\n"
	.string "You can't see the value of my POKéMON.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMIntro::
	.string "Howdy, {STR_VAR_1} here.\n"
	.string "POKéMON RANGER, on patrol.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon1::
	.string "Riding along with me are one\n"
	.string "{STR_VAR_1}-using {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "We'd make a mighty impressive\n"
	.string "tag team, don't you think?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMAccept::
	.string "That's great!\n"
	.string "I'll handle the registration now.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMReject::
	.string "Next time, pick my POKéMON,\n"
	.string "okay?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFIntro::
	.string "{STR_VAR_1}, POKéMON RANGER!\n"
	.string "Glad to meet you!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon1::
	.string "Let me introduce my team. I'm bringing\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Want to join forces with my little\n"
	.string "posse as a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFAccept::
	.string "We'll give it everything!\n"
	.string "I'll register us quick!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFReject::
	.string "I'll be hoping you pick my POKéMON\n"
	.string "next time.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyIntro::
	.string "Delighted to meet you. I'm {STR_VAR_1},\n"
	.string "an AROMA LADY of some renown.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon1::
	.string "In my travels I keep one {STR_VAR_2}\n"
	.string "that uses {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon2Ask::
	.string "one {STR_VAR_2} that uses\n"
	.string "{STR_VAR_1}.\p"
	.string "I do hope they strike your fancy.\n"
	.string "Will you honor me as my partner?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyAccept::
	.string "I am honored that you accepted.\n"
	.string "I shall register us at once.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyReject::
	.string "How lovely it would be if we could\n"
	.string "team up when next we meet.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacIntro::
	.string "Care to know who I am?\n"
	.string "{STR_VAR_1}, the RUIN MANIAC, that's who!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon1::
	.string "The POKéMON that I unearthed are\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Fascinating, eh?\n"
	.string "Shall you and I join forces?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacAccept::
	.string "A wise decision!\n"
	.string "I'll go see to the paperwork.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacReject::
	.string "Hmm…\n"
	.string "I do believe my POKéMON are tough…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMIntro::
	.string "COOLTRAINER {STR_VAR_1}, cool as ever!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon1::
	.string "My lineup at the moment is one\n"
	.string "{STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\n"
	.string "Cool, huh?\p"
	.string "Wouldn't it be way cool if we\n"
	.string "formed a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMAccept::
	.string "Cool!\n"
	.string "I'll register us in a flash!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMReject::
	.string "I figured we'd be the greatest\n"
	.string "team ever.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFIntro::
	.string "COOLTRAINER {STR_VAR_1}, and I never lose\n"
	.string "my cool!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon1::
	.string "My training has produced one\n"
	.string "{STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "How's that for a lineup?\n"
	.string "Shall we become tag partners?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFAccept::
	.string "Perfect!\n"
	.string "I'd better go register us.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFReject::
	.string "I thought we'd make an awesome\n"
	.string "team…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacIntro::
	.string "Heyo! They call me {STR_VAR_1},\n"
	.string "the POKéMANIAC!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon1::
	.string "What's a guy like me packing?\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1},\n"
	.string "that's the deal!\p"
	.string "Let's go!\n"
	.string "We'll stomp them as a tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacAccept::
	.string "Great call!\n"
	.string "I'll go register the pair of us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacReject::
	.string "My POKéMON are ferocious!\n"
	.string "Don't blame me if you regret this!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerIntro::
	.string "Yo! KINDLER {STR_VAR_1} here,\n"
	.string "and I'm fired up!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon1::
	.string "You know what my training fired up?\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "So, what's the word?\n"
	.string "Want to light it up as a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerAccept::
	.string "Awesome!\n"
	.string "I'll go take care of the registration.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerReject::
	.string "Promise you'll team up with me\n"
	.string "when we run into each other again.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperIntro::
	.string "CAMPER {STR_VAR_1}, ready to rough it!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon1::
	.string "I've been training one {STR_VAR_1}-\n"
	.string "using {STR_VAR_2} and one$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon2Ask::
	.string "{STR_VAR_1}-using {STR_VAR_2}.\p"
	.string "Sounds like a fun team-up, right?\n"
	.string "I bet it'd be a blast!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperAccept::
	.string "Yeah, let's go!\n"
	.string "I'm off to register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperReject::
	.string "Maybe next time, okay?\n"
	.string "I really want to be on your team.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerIntro::
	.string "Hello!\n"
	.string "PICNICKER {STR_VAR_1} here!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon1::
	.string "The POKéMON I packed for today are\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Would you like to join me\n"
	.string "as a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerAccept::
	.string "Oh, thank you!\n"
	.string "I'll go register us now.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerReject::
	.string "Perhaps I could join you some\n"
	.string "other time.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMIntro::
	.string "PSYCHIC {STR_VAR_1}. I sensed you coming.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon1::
	.string "The pair I've been nurturing are\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Would you join me in a tag team?\n"
	.string "I foresee a good result.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMAccept::
	.string "Of course!\n"
	.string "I'll handle the registration!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMReject::
	.string "If we meet again, perhaps then you\n"
	.string "will team up with me.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFIntro::
	.string "I am {STR_VAR_1}, a PSYCHIC.\n"
	.string "Your aura drew me here.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon1::
	.string "My pupils: one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}--\n"
	.string "they are my students.\p"
	.string "Doesn't the idea of a tag-team\n"
	.string "partnership appeal to you?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFAccept::
	.string "Thank you.\n"
	.string "I'll go see to the registration.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFReject::
	.string "May the stars grant us another chance\n"
	.string "to forge an alliance.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFIntro::
	.string "SCHOOL KID {STR_VAR_1}, reporting for\n"
	.string "duty!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon1::
	.string "The POKéMON I trained for this are one\n"
	.string "{STR_VAR_1}-using {STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon2Ask::
	.string "and one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2}. That's my whole team.\p"
	.string "May I please be your partner on\n"
	.string "a tag team?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFAccept::
	.string "Ooh, thank you! I'll register us at the\n"
	.string "counter right now!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFReject::
	.string "Please?\n"
	.string "Can I join you next time?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFIntro::
	.string "Hiya! {STR_VAR_1} here,\n"
	.string "POKéMON BREEDER and proud of it!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon1::
	.string "The POKéMON I've raised with love are\n"
	.string "one {STR_VAR_1}-using {STR_VAR_2} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon2Ask::
	.string "one {STR_VAR_1}-using {STR_VAR_2}.\p"
	.string "Sounds promising, right?\n"
	.string "So let's make a tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFAccept::
	.string "All righty!\n"
	.string "Leave the paperwork to me!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFReject::
	.string "Next time you're teaming up with me,\n"
	.string "understood?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFIntro::
	.string "I'm {STR_VAR_1}, and I proudly declare\n"
	.string "myself a POKéFAN.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon1::
	.string "The darlings I've doted on are\n"
	.string "one {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}.\p"
	.string "Aren't they simply adorable?\n"
	.string "We simply must team up!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFAccept::
	.string "Thank you, dear!\n"
	.string "I'll skip off to register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFReject::
	.string "My darlings are the very best,\n"
	.string "I'll have you know. How vexing!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFIntro::
	.string "Hey, SWIMMER {STR_VAR_1} here.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon1::
	.string "I've been training one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon2Ask::
	.string "and one {STR_VAR_1}-using\n"
	.string "{STR_VAR_2} in the water.\p"
	.string "You and me, let's make a splash\n"
	.string "as a tag team.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFAccept::
	.string "Sweet!\n"
	.string "I'll register the two of us.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFReject::
	.string "If we meet again, you owe me\n"
	.string "one tag team!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMIntro::
	.string "What's up?\n"
	.string "{STR_VAR_1} here, TRIATHLETE and proud.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon1::
	.string "I've got a couple of solid POKéMON.\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "We'd make a cool tag team,\n"
	.string "wouldn't you say?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMAccept::
	.string "Right on!\n"
	.string "Wait here while I go register us!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMReject::
	.string "I'll expect you to let me join\n"
	.string "next time, deal?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFIntro::
	.string "TRIATHLETE {STR_VAR_1}, ready to dive in!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon1::
	.string "My POKéMON are one {STR_VAR_2} with\n"
	.string "{STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon2Ask::
	.string "another {STR_VAR_2} that has mastered\n"
	.string "{STR_VAR_1}. That's my pair.\p"
	.string "Thoughts?\n"
	.string "I say we'd make a great team.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFAccept::
	.string "That's the answer I wanted!\n"
	.string "Registration will be done fast!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFReject::
	.string "You'll give me another shot at a\n"
	.string "partnership, right?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMIntro::
	.string "Hi, there! Hello!\n"
	.string "SWIMMER {STR_VAR_1}, fresh from the surf!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon1::
	.string "Take a look at my training results!\n"
	.string "One {STR_VAR_2} with {STR_VAR_1} and$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon2Ask::
	.string "one {STR_VAR_2} with {STR_VAR_1}!\p"
	.string "Sweet, huh?\n"
	.string "Teaming up would be even sweeter!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMAccept::
	.string "Much obliged!\n"
	.string "I'll get this registration done!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMReject::
	.string "If we meet again, you'd better team\n"
	.string "up with me. Deal?$"

