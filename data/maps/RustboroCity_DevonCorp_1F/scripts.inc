RustboroCity_DevonCorp_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_1F_OnTransition
	.byte 0

RustboroCity_DevonCorp_1F_OnTransition:
	call_if_unset FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_BlockStairs
	end

RustboroCity_DevonCorp_1F_EventScript_BlockStairs::
	setobjectxyperm LOCALID_DEVON_CORP_STAIR_GUARD, 14, 2
	setobjectmovementtype LOCALID_DEVON_CORP_STAIR_GUARD, MOVEMENT_TYPE_FACE_DOWN
	return

RustboroCity_DevonCorp_1F_EventScript_Employee::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, EventScript_DBZ_GravityRoom
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright
	msgbox RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright::
	msgbox RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GoodsRecovered::
	msgbox RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StairGuard::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome::
	msgbox RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_Greeter::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp::
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_StaffGotRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RocksMetalDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_EventScript_ProductsDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_ProductDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp:
	.string "Hello, and welcome to CAPSULE CORP.\p"
	.string "We're proud to make items and medicine\n"
	.string "that make your life better.$"

RustboroCity_DevonCorp_1F_Text_StaffGotRobbed:
	.string "One of our researchers got robbed of an\n"
	.string "important parcel. Careless, if you ask\l"
	.string "me.$"

RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct:
	.string "Hey, those RUNNING SHOES!\n"
	.string "One of our products!\p"
	.string "It makes me so happy to see someone\n"
	.string "using something we made.$"

RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright:
	.string "That stolen parcel…\p"
	.string "Sure, it's important, but it's nothing\n"
	.string "anyone else could actually use.\p"
	.string "If you ask me, that robber wasn't very\n"
	.string "bright.$"

RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered:
	.string "Word is the stolen CC GOODS have been\n"
	.string "recovered.$"

RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter:
	.string "I'm sorry, only authorized personnel\n"
	.string "may enter here.$"

RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed:
	.string "It makes no sense.\n"
	.string "How could we get robbed?$"

RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere:
	.string "Hi there!\n"
	.string "You're always welcome here!$"

RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay:
	.string "Samples of rocks and metal are\n"
	.string "displayed in the glass case.\p"
	.string "There's a panel with some writing on it…\p"
	.string "“CAPSULE CORP. began as a workshop\n"
	.string "that shaped stone from quarries.\p"
	.string "“It also forged iron from filings found\n"
	.string "in the sand.\p"
	.string "“From those humble beginnings, DR.\n"
	.string "BRIEF invented the HOI-POI CAPSULE,\l"
	.string "and the company took off.\p"
	.string "“CAPSULE CORP. now manufactures a wide\n"
	.string "range of products for everyday life.”$"

RustboroCity_DevonCorp_1F_Text_ProductDisplay:
	.string "Prototypes and test products fill the\n"
	.string "glass display case.\p"
	.string "There's a panel with a description…\p"
	.string "“Besides capsules and industrial\n"
	.string "products, CAPSULE CORP. now markets\l"
	.string "sundries and medicine for better living.\p"
	.string "“Recently, we've begun marketing tools\n"
	.string "for POKéMON TRAINERS, including POKé\l"
	.string "BALLS and POKéNAV systems.”$"

