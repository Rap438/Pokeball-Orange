JaggedPass_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, JaggedPass_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, JaggedPass_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, JaggedPass_OnLoad
	.byte 0

JaggedPass_OnResume:
	setstepcallback STEP_CB_ASH
	call_if_eq VAR_JAGGED_PASS_STATE, 0, JaggedPass_EventScript_CheckHasMagmaEmblem
	end

JaggedPass_EventScript_CheckHasMagmaEmblem::
	checkitem ITEM_MAGMA_EMBLEM
	goto_if_eq VAR_RESULT, TRUE, JaggedPass_EventScript_SetReadyToOpenHideout
	return

JaggedPass_EventScript_SetReadyToOpenHideout::
	setvar VAR_JAGGED_PASS_STATE, 1
	return

JaggedPass_OnTransition:
	call_if_eq VAR_JAGGED_PASS_ASH_WEATHER, 1, JaggedPass_EventScript_SetWeatherAsh
	end

JaggedPass_EventScript_SetWeatherAsh::
	setweather WEATHER_VOLCANIC_ASH
	doweather
	return

JaggedPass_OnLoad:
	goto_if_le VAR_JAGGED_PASS_STATE, 1, JaggedPass_EventScript_ConcealHideoutEntrance
	end

JaggedPass_EventScript_ConcealHideoutEntrance::
	setmetatile 16, 17, METATILE_Lavaridge_RockWall, TRUE
	setmetatile 16, 18, METATILE_Lavaridge_RockWall, TRUE
	end

JaggedPass_EventScript_OpenMagmaHideout::
	lockall
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox JaggedPass_Text_BoulderShakingInResponseToEmblem, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, 1   @ vertical pan
	setvar VAR_0x8005, 1   @ horizontal pan
	setvar VAR_0x8006, 16  @ num shakes
	setvar VAR_0x8007, 3   @ shake delay
	special ShakeCamera
	waitstate
	playse SE_EFFECTIVE
	setmetatile 16, 17, METATILE_Lavaridge_CaveEntrance_Top, TRUE
	setmetatile 16, 18, METATILE_Lavaridge_CaveEntrance_Bottom, FALSE
	special DrawWholeMapView
	delay 30
	setvar VAR_JAGGED_PASS_STATE, 2
	waitse
	releaseall
	end

JaggedPass_EventScript_MagmaHideoutGuard::
	lockall
	goto_if_set FLAG_BEAT_MAGMA_GRUNT_JAGGED_PASS, JaggedPass_EventScript_GuardDefeated
	waitse
	playse SE_PIN
	applywaitmovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_Delay48
	applywaitmovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FacePlayer
	msgbox JaggedPass_Text_GruntIntro, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_GRUNT_JAGGED_PASS, JaggedPass_Text_GruntDefeat
	setflag FLAG_BEAT_MAGMA_GRUNT_JAGGED_PASS
	applywaitmovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FaceOriginalDirection
	releaseall
	end

JaggedPass_EventScript_GuardDefeated::
	applywaitmovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FacePlayer
	msgbox JaggedPass_Text_GoWhereverYouWant, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FaceOriginalDirection
	releaseall
	end

JaggedPass_EventScript_Eric::
	trainerbattle_single TRAINER_ERIC, JaggedPass_Text_EricIntro, JaggedPass_Text_EricDefeat
	msgbox JaggedPass_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Diana::
	trainerbattle_single TRAINER_DIANA_1, JaggedPass_Text_DianaIntro, JaggedPass_Text_DianaDefeat, JaggedPass_EventScript_RegisterDiana
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, JaggedPass_EventScript_DianaRematch
	msgbox JaggedPass_Text_DianaPostBattle, MSGBOX_DEFAULT
	release
	end

JaggedPass_EventScript_RegisterDiana::
	special PlayerFaceTrainerAfterBattle
	msgbox JaggedPass_Text_DianaRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_DIANA_1
	release
	end

JaggedPass_EventScript_DianaRematch::
	trainerbattle_rematch TRAINER_DIANA_1, JaggedPass_Text_DianaRematchIntro, JaggedPass_Text_DianaRematchDefeat
	msgbox JaggedPass_Text_DianaPostRematch, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Ethan::
	trainerbattle_single TRAINER_ETHAN_1, JaggedPass_Text_EthanIntro, JaggedPass_Text_EthanDefeat, JaggedPass_EventScript_RegisterEthan
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, JaggedPass_EventScript_EthanRematch
	msgbox JaggedPass_Text_EthanPostBattle, MSGBOX_DEFAULT
	release
	end

JaggedPass_EventScript_RegisterEthan::
	special PlayerFaceTrainerAfterBattle
	msgbox JaggedPass_Text_EthanRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_ETHAN_1
	release
	end

JaggedPass_EventScript_EthanRematch::
	trainerbattle_rematch TRAINER_ETHAN_1, JaggedPass_Text_EthanRematchIntro, JaggedPass_Text_EthanRematchDefeat
	msgbox JaggedPass_Text_EthanPostRematch, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Julio::
	trainerbattle_single TRAINER_JULIO, JaggedPass_Text_JulioIntro, JaggedPass_Text_JulioDefeat
	msgbox JaggedPass_Text_JulioPostBattle, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Autumn::
	trainerbattle_single TRAINER_AUTUMN, JaggedPass_Text_AutumnIntro, JaggedPass_Text_AutumnDefeat
	msgbox JaggedPass_Text_AutumnPostBattle, MSGBOX_AUTOCLOSE
	end

JaggedPass_Text_EricIntro:
	.string "MT. CHIMNEY's JAGGED PASS… Sharp rocks,\n"
	.string "thin air, and not a soul around.\p"
	.string "This is the mountain I've always\n"
	.string "dreamed of training on!\p"
	.string "Every step is a sparring match with the\n"
	.string "mountain itself!$"

JaggedPass_Text_EricDefeat:
	.string "Beaten by a guy in a gi! That stings\n"
	.string "worse than the rocks!$"

JaggedPass_Text_EricPostBattle:
	.string "Sure, I lost the POKéMON battle…\p"
	.string "But when it comes to the love of the\n"
	.string "mountains, nobody out-trains me!$"

JaggedPass_Text_DianaIntro:
	.string "This is no casual hike. Nobody packs a\n"
	.string "picnic for this climb.$"

JaggedPass_Text_DianaDefeat:
	.string "Oh no! I lost my footing on all these\n"
	.string "rocks…$"

JaggedPass_Text_DianaPostBattle:
	.string "Did you know?\p"
	.string "Some people ride their bikes up this\n"
	.string "horribly bumpy pass. They say it's great\l"
	.string "leg training.$"

JaggedPass_Text_DianaRegister:
	.string "Will you be passing through again\n"
	.string "someday? If you do, I'd love a rematch.$"

JaggedPass_Text_DianaRematchIntro:
	.string "A picnic is fun anywhere you go! Just\n"
	.string "like POKéMON battles!$"

JaggedPass_Text_DianaRematchDefeat:
	.string "I only lost because the ground is so\n"
	.string "rocky!$"

JaggedPass_Text_DianaPostRematch:
	.string "I'll forget about the loss and enjoy\n"
	.string "this bumpy hike.$"

JaggedPass_Text_EthanIntro:
	.string "JAGGED PASS is hard to walk on. That's\n"
	.string "what makes it perfect for training.$"

JaggedPass_Text_EthanDefeat:
	.string "It was over before we even found our\n"
	.string "footing…$"

JaggedPass_Text_EthanPostBattle:
	.string "If I had a HOVER BIKE, I could hop right\n"
	.string "over these ledges.$"

JaggedPass_Text_EthanRegister:
	.string "Once I get used to this rough ground,\n"
	.string "I'll win for sure!\p"
	.string "Can you register me in your POKéNAV?$"

JaggedPass_Text_EthanRematchIntro:
	.string "I got used to the bumps. I even sing\n"
	.string "while I climb now!$"

JaggedPass_Text_EthanRematchDefeat:
	.string "Battling on this rough ground is still\n"
	.string "no picnic…$"

JaggedPass_Text_EthanPostRematch:
	.string "I really should get a HOVER BIKE from\n"
	.string "RYDEL in MAUVILLE CITY…$"

JaggedPass_Text_GruntIntro:
	.string "Wah! Who are you?! What are you doing\n"
	.string "here?\p"
	.string "What am I doing in a place like this? Uh…\n"
	.string "guarding a rock! It's top secret!\p"
	.string "What business is it of yours?!$"

JaggedPass_Text_GruntDefeat:
	.string "Urrrgh…\p"
	.string "I should've run back to the RED RIBBON\n"
	.string "BASE right away…$"

JaggedPass_Text_GoWhereverYouWant:
	.string "Okay, oh-kay! I salute you! You're\n"
	.string "strong!\p"
	.string "I won't try to stop you. Go wherever you\n"
	.string "want!$"

JaggedPass_Text_BoulderShakingInResponseToEmblem:
	.string "Whoa! The boulder is trembling! It's\n"
	.string "reacting to the RR EMBLEM!$"

JaggedPass_Text_JulioIntro:
	.string "Aiyeeh! Racing down the mountain at\n"
	.string "full speed is terrifying!$"

JaggedPass_Text_JulioDefeat:
	.string "I feel like I'm coming apart at the\n"
	.string "seams…$"

JaggedPass_Text_JulioPostBattle:
	.string "My bike bounced around so much that my\n"
	.string "rear end is sore…$"

JaggedPass_Text_AutumnIntro:
	.string "I climb this hill every day to train. I\n"
	.string "have confidence in my strength!$"

JaggedPass_Text_AutumnDefeat:
	.string "Hmm… What went wrong?$"

JaggedPass_Text_AutumnPostBattle:
	.string "Have you noticed that odd rock\n"
	.string "sticking out a little way up the hill? It\l"
	.string "looks like it's hiding something.$"

