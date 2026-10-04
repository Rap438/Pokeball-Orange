BattleFrontier_Lounge2_MapScripts::
	.byte 0

@ This NPC gives hints about a random facility or battle mode.
@ For battle modes he gives generic advice
@ For facilities, depending on how far the player has progressed he will say either
@    The name of the Frontier Brain there
@    The type and description of the 3 pokemon they use in their silver battle
@    The type and description of the 3 pokemon they use in their gold battle
BattleFrontier_Lounge2_EventScript_FrontierManiac::
	lock
	faceplayer
	goto_if_set FLAG_MET_BATTLE_FRONTIER_MANIAC, BattleFrontier_Lounge2_EventScript_AlreadyMetManiac
	setflag FLAG_MET_BATTLE_FRONTIER_MANIAC
	msgbox BattleFrontier_Lounge2_Text_FrontierManiacIntro, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge2_EventScript_GiveAdvice
	end

BattleFrontier_Lounge2_EventScript_AlreadyMetManiac::
	msgbox BattleFrontier_Lounge2_Text_SwingByForTheLatestWord, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge2_EventScript_GiveAdvice
	end

BattleFrontier_Lounge2_EventScript_GiveAdvice::
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_SINGLES, BattleFrontier_Lounge2_EventScript_BufferSingle
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_DOUBLES, BattleFrontier_Lounge2_EventScript_BufferDouble
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_MULTIS, BattleFrontier_Lounge2_EventScript_BufferMulti
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_LINK, BattleFrontier_Lounge2_EventScript_BufferMultiLink
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_DOME, BattleFrontier_Lounge2_EventScript_BufferBattleDome
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_FACTORY, BattleFrontier_Lounge2_EventScript_BufferBattleFactory
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PALACE, BattleFrontier_Lounge2_EventScript_BufferBattlePalace
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_ARENA, BattleFrontier_Lounge2_EventScript_BufferBattleArena
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PIKE, BattleFrontier_Lounge2_EventScript_BufferBattlePike
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PYRAMID, BattleFrontier_Lounge2_EventScript_BufferBattlePyramid
@ <= FRONTIER_MANIAC_TOWER_LINK is any Battle Tower mode
	call_if_le VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_LINK, BattleFrontier_Lounge2_EventScript_BattleTowerNews
@ >= FRONTIER_MANIAC_DOME is any facility other than Battle Tower
	call_if_ge VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_DOME, BattleFrontier_Lounge2_EventScript_FacilityNews
	special ShowFrontierManiacMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge2_EventScript_BattleTowerNews::
	msgbox BattleFrontier_Lounge2_Text_BattleTowerIsHottest, MSGBOX_DEFAULT
	return

BattleFrontier_Lounge2_EventScript_FacilityNews::
	msgbox BattleFrontier_Lounge2_Text_FacilityIsHottest, MSGBOX_DEFAULT
	return

BattleFrontier_Lounge2_EventScript_BufferSingle::
	bufferstdstring STR_VAR_1, STDSTRING_SINGLE
	return

BattleFrontier_Lounge2_EventScript_BufferDouble::
	bufferstdstring STR_VAR_1, STDSTRING_DOUBLE
	return

BattleFrontier_Lounge2_EventScript_BufferMulti::
	bufferstdstring STR_VAR_1, STDSTRING_MULTI
	return

BattleFrontier_Lounge2_EventScript_BufferMultiLink::
	bufferstdstring STR_VAR_1, STDSTRING_MULTI_LINK
	return

BattleFrontier_Lounge2_EventScript_BufferBattleDome::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_DOME
	return

BattleFrontier_Lounge2_EventScript_BufferBattleFactory::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_FACTORY
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePalace::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PALACE
	return

BattleFrontier_Lounge2_EventScript_BufferBattleArena::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_ARENA
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePike::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PIKE
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePyramid::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PYRAMID
	return

BattleFrontier_Lounge2_EventScript_Maniac1::
	lock
	msgbox BattleFrontier_Lounge2_Text_NewsGatheringPower, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_Maniac2::
	lock
	msgbox BattleFrontier_Lounge2_Text_AmazingPowersOfObservation, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_Maniac3::
	lock
	msgbox BattleFrontier_Lounge2_Text_AmazingPowerOfPersuasion, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_TriathleteF::
	msgbox BattleFrontier_Lounge2_Text_ThisPlaceIsScaringMe, MSGBOX_NPC
	end

BattleFrontier_Lounge2_Text_FrontierManiacIntro::
	.string "Yo! When it comes to news about\n"
	.string "the BATTLE FRONTIER, nobody beats me.\p"
	.string "Call me the FRONTIER MANIAC.\p"
	.string "Just checking, but you're a TRAINER,\n"
	.string "right?\p"
	.string "Then I'll gladly share the hottest\n"
	.string "scoop I've collected about the\l"
	.string "BATTLE FRONTIER.$"

BattleFrontier_Lounge2_Text_SwingByForTheLatestWord::
	.string "Yo! Did you swing by to squeeze me\n"
	.string "for the latest word? Fine, fine!$"

@ Unused
BattleFrontier_Lounge2_Text_MyInformationsBeenUsefulRight::
	.string "Well? Well? Well?\p"
	.string "My info's been a real power boost\n"
	.string "for you, hasn't it?$"

BattleFrontier_Lounge2_Text_FacilityIsHottest::
	.string "Let me see…\p"
	.string "Word is the {STR_VAR_1}\n"
	.string "is where the action is right now.$"

BattleFrontier_Lounge2_Text_BattleTowerIsHottest::
	.string "Let me see…\p"
	.string "Word is BATTLE TOWER\n"
	.string "{STR_VAR_1} is where it's at.$"

BattleFrontier_Lounge2_Text_SalonMaidenIsThere::
	.string "Bet you didn't know this!\p"
	.string "There, one of the seven top fighters\n"
	.string "ANNOUNCER handpicked as the FRONTIER\l"
	.string "BRAINS holds court.\p"
	.string "A mysterious TRAINER known as the\n"
	.string "SALON MAIDEN runs the place.$"

BattleFrontier_Lounge2_Text_SalonMaidenSilverMons::
	.string "Ever fought the SALON MAIDEN?\p"
	.string "When she's just sizing up a challenger,\n"
	.string "these are the POKéMON she uses:\p"
	.string "A PSYCHIC-type PSI POKéMON,\n"
	.string "a FIRE-type VOLCANO POKéMON,\l"
	.string "and a NORMAL-type SLEEPING POKéMON.$"

BattleFrontier_Lounge2_Text_SalonMaidenGoldMons::
	.string "Ever fought the SALON MAIDEN when\n"
	.string "she's dead serious?\p"
	.string "When she goes all out, these are\n"
	.string "the POKéMON she uses:\p"
	.string "A DRAGON & PSYCHIC EON POKéMON,\n"
	.string "an ELECTRIC-type THUNDER POKéMON,\l"
	.string "and a NORMAL-type SLEEPING POKéMON.$"

BattleFrontier_Lounge2_Text_DomeAceIsThere::
	.string "Bet you didn't know this!\p"
	.string "There, one of the seven top fighters\n"
	.string "ANNOUNCER handpicked as the FRONTIER\l"
	.string "BRAINS holds court.\p"
	.string "A flamboyant TRAINER known as the\n"
	.string "DOME ACE runs the place.$"

BattleFrontier_Lounge2_Text_DomeAceSilverMons::
	.string "Ever fought the DOME ACE?\p"
	.string "When he's going easy on a challenger,\n"
	.string "he sends out these three POKéMON:\p"
	.string "A DRAGON & FLYING DRAGON POKéMON,\n"
	.string "a WATER & GROUND MUD FISH POKéMON,\l"
	.string "and a FIRE & FLYING FLAME POKéMON.$"

BattleFrontier_Lounge2_Text_DomeAceGoldMons::
	.string "Ever fought the DOME ACE when he's\n"
	.string "serious?\p"
	.string "When he shows off his real strategy,\n"
	.string "he sends out these three POKéMON:\p"
	.string "A DRAGON & FLYING EON POKéMON,\n"
	.string "a WATER & GROUND MUD FISH POKéMON,\l"
	.string "and a STEEL- & PSYCHIC-type IRON LEG\l"
	.string "POKéMON.$"

BattleFrontier_Lounge2_Text_FactoryHeadIsThere::
	.string "Bet you didn't know this!\p"
	.string "There, one of the seven top fighters\n"
	.string "ANNOUNCER handpicked as the FRONTIER\l"
	.string "BRAINS holds court.\p"
	.string "A freaky TRAINER known as the\n"
	.string "FACTORY HEAD runs the place.$"

BattleFrontier_Lounge2_Text_FactoryHeadSilverMons::
	.string "Ever fought the FACTORY HEAD?\p"
	.string "Let me think… When he steps into\n"
	.string "battle, he just takes three rental\l"
	.string "POKéMON.\p"
	.string "He fights under pretty much the same\n"
	.string "rules as you do.$"

BattleFrontier_Lounge2_Text_FactoryHeadGoldMons::
	.string "Ever fought the FACTORY HEAD when\n"
	.string "he's serious?\p"
	.string "When he fights in earnest, he still\n"
	.string "takes three rental POKéMON.\p"
	.string "He fights under virtually the same\n"
	.string "rules as you do.$"

BattleFrontier_Lounge2_Text_PikeQueenIsThere::
	.string "Bet you didn't know this!\p"
	.string "There, one of the seven top fighters\n"
	.string "ANNOUNCER handpicked as the FRONTIER\l"
	.string "BRAINS holds court.\p"
	.string "A scary TRAINER known as the\n"
	.string "PIKE QUEEN runs the place.$"

BattleFrontier_Lounge2_Text_PikeQueenSilverMons::
	.string "Ever fought the PIKE QUEEN?\p"
	.string "When she's in a good mood, rumor says\n"
	.string "she sends out these three POKéMON:\p"
	.string "A POISON-type FANG SNAKE POKéMON,\n"
	.string "a BUG & ROCK MOLD POKéMON,\l"
	.string "and a WATER-type TENDER POKéMON.$"

BattleFrontier_Lounge2_Text_PikeQueenGoldMons::
	.string "Ever fought the PIKE QUEEN when she's\n"
	.string "serious?\p"
	.string "When she's truly ticked off, they say\n"
	.string "she sends out these three POKéMON:\p"
	.string "A POISON-type FANG SNAKE POKéMON,\n"
	.string "a STEEL & GROUND IRON SNAKE POKéMON,\l"
	.string "and a WATER- & FLYING-type ATROCIOUS\l"
	.string "POKéMON.$"

BattleFrontier_Lounge2_Text_ArenaTycoonIsThere::
	.string "Bet you didn't know this!\p"
	.string "There, one of the seven top fighters\n"
	.string "ANNOUNCER handpicked as the FRONTIER\l"
	.string "BRAINS holds court.\p"
	.string "A cute TRAINER known as the\n"
	.string "ARENA TYCOON runs the place.$"

BattleFrontier_Lounge2_Text_ArenaTycoonSilverMons::
	.string "Ever fought the ARENA TYCOON?\p"
	.string "When she's measuring a foe's power,\n"
	.string "these are supposedly the POKéMON\l"
	.string "she uses:\p"
	.string "A BUG & FIGHTING SINGLE HORN POKéMON,\n"
	.string "a DARK-type MOONLIGHT POKéMON,\l"
	.string "and a BUG & GHOST SHED POKéMON.$"

BattleFrontier_Lounge2_Text_ArenaTycoonGoldMons::
	.string "Ever fought the ARENA TYCOON when\n"
	.string "she's serious?\p"
	.string "When she fights for keeps, these are\n"
	.string "supposedly the POKéMON she uses:\p"
	.string "A DARK-type MOONLIGHT POKéMON,\n"
	.string "a GHOST & POISON SHADOW POKéMON\l"
	.string "and a GRASS- & FIGHTING-type\l"
	.string "MUSHROOM POKéMON.$"

BattleFrontier_Lounge2_Text_PalaceMavenIsThere::
	.string "Bet you didn't know this!\p"
	.string "There, one of the seven top fighters\n"
	.string "ANNOUNCER handpicked as the FRONTIER\l"
	.string "BRAINS holds court.\p"
	.string "A sinister TRAINER known as the\n"
	.string "PALACE MAVEN runs the place.$"

BattleFrontier_Lounge2_Text_PalaceMavenSilverMons::
	.string "Ever fought the PALACE MAVEN?\p"
	.string "When he's testing a challenger's\n"
	.string "spirit, he's said to use these\l"
	.string "POKéMON:\p"
	.string "A POISON & FLYING-type BAT POKéMON,\n"
	.string "a NORMAL-type LAZY POKéMON, and a\l"
	.string "WATER- & ICE-type TRANSPORT POKéMON.$"

BattleFrontier_Lounge2_Text_PalaceMavenGoldMons::
	.string "Ever fought the PALACE MAVEN when\n"
	.string "he's serious?\p"
	.string "When he throws his full might into\n"
	.string "battle, he's said to use these\l"
	.string "POKéMON:\p"
	.string "A FIRE-type LEGENDARY POKéMON,\n"
	.string "a NORMAL-type LAZY POKéMON,\l"
	.string "and a WATER-type AURORA POKéMON.$"

BattleFrontier_Lounge2_Text_PyramidKingIsThere::
	.string "Bet you didn't know this!\p"
	.string "There, one of the seven top fighters\n"
	.string "ANNOUNCER handpicked as the FRONTIER\l"
	.string "BRAINS holds court.\p"
	.string "A fiery-hot TRAINER known as the\n"
	.string "PYRAMID KING runs the place.$"

BattleFrontier_Lounge2_Text_PyramidKingSilverMons::
	.string "Ever fought the PYRAMID KING?\p"
	.string "When he's checking a foe's strength,\n"
	.string "he apparently uses these POKéMON:\p"
	.string "A ROCK-type ROCK PEAK POKéMON,\n"
	.string "an ICE-type ICEBERG POKéMON,\l"
	.string "and a STEEL-type IRON POKéMON.$"

BattleFrontier_Lounge2_Text_PyramidKingGoldMons::
	.string "Ever fought the PYRAMID KING when\n"
	.string "he's serious?\p"
	.string "When his power is burning at its peak,\n"
	.string "he apparently uses these POKéMON:\p"
	.string "An ICE & FLYING FREEZE POKéMON, an\n"
	.string "ELECTRIC & FLYING ELECTRIC POKéMON,\l"
	.string "and a FIRE- & FLYING-type FLAME\l"
	.string "POKéMON.$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice1::
	.string "Sure, there are plenty of places to\n"
	.string "enter DOUBLE BATTLES.\p"
	.string "But the DOUBLE BATTLE ROOMS of the\n"
	.string "BATTLE TOWER are the basics!\p"
	.string "That's where you learn how DOUBLE\n"
	.string "BATTLES are fought here in the\l"
	.string "BATTLE FRONTIER.$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice2::
	.string "Keep your guard up in the battles\n"
	.string "here.\p"
	.string "I hear some TRAINERS have strategies\n"
	.string "they developed just for DOUBLE\l"
	.string "BATTLES.$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice3::
	.string "Once you've got a handle on DOUBLE\n"
	.string "BATTLES here, think about taking on\l"
	.string "the other places that offer them.$"

BattleFrontier_Lounge2_Text_MultiBattleAdvice::
	.string "All kinds of fighters gather in\n"
	.string "the BATTLE SALON.\p"
	.string "Think about it--you might meet up with\n"
	.string "your friends or old rivals!\l"
	.string "Keep your eyes open!$"

BattleFrontier_Lounge2_Text_LinkMultiBattleAdvice::
	.string "If you've got a friend with you, head\n"
	.string "for the LINK MULTI BATTLE ROOM.\p"
	.string "Team up with a strong friend and\n"
	.string "you can expect some fearsome foes!$"

BattleFrontier_Lounge2_Text_NewsGatheringPower::
	.string "Incredible news-gathering power!\n"
	.string "There's no master like my master!$"

BattleFrontier_Lounge2_Text_AmazingPowersOfObservation::
	.string "Incredible powers of observation!\n"
	.string "There's no master like my master!$"

BattleFrontier_Lounge2_Text_AmazingPowerOfPersuasion::
	.string "Incredible power of persuasion!\n"
	.string "There's no master like my master!$"

BattleFrontier_Lounge2_Text_ThisPlaceIsScaringMe::
	.string "…Where am I?\n"
	.string "This place is scaring me…$"

