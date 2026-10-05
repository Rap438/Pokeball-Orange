RustboroCity_House1_MapScripts::
	.byte 0

RustboroCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_RUSTBORO_NPC_TRADE_COMPLETED, RustboroCity_House1_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_SEEDOT, RustboroCity_House1_Text_IllTradeIfYouWant, RustboroCity_House1_EventScript_DeclineTrade, RustboroCity_House1_EventScript_NotRequestedMon, RustboroCity_House1_Text_PleaseBeGoodToMyPokemon
	setflag FLAG_RUSTBORO_NPC_TRADE_COMPLETED
	release
	end

RustboroCity_House1_EventScript_DeclineTrade::
	msgbox RustboroCity_House1_Text_YouDontWantToThatsOkay, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox RustboroCity_House1_Text_DoesntLookLikeMonToMe, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_TradeCompleted::
	msgbox RustboroCity_House1_Text_AnyPokemonCanBeCute, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_Hiker::
	msgbox RustboroCity_House1_Text_AllSortsOfPlaces, MSGBOX_NPC
	end

RustboroCity_House1_Text_IllTradeIfYouWant:
	.string "Huh? You think my POKéMON is cute?\n"
	.string "Sure, I knew that.\p"
	.string "But if you really want it, I'm willing to\n"
	.string "trade it to you.\p"
	.string "I'll trade you my {STR_VAR_2} for a {STR_VAR_1},\n"
	.string "if you like.$"

RustboroCity_House1_Text_PleaseBeGoodToMyPokemon:
	.string "Eheheh…\n"
	.string "Please be good to my POKéMON.$"

RustboroCity_House1_Text_DoesntLookLikeMonToMe:
	.string "Huh? That doesn't look anything like a\n"
	.string "{STR_VAR_1} to me.$"

RustboroCity_House1_Text_YouDontWantToThatsOkay:
	.string "Oh, if you don't want to, that's okay.\n"
	.string "But my POKéMON is cute, you know…$"

RustboroCity_House1_Text_AnyPokemonCanBeCute:
	.string "Any POKéMON can be cute if you raise it\n"
	.string "with care and kindness.$"

RustboroCity_House1_Text_AllSortsOfPlaces:
	.string "Wherever you go, there are all sorts of\n"
	.string "POKéMON and people.\p"
	.string "I find that fascinating, so I travel to\n"
	.string "all sorts of places.$"

