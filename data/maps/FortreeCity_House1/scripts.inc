FortreeCity_House1_MapScripts::
	.byte 0

FortreeCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_FORTREE_NPC_TRADE_COMPLETED, FortreeCity_House1_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_PLUSLE, FortreeCity_House1_Text_YouWillTradeWontYou, FortreeCity_House1_EventScript_DeclineTrade, FortreeCity_House1_EventScript_NotRequestedMon, FortreeCity_House1_Text_MonYouTakeCare
	setflag FLAG_FORTREE_NPC_TRADE_COMPLETED
	release
	end

FortreeCity_House1_EventScript_DeclineTrade::
	msgbox FortreeCity_House1_Text_YouWontTradeMe, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox FortreeCity_House1_Text_ThisIsntAMon, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_TradeCompleted::
	msgbox FortreeCity_House1_Text_GoingToMakeVolbeatStrong, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_ExpertF::
	msgbox FortreeCity_House1_Text_TradingMemoriesWithOthers, MSGBOX_NPC
	end

FortreeCity_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox FortreeCity_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FortreeCity_House1_Text_YouWillTradeWontYou:
	.string "Grooooaaar! I'm starving for it! I have\n"
	.string "to get me a {STR_VAR_1}! I'd trade my whole\l"
	.string "dinner for one!\p"
	.string "…Uh… Did you hear that? That was my\n"
	.string "shout from the bottom of my heart. Well,\l"
	.string "my stomach, too.\p"
	.string "Having heard that, you will trade your\n"
	.string "{STR_VAR_1} for my {STR_VAR_2}, won't you?$"

FortreeCity_House1_Text_MonYouTakeCare:
	.string "Yeah! Right on!\p"
	.string "{STR_VAR_1}, welcome home!\n"
	.string "{STR_VAR_2}, be good out there!$"

FortreeCity_House1_Text_ThisIsntAMon:
	.string "Hmm, nope.\n"
	.string "That's not a {STR_VAR_1}.$"

FortreeCity_House1_Text_YouWontTradeMe:
	.string "What? You won't trade?\n"
	.string "After I bared my soul like that?$"

FortreeCity_House1_Text_GoingToMakeVolbeatStrong:
	.string "I'm going to train this VOLBEAT until\n"
	.string "its power level goes through the roof!\p"
	.string "I hope you do the same with that\n"
	.string "PLUSLE!$"

FortreeCity_House1_Text_TradingMemoriesWithOthers:
	.string "Trading POKéMON with someone else…\p"
	.string "It's like trading memories with them.\n"
	.string "That's what a bond between fighters is.$"

FortreeCity_House1_Text_Zigzagoon:
	.string "ZIGZAGOON: Gumomoh?$"

