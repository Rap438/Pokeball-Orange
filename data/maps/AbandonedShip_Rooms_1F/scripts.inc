AbandonedShip_Rooms_1F_MapScripts::
	.byte 0

AbandonedShip_Rooms_1F_EventScript_Gentleman::
	msgbox AbandonedShip_Rooms_1F_Text_TakingALookAround, MSGBOX_NPC
	end

AbandonedShip_Rooms_1F_EventScript_Demetrius::
	trainerbattle_single TRAINER_DEMETRIUS, AbandonedShip_Rooms_1F_Text_DemetriusIntro, AbandonedShip_Rooms_1F_Text_DemetriusDefeat
	msgbox AbandonedShip_Rooms_1F_Text_DemetriusPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms_1F_EventScript_Thalia::
	trainerbattle_single TRAINER_THALIA_1, AbandonedShip_Rooms_1F_Text_ThaliaIntro, AbandonedShip_Rooms_1F_Text_ThaliaDefeat, AbandonedShip_Rooms_1F_EventScript_RegisterThalia
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms_1F_EventScript_ThaliaRematch
	msgbox AbandonedShip_Rooms_1F_Text_ThaliaPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms_1F_EventScript_RegisterThalia::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox AbandonedShip_Rooms_1F_Text_ThaliaRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_THALIA_1
	release
	end

AbandonedShip_Rooms_1F_EventScript_ThaliaRematch::
	trainerbattle_rematch TRAINER_THALIA_1, AbandonedShip_Rooms_1F_Text_ThaliaRematchIntro, AbandonedShip_Rooms_1F_Text_ThaliaRematchDefeat
	msgbox AbandonedShip_Rooms_1F_Text_ThaliaPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms_1F_Text_TakingALookAround:
	.string "A ship like this is rare, so I'm taking a\n"
	.string "look around.\p"
	.string "Hmhm…\p"
	.string "There seem to be other cabins…$"

AbandonedShip_Rooms_1F_Text_ThaliaIntro:
	.string "What brings you to a place like this?\n"
	.string "Curiosity, I'd bet!$"

AbandonedShip_Rooms_1F_Text_ThaliaDefeat:
	.string "Not just curious, but strong…$"

AbandonedShip_Rooms_1F_Text_ThaliaPostBattle:
	.string "The man next door…\p"
	.string "He says he's only sightseeing, but I\n"
	.string "don't buy it.$"

AbandonedShip_Rooms_1F_Text_ThaliaRegister:
	.string "Such a tough fighter!\p"
	.string "Let me register you as a keepsake!$"

AbandonedShip_Rooms_1F_Text_ThaliaRematchIntro:
	.string "What brings you back here? You must be\n"
	.string "really curious!$"

AbandonedShip_Rooms_1F_Text_ThaliaRematchDefeat:
	.string "Aren't you too strong?$"

AbandonedShip_Rooms_1F_Text_ThaliaPostRematch:
	.string "I'm sure that man is up to something!\p"
	.string "He acts so suspicious!$"

AbandonedShip_Rooms_1F_Text_DemetriusIntro:
	.string "Waaah!\p"
	.string "I've been found! …Huh?$"

AbandonedShip_Rooms_1F_Text_DemetriusDefeat:
	.string "Oh, you're not my mom.$"

AbandonedShip_Rooms_1F_Text_DemetriusPostBattle:
	.string "My mom wants me to do my homework, so I\n"
	.string "ran away! Don't tell her where I am!$"

