VictoryRoad_B1F_MapScripts::
	.byte 0

VictoryRoad_B1F_EventScript_Samuel::
	trainerbattle_single TRAINER_SAMUEL, VictoryRoad_B1F_Text_SamuelIntro, VictoryRoad_B1F_Text_SamuelDefeat
	msgbox VictoryRoad_B1F_Text_SamuelPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Shannon::
	trainerbattle_single TRAINER_SHANNON, VictoryRoad_B1F_Text_ShannonIntro, VictoryRoad_B1F_Text_ShannonDefeat
	msgbox VictoryRoad_B1F_Text_ShannonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Michelle::
	trainerbattle_single TRAINER_MICHELLE, VictoryRoad_B1F_Text_MichelleIntro, VictoryRoad_B1F_Text_MichelleDefeat
	msgbox VictoryRoad_B1F_Text_MichellePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Mitchell::
	trainerbattle_single TRAINER_MITCHELL, VictoryRoad_B1F_Text_MitchellIntro, VictoryRoad_B1F_Text_MitchellDefeat
	msgbox VictoryRoad_B1F_Text_MitchellPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Halle::
	trainerbattle_single TRAINER_HALLE, VictoryRoad_B1F_Text_HalleIntro, VictoryRoad_B1F_Text_HalleDefeat
	msgbox VictoryRoad_B1F_Text_HallePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_Text_SamuelIntro:
	.string "Knowing I'm getting closer to the\n"
	.string "POKéMON LEAGUE…\p"
	.string "I'm getting stage fright…$"

VictoryRoad_B1F_Text_SamuelDefeat:
	.string "I couldn't do a thing…$"

VictoryRoad_B1F_Text_SamuelPostBattle:
	.string "The POKéMON LEAGUE slipped out of\n"
	.string "reach again… What a letdown…$"

VictoryRoad_B1F_Text_ShannonIntro:
	.string "To fight your way through the POKéMON\n"
	.string "LEAGUE, you need your POKéMON's trust.$"

VictoryRoad_B1F_Text_ShannonDefeat:
	.string "Your bond is built on solid trust.$"

VictoryRoad_B1F_Text_ShannonPostBattle:
	.string "Training side by side every day is how\n"
	.string "trust grows between POKéMON and\l"
	.string "TRAINERS.$"

VictoryRoad_B1F_Text_MichelleIntro:
	.string "This isn't the goal. It's only a stop on\n"
	.string "the way to the POKéMON LEAGUE.$"

VictoryRoad_B1F_Text_MichelleDefeat:
	.string "That's the spirit!$"

VictoryRoad_B1F_Text_MichellePostBattle:
	.string "You'll do fine, for sure!\n"
	.string "Your POKéMON are all raring to go!$"

VictoryRoad_B1F_Text_MitchellIntro:
	.string "My POKéMON are cosmically\n"
	.string "awe-inspiring!$"

VictoryRoad_B1F_Text_MitchellDefeat:
	.string "I've never met anyone like you before.$"

VictoryRoad_B1F_Text_MitchellPostBattle:
	.string "Even outside of battle, I can sense\n"
	.string "incredible power rising off you and your\l"
	.string "POKéMON.$"

VictoryRoad_B1F_Text_HalleIntro:
	.string "Okay, no need to get all fired up!\n"
	.string "Relax, let's take it easy!$"

VictoryRoad_B1F_Text_HalleDefeat:
	.string "Whoa!\n"
	.string "Wonderful!$"

VictoryRoad_B1F_Text_HallePostBattle:
	.string "Sure, this is VICTORY ROAD.\p"
	.string "But it's not all that different from the\n"
	.string "road you've already traveled.\p"
	.string "Try to enjoy yourself the rest of the\n"
	.string "way!$"

