SootopolisCity_LotadAndSeedotHouse_MapScripts::
	.byte 0

SootopolisCity_LotadAndSeedotHouse_EventScript_SeedotBrother::
	special GetSeedotSizeRecordInfo
	lock
	faceplayer
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigSeedot, MSGBOX_DEFAULT
	setvar VAR_0x8009, SPECIES_SEEDOT
	chooseboxmon SELECT_PC_MON_TRADE
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowSeedot
	special CompareSeedotSize
	goto_if_eq VAR_RESULT, COMPARE_SIZE_INCORRECT_SPECIES, SootopolisCity_LotadAndSeedotHouse_EventScript_NotSeedot
	goto_if_eq VAR_RESULT, COMPARE_SIZE_SMALLER, SootopolisCity_LotadAndSeedotHouse_EventScript_SmallSeedot
	goto_if_eq VAR_RESULT, COMPARE_SIZE_LARGER, SootopolisCity_LotadAndSeedotHouse_EventScript_BigSeedot
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigSeedot, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NotSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotSeedot, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_SmallSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerSeedot, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_BigSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_GoshMightBeBiggerThanLotad, MSGBOX_DEFAULT
	giveitem ITEM_ELIXIR
	goto_if_eq VAR_RESULT, FALSE, SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir1
	closemessage
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir1::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull1, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_LotadBrother::
	special GetLotadSizeRecordInfo
	lock
	faceplayer
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigLotad, MSGBOX_DEFAULT
	setvar VAR_0x8009, SPECIES_LOTAD
	chooseboxmon SELECT_PC_MON_TRADE
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowLotad
	special CompareLotadSize
	goto_if_eq VAR_RESULT, COMPARE_SIZE_INCORRECT_SPECIES, SootopolisCity_LotadAndSeedotHouse_EventScript_NotLotad
	goto_if_eq VAR_RESULT, COMPARE_SIZE_SMALLER, SootopolisCity_LotadAndSeedotHouse_EventScript_SmallLotad
	goto_if_eq VAR_RESULT, COMPARE_SIZE_LARGER, SootopolisCity_LotadAndSeedotHouse_EventScript_BigLotad
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigLotad, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NotLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotLotad, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_SmallLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerLotad, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_BigLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_WowMightBeBiggerThanSeedot, MSGBOX_DEFAULT
	giveitem ITEM_ELIXIR
	goto_if_eq VAR_RESULT, FALSE, SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir2
	closemessage
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir2::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull2, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_SeedotSizeRecord::
	special GetSeedotSizeRecordInfo
	lockall
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BiggestSeedotInHistory, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_LotadSizeRecord::
	special GetLotadSizeRecordInfo
	lockall
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BiggestLotadInHistory, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigSeedot:
	.string "Do you know the POKéMON SEEDOT? It's\n"
	.string "hardly ever seen in SOOTOPOLIS.\p"
	.string "Anyway, I love big SEEDOT. The bigger\n"
	.string "the better.\p"
	.string "But my younger brother says LOTAD is\n"
	.string "bigger.\p"
	.string "That's silly. SEEDOT has to be bigger\n"
	.string "than that!\p"
	.string "Huh? Do you have a SEEDOT with you?\n"
	.string "P-p-please, show me!$"

SootopolisCity_LotadAndSeedotHouse_Text_GoshMightBeBiggerThanLotad:
	.string "{STR_VAR_2}!\n"
	.string "Oh, my gosh, this is a big one!\p"
	.string "It might even beat the big LOTAD my\n"
	.string "younger brother saw!\p"
	.string "Thanks for showing me. This is my\n"
	.string "thanks!$"

@ Unused
SootopolisCity_LotadAndSeedotHouse_Text_ReceivedPotion1:
	.string "{PLAYER} received a SENZU SPROUT.$"

SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull1:
	.string "Hunh?\p"
	.string "Your BAG is crammed full.$"

SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerSeedot:
	.string "{STR_VAR_2}, is it?\p"
	.string "Hmm… I've seen a bigger SEEDOT\n"
	.string "than this one.$"

SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotSeedot:
	.string "Oh, now this is quite something…\p"
	.string "But it's not a SEEDOT!$"

SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigSeedot:
	.string "You don't have a big SEEDOT? That's too\n"
	.string "bad…\p"
	.string "If you get a big SEEDOT, please come\n"
	.string "show me.$"

SootopolisCity_LotadAndSeedotHouse_Text_BiggestSeedotInHistory:
	.string "The biggest SEEDOT in history!\n"
	.string "{STR_VAR_2}'s {STR_VAR_3} giant!\p"
	.string "A SEEDOT bigger than a LOTAD\n"
	.string "always wanted!$"

SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigLotad:
	.string "Do you know the POKéMON LOTAD? It's\n"
	.string "rarely seen in SOOTOPOLIS.\p"
	.string "I love, I mean love, big LOTAD!\p"
	.string "My big brother says SEEDOT is bigger.\p"
	.string "But that's wrong! Everyone knows LOTAD\n"
	.string "is a lot bigger.\p"
	.string "Hunh? Do you have a LOTAD? P-p-please\n"
	.string "show me!$"

SootopolisCity_LotadAndSeedotHouse_Text_WowMightBeBiggerThanSeedot:
	.string "{STR_VAR_2}!\n"
	.string "Wow, that is big!\p"
	.string "It might be even bigger than the huge\n"
	.string "SEEDOT my big brother saw.\p"
	.string "Thanks for showing me! This is my\n"
	.string "thanks!$"

@ Unused
SootopolisCity_LotadAndSeedotHouse_Text_ReceivedPotion2:
	.string "{PLAYER} received a SENZU SPROUT.$"

SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull2:
	.string "Hunh?\p"
	.string "Your BAG is crammed full.$"

SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerLotad:
	.string "{STR_VAR_2}?\p"
	.string "Hmm… I've seen a bigger LOTAD\n"
	.string "than this one here.$"

SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotLotad:
	.string "Well, isn't this something!\p"
	.string "But it's no LOTAD!$"

SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigLotad:
	.string "Don't you have a big LOTAD? How\n"
	.string "disappointing…\p"
	.string "If you get a big LOTAD, please come\n"
	.string "show me!$"

SootopolisCity_LotadAndSeedotHouse_Text_BiggestLotadInHistory:
	.string "The biggest LOTAD in history!\n"
	.string "{STR_VAR_2}'s {STR_VAR_3} colossus!\p"
	.string "A LOTAD bigger than a SEEDOT\n"
	.string "always wanted!$"

