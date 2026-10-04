PacifidlogTown_House4_MapScripts::
	.byte 0

PacifidlogTown_House4_EventScript_LittleGirl::
	msgbox PacifidlogTown_House4_Text_SkyPokemon, MSGBOX_NPC
	end

PacifidlogTown_House4_EventScript_Woman::
	msgbox PacifidlogTown_House4_Text_PeopleSawHighFlyingPokemon, MSGBOX_NPC
	end

PacifidlogTown_House4_EventScript_Boy::
	lock
	faceplayer
	msgbox PacifidlogTown_House4_Text_WhereDidYouComeFrom, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PacifidlogTown_House4_EventScript_Yes
	goto_if_eq VAR_RESULT, NO, PacifidlogTown_House4_EventScript_No
	end

PacifidlogTown_House4_EventScript_Yes::
	msgbox PacifidlogTown_House4_Text_YesTown, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House4_EventScript_No::
	msgbox PacifidlogTown_House4_Text_YouHaveToComeFromSomewhere, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House4_Text_PeopleSawHighFlyingPokemon:
	.string "People say they saw a POKéMON flying\n"
	.string "high above HOENN.\p"
	.string "Does it fly around all the time? Doesn't\n"
	.string "it ever need to rest somewhere?$"

PacifidlogTown_House4_Text_SkyPokemon:
	.string "A sky POKéMON!\p"
	.string "A sky POKéMON!$"

PacifidlogTown_House4_Text_WhereDidYouComeFrom:
	.string "Where did you come from?$"

PacifidlogTown_House4_Text_YesTown:
	.string "Yes?\p"
	.string "YES TOWN?\p"
	.string "I've never heard of a place like that.$"

PacifidlogTown_House4_Text_YouHaveToComeFromSomewhere:
	.string "No? That makes no sense. You have to\n"
	.string "come from somewhere.\p"
	.string "Oh! Wait! Don't tell me you came here in\n"
	.string "a space pod!$"

