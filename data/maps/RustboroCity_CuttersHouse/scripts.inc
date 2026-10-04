RustboroCity_CuttersHouse_MapScripts::
	.byte 0

RustboroCity_CuttersHouse_EventScript_Cutter::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_CUT, RustboroCity_CuttersHouse_EventScript_ExplainCut
	msgbox RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse, MSGBOX_DEFAULT
	giveitem ITEM_HM_CUT
	setflag FLAG_RECEIVED_HM_CUT
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_ExplainCut::
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_Lass::
	msgbox RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees, MSGBOX_NPC
	end

RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse:
	.string "That fire in your eyes…\n"
	.string "That light, balanced stance…\l"
	.string "And your well-trained POKéMON…\p"
	.string "You're obviously a skilled TRAINER!\p"
	.string "No, wait, don't say a word. I can read a\n"
	.string "fighter's strength just by looking, you\l"
	.string "know.\p"
	.string "I'm sure you can put this HIDDEN\n"
	.string "MACHINE to good use.\p"
	.string "No need to be modest or shy. Go on, take\n"
	.string "it!$"

RustboroCity_CuttersHouse_Text_ExplainCut:
	.string "That HIDDEN MACHINE, or HM for short, is\n"
	.string "CUT.\p"
	.string "An HM move is one that POKéMON can use\n"
	.string "outside of battle.\p"
	.string "Any POKéMON that has learned CUT can\n"
	.string "chop down thin trees, as long as the\l"
	.string "TRAINER has earned the STONE BADGE.\p"
	.string "And unlike a TM, an HM can be used more\n"
	.string "than once.$"

RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees:
	.string "When they were expanding the city of\n"
	.string "RUSTBORO, my dad pitched in.\p"
	.string "He had his POKéMON use CUT to clear the\n"
	.string "land of trees.$"

