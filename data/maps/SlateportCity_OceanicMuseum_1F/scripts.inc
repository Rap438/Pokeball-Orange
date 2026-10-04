SlateportCity_OceanicMuseum_1F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_1F_EventScript_EntranceAttendant::
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeLeft::
	lockall
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeRight::
	lockall
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee::
	showmoneybox 0, 0
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee
	closemessage
	hidemoneybox
	applywaitmovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee::
	checkmoney 50
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney
	playse SE_SHOP
	removemoney 50
	updatemoneybox
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney::
	goto_if_unset FLAG_DELIVERED_DEVON_GOODS, SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway
	msgbox SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney, MSGBOX_DEFAULT
	closemessage
	hidemoneybox
	applywaitmovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt5::
	msgbox SlateportCity_OceanicMuseum_1F_Text_RustboroBungled, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt6::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_WhirlpoolExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_WaterfallExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanSoilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_BeachSandDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_FossilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_FossilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_DepthMeasuringMachine::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGrunt::
	lock
	faceplayer
	delay 8
	playse SE_PIN
	applywaitmovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_Delay48
	msgbox SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_THIEF
	goto_if_eq VAR_RESULT, 0, SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief
	setflag FLAG_RECEIVED_TM_THIEF
	msgbox SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_FACING, DIR_NORTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth
	goto_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth
	goto_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	goto_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth::
	applywaitmovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited::
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_FAMILIAR_AQUA_GRUNT
	playse SE_EXIT
	removeobject LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT
	release
	end

SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief::
	msgbox SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis, MSGBOX_DEFAULT
	release
	end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast:
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit:
	face_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter:
	.string "Welcome to the OCEANIC MUSEUM.\p"
	.string "The entrance fee is ¥50.\n"
	.string "Would you like to go in?$"

SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself:
	.string "Please, enjoy yourself.$"

SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney:
	.string "Oh? I'm sorry, but you don't seem to\n"
	.string "have enough money.$"

SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup:
	.string "Oh? Are you with the group that went in\n"
	.string "earlier?\p"
	.string "You're the only one running late. You'd\n"
	.string "better hurry and catch up with them!$"

SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll:
	.string "The RED RIBBON ARMY will bring a new age\n"
	.string "to the world! All hail KID BUU! Sir!$"

SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere:
	.string "We were ordered to assemble here, so we\n"
	.string "did, but…\p"
	.string "DR. GERO, the guy in charge, hasn't\n"
	.string "shown up yet. Nobody knows what to do!$"

SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich:
	.string "If I swiped the stuff in here, would I\n"
	.string "get rich enough to quit the army?$"

SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds:
	.string "Whatever I learn here, I can use for our\n"
	.string "next evil plan… Heh heh!$"

SlateportCity_OceanicMuseum_1F_Text_RustboroBungled:
	.string "If those clowns hadn't botched the job\n"
	.string "in RUSTBORO, we wouldn't be stuck\l"
	.string "cleaning up after them!$"

SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney:
	.string "I didn't have ¥50, so it took forever to\n"
	.string "get past the receptionist.$"

SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling:
	.string "I want to learn all about the sea and\n"
	.string "put that knowledge to use in battle.$"

SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel:
	.string "I get all giddy when I see the sea!\p"
	.string "For me, CAPT. STERN is the number one\n"
	.string "role model!$"

SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow:
	.string "The sea is vast without end, and\n"
	.string "infinitely deep…\p"
	.string "There must be so many POKéMON out\n"
	.string "there that we don't know about.$"

SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon:
	.string "I want a sea POKéMON.\p"
	.string "I bet it feels cool and nice to hug.$"

SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis:
	.string "Aiyeeeh!\n"
	.string "What are you doing here?!\p"
	.string "It's me! The RED RIBBON soldier you\n"
	.string "flattened back in RUSTURF TUNNEL,\l"
	.string "remember?\p"
	.string "Here, take this!\n"
	.string "Just don't hit me again!$"

SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain:
	.string "That TM suits you better than it ever\n"
	.string "suited me.\p"
	.string "Hope I never see you again! Wahahaha!$"

SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis:
	.string "Awww, come on!\n"
	.string "Take it and let me go!$"

SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment:
	.string "Blue fluid spirals inside a glass vessel.\p"
	.string "“This experiment creates a WHIRLPOOL\n"
	.string "artificially, using wind.”$"

SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment:
	.string "A red ball bobs up and down inside a\n"
	.string "glass vessel.\p"
	.string "“This experiment simulates a\n"
	.string "WATERFALL, using the buoyancy of the\l"
	.string "ball.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay:
	.string "A sample of soil from the ocean floor\n"
	.string "sits on display.\p"
	.string "“Over many years, the remains of living\n"
	.string "things settle on the seabed and form\l"
	.string "layers of sediment.\p"
	.string "“By studying these layers, we can\n"
	.string "uncover the ancient past.”$"

SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay:
	.string "A sample of beach sand sits on display.\p"
	.string "“Stones from the mountains are carried\n"
	.string "down by rivers, where they are chipped\l"
	.string "and ground smaller.\p"
	.string "“They wear down into grains and end up\n"
	.string "as the sand on the beach.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1:
	.string "“OCEANIC MINIFACT 1\n"
	.string "Why is seawater blue?\p"
	.string "“Light is made up of many colors.\p"
	.string "“When light passes through water, most\n"
	.string "colors fade away.\p"
	.string "“Blue light keeps its color, which is why\n"
	.string "the sea looks blue.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2:
	.string "“OCEANIC MINIFACT 2\n"
	.string "Why is the sea salty?\p"
	.string "“Seawater holds dissolved salt in the\n"
	.string "form of sodium and chlorine ions.\p"
	.string "“These ions leach out of rocks and are\n"
	.string "carried to the sea by rain.\p"
	.string "“All that dissolved salt is what makes\n"
	.string "the sea salty.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3:
	.string "“OCEANIC MINIFACT 3\n"
	.string "Which is bigger, the sea or the land?\p"
	.string "“The sea covers about 70% of the\n"
	.string "planet, and the rest is land.\p"
	.string "“That makes the sea more than twice\n"
	.string "the size of the land.”$"

SlateportCity_OceanicMuseum_1F_Text_FossilDisplay:
	.string "A fossil with wavy ridges on it sits\n"
	.string "behind glass.\p"
	.string "“The tide scours the soil on the ocean\n"
	.string "floor.\p"
	.string "“Flowing seawater marks the soil with\n"
	.string "small ridges and valleys.\p"
	.string "“When that soil turns to fossil, it is\n"
	.string "called a ripple mark.”$"

SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine:
	.string "A strange machine rotates under a\n"
	.string "glass dome.\p"
	.string "Maybe it measures how deep the sea\n"
	.string "goes…$"

