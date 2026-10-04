Route111_WinstrateFamilysHouse_MapScripts::
	.byte 0

Route111_WinstrateFamilysHouse_EventScript_Victor::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICTOR
	msgbox Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Victoria::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICTORIA
	goto_if_set FLAG_RECEIVED_MACHO_BRACE, Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace
	msgbox Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace, MSGBOX_DEFAULT
	giveitem ITEM_MACHO_BRACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MACHO_BRACE
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace::
	msgbox Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vivi::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VIVI
	msgbox Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vicky::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICKY
	goto_if_set FLAG_TEMP_4, Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrong, MSGBOX_DEFAULT
	setflag FLAG_TEMP_4
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo::
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection::
	closemessage
	applywaitmovement VAR_0x8008, Common_Movement_FaceOriginalDirection
	release
	end

Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou:
	.string "Not bad at all! I've rarely seen a\n"
	.string "TRAINER command POKéMON that well.\p"
	.string "But I'll tell you something: my son is\n"
	.string "stronger than you.\p"
	.string "He even took on the POKéMON LEAGUE,\n"
	.string "you know.$"

Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace:
	.string "We use this MACHO BRACE to make our\n"
	.string "POKéMON's training harder, and the\l"
	.string "results bigger. It's like the heavy\l"
	.string "weighted gear martial artists wear.\p"
	.string "Since you beat all of us, you probably\n"
	.string "don't need it, but we'd like you to have\l"
	.string "it.$"

Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles:
	.string "When a battle starts in this family, we\n"
	.string "all get fired up.\p"
	.string "It's how we show our love.$"

Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers:
	.string "Mommy beats Daddy.\p"
	.string "I beat Mommy.\p"
	.string "And Grandma beats me!\p"
	.string "But my big brother beats Grandma, too!$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrong:
	.string "There's no question you're strong.\p"
	.string "But if you battled my grandson, you'd\n"
	.string "end up crying in frustration.\p"
	.string "He's far stronger than any TRAINER our\n"
	.string "family knows.\p"
	.string "He must be challenging the POKéMON\n"
	.string "LEAGUE CHAMPION by now.\p"
	.string "Knowing my grandson, he might have\n"
	.string "knocked MR. SATAN off the throne\l"
	.string "already!$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort:
	.string "My grandson must be challenging the\n"
	.string "POKéMON LEAGUE CHAMPION by now.\p"
	.string "Knowing him, he might be the CHAMPION\n"
	.string "already!$"

