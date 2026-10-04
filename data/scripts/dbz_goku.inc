@ PokeBall Orange: GOKU menu (START), technique announcements, gravity room

EventScript_DBZ_GokuMenu::
	lockall
	multichoice 17, 0, MULTI_DBZ_GOKU, FALSE
	switch VAR_RESULT
	case 0, EventScript_DBZ_GokuStatus
	case 1, EventScript_DBZ_GokuInstantTrans
	case 2, EventScript_DBZ_GokuHideKi
	releaseall
	end

EventScript_DBZ_GokuStatus::
	special DBZ_ShowGokuStatus
	msgbox Text_DBZ_GokuStatus, MSGBOX_DEFAULT
	specialvar VAR_RESULT, DBZ_GravityStepsLeft
	goto_if_eq VAR_RESULT, 0, EventScript_DBZ_GokuMenuEnd
	buffernumberstring STR_VAR_1, VAR_RESULT
	msgbox Text_DBZ_GravityLeft, MSGBOX_DEFAULT
EventScript_DBZ_GokuMenuEnd::
	releaseall
	end

EventScript_DBZ_GokuInstantTrans::
	specialvar VAR_RESULT, DBZ_CanUseInstantTransmission
	goto_if_eq VAR_RESULT, 0, EventScript_DBZ_ITNotLearned
	goto_if_eq VAR_RESULT, 1, EventScript_DBZ_ITCantHere
	special DBZ_QueueInstantTransmission
	releaseall
	end

EventScript_DBZ_ITNotLearned::
	msgbox Text_DBZ_ITNotLearned, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_ITCantHere::
	msgbox Text_DBZ_ITCantHere, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_GokuHideKi::
	specialvar VAR_RESULT, DBZ_ToggleHideKi
	goto_if_eq VAR_RESULT, 0, EventScript_DBZ_KiShown
	msgbox Text_DBZ_KiHidden, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_KiShown::
	msgbox Text_DBZ_KiShown, MSGBOX_DEFAULT
	releaseall
	end

@ called from the gym fight script after the form hints
EventScript_DBZ_TechniqueHints::
	specialvar VAR_0x8009, DBZ_GetNewTechniqueHint
	goto_if_eq VAR_0x8009, 0, EventScript_DBZ_TechReturn
	call_if_eq VAR_0x8009, DBZ_TECH_KAIOKEN, EventScript_DBZ_HintKaioken
	call_if_eq VAR_0x8009, DBZ_TECH_INSTANT_TRANS, EventScript_DBZ_HintIT
	call_if_eq VAR_0x8009, DBZ_TECH_SPIRIT_BOMB, EventScript_DBZ_HintSpiritBomb
	goto EventScript_DBZ_TechniqueHints
EventScript_DBZ_TechReturn::
	return

EventScript_DBZ_HintKaioken::
	playfanfare MUS_OBTAIN_TMHM
	message Text_DBZ_HintKaioken
	waitfanfare
	waitmessage
	waitbuttonpress
	return

EventScript_DBZ_HintIT::
	playfanfare MUS_OBTAIN_TMHM
	message Text_DBZ_HintIT
	waitfanfare
	waitmessage
	waitbuttonpress
	return

EventScript_DBZ_HintSpiritBomb::
	playfanfare MUS_OBTAIN_TMHM
	message Text_DBZ_HintSpiritBomb
	waitfanfare
	waitmessage
	waitbuttonpress
	return

Text_DBZ_GokuStatus:
	.string "GOKU  Lv. {STR_VAR_1}   HP {STR_VAR_3}\n"
	.string "{STR_VAR_2} EXP. to the next level.$"

Text_DBZ_GravityLeft:
	.string "10x GRAVITY training:\n"
	.string "{STR_VAR_1} steps left.$"

Text_DBZ_ITNotLearned:
	.string "GOKU hasn't learned INSTANT\n"
	.string "TRANSMISSION yet…$"

Text_DBZ_ITCantHere:
	.string "GOKU can't lock onto anyone's ki\n"
	.string "from here.$"

Text_DBZ_KiHidden:
	.string "GOKU hid his ki.\p"
	.string "Weak wild POKéMON and RED RIBBON\n"
	.string "soldiers won't notice him now.$"

Text_DBZ_KiShown:
	.string "GOKU stopped hiding his ki.$"

Text_DBZ_HintKaioken:
	.string "KING KAI's voice echoes in GOKU's\n"
	.string "head: “Time you learned the KAIO-KEN!”\p"
	.string "In an overworld fight, press SELECT\n"
	.string "to power up x1.5 for a short time.\l"
	.string "It strains the body, so watch your HP!$"

Text_DBZ_HintIT:
	.string "GOKU learned INSTANT TRANSMISSION!\p"
	.string "Choose GOKU in the START menu to\n"
	.string "teleport to any town he's visited.$"

Text_DBZ_HintSpiritBomb:
	.string "KING KAI: GOKU! Raise your hands and\n"
	.string "gather energy from all living things!\p"
	.string "GOKU learned the SPIRIT BOMB!\p"
	.string "Hold L and tap R to select it, then\n"
	.string "hold L until it's fully charged.$"
