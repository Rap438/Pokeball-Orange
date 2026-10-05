gText_PkmnTransferredSomeonesPC::
	.string "{STR_VAR_2} was sent over to\n"
	.string "SOMEONE'S PC.\p"
	.string "It was placed in \n"
	.string "BOX “{STR_VAR_1}.”$"

gText_PkmnTransferredLanettesPC::
	.string "{STR_VAR_2} was transferred to\n"
#if IS_FRLG
	.string "BILL'S PC.\p"
#else
	.string "LANETTE'S PC.\p"
#endif
	.string "It was placed in \n"
	.string "BOX “{STR_VAR_1}.”$"

gText_PkmnTransferredSomeonesPCBoxFull::
	.string "BOX “{STR_VAR_3}” on\n"
	.string "SOMEONE'S PC was full.\p"
	.string "{STR_VAR_2} was sent to\n"
	.string "BOX “{STR_VAR_1}.”$"

gText_PkmnTransferredLanettesPCBoxFull::
	.string "BOX “{STR_VAR_3}” on\n"
#if IS_FRLG
	.string "BILL'S PC was full.\p"
#else
	.string "LANETTE'S PC was full.\p"
#endif
	.string "{STR_VAR_2} was transferred to\n"
	.string "BOX “{STR_VAR_1}.”$"

gText_PkmnSentToPCAfterCatch::
	.string "{STR_VAR_2} was sent to\n"
	.string "{B_PC_CREATOR_NAME} PC.\p"
	.string "It was placed in \n"
	.string "BOX “{STR_VAR_1}”.$"

gText_PlayerObtainedTheMon::
	.string "{PLAYER} obtained\n"
	.string "the {STR_VAR_1}!$"

gText_NoMoreRoomForPokemon::
	.string "There's no more room for POKéMON!\p"
	.string "Every POKéMON BOX is packed full\n"
	.string "and can't hold another one!$"

gText_NicknameThisPokemon::
	.string "Want to give this {STR_VAR_1}\n"
	.string "a nickname?$"
