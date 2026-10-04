MtPyre_1F_MapScripts::
	.byte 0

MtPyre_1F_EventScript_CleanseTagWoman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CLEANSE_TAG, MtPyre_1F_EventScript_ReceivedCleanseTag
	msgbox MtPyre_1F_Text_TakeThisForYourOwnGood, MSGBOX_DEFAULT
	giveitem ITEM_CLEANSE_TAG
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_CLEANSE_TAG
	release
	end

MtPyre_1F_EventScript_ReceivedCleanseTag::
	msgbox MtPyre_1F_Text_ExplainCleanseTag, MSGBOX_DEFAULT
	release
	end

MtPyre_1F_EventScript_PokefanF::
	msgbox MtPyre_1F_Text_ComeToPayRespects, MSGBOX_NPC
	end

MtPyre_1F_EventScript_Man::
	msgbox MtPyre_1F_Text_RestingPlaceOfZigzagoon, MSGBOX_NPC
	end

MtPyre_1F_Text_TakeThisForYourOwnGood:
	.string "All sorts of beings wander the slopes\n"
	.string "of MT. PYRE, and not all of them are\l"
	.string "alive…\p"
	.string "There is no telling what may happen.\n"
	.string "Take this. It's for your own good.$"

MtPyre_1F_Text_ExplainCleanseTag:
	.string "Let one of your POKéMON hold that\n"
	.string "CLEANSE TAG.\p"
	.string "It will keep wild POKéMON from bothering\n"
	.string "you.$"

MtPyre_1F_Text_ComeToPayRespects:
	.string "Have you come to honor the spirits of\n"
	.string "POKéMON who have passed on?\p"
	.string "You must care deeply for your partners.\n"
	.string "Even in the Other World, they would know\l"
	.string "it.$"

MtPyre_1F_Text_RestingPlaceOfZigzagoon:
	.string "My dear ZIGZAGOON rests here now.\n"
	.string "I loved it so very much…$"

