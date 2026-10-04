MauvilleCity_House2_MapScripts::
	.byte 0

MauvilleCity_House2_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_COIN_CASE, MauvilleCity_House2_EventScript_ReceivedCoinCase
	msgbox MauvilleCity_House2_Text_BuyHarborMailAtSlateport, MSGBOX_DEFAULT
	checkitem ITEM_HARBOR_MAIL
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_House2_EventScript_AskToTradeForHarborMail
	release
	end

MauvilleCity_House2_EventScript_AskToTradeForHarborMail::
	playse SE_PIN
	applywaitmovement VAR_LAST_TALKED, Common_Movement_ExclamationMark
	applywaitmovement VAR_LAST_TALKED, Common_Movement_Delay48
	msgbox MauvilleCity_House2_Text_TradeHarborMailForCoinCase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_House2_EventScript_AcceptTrade
	goto_if_eq VAR_RESULT, NO, MauvilleCity_House2_EventScript_DeclineTrade
	end

MauvilleCity_House2_EventScript_AcceptTrade::
	msgbox MauvilleCity_House2_Text_IllTradeYouCoinCase, MSGBOX_DEFAULT
	removeitem ITEM_HARBOR_MAIL
	giveitem ITEM_COIN_CASE
	setflag FLAG_RECEIVED_COIN_CASE
	goto MauvilleCity_House2_EventScript_ReceivedCoinCase
	end

MauvilleCity_House2_EventScript_ReceivedCoinCase::
	msgbox MauvilleCity_House2_Text_UseCoinCaseAtGameCorner, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House2_EventScript_DeclineTrade::
	msgbox MauvilleCity_House2_Text_ThatsDisappointing, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House2_Text_BuyHarborMailAtSlateport:
	.string "If I had a BIKE, I could pedal to\n"
	.string "SLATEPORT for some shopping.\p"
	.string "I'd buy HARBOR MAIL at the POKé MART in\n"
	.string "SLATEPORT…$"

MauvilleCity_House2_Text_TradeHarborMailForCoinCase:
	.string "Oh! You have HARBOR MAIL?\n"
	.string "Will you trade it to me for a COIN CASE?$"

MauvilleCity_House2_Text_IllTradeYouCoinCase:
	.string "Oh, I'm so happy!\n"
	.string "Okay, here's your COIN CASE!$"

MauvilleCity_House2_Text_UseCoinCaseAtGameCorner:
	.string "You can use that COIN CASE at the GAME\n"
	.string "CORNER.$"

MauvilleCity_House2_Text_ThatsDisappointing:
	.string "Oh, that's too bad.\p"
	.string "You need a COIN CASE for the GAME\n"
	.string "CORNER.$"

