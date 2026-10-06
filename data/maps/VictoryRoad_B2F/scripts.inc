VictoryRoad_B2F_MapScripts::
	.byte 0

VictoryRoad_B2F_EventScript_Vito::
	trainerbattle_single TRAINER_VITO, VictoryRoad_B2F_Text_VitoIntro, VictoryRoad_B2F_Text_VitoDefeat
	msgbox VictoryRoad_B2F_Text_VitoPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Owen::
	trainerbattle_single TRAINER_OWEN, VictoryRoad_B2F_Text_OwenIntro, VictoryRoad_B2F_Text_OwenDefeat
	msgbox VictoryRoad_B2F_Text_OwenPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Caroline::
	trainerbattle_single TRAINER_CAROLINE, VictoryRoad_B2F_Text_CarolineIntro, VictoryRoad_B2F_Text_CarolineDefeat
	msgbox VictoryRoad_B2F_Text_CarolinePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Julie::
	trainerbattle_single TRAINER_JULIE, VictoryRoad_B2F_Text_JulieIntro, VictoryRoad_B2F_Text_JulieDefeat
	msgbox VictoryRoad_B2F_Text_JuliePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Felix::
	trainerbattle_single TRAINER_FELIX, VictoryRoad_B2F_Text_FelixIntro, VictoryRoad_B2F_Text_FelixDefeat
	msgbox VictoryRoad_B2F_Text_FelixPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Dianne::
	trainerbattle_single TRAINER_DIANNE, VictoryRoad_B2F_Text_DianneIntro, VictoryRoad_B2F_Text_DianneDefeat
	msgbox VictoryRoad_B2F_Text_DiannePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_Text_VitoIntro:
	.string "I trained together with my whole family,\n"
	.string "every last one of us!\p"
	.string "I'm not losing to anyone!$"

VictoryRoad_B2F_Text_VitoDefeat:
	.string "Stronger than my whole family?!\n"
	.string "Is that even possible?!$"

VictoryRoad_B2F_Text_VitoPostBattle:
	.string "I was the best in my family. I'd never\n"
	.string "lost before…\p"
	.string "I think I've lost my confidence…\n"
	.string "Maybe I'll go home…$"

VictoryRoad_B2F_Text_OwenIntro:
	.string "I heard there's a tough spiky-haired\n"
	.string "guy around. Is that you?$"

VictoryRoad_B2F_Text_OwenDefeat:
	.string "That guy is seriously tough!$"

VictoryRoad_B2F_Text_OwenPostBattle:
	.string "The rumor I heard says that tough guy\n"
	.string "is best friends with PETALBURG CITY's\l"
	.string "GYM LEADER.$"

VictoryRoad_B2F_Text_CarolineIntro:
	.string "You must be getting a little tired by\n"
	.string "now.$"

VictoryRoad_B2F_Text_CarolineDefeat:
	.string "No signs of tiring at all!$"

VictoryRoad_B2F_Text_CarolinePostBattle:
	.string "VICTORY ROAD and the POKéMON LEAGUE\n"
	.string "are long, grueling challenges.\p"
	.string "Beware of fatigue! Even a SAIYAN has to\n"
	.string "eat and rest.$"

VictoryRoad_B2F_Text_JulieIntro:
	.string "Don't get complacent just because you\n"
	.string "have lots of GYM BADGES.\p"
	.string "There's always someone out there who's\n"
	.string "stronger than you!$"

VictoryRoad_B2F_Text_JulieDefeat:
	.string "You're stronger than me!$"

VictoryRoad_B2F_Text_JuliePostBattle:
	.string "Look over your BADGES and remember\n"
	.string "every fighter you've faced to get here.$"

VictoryRoad_B2F_Text_FelixIntro:
	.string "I've come this far, but the tension is\n"
	.string "giving me an awful stomachache…$"

VictoryRoad_B2F_Text_FelixDefeat:
	.string "Ooh…\n"
	.string "That hurts…$"

VictoryRoad_B2F_Text_FelixPostBattle:
	.string "I can't help getting tense knowing I'm\n"
	.string "close to the POKéMON LEAGUE.\p"
	.string "All I can do is pretend to stay calm.$"

VictoryRoad_B2F_Text_DianneIntro:
	.string "The elite among the elite gather in this\n"
	.string "cave.\p"
	.string "How are you finding it?$"

VictoryRoad_B2F_Text_DianneDefeat:
	.string "Not rattled in the least!$"

VictoryRoad_B2F_Text_DiannePostBattle:
	.string "You've got guts! I like that!\n"
	.string "Keep it going!$"

