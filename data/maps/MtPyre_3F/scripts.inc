MtPyre_3F_MapScripts::
	.byte 0

MtPyre_3F_EventScript_William::
	trainerbattle_single TRAINER_WILLIAM, MtPyre_3F_Text_WilliamIntro, MtPyre_3F_Text_WilliamDefeat
	msgbox MtPyre_3F_Text_WilliamPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_EventScript_Kayla::
	trainerbattle_single TRAINER_KAYLA, MtPyre_3F_Text_KaylaIntro, MtPyre_3F_Text_KaylaDefeat
	msgbox MtPyre_3F_Text_KaylaPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_EventScript_Gabrielle::
	trainerbattle_single TRAINER_GABRIELLE_1, MtPyre_3F_Text_GabrielleIntro, MtPyre_3F_Text_GabrielleDefeat, MtPyre_3F_EventScript_RegisterGabrielle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtPyre_3F_EventScript_RematchGabrielle
	msgbox MtPyre_3F_Text_GabriellePostBattle, MSGBOX_DEFAULT
	release
	end

MtPyre_3F_EventScript_RegisterGabrielle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtPyre_3F_Text_GabrielleRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_GABRIELLE_1
	release
	end

MtPyre_3F_EventScript_RematchGabrielle::
	trainerbattle_rematch TRAINER_GABRIELLE_1, MtPyre_3F_Text_GabrielleRematchIntro, MtPyre_3F_Text_GabrielleRematchDefeat
	msgbox MtPyre_3F_Text_GabriellePostRematch, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_Text_WilliamIntro:
	.string "The spiritual air of this mountain has\n"
	.string "raised my psychic power!\p"
	.string "A mere child like you…\n"
	.string "Do you dream of winning?$"

MtPyre_3F_Text_WilliamDefeat:
	.string "I'm drowning in self-pity…$"

MtPyre_3F_Text_WilliamPostBattle:
	.string "My psychic powers have surely grown\n"
	.string "many times over, but…$"

MtPyre_3F_Text_KaylaIntro:
	.string "Ahahahaha!\p"
	.string "This is no place for kids, least of all\n"
	.string "you!$"

MtPyre_3F_Text_KaylaDefeat:
	.string "Beaten clean…$"

MtPyre_3F_Text_KaylaPostBattle:
	.string "So my training still isn't enough…\p"
	.string "I have to keep working my way to the\n"
	.string "summit…\p"
	.string "Go, me!$"

MtPyre_3F_Text_GabrielleIntro:
	.string "Why have you come here?$"

MtPyre_3F_Text_GabrielleDefeat:
	.string "That was amazing!\n"
	.string "You are a very special TRAINER.$"

MtPyre_3F_Text_GabriellePostBattle:
	.string "POKéMON no longer of this world.\n"
	.string "POKéMON who are with you now.\p"
	.string "And the POKéMON you will meet in the\n"
	.string "future…\p"
	.string "All of them are to be cherished equally.\n"
	.string "Please remember that.$"

MtPyre_3F_Text_GabrielleRegister:
	.string "I would like to see your POKéMON once\n"
	.string "they have grown some more…\p"
	.string "Please, I need to see your POKéNAV.$"

MtPyre_3F_Text_GabrielleRematchIntro:
	.string "Oh, it's you…\p"
	.string "Have you come to show me how your\n"
	.string "POKéMON have grown?$"

MtPyre_3F_Text_GabrielleRematchDefeat:
	.string "How wonderful!\n"
	.string "You are a remarkable person.$"

MtPyre_3F_Text_GabriellePostRematch:
	.string "POKéMON no longer of this world.\n"
	.string "POKéMON who are with you now.\p"
	.string "And the POKéMON you will meet in the\n"
	.string "future…\p"
	.string "All of them are to be cherished equally.\n"
	.string "I see you have not forgotten that.$"

