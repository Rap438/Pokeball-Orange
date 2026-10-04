BattleFrontier_Lounge5_MapScripts::
	.byte 0

BattleFrontier_Lounge5_EventScript_NatureGirl::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge5_Text_NatureGirlGreeting, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	special ChoosePartyMon
	lock
	faceplayer
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, BattleFrontier_Lounge5_EventScript_NatureGirlEgg
	special ShowNatureGirlMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlEgg::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlEgg, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlNoneShown, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_Gentleman::
	msgbox BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_BlackBelt::
	msgbox BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_LittleBoy::
	msgbox BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot, MSGBOX_NPC
	end

BattleFrontier_Lounge5_Text_NatureGirlGreeting::
	.string "Ehehe!\n"
	.string "I can sense what POKéMON are feeling,\l"
	.string "just like reading ki!\p"
	.string "Please!\n"
	.string "Can I see your POKéMON?$"

BattleFrontier_Lounge5_Text_NatureGirlNoneShown::
	.string "Boo!\n"
	.string "You're no fun!$"

BattleFrontier_Lounge5_Text_NatureGirlHardy::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "It keeps fighting even when it has\l"
	.string "big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlLonely::
	.string "Hmhm…\p"
	.string "This one says it loves playing tricks!\n"
	.string "But when it gets enough owies,\l"
	.string "it hits back!$"

BattleFrontier_Lounge5_Text_NatureGirlBrave::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "But when it gets enough owies,\l"
	.string "it starts looking out for itself!$"

BattleFrontier_Lounge5_Text_NatureGirlAdamant::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "It keeps fighting even when it has\l"
	.string "big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlNaughty::
	.string "Hmhm…\p"
	.string "This one says it likes to guard itself!\n"
	.string "But when it gets enough owies,\l"
	.string "it hits back!$"

BattleFrontier_Lounge5_Text_NatureGirlBold::
	.string "Hmhm…\p"
	.string "This one says it loves playing tricks!\n"
	.string "But when it gets enough owies,\l"
	.string "it starts looking out for itself!$"

BattleFrontier_Lounge5_Text_NatureGirlDocileNaiveQuietQuirky::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "It keeps fighting even when it has\l"
	.string "big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlRelaxed::
	.string "Hmhm…\p"
	.string "This one says it loves playing tricks!\n"
	.string "But when it gets enough owies,\l"
	.string "it hits back!$"

BattleFrontier_Lounge5_Text_NatureGirlImpish::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "But when it gets enough owies,\l"
	.string "it starts looking out for itself!$"

BattleFrontier_Lounge5_Text_NatureGirlLax::
	.string "Hmhm…\p"
	.string "This one says it loves playing tricks!\n"
	.string "It keeps playing tricks even when\l"
	.string "it has big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlTimid::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "But when it gets enough owies,\l"
	.string "it switches to tricks!$"

BattleFrontier_Lounge5_Text_NatureGirlHasty::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "It keeps fighting even when it has\l"
	.string "big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlSerious::
	.string "Hmhm…\p"
	.string "This one says it loves playing tricks!\n"
	.string "It keeps playing tricks even when\l"
	.string "it has big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlJolly::
	.string "Hmhm…\p"
	.string "This one says it loves playing tricks!\n"
	.string "But when it gets enough owies,\l"
	.string "it starts looking out for itself!$"

BattleFrontier_Lounge5_Text_NatureGirlModest::
	.string "Hmhm…\p"
	.string "This one says it likes to guard itself!\n"
	.string "It keeps looking out for itself, owies\l"
	.string "or no owies!$"

BattleFrontier_Lounge5_Text_NatureGirlMild::
	.string "Hmhm…\p"
	.string "This one says it likes to guard itself!\n"
	.string "But when it gets enough owies,\l"
	.string "it switches to tricks!$"

BattleFrontier_Lounge5_Text_NatureGirlBashful::
	.string "Hmhm…\p"
	.string "This one says it likes to guard itself!\n"
	.string "It keeps looking out for itself even\l"
	.string "when it has big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlRash::
	.string "Hmhm…\p"
	.string "This one says it loves playing tricks!\n"
	.string "It keeps playing tricks even when\l"
	.string "it has big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlCalm::
	.string "Hmhm…\p"
	.string "This one says it likes to guard itself!\n"
	.string "It keeps looking out for itself even\l"
	.string "when it has big owies!$"

BattleFrontier_Lounge5_Text_NatureGirlGentle::
	.string "Hmhm…\p"
	.string "This one says it likes to guard itself!\n"
	.string "But when it gets enough owies,\l"
	.string "it hits back!$"

BattleFrontier_Lounge5_Text_NatureGirlSassy::
	.string "Hmhm…\p"
	.string "This one says it loves to fight!\n"
	.string "But when it gets enough owies,\l"
	.string "it switches to tricks!$"

BattleFrontier_Lounge5_Text_NatureGirlCareful::
	.string "Hmhm…\p"
	.string "This one says it likes to guard itself!\n"
	.string "But when it gets enough owies,\l"
	.string "it switches to tricks!$"

BattleFrontier_Lounge5_Text_NatureGirlEgg::
	.string "Silly! An EGG is fast asleep!\n"
	.string "It can't talk to me!$"

BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon::
	.string "How adorable!\n"
	.string "That little girl claims she can\l"
	.string "sense what POKéMON are feeling!$"

BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound::
	.string "I get the feeling that little girl is\n"
	.string "saying something very wise.$"

BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot::
	.string "I've got a secret!\p"
	.string "That little girl hangs out at the red\n"
	.string "house all the time!$"

