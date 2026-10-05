SlateportCity_OceanicMuseum_2F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_2F_EventScript_CaptStern::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, TRUE
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach
	waitmovement 0
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	call_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts, MSGBOX_DEFAULT
	call_if_ne VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou, MSGBOX_DEFAULT
	msgbox SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth
	call_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_1, SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat
	msgbox SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	msgbox SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_2, SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_MeddlingKid, MSGBOX_DEFAULT
	closemessage
	delay 35
	addobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	applymovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_ArchieWarning, MSGBOX_DEFAULT
	closemessage
	savebgm MUS_DUMMY
	fadedefaultbgm
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	fadescreen FADE_FROM_BLACK
	delay 30
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_DEVON_PARTS
	call Common_EventScript_PlayerHandedOverTheItem
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	removeobject LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN
	setflag FLAG_HIDE_ROUTE_110_TEAM_AQUA
	call_if_eq VAR_REGISTER_BIRCH_STATE, 0, SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch
	setflag FLAG_DELIVERED_DEVON_GOODS
	clearflag FLAG_HIDE_ROUTE_116_DEVON_EMPLOYEE
	setflag FLAG_HIDE_RUSTBORO_CITY_DEVON_CORP_3F_EMPLOYEE
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1
	fadescreen FADE_FROM_BLACK
	release
	end

SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch::
	setvar VAR_REGISTER_BIRCH_STATE, 1
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts::
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	return

SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts::
	applywaitmovement LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN, Common_Movement_WalkInPlaceFasterLeft
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth::
	applywaitmovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest::
	applywaitmovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest
	return

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth:
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest:
	walk_up
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_Unused:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	delay_8
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter:
	walk_down
	walk_right
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated:
	lock_facing_direction
	walk_left
	unlock_facing_direction
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_8
	walk_fast_down
	walk_in_place_faster_up
	step_end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_PressureExperiment::
	msgbox SlateportCity_OceanicMuseum_2F_Text_PressureExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_HoennModel::
	msgbox SlateportCity_OceanicMuseum_2F_Text_HoennModel, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_DeepSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SurfaceSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSTidalReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmarineReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmersibleReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSAnneReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron3::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine, MSGBOX_DEFAULT
	closemessage
	applywaitmovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	release
	end

SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts:
	.string "Yes? If you're looking for STERN, you've\n"
	.string "found him.\p"
	.string "Ah! Is that the CC GOODS I ordered from\n"
	.string "DR. BRIEF at CAPSULE CORP.? Wonderful!\p"
	.string "These parts are exactly what the\n"
	.string "submarine needs. Now we can finally get\l"
	.string "ready for the expedition.$"

SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts:
	.string "Hee hee, hold it right there, kid!\n"
	.string "Hand over those parts!$"

SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou:
	.string "CAPT. STERN: Wh-what is this?\n"
	.string "Who are you people?$"

SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua:
	.string "We're the RED RIBBON ARMY, SEA squad!\p"
	.string "DR. GERO needs those parts, so shut up\n"
	.string "and give them here! Or we'll take them\l"
	.string "the hard way!$"

SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat:
	.string "Waaaaah!\n"
	.string "A kid pounded me!$"

SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious:
	.string "Oh no, oh no… DR. GERO is going to have\n"
	.string "my head for this…$"

SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis:
	.string "Bah, you useless wimp!\n"
	.string "Out of the way, I'll handle this brat!$"

SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat:
	.string "What?!\n"
	.string "I got beat, too?!$"

SlateportCity_OceanicMuseum_2F_Text_MeddlingKid:
	.string "Now what? If we don't bring those parts\n"
	.string "back, the colonel will have us scrubbing\l"
	.string "decks for a year!\p"
	.string "How could we lose to some kid who just\n"
	.string "wandered in?!$"

SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong:
	.string "Kyahahaha… I came to see what was\n"
	.string "taking so long.\p"
	.string "And what do I find? My soldiers, held up\n"
	.string "by one child.$"

SlateportCity_OceanicMuseum_2F_Text_ArchieWarning:
	.string "I am DR. GERO, chief scientist of the\n"
	.string "RED RIBBON ARMY, and master of its SEA\l"
	.string "squad.\p"
	.string "Hmph. So you're the brat who wrecked my\n"
	.string "men. I see no uniform. You are no soldier\l"
	.string "of ours.\p"
	.string "Then why meddle? Do you even know what\n"
	.string "sleeps beneath these waves?\p"
	.string "KYOGRE. A titan of rain and tide. With\n"
	.string "the right machinery, I will wake it and\l"
	.string "make the very sea obey me.\p"
	.string "KID BUU wants the titans loose to tear\n"
	.string "the world apart for fun. Fool. I intend\l"
	.string "to hold the leash.\p"
	.string "Those parts would have saved me weeks\n"
	.string "of work. No matter. There is more than\l"
	.string "one way to skin a Magikarp.\p"
	.string "{PLAYER}, is it? A boy with a little power\n"
	.string "and a big mouth. A scouter wouldn't even\l"
	.string "bother to read you.\p"
	.string "But be warned: stand in my way again,\n"
	.string "and I will crush you.\p"
	.string "Now get out of my sight.$"

SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs:
	.string "CAPT. STERN: You're…\n"
	.string "Ah, you must be {PLAYER}{KUN}…\p"
	.string "Phew, that was close! Thank you for\n"
	.string "saving us!\p"
	.string "Oh, and I nearly forgot! You even\n"
	.string "brought the parts from CAPSULE CORP.!$"

SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo:
	.string "CAPT. STERN: Whoops! I'm wasting time!\p"
	.string "The expedition to the ocean floor\n"
	.string "leaves very soon, and the crew is\l"
	.string "waiting.\p"
	.string "Thanks again. I owe you one, kid!\p"
	.string "Feel free to look around the museum and\n"
	.string "the shipyard while I'm gone.$"

SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip:
	.string "There's a model ship over there.\p"
	.string "It reminded me of that ABANDONED SHIP\n"
	.string "near DEWFORD TOWN…$"

SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum:
	.string "No running in the MUSEUM!\p"
	.string "Save the sprinting for your training!$"

SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine:
	.string "Wow, a real submarine!\n"
	.string "I want a ride, I want a ride!$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1:
	.string "“WATER QUALITY SAMPLE 1,” says the\n"
	.string "label.\p"
	.string "The sea is all connected, but the water\n"
	.string "seems to differ from region to region.$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2:
	.string "“WATER QUALITY SAMPLE 2,” says the\n"
	.string "label.\p"
	.string "Does the saltiness of seawater differ\n"
	.string "by region, too?$"

SlateportCity_OceanicMuseum_2F_Text_PressureExperiment:
	.string "A rubber ball swells and shrinks.\p"
	.string "“In the sea, the weight of the water\n"
	.string "itself exerts pressure.\p"
	.string "“In shallow water, the pressure is\n"
	.string "light.\p"
	.string "“In extremely deep water, though, the\n"
	.string "pressure can reach tens of thousands\l"
	.string "of tons on a small area.”$"

SlateportCity_OceanicMuseum_2F_Text_HoennModel:
	.string "“MODEL OF HOENN REGION”\p"
	.string "It's a miniature diorama of the HOENN\n"
	.string "region.\p"
	.string "Where is LITTLEROOT TOWN on it?$"

SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay:
	.string "It's a display about how seawater\n"
	.string "flows.\p"
	.string "“Near the seabed, water flows because\n"
	.string "of differences in factors like\l"
	.string "temperature and salinity.”$"

SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay:
	.string "It's a display about how seawater\n"
	.string "flows.\p"
	.string "“Near the surface, seawater moves as\n"
	.string "currents driven by the wind.”$"

SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica:
	.string "“THE FERRY S.S. TIDAL\p"
	.string "“A scale replica of the ship under\n"
	.string "construction at STERN'S SHIPYARD.”$"

SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica:
	.string "“SUBMARINE EXPLORER 1\p"
	.string "“A replica of the high-performance\n"
	.string "submarine built for exploring the ocean\l"
	.string "floor.”$"

SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica:
	.string "“SUBMERSIBLE POD\p"
	.string "“A replica of a compact, unmanned pod\n"
	.string "for exploring the seafloor.”$"

SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica:
	.string "“S.S. ANNE\p"
	.string "“A replica of the luxury liner that\n"
	.string "circles the globe.”$"
