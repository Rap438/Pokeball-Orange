Roulette_EventScript_Table1::
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_GameCorner_EventScript_NoCoinCase
	setvar VAR_0x8004, 0
	getpokenewsactive POKENEWS_GAME_CORNER
	goto_if_eq VAR_RESULT, FALSE, Roulette_EventScript_Play
	addvar VAR_0x8004, ROULETTE_SPECIAL_RATE
	goto Roulette_EventScript_Play
	end

Roulette_EventScript_Table2::
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_GameCorner_EventScript_NoCoinCase
	setvar VAR_0x8004, 1
	getpokenewsactive POKENEWS_GAME_CORNER
	goto_if_eq VAR_RESULT, FALSE, Roulette_EventScript_Play
	addvar VAR_0x8004, ROULETTE_SPECIAL_RATE
	goto Roulette_EventScript_Play
	end

Roulette_EventScript_Play::
	special PlayRoulette
	end

Roulette_Text_PlayMinimumWagerIsX::
	.string "The minimum bet at this table is\n"
	.string "{STR_VAR_1}. Care to spin the wheel?$"

Roulette_Text_NotEnoughCoins::
	.string "You don't have enough COINS.$"

Roulette_Text_SpecialRateTable::
	.string "This is today's special rate table!$"

Roulette_Text_ControlsInstruction::
	.string "Place your bet with the + Control\n"
	.string "Pad, then press the A Button.$"

Roulette_Text_ItsAHit::
	.string "A hit! Nice read!$"

Roulette_Text_Jackpot::
	.string "Jackpot! Your luck is off the\n"
	.string "scale!$"

Roulette_Text_NothingDoing::
	.string "Nothing doing!$"

Roulette_Text_YouveWonXCoins::
	.string "You won {STR_VAR_1} COINS!$"

Roulette_Text_NoCoinsLeft::
	.string "You're out of COINS…$"

Roulette_Text_KeepPlaying::
	.string "Keep playing?$"

Roulette_Text_BoardWillBeCleared::
	.string "The ROULETTE board will be cleared.$"

@ Unused
Roulette_Text_YouDontHaveACoinCase::
	.string "You don't have a COIN CASE.$"

Roulette_Text_CoinCaseIsFull::
	.string "Your COIN CASE is full!\n"
	.string "Trade COINS in for prizes.$"
