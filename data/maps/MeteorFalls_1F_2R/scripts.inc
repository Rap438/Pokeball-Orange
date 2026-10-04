MeteorFalls_1F_2R_MapScripts::
	.byte 0

MeteorFalls_1F_2R_EventScript_Nicolas::
	trainerbattle_single TRAINER_NICOLAS_1, MeteorFalls_1F_2R_Text_NicolasIntro, MeteorFalls_1F_2R_Text_NicolasDefeat, MeteorFalls_1F_2R_EventScript_RegisterNicolas
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchNicolas
	msgbox MeteorFalls_1F_2R_Text_NicolasPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterNicolas::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MeteorFalls_1F_2R_Text_NicolasRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_NICOLAS_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchNicolas::
	trainerbattle_rematch TRAINER_NICOLAS_1, MeteorFalls_1F_2R_Text_NicolasRematchIntro, MeteorFalls_1F_2R_Text_NicolasRematchDefeat
	msgbox MeteorFalls_1F_2R_Text_NicolasPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_EventScript_John::
	trainerbattle_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JohnIntro, MeteorFalls_1F_2R_Text_JohnDefeat, MeteorFalls_1F_2R_Text_JohnNotEnoughMons, MeteorFalls_1F_2R_EventScript_RegisterJohn
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchJohn
	msgbox MeteorFalls_1F_2R_Text_JohnPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterJohn::
	msgbox MeteorFalls_1F_2R_Text_JohnRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_JOHN_AND_JAY_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchJohn::
	trainerbattle_rematch_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JohnRematchIntro, MeteorFalls_1F_2R_Text_JohnRematchDefeat, MeteorFalls_1F_2R_Text_JohnRematchNotEnoughMons
	msgbox MeteorFalls_1F_2R_Text_JohnPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_EventScript_Jay::
	trainerbattle_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JayIntro, MeteorFalls_1F_2R_Text_JayDefeat, MeteorFalls_1F_2R_Text_JayNotEnoughMons, MeteorFalls_1F_2R_EventScript_RegisterJay
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchJay
	msgbox MeteorFalls_1F_2R_Text_JayPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterJay::
	msgbox MeteorFalls_1F_2R_Text_JohnRegister, MSGBOX_DEFAULT  @ John speaks for both during register
	register_matchcall TRAINER_JOHN_AND_JAY_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchJay::
	trainerbattle_rematch_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JayRematchIntro, MeteorFalls_1F_2R_Text_JayRematchDefeat, MeteorFalls_1F_2R_Text_JayRematchNotEnoughMons
	msgbox MeteorFalls_1F_2R_Text_JayPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_Text_NicolasIntro:
	.string "This is where we DRAGON users train.\p"
	.string "Even SUPREME KAI is said to come and\n"
	.string "meditate here.\p"
	.string "Now do you see how special this place\n"
	.string "is?$"

MeteorFalls_1F_2R_Text_NicolasDefeat:
	.string "Gah! Your power level is way higher than\n"
	.string "I figured!$"

MeteorFalls_1F_2R_Text_NicolasPostBattle:
	.string "The road to the top is long and brutal.\p"
	.string "When will my POKéMON and I be the best?$"

MeteorFalls_1F_2R_Text_NicolasRegister:
	.string "I want to see more of your power.\n"
	.string "Let me register you in my POKéNAV.$"

MeteorFalls_1F_2R_Text_NicolasRematchIntro:
	.string "Since we met, we've trained day and\n"
	.string "night with our eyes on the top.\p"
	.string "Help us see how much stronger we've\n"
	.string "become!$"

MeteorFalls_1F_2R_Text_NicolasRematchDefeat:
	.string "Gah!\n"
	.string "You got even stronger than before!$"

MeteorFalls_1F_2R_Text_NicolasPostRematch:
	.string "I can tell you've kept training your\n"
	.string "POKéMON.\p"
	.string "As long as you keep rising, I can keep\n"
	.string "rising too!$"

MeteorFalls_1F_2R_Text_JohnIntro:
	.string "JOHN: We've always fought side by side\n"
	.string "as a pair. We trust each other\l"
	.string "completely.$"

MeteorFalls_1F_2R_Text_JohnDefeat:
	.string "JOHN: Oh my.\n"
	.string "We lost, dear wife.$"

MeteorFalls_1F_2R_Text_JohnPostBattle:
	.string "JOHN: Fifty years of marriage, and I've\n"
	.string "yet to beat my dear wife in a fight.\p"
	.string "I doubt I ever will.$"

MeteorFalls_1F_2R_Text_JohnNotEnoughMons:
	.string "JOHN: Well, well, such a young TRAINER!\p"
	.string "You'd like to fight us? Then you'll need\n"
	.string "to come back with more POKéMON.$"

MeteorFalls_1F_2R_Text_JohnRegister:
	.string "JOHN: Young TRAINER, if the chance\n"
	.string "comes, will you fight us again?$"

MeteorFalls_1F_2R_Text_JayIntro:
	.string "JAY: We've been married for fifty years.\p"
	.string "No fight in the world could break the\n"
	.string "bond we share.$"

MeteorFalls_1F_2R_Text_JayDefeat:
	.string "JAY: Oh dear.\n"
	.string "We lost, dear husband.$"

MeteorFalls_1F_2R_Text_JayPostBattle:
	.string "JAY: Fifty years of marriage…\p"
	.string "Whenever we argued, we settled it with a\n"
	.string "POKéMON battle…$"

MeteorFalls_1F_2R_Text_JayNotEnoughMons:
	.string "JAY: Well, well, what a young TRAINER!\p"
	.string "If you'd like to fight us, come back with\n"
	.string "more POKéMON.$"

MeteorFalls_1F_2R_Text_JohnRematchIntro:
	.string "JOHN: We've always fought side by side\n"
	.string "as a pair. We trust each other\l"
	.string "completely.$"

MeteorFalls_1F_2R_Text_JohnRematchDefeat:
	.string "JOHN: Oh my.\n"
	.string "We lost, dear wife.$"

MeteorFalls_1F_2R_Text_JohnPostRematch:
	.string "JOHN: Fifty years of marriage…\p"
	.string "Come to think of it, my wife and I have\n"
	.string "sparred day in and day out all that\l"
	.string "time…$"

MeteorFalls_1F_2R_Text_JohnRematchNotEnoughMons:
	.string "JOHN: Well, well, such a young TRAINER!\p"
	.string "You'd like to fight us? Then you'll need\n"
	.string "to come back with more POKéMON.$"

MeteorFalls_1F_2R_Text_JayRematchIntro:
	.string "JAY: We've been married for fifty years.\p"
	.string "We've held each other up all that time,\n"
	.string "and it's made us strong.$"

MeteorFalls_1F_2R_Text_JayRematchDefeat:
	.string "JAY: Oh dear.\n"
	.string "We lost, dear husband.$"

MeteorFalls_1F_2R_Text_JayPostRematch:
	.string "JAY: Fifty years of marriage…\n"
	.string "So much has happened.\p"
	.string "I hope we keep making happy memories\n"
	.string "together.$"

MeteorFalls_1F_2R_Text_JayRematchNotEnoughMons:
	.string "JAY: Well, well, what a young TRAINER!\p"
	.string "If you'd like to fight us, come back with\n"
	.string "more POKéMON.$"

