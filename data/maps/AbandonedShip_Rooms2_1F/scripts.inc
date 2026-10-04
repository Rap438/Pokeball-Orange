AbandonedShip_Rooms2_1F_MapScripts::
	.byte 0

AbandonedShip_Rooms2_1F_EventScript_Dan::
	trainerbattle_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_DanIntro, AbandonedShip_Rooms2_1F_Text_DanDefeat, AbandonedShip_Rooms2_1F_Text_DanNotEnoughMons, AbandonedShip_Rooms2_1F_EventScript_RegisterDan
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms2_1F_EventScript_DanRematch
	msgbox AbandonedShip_Rooms2_1F_Text_DanPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms2_1F_EventScript_RegisterDan::
	msgbox AbandonedShip_Rooms2_1F_Text_KiraRegister, MSGBOX_DEFAULT  @ Kira speaks for both when registering KiraAndDan
	register_matchcall TRAINER_KIRA_AND_DAN_1
	release
	end

AbandonedShip_Rooms2_1F_EventScript_DanRematch::
	trainerbattle_rematch_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_DanRematchIntro, AbandonedShip_Rooms2_1F_Text_DanRematchDefeat, AbandonedShip_Rooms2_1F_Text_DanRematchNotEnoughMons
	msgbox AbandonedShip_Rooms2_1F_Text_DanPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Kira::
	trainerbattle_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_KiraIntro, AbandonedShip_Rooms2_1F_Text_KiraDefeat, AbandonedShip_Rooms2_1F_Text_KiraNotEnoughMons, AbandonedShip_Rooms2_1F_EventScript_RegisterKira
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms2_1F_EventScript_KiraRematch
	msgbox AbandonedShip_Rooms2_1F_Text_KiraPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms2_1F_EventScript_RegisterKira::
	msgbox AbandonedShip_Rooms2_1F_Text_KiraRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_KIRA_AND_DAN_1
	release
	end

AbandonedShip_Rooms2_1F_EventScript_KiraRematch::
	trainerbattle_rematch_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_KiraRematchIntro, AbandonedShip_Rooms2_1F_Text_KiraRematchDefeat, AbandonedShip_Rooms2_1F_Text_KiraRematchNotEnoughMons
	msgbox AbandonedShip_Rooms2_1F_Text_KiraPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Jani::
	trainerbattle_single TRAINER_JANI, AbandonedShip_Rooms2_1F_Text_JaniIntro, AbandonedShip_Rooms2_1F_Text_JaniDefeat
	msgbox AbandonedShip_Rooms2_1F_Text_JaniPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Garrison::
	trainerbattle_single TRAINER_GARRISON, AbandonedShip_Rooms2_1F_Text_GarrisonIntro, AbandonedShip_Rooms2_1F_Text_GarrisonDefeat
	msgbox AbandonedShip_Rooms2_1F_Text_GarrisonPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_Text_DanIntro:
	.string "DAN: We came hunting for treasure, and\n"
	.string "what do we find? A TRAINER!$"

AbandonedShip_Rooms2_1F_Text_DanDefeat:
	.string "DAN: We teamed up and still lost…$"

AbandonedShip_Rooms2_1F_Text_DanPostBattle:
	.string "DAN: Not a single treasure anywhere…\p"
	.string "Did somebody already beat us to it?$"

AbandonedShip_Rooms2_1F_Text_DanNotEnoughMons:
	.string "DAN: You don't even have two POKéMON.\p"
	.string "We can't fight fair like that.$"

AbandonedShip_Rooms2_1F_Text_KiraIntro:
	.string "KIRA: Oh? We came here for treasure.\p"
	.string "But we found a TRAINER instead!$"

AbandonedShip_Rooms2_1F_Text_KiraDefeat:
	.string "KIRA: Ooh, you're strong!$"

AbandonedShip_Rooms2_1F_Text_KiraPostBattle:
	.string "KIRA: Where could the treasure be?\p"
	.string "I've already decided what I'm buying\n"
	.string "when we find it!$"

AbandonedShip_Rooms2_1F_Text_KiraNotEnoughMons:
	.string "KIRA: Oh, you don't have two POKéMON?\p"
	.string "We'll have to battle another time!$"

AbandonedShip_Rooms2_1F_Text_KiraRegister:
	.string "KIRA: Ooh, you make my blood boil!\p"
	.string "I'm registering you so we can have a\n"
	.string "rematch!$"

AbandonedShip_Rooms2_1F_Text_DanRematchIntro:
	.string "DAN: We've been combing this ship for\n"
	.string "treasure all this time.\p"
	.string "Our POKéMON got stronger from the\n"
	.string "training, too.\p"
	.string "Let us show you, okay?$"

AbandonedShip_Rooms2_1F_Text_DanRematchDefeat:
	.string "DAN: Still as strong as ever!$"

AbandonedShip_Rooms2_1F_Text_DanPostRematch:
	.string "DAN: No treasure, and we keep losing…\p"
	.string "I want to go home… But if I say so, she\n"
	.string "gets mad at me…$"

AbandonedShip_Rooms2_1F_Text_DanRematchNotEnoughMons:
	.string "DAN: You don't even have two POKéMON.\p"
	.string "We can't fight fair like that.$"

AbandonedShip_Rooms2_1F_Text_KiraRematchIntro:
	.string "KIRA: Oh? We meet again!\p"
	.string "You haven't given up the treasure hunt\n"
	.string "either, have you?\p"
	.string "Let's make a deal. The loser quits\n"
	.string "searching!$"

AbandonedShip_Rooms2_1F_Text_KiraRematchDefeat:
	.string "KIRA: Oh, we lost again…$"

AbandonedShip_Rooms2_1F_Text_KiraPostRematch:
	.string "KIRA: We're not leaving until we've\n"
	.string "trained our POKéMON harder and found\l"
	.string "the treasure here!$"

AbandonedShip_Rooms2_1F_Text_KiraRematchNotEnoughMons:
	.string "KIRA: Oh, you don't have two POKéMON?\p"
	.string "We'll have to battle another time!$"

AbandonedShip_Rooms2_1F_Text_JaniIntro:
	.string "I'm no good at swimming, but I'm great at\n"
	.string "fighting!$"

AbandonedShip_Rooms2_1F_Text_JaniDefeat:
	.string "Oops.\p"
	.string "That didn't go so well.$"

AbandonedShip_Rooms2_1F_Text_JaniPostBattle:
	.string "Walking barefoot around this old ship is\n"
	.string "kind of gross. I'm putting my boots back\l"
	.string "on.$"

AbandonedShip_Rooms2_1F_Text_GarrisonIntro:
	.string "Strength and compassion…\p"
	.string "Those are a TRAINER's true treasures!$"

AbandonedShip_Rooms2_1F_Text_GarrisonDefeat:
	.string "Ah, there is something about you that\n"
	.string "shines.$"

AbandonedShip_Rooms2_1F_Text_GarrisonPostBattle:
	.string "In one of the cabins on this ship, I saw\n"
	.string "something sparkle.$"
