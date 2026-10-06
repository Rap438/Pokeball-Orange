@ PokeBall Orange: Dragon Balls and Shenron

EventScript_DBZ_DragonBall::
	lock
	specialvar VAR_0x8004, DBZ_GetTalkedDragonBallItem
	bufferitemname STR_VAR_2, VAR_0x8004
	playfanfare MUS_OBTAIN_ITEM
	removeobject VAR_LAST_TALKED
	special DBZ_CollectTalkedDragonBall
	additem VAR_0x8004
	message Text_DBZ_FoundDragonBall
	waitfanfare
	waitmessage
	waitbuttonpress
	specialvar VAR_RESULT, DBZ_CountDragonBalls
	goto_if_eq VAR_RESULT, 7, EventScript_DBZ_AllDragonBalls
	buffernumberstring STR_VAR_1, VAR_RESULT
	msgbox Text_DBZ_DragonBallCount, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_AllDragonBalls::
	msgbox Text_DBZ_AllDragonBalls, MSGBOX_DEFAULT
	release
	end

EventScript_DBZ_SummonShenron::
	lockall
	msgbox Text_DBZ_Summon, MSGBOX_DEFAULT
	closemessage
	special DBZ_ShenronAppear
	waitstate
	msgbox Text_DBZ_ShenronSpeaks, MSGBOX_DEFAULT
EventScript_DBZ_ChooseWish::
	message Text_DBZ_NameYourWish
	waitmessage
	multichoice 17, 0, MULTI_DBZ_WISH, FALSE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, EventScript_DBZ_WishCancel
	goto_if_eq VAR_RESULT, 0, EventScript_DBZ_ChoosePokemonWish
	copyvar VAR_0x8005, VAR_RESULT
	goto EventScript_DBZ_PowerWish

EventScript_DBZ_ChoosePokemonWish::
	message Text_DBZ_NameYourPokemon
	waitmessage
	special DBZ_ChooseWish
	waitstate
	goto_if_eq VAR_RESULT, SPECIES_NONE, EventScript_DBZ_WishCancel
	copyvar VAR_0x8006, VAR_RESULT
	bufferspeciesname STR_VAR_1, VAR_0x8006
	msgbox Text_DBZ_ConfirmWish, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DBZ_ChooseWish
	specialvar VAR_RESULT, DBZ_GrantWish
	copyvar VAR_0x8007, VAR_RESULT   @ keep it: the messages below can clobber VAR_RESULT
	goto_if_eq VAR_0x8007, MON_CANT_GIVE, EventScript_DBZ_WishNoRoom
	bufferspeciesname STR_VAR_1, VAR_0x8006
	msgbox Text_DBZ_WishGranted, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message Text_DBZ_ReceivedWish
	waitfanfare
	waitmessage
	waitbuttonpress
	call_if_eq VAR_0x8007, MON_GIVEN_TO_PC, EventScript_DBZ_WishSentToPC
	msgbox Text_DBZ_Farewell, MSGBOX_DEFAULT
	closemessage
	special DBZ_ShenronDepart
	waitstate
	msgbox Text_DBZ_BallsScatter, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DBZ_WishSentToPC::
	msgbox Text_DBZ_WishSentToPC, MSGBOX_DEFAULT
	return

EventScript_DBZ_WishNoRoom::
	msgbox Text_DBZ_WishNoRoom, MSGBOX_DEFAULT
	goto EventScript_DBZ_WishLeave

EventScript_DBZ_WishCancel::
	msgbox Text_DBZ_WishCancel, MSGBOX_DEFAULT
EventScript_DBZ_WishLeave::
	closemessage
	special DBZ_ShenronDepart
	waitstate
	releaseall
	end

Text_DBZ_FoundDragonBall:
	.string "{PLAYER} found the {STR_VAR_2}!$"

Text_DBZ_DragonBallCount:
	.string "That makes {STR_VAR_1} of the seven\n"
	.string "DRAGON BALLS.$"

Text_DBZ_AllDragonBalls:
	.string "All seven DRAGON BALLS are glowing!\p"
	.string "Use one from the BAG outdoors to\n"
	.string "summon the Eternal Dragon!$"

Text_DBZ_Summon:
	.string "Eternal Dragon, by your name,\n"
	.string "I summon you forth…\p"
	.string "SHENRON!$"

Text_DBZ_ShenronSpeaks:
	.string "SHENRON: I am the Eternal Dragon.\p"
	.string "You who have gathered the seven\n"
	.string "DRAGON BALLS…\p"
	.string "I shall grant you one wish.\n"
	.string "Any POKéMON in the world, power\l"
	.string "beyond measure, or riches…$"

Text_DBZ_NameYourWish:
	.string "Now, speak your wish!$"

Text_DBZ_NameYourPokemon:
	.string "Which POKéMON do you wish for?$"

Text_DBZ_ConfirmWish:
	.string "SHENRON: You wish for\n"
	.string "{STR_VAR_1}?$"

Text_DBZ_WishGranted:
	.string "SHENRON: Your wish has been\n"
	.string "granted!$"

Text_DBZ_ReceivedWish:
	.string "{PLAYER} received {STR_VAR_1}!$"

Text_DBZ_WishSentToPC:
	.string "There was no room in the party,\n"
	.string "so {STR_VAR_1} was sent to the PC.$"

Text_DBZ_WishNoRoom:
	.string "SHENRON: You have no room for\n"
	.string "another POKéMON!\p"
	.string "Make room, then call me again.$"

Text_DBZ_WishCancel:
	.string "SHENRON: Then I shall rest.\n"
	.string "Call me when your wish is clear.$"

Text_DBZ_Farewell:
	.string "SHENRON: Farewell!$"

Text_DBZ_BallsScatter:
	.string "The DRAGON BALLS turned to stone\n"
	.string "and flew off across HOENN!\p"
	.string "After a long journey, they'll glow\n"
	.string "again somewhere new…$"
