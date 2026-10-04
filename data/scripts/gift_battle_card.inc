MysteryGiftScript_BattleCard::
	setvaddress MysteryGiftScript_BattleCard
	vgoto_if_set FLAG_MYSTERY_GIFT_DONE, MysteryGiftScript_BattleCardInfo
	setorcopyvar VAR_RESULT, GET_CARD_BATTLES_WON
	specialvar VAR_0x8008, GetMysteryGiftCardStat
	vgoto_if_ne VAR_0x8008, REQUIRED_CARD_BATTLES, MysteryGiftScript_BattleCardInfo
	lock
	faceplayer
	vmessage sText_MysteryGiftBattleCountCard_WonPrize
	waitmessage
	waitbuttonpress
	giveitem ITEM_POTION
	release
	setflag FLAG_MYSTERY_GIFT_DONE
	end

MysteryGiftScript_BattleCardInfo:
	lock
	faceplayer
	vmessage sText_MysteryGiftBattleCountCard
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftBattleCountCard:
	.string "Thanks for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "Your BATTLE COUNT CARD logs your\n"
	.string "fight record against TRAINERS\l"
	.string "holding the same CARD.\p"
	.string "Hunt down TRAINERS who carry the\n"
	.string "same CARD as you and throw down!\p"
	.string "You can check the overall\n"
	.string "rankings by reading the NEWS.\p"
	.string "Give it a shot. Rank up and show\n"
	.string "them your power!$"

sText_MysteryGiftBattleCountCard_WonPrize:
	.string "Thanks for using the MYSTERY\n"
	.string "GIFT System.\p"
	.string "Congratulations!\p"
	.string "You've earned a prize for winning\n"
	.string "three battles!\p"
	.string "Keep training and keep fighting.\n"
	.string "There's always a stronger opponent!$"
