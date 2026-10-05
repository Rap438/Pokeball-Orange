PacifidlogTown_House3_MapScripts::
	.byte 0

PacifidlogTown_House3_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED, PacifidlogTown_House3_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_HORSEA, PacifidlogTown_House3_Text_WillingToTradeIt, PacifidlogTown_House3_EventScript_DeclineTrade, PacifidlogTown_House3_EventScript_NotRequestedMon, PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou
	setflag FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED
	release
	end

PacifidlogTown_House3_EventScript_DeclineTrade::
	msgbox PacifidlogTown_House3_Text_NotDesperateOrAnything, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_TradeCompleted::
	msgbox PacifidlogTown_House3_Text_ReallyWantedToGetBagon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_Girl::
	msgbox PacifidlogTown_House3_Text_IsThatAPokedex, MSGBOX_NPC
	end

PacifidlogTown_House3_Text_WillingToTradeIt:
	.string "Check out this {STR_VAR_2}!\p"
	.string "I caught this {STR_VAR_2} yesterday to\n"
	.string "celebrate my birthday!\p"
	.string "Oh, I can see you want it! After all, it's\n"
	.string "priceless!\p"
	.string "Tell you what. I might be willing to\n"
	.string "trade it for a {STR_VAR_1}.$"

PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou:
	.string "Oh, so this is a {STR_VAR_1}?\p"
	.string "It's kind of like a {STR_VAR_2}, and yet it's\n"
	.string "subtly different.\p"
	.string "Thank you!$"

PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon:
	.string "No, no, no! I won't accept anything less\n"
	.string "than a real {STR_VAR_1}!$"

PacifidlogTown_House3_Text_NotDesperateOrAnything:
	.string "Oh, so you're not going through with it?\p"
	.string "That's cool. I'm not desperate to make a\n"
	.string "trade or anything.$"

PacifidlogTown_House3_Text_ReallyWantedToGetBagon:
	.string "I could go looking for one on my own,\n"
	.string "but…\p"
	.string "I really wanted a BAGON that another\n"
	.string "TRAINER caught…$"

PacifidlogTown_House3_Text_IsThatAPokedex:
	.string "Is that a POKéDEX?\p"
	.string "Did you get to meet lots of different\n"
	.string "POKéMON?\p"
	.string "I wish I could travel like you.$"

